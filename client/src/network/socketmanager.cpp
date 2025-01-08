#include "socketmanager.h"
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QCoreApplication>

SocketManager::SocketManager(QObject *parent)
    : QObject(parent)
    , m_serverUrl("wss://abinder.eu/socket.io/?EIO=4&transport=websocket")
    , m_isConnected(false)
    , m_lastServerAvailable(false)
    , m_connectionStatus(ConnectionStatus::Disconnected)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_pingTimer(nullptr)
{
    qDebug() << "\n=== SocketManager wird initialisiert ===";

    // SSL Konfiguration
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);  // Für Entwicklung
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);  // Mindestens TLS 1.2 verwenden
    m_webSocket.setSslConfiguration(sslConfig);
    
    // Timeout-Timer für die Verbindung
    m_connectionTimer = new QTimer(this);
    m_connectionTimer->setSingleShot(true);
    m_connectionTimer->setInterval(5000); // 5 Sekunden Timeout
    
    // Timer für periodische Server-Prüfung
    m_serverCheckTimer.setInterval(5000); // Alle 5 Sekunden prüfen
    m_serverCheckTimer.setParent(this);
    connect(&m_serverCheckTimer, &QTimer::timeout, this, &SocketManager::checkServerAvailability);
    m_serverCheckTimer.start();
    
    connect(m_connectionTimer, &QTimer::timeout, this, &SocketManager::onConnectionTimeout);
    connect(&m_webSocket, &QWebSocket::connected, this, &SocketManager::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &SocketManager::onDisconnected);
    connect(&m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &SocketManager::onError);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &SocketManager::handleTextMessage);

    // Konfiguriere WebSocket
    m_webSocket.setParent(this);
}

SocketManager::~SocketManager()
{
    disconnectFromServer();
}

SocketManager& SocketManager::instance()
{
    static SocketManager instance;
    return instance;
}

void SocketManager::connectToServer()
{
    if (m_authToken.isEmpty()) {
        qDebug() << "Keine Authentifizierung - Verbindung nicht möglich";
        return;
    }

    // Create Socket.IO URL with auth data
    QString wsUrl = QString("wss://abinder.eu/socket.io/?EIO=4&transport=websocket&token=%1&username=%2")
        .arg(m_authToken)
        .arg(m_username);
    QUrl url(wsUrl);
    
    qDebug() << "\n=== Verbindungsaufbau ===";
    qDebug() << "WebSocket URL:" << url.toString();
    qDebug() << "Auth Token vorhanden:" << !m_authToken.isEmpty();
    qDebug() << "Username:" << m_username;

    // Configure SSL
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);
    m_webSocket.setSslConfiguration(sslConfig);
    
    // Connect
    m_webSocket.open(url);
    m_connectionTimer->start(5000);
}

void SocketManager::disconnectFromServer()
{
    if (m_isConnected) {
        // Stoppe Ping Timer
        if (m_pingTimer) {
            m_pingTimer->stop();
        }
        
        m_connectionStatus = ConnectionStatus::Disconnected;
        m_webSocket.close();
    }
}

bool SocketManager::isConnected() const
{
    return m_isConnected;
}

