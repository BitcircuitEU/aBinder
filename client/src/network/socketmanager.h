#pragma once

#include <QObject>
#include <QWebSocket>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUrl>
#include <QNetworkAccessManager>

class SocketManager : public QObject
{
    Q_OBJECT

public:
    static SocketManager& instance();
    void connectToServer();
    void disconnectFromServer();
    bool isConnected() const;

    // Auth Methods
    void login(const QString& username, const QString& password);
    void registerUser(const QString& username, const QString& password);
    bool loadStoredAuth();
    void logout();

signals:
    void connected();
    void disconnected();
    void loginSuccess(const QString& username);
    void loginError(const QString& message);
    void registerSuccess();
    void registerError(const QString& message);

private:
    explicit SocketManager(QObject *parent = nullptr);
    ~SocketManager();
    
    void initializeSocket();
    void handleTextMessage(const QString& message);
    void emitEvent(const QString& event, const QJsonObject& data);
    void setupEventHandlers();
    void saveAuthData(const QString& token, const QString& username);

    QWebSocket m_webSocket;
    QString m_serverUrl;
    bool m_isConnected;
    QTimer m_pingTimer;
    QNetworkAccessManager* m_networkManager;
    QString m_authToken;
    QString m_username;

    // Prevent copying
    SocketManager(const SocketManager&) = delete;
    SocketManager& operator=(const SocketManager&) = delete;
}; 