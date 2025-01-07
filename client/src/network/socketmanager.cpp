#include "socketmanager.h"
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QStandardPaths>
#include <QFile>
#include <QDir>

SocketManager::SocketManager(QObject *parent)
    : QObject(parent)
    , m_serverUrl("ws://localhost:4325/socket.io/?EIO=4&transport=websocket")
    , m_isConnected(false)
    , m_networkManager(new QNetworkAccessManager(this))
{
    initializeSocket();
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

void SocketManager::initializeSocket()
{
    connect(&m_webSocket, &QWebSocket::connected, this, [this]() {
        m_isConnected = true;
        emit connected();
        
        // Start ping timer
        m_pingTimer.setInterval(25000); // 25 seconds
        connect(&m_pingTimer, &QTimer::timeout, this, [this]() {
            if (m_isConnected) {
                m_webSocket.sendTextMessage("2"); // Socket.IO ping message
            }
        });
        m_pingTimer.start();
    });

    connect(&m_webSocket, &QWebSocket::disconnected, this, [this]() {
        m_isConnected = false;
        m_pingTimer.stop();
        emit disconnected();
    });

    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &SocketManager::handleTextMessage);
}

void SocketManager::connectToServer()
{
    if (!m_isConnected) {
        m_webSocket.open(QUrl(m_serverUrl));
    }
}

void SocketManager::disconnectFromServer()
{
    if (m_isConnected) {
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

    switch (type.toLatin1()) {
        case '0': // Connect
            setupEventHandlers();
            break;
        case '3': // Pong
            break;
        case '4': { // Message
            QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
            if (!doc.isArray()) return;

            QJsonArray arr = doc.array();
            if (arr.isEmpty()) return;

            QString event = arr[0].toString();
            QJsonObject payload = arr.size() > 1 ? arr[1].toObject() : QJsonObject();

            // Handle other socket events here
            break;
        }
    }
}

void SocketManager::emitEvent(const QString& event, const QJsonObject& data)
{
    if (!m_isConnected) return;

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
    QNetworkRequest request(QUrl("http://localhost:4325/api/auth/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject data;
    data["username"] = username;
    data["password"] = password;

    QByteArray jsonData = QJsonDocument(data).toJson();
    
    QNetworkReply *reply = m_networkManager->post(request, jsonData);
    
    connect(reply, &QNetworkReply::finished, this, [this, reply, username]() {
        reply->deleteLater();
        
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject obj = doc.object();
            
            if (obj.contains("token")) {
                m_authToken = obj["token"].toString();
                m_username = username;
                saveAuthData(m_authToken, username);
                emit loginSuccess(username);
            }
        } else {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject obj = doc.object();
            emit loginError(obj["error"].toString());
        }
    });
}

void SocketManager::registerUser(const QString& username, const QString& password)
{
    QNetworkRequest request(QUrl("http://localhost:4325/api/auth/register"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject data;
    data["username"] = username;
    data["password"] = password;

    QByteArray jsonData = QJsonDocument(data).toJson();
    
    QNetworkReply *reply = m_networkManager->post(request, jsonData);
    
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        
        if (reply->error() == QNetworkReply::NoError) {
            emit registerSuccess();
        } else {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject obj = doc.object();
            emit registerError(obj["error"].toString());
        }
    });
}

void SocketManager::saveAuthData(const QString& token, const QString& username)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    
    QFile file(path + "/auth.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject authData;
        authData["token"] = token;
        authData["username"] = username;
        
        file.write(QJsonDocument(authData).toJson(QJsonDocument::Compact));
        file.close();
    }
}

bool SocketManager::loadStoredAuth()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile file(path + "/auth.dat");
    
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        
        if (doc.isObject()) {
            QJsonObject authData = doc.object();
            if (authData.contains("token") && authData.contains("username")) {
                m_authToken = authData["token"].toString();
                m_username = authData["username"].toString();
                emit loginSuccess(m_username);
                return true;
            }
        }
    }
    return false;
}

void SocketManager::logout()
{
    m_authToken.clear();
    m_username.clear();
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile::remove(path + "/auth.dat");
} 