void SocketManager::handleTextMessage(const QString& message)
{
    // Socket.IO message format: <packet type>[<data>]
    if (message.isEmpty()) return;

    QChar type = message[0];
    QString data = message.mid(1);

    qDebug() << "Socket.IO Nachricht empfangen - Typ:" << type << "Daten:" << data;

    switch (type.toLatin1()) {
        case '0': { // Connect
            qDebug() << "Socket.IO: Handshake empfangen";
            
            // Parse Socket.IO Handshake-Daten
            QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                int pingInterval = obj["pingInterval"].toInt();
                int pingTimeout = obj["pingTimeout"].toInt();
                
                // Starte Ping Timer
                if (!m_pingTimer) {
                    m_pingTimer = new QTimer(this);
                }
                disconnect(m_pingTimer, nullptr, nullptr, nullptr);
                connect(m_pingTimer, &QTimer::timeout, this, [this]() {
                    if (m_isConnected) {
                        qDebug() << "Socket.IO: Sende Ping";
                        m_webSocket.sendTextMessage("2"); // Einfaches Ping ohne "probe"
                    }
                });
                m_pingTimer->setInterval(pingInterval);
                m_pingTimer->start();

                // Sende Socket.IO Connect
                m_webSocket.sendTextMessage("40");
                qDebug() << "Socket.IO: Sende Connect";
            }
            break;
        }
        case '4': {
            if (data.startsWith("0")) {
                // Connect bestätigt
                qDebug() << "Socket.IO: Verbindung hergestellt";
                m_isConnected = true;
                m_connectionTimer->stop();
                updateConnectionStatus(ConnectionStatus::Connected);
                emit connected();

                // Automatisch dem Global-Channel beitreten
                QJsonObject joinData;
                joinData.insert("channel", "global");
                emitEvent("chat:join", joinData);
            }
            else if (data.startsWith("2")) {
                // Event
                data = data.mid(1);
                QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
                if (!doc.isArray()) {
                    qDebug() << "Ungültiges Message-Format:" << data;
                    return;
                }

                QJsonArray arr = doc.array();
                if (arr.size() < 2) {
                    qDebug() << "Ungültiges Event-Format:" << data;
                    return;
                }

                QString event = arr[0].toString();
                QJsonObject payload = arr[1].toObject();

                qDebug() << "Socket.IO Event empfangen:" << event << "Payload:" << payload;

                if (event == "chat:joined") {
                    QString channel = payload["channel"].toString().toLower();
                    qDebug() << "Chat: Channel" << channel << "betreten";
                    
                    m_joinedChannels.insert(channel);
                    emit channelJoined(channel);
                }
                else if (event == "chat:message") {
                    QString channel = payload["channel"].toString().toLower();
                    QString from = payload["from"].toString();
                    QString content = payload["content"].toString();
                    QDateTime timestamp = QDateTime::fromString(payload["timestamp"].toString(), Qt::ISODate);
                    
                    qDebug() << "Chat: Nachricht empfangen in" << channel << "von" << from << "-" << content;
                    
                    if (m_joinedChannels.contains(channel)) {
                        emit channelMessageReceived(channel, from, content, timestamp);
                    }
                }
                else if (event == "chat:userList") {
                    QJsonArray users = payload["users"].toArray();
                    QStringList userList;
                    for (const QJsonValue& user : users) {
                        userList.append(user.toString());
                    }
                    qDebug() << "Chat: Benutzerliste aktualisiert -" << userList;
                    emit userListReceived(userList);
                }
                else if (event == "chat:userStatus") {
                    QString username = payload["username"].toString();
                    bool online = payload["online"].toBool();
                    qDebug() << "Chat: Benutzer" << username << (online ? "ist online" : "ist offline");
                    emit userStatusChanged(username, online);
                }
                else if (event == "chat:error") {
                    QString errorMsg = payload["message"].toString();
                    qDebug() << "Chat: Fehler -" << errorMsg;
                    emit chatError(errorMsg);
                }
                else if (event == "chat:history") {
                    QString channel = payload["channel"].toString().toLower();
                    QJsonArray messages = payload["messages"].toArray();
                    qDebug() << "Chat: Historie für" << channel << "empfangen -" << messages.size() << "Nachrichten";
                    
                    QVector<ChatMessage> history;
                    for (const QJsonValue& msgVal : messages) {
                        QJsonObject msg = msgVal.toObject();
                        ChatMessage chatMsg;
                        chatMsg.from = msg["from"].toString();
                        chatMsg.content = msg["content"].toString();
                        chatMsg.timestamp = QDateTime::fromString(msg["timestamp"].toString(), Qt::ISODate);
                        history.append(chatMsg);
                    }
                    emit chatHistoryReceived(channel, history);
                }
            }
            break;
        }
        case '2': { // Ping
            qDebug() << "Socket.IO: Ping empfangen";
            m_webSocket.sendTextMessage("3"); // Pong senden
            break;
        }
        case '3': { // Pong
            qDebug() << "Socket.IO: Pong empfangen";
            break;
        }
    }
}

