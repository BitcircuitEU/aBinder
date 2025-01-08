#include "chatwidget.h"
#include <QDateTime>

ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
    m_historyLoaded = false;  // Initialisiere History-Status
}

void ChatWidget::setupUi()
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Hauptsplitter erstellen
    mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(mainSplitter);

    // Benutzerliste
    auto userListWidget = new QWidget(this);
    auto userListLayout = new QVBoxLayout(userListWidget);
    userListLayout->setContentsMargins(10, 10, 10, 10);

    auto userListLabel = new QLabel("Online Benutzer", this);
    userListLayout->addWidget(userListLabel);

    userList = new QListWidget(this);
    userList->setMinimumWidth(200);
    userListLayout->addWidget(userList);

    mainSplitter->addWidget(userListWidget);

    // Chat-Bereich
    auto chatWidget = new QWidget(this);
    auto chatLayout = new QVBoxLayout(chatWidget);
    chatLayout->setContentsMargins(10, 10, 10, 10);

    chatTabs = new QTabWidget(this);
    chatLayout->addWidget(chatTabs);

    // Standard-Tabs erstellen
    auto globalChat = new QTextEdit(this);
    globalChat->setReadOnly(true);
    chatTabs->addTab(globalChat, "global");
    chatViews["global"] = globalChat;

    // Nachrichteneingabe
    auto inputWidget = new QWidget(this);
    auto inputLayout = new QHBoxLayout(inputWidget);
    inputLayout->setContentsMargins(0, 10, 0, 0);

    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("Nachricht eingeben...");
    inputLayout->addWidget(messageInput);

    sendButton = new QPushButton("Senden", this);
    inputLayout->addWidget(sendButton);

    chatLayout->addWidget(inputWidget);

    mainSplitter->addWidget(chatWidget);
    mainSplitter->setStretchFactor(1, 1);
}

void ChatWidget::setupConnections()
{
    connect(sendButton, &QPushButton::clicked, this, &ChatWidget::onMessageSent);
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatWidget::onMessageSent);

    // Socket.IO Verbindungen
    auto& socket = SocketManager::instance();
    
    // Chat-Nachrichten
    connect(&socket, &SocketManager::channelMessageReceived,
            this, &ChatWidget::onChannelMessageReceived);
    
    // Chat-Historie
    connect(&socket, &SocketManager::chatHistoryReceived,
            this, &ChatWidget::loadChannelHistory);
    
    // Benutzer-Status
    connect(&socket, &SocketManager::userStatusChanged,
            this, &ChatWidget::onUserStatusChanged);
    connect(&socket, &SocketManager::userListReceived,
            this, &ChatWidget::updateOnlineUsers);
    
    // Channel-Events
    connect(&socket, &SocketManager::channelJoined,
            this, &ChatWidget::onChannelJoined);
    connect(&socket, &SocketManager::channelLeft,
            this, &ChatWidget::onChannelLeft);

    // Setze initialen Channel
    currentChannel = "global";
}

void ChatWidget::updateOnlineUsers(const QStringList& users)
{
    userList->clear();
    for (const QString& username : users) {
        userList->addItem(username);
    }
}

void ChatWidget::onMessageSent()
{
    QString message = messageInput->text().trimmed();
    if (message.isEmpty()) return;

    auto& socket = SocketManager::instance();
    socket.sendChannelMessage(currentChannel, message);

    messageInput->clear();
}

void ChatWidget::onChannelChanged(int index)
{
    currentChannel = chatTabs->tabText(index);
    messageInput->setPlaceholderText("Nachricht an " + currentChannel + "...");
}

void ChatWidget::onChannelMessageReceived(const QString& channel, const QString& from, const QString& message, const QDateTime& timestamp)
{
    if (!chatViews.contains(channel)) return;

    QTextEdit* chatView = chatViews[channel];
    QString timeStr = timestamp.toString("HH:mm:ss");
    
    // HTML-formatierte Nachricht
    QString formattedMessage = QString(
        "<div style='margin: 2px 0;'>"
        "<span style='color: #888;'>[%1]</span> "
        "<span style='color: %2; font-weight: bold;'>%3:</span> "
        "<span style='color: #fff;'>%4</span>"
        "</div>"
    ).arg(
        timeStr,
        from == SocketManager::instance().username() ? "#55aa55" : "#5555ff",
        from,
        message
    );
    
    chatView->append(formattedMessage);
}

void ChatWidget::onUserStatusChanged(const QString& username, bool online)
{
    // Benutzer zur Liste hinzufügen oder entfernen
    QList<QListWidgetItem*> items = userList->findItems(username, Qt::MatchExactly);
    
    if (online) {
        if (items.isEmpty()) {
            auto item = new QListWidgetItem(username);
            item->setForeground(Qt::green);
            userList->addItem(item);
            
            // Systemnachricht in allen relevanten Chats
            QString statusMsg = username + " ist jetzt online";
            for (auto chatView : chatViews) {
                chatView->append(QString("<div style='color: #888; font-style: italic;'>%1</div>").arg(statusMsg));
            }
        }
    } else {
        for (auto item : items) {
            delete userList->takeItem(userList->row(item));
            
            // Systemnachricht in allen relevanten Chats
            QString statusMsg = username + " ist jetzt offline";
            for (auto chatView : chatViews) {
                chatView->append(QString("<div style='color: #888; font-style: italic;'>%1</div>").arg(statusMsg));
            }
        }
    }
}

void ChatWidget::onChannelJoined(const QString& channel)
{
    if (!chatViews.contains(channel)) {
        auto chatView = new QTextEdit(this);
        chatView->setReadOnly(true);
        chatViews[channel] = chatView;
        chatTabs->addTab(chatView, channel);
    }
}

void ChatWidget::onChannelLeft(const QString& channel)
{
    if (chatViews.contains(channel)) {
        int index = chatTabs->indexOf(chatViews[channel]);
        if (index != -1) {
            chatTabs->removeTab(index);
        }
        delete chatViews[channel];
        chatViews.remove(channel);
    }
}

void ChatWidget::loadChannelHistory(const QString& channel, const QVector<ChatMessage>& messages)
{
    // Nur laden wenn History noch nicht geladen wurde
    if (m_historyLoaded) return;

    if (!chatViews.contains(channel)) return;
    QTextEdit* chatView = chatViews[channel];
    
    // Lösche vorherige Nachrichten
    chatView->clear();
    
    // Füge History-Nachrichten hinzu
    for (const auto& msg : messages) {
        QString timeStr = msg.timestamp.toString("HH:mm:ss");
        
        QString formattedMessage = QString(
            "<div style='margin: 2px 0;'>"
            "<span style='color: #888;'>[%1]</span> "
            "<span style='color: %2; font-weight: bold;'>%3:</span> "
            "<span style='color: #fff;'>%4</span>"
            "</div>"
        ).arg(
            timeStr,
            msg.from == SocketManager::instance().username() ? "#55aa55" : "#5555ff",
            msg.from,
            msg.content
        );
        
        chatView->append(formattedMessage);
    }

    m_historyLoaded = true;  // Markiere History als geladen
} 