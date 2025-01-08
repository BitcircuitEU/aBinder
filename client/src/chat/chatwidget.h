#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTabWidget>
#include <QLabel>
#include "../network/socketmanager.h"

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = nullptr);

private slots:
    void onMessageSent();
    void onChannelChanged(int index);
    void onChannelMessageReceived(const QString& channel, const QString& from, const QString& message, const QDateTime& timestamp);
    void onUserStatusChanged(const QString& username, bool online);
    void onChannelJoined(const QString& channel);
    void onChannelLeft(const QString& channel);
    void loadChannelHistory(const QString& channel, const QVector<ChatMessage>& messages);

private:
    void setupUi();
    void setupConnections();
    void updateOnlineUsers(const QStringList& users);

    QSplitter* mainSplitter;
    QListWidget* userList;
    QTabWidget* chatTabs;
    QMap<QString, QTextEdit*> chatViews;
    QLineEdit* messageInput;
    QPushButton* sendButton;

    QString currentChannel;
    bool m_historyLoaded;  // Flag für geladene History
}; 