void SocketManager::emitEvent(const QString& event, const QJsonObject& data)
{
    if (!m_isConnected) {
        qDebug() << "Versuche Event zu senden, aber nicht verbunden:" << event;
        return;
    }

    QJsonArray arr;
    arr.append(event);
    arr.append(data);

    QString message = "42" + QJsonDocument(arr).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(message);
}

void SocketManager::setupEventHandlers()
{
    // Hier können weitere Event-Handler hinzugefügt werden
}

void SocketManager::login(const QString& username, const QString& password)
{
    qDebug() << "\n=== SocketManager: Login-Prozess gestartet ===";
    
    QUrl loginUrl("https://abinder.eu/api/auth/login");
    QNetworkRequest request(loginUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject data;
    data.insert("username", username);
    data.insert("password", password);

    QByteArray jsonData = QJsonDocument(data).toJson(QJsonDocument::Compact);
    
    QNetworkReply *reply = m_networkManager->post(request, jsonData);
    
    connect(reply, &QNetworkReply::finished, this, [this, reply, username]() {
        reply->deleteLater();
        
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                if (obj.contains("token")) {
                    m_authToken = obj["token"].toString();
                    m_username = username;  // Store the username
                    emit loginSuccess(username);
                    connectToServer();  // Connect with the new token
                } else {
                    emit loginError("Ungültige Server-Antwort");
                }
            } else {
                emit loginError("Ungültige Server-Antwort");
            }
        } else {
            QString errorMsg = "Netzwerkfehler: " + reply->errorString();
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            if (doc.isObject() && doc.object().contains("error")) {
                errorMsg = doc.object()["error"].toString();
            }
            emit loginError(errorMsg);
        }
    });
}

void SocketManager::registerUser(const QString& username, const QString& password)
{
    qDebug() << "\n=== SocketManager: Registrierung gestartet ===";
    qDebug() << "Starte Registrierung für Benutzer:" << username;
    
    QUrl registerUrl("https://abinder.eu/api/auth/register");
    qDebug() << "Register-URL:" << registerUrl.toString();
    
    QNetworkRequest request(registerUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject data;
    data.insert("username", username);
    data.insert("password", password);

    QByteArray jsonData = QJsonDocument(data).toJson(QJsonDocument::Compact);
    qDebug() << "Sende Registrierungsanfrage:" << jsonData;
    
    QNetworkReply *reply = m_networkManager->post(request, jsonData);
    
    connect(reply, &QNetworkReply::finished, this, [this, reply, username]() {
        reply->deleteLater();
        
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            qDebug() << "Registrierung erfolgreich. Server-Antwort:" << response;
            
            QJsonDocument doc = QJsonDocument::fromJson(response);
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                
                if (obj.contains("error")) {
                    QString errorMsg = obj["error"].toString();
                    qDebug() << "Server meldet Fehler:" << errorMsg;
                    emit registerError(errorMsg);
                } else {
                    qDebug() << "Registrierung erfolgreich abgeschlossen";
                    emit registerSuccess();
                }
            } else {
                qDebug() << "Leere oder ungültige Server-Antwort";
                emit registerError("Ungültige Server-Antwort");
            }
        } else {
            QByteArray response = reply->readAll();
            qDebug() << "Netzwerkfehler:" << reply->errorString();
            qDebug() << "Server-Fehlerantwort:" << response;
            qDebug() << "HTTP Status Code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            
            QString errorMsg;
            QJsonDocument doc = QJsonDocument::fromJson(response);
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                if (obj.contains("error")) {
                    errorMsg = obj["error"].toString();
                }
            }
            
            if (errorMsg.isEmpty()) {
                errorMsg = "Netzwerkfehler: " + reply->errorString();
            }
            
            emit registerError(errorMsg);
        }
    });
}

