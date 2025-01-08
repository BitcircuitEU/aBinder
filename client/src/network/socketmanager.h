#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QSet>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslSocket>

struct ChatMessage {
    QString from;
    QString content;
    QDateTime timestamp;
};

class SocketManager : public QObject
{
    Q_OBJECT
public:
    enum ConnectionStatus {
        Disconnected,
        Connecting,
        Connected,
        Error
    };
    Q_ENUM(ConnectionStatus)

    static SocketManager& instance();
    
    void connectToServer();
    void disconnectFromServer();
    bool isConnected() const;
    ConnectionStatus getConnectionStatus() const { return m_connectionStatus; }
    void checkServerAvailability();
    
    // Auth
    void login(const QString& username, const QString& password);
    void registerUser(const QString& username, const QString& password);
    void logout();
    
    // Chat
    void sendChannelMessage(const QString& channel, const QString& message);
    void joinChannel(const QString& channel);
    void leaveChannel(const QString& channel);

    QString username() const { return m_username; }

signals:
    void loginSuccess(const QString& username);
    void loginError(const QString& message);
    void registerSuccess();
    void registerError(const QString& message);
    void serverUnavailable();  // Für Fehlermeldungen
    void serverAvailable();    // Für Formular-Aktivierung
    void connected();
    void disconnected();
    void channelMessageReceived(const QString& channel, const QString& from, const QString& content, const QDateTime& timestamp);
    void userListReceived(const QStringList& users);
    void userStatusChanged(const QString& username, bool online);
    void channelJoined(const QString& channel);
    void channelLeft(const QString& channel);
    void chatError(const QString& message);
    void chatHistoryReceived(const QString& channel, const QVector<ChatMessage>& messages);
    void connectionStatusChanged(ConnectionStatus status);

private:
    explicit SocketManager(QObject *parent = nullptr);
    ~SocketManager();
    SocketManager(const SocketManager&) = delete;
    SocketManager& operator=(const SocketManager&) = delete;
    
    void handleTextMessage(const QString& message);
    void emitEvent(const QString& event, const QJsonObject& data = QJsonObject());
    void setupEventHandlers();
    void onConnectionTimeout();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
    void updateConnectionStatus(ConnectionStatus status);
    
    QWebSocket m_webSocket;
    QString m_serverUrl;
    QString m_authToken;
    QString m_username;
    bool m_isConnected;
    bool m_lastServerAvailable;
    ConnectionStatus m_connectionStatus;
    QTimer* m_connectionTimer;
    QTimer* m_pingTimer = nullptr;  // Timer für Socket.IO Ping/Pong
    QTimer m_serverCheckTimer;
    QNetworkAccessManager* m_networkManager;
    QSet<QString> m_joinedChannels;  // Tracking der betretenen Channel
};

#endif // SOCKETMANAGER_H 