void SocketManager::logout()
{
    // Trenne die Socket-Verbindung
    disconnectFromServer();
    
    m_authToken.clear();
    m_username.clear();
}

void SocketManager::sendChannelMessage(const QString& channel, const QString& message)
{
    QString normalizedChannel = channel.toLower();
    
    if (!m_isConnected) {
        qDebug() << "Warnung: Versuche Nachricht zu senden, aber nicht verbunden";
        return;
    }

    if (!m_joinedChannels.contains(normalizedChannel)) {
        qDebug() << "Warnung: Versuche Nachricht in nicht-betretenen Channel zu senden:" << channel;
        return;
    }

    QJsonObject data;
    data.insert("channel", normalizedChannel);
    data.insert("content", message);
    qDebug() << "Chat: Sende Nachricht an" << normalizedChannel << "-" << message;
    emitEvent("chat:message", data);
}

void SocketManager::joinChannel(const QString& channel)
{
    QString normalizedChannel = channel.toLower();
    QJsonObject data;
    data.insert("channel", normalizedChannel);
    emitEvent("chat:join", data);
}

void SocketManager::leaveChannel(const QString& channel)
{
    QString normalizedChannel = channel.toLower();
    QJsonObject data;
    data.insert("channel", normalizedChannel);
    emitEvent("chat:leave", data);
}

void SocketManager::onConnectionTimeout()
{
    m_webSocket.abort();
    updateConnectionStatus(ConnectionStatus::Error);
}

void SocketManager::onConnected()
{
    qDebug() << "WebSocket verbunden - Warte auf Socket.IO Handshake...";
    // Der Handshake wird automatisch vom Server gesendet
}

void SocketManager::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "WebSocket Fehler:" << error;
    m_connectionTimer->stop();
    
    if (error == QAbstractSocket::ConnectionRefusedError) {
        emit serverUnavailable();
    }
}

void SocketManager::onDisconnected()
{
    qDebug() << "WebSocket getrennt";
    m_isConnected = false;
    m_joinedChannels.clear();
    
    // Stoppe Ping Timer
    if (m_pingTimer) {
        m_pingTimer->stop();
        disconnect(m_pingTimer, nullptr, nullptr, nullptr);
    }
    
    updateConnectionStatus(ConnectionStatus::Disconnected);
    emit disconnected();

    // Automatische Wiederverbindung, wenn wir einen Token haben
    if (!m_authToken.isEmpty()) {
        QTimer::singleShot(1000, this, [this]() {
            if (!m_isConnected) {
                qDebug() << "Versuche Wiederverbindung...";
                connectToServer();
            }
        });
    }
}

void SocketManager::checkServerAvailability()
{
    QUrl healthCheckUrl("https://abinder.eu/api/auth/health");
    QNetworkRequest request(healthCheckUrl);
    
    // SSL-Konfiguration für den Health Check
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);
    request.setSslConfiguration(sslConfig);
    
    QNetworkReply *reply = m_networkManager->get(request);
    
    // SSL-Fehler-Handling für den Network Manager
    connect(reply, &QNetworkReply::sslErrors, this, [reply](const QList<QSslError> &errors) {
        qDebug() << "Health Check SSL Fehler:";
        for (const QSslError &error : errors) {
            qDebug() << " -" << error.errorString();
        }
        reply->ignoreSslErrors();
    });
    
    // Timeout nach 2 Sekunden
    QTimer::singleShot(2000, reply, [reply]() {
        if (reply->isRunning()) {
            reply->abort();
        }
    });
    
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Server erreichbar";
            m_lastServerAvailable = true;
            emit serverAvailable();
        } else {
            qDebug() << "Server nicht erreichbar. Fehler:" << reply->errorString();
            qDebug() << "HTTP Status Code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            m_lastServerAvailable = false;
            emit serverUnavailable();
        }
    });
}

void SocketManager::updateConnectionStatus(ConnectionStatus status)
{
    if (m_connectionStatus != status) {
        m_connectionStatus = status;
        emit connectionStatusChanged(status);
    }
} 