#include "mainwindow.h"
#include <QFile>
#include <QLabel>
#include <QMouseEvent>
#include <QStandardPaths>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isDragging(false)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    
    setupUi();
    loadStyleSheet();
}

void MainWindow::setupUi()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    resize(1280, 700);

    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Erstelle Titelleiste
    createTitleBar();

    // Container für Hauptinhalt
    QWidget *contentWidget = new QWidget(this);
    contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    createSidebar();
    createPages();

    mainLayout->addWidget(contentWidget);
}

void MainWindow::createTitleBar()
{
    titleBar = new QWidget(this);
    titleBar->setObjectName("titleBar");
    titleBar->setFixedHeight(30);
    
    titleBarLayout = new QHBoxLayout(titleBar);
    titleBarLayout->setSpacing(0);
    titleBarLayout->setContentsMargins(10, 0, 0, 0);

    // Titel
    titleLabel = new QLabel("aBinder - Startseite", this);
    titleLabel->setObjectName("titleLabel");
    
    // Socket Status
    socketStatusLabel = new QLabel(this);
    socketStatusLabel->setObjectName("socketStatusLabel");
    socketStatusLabel->setAlignment(Qt::AlignCenter);
    socketStatusLabel->setMinimumWidth(150);
    socketStatusLabel->setStyleSheet("QLabel { background-color: transparent; }");
    
    // Logout Button
    logoutButton = new QPushButton(this);
    logoutButton->setObjectName("logoutButton");
    logoutButton->setFixedSize(30, 30);
    logoutButton->setIcon(QIcon(":/icons/signout.svg"));
    logoutButton->setIconSize(QSize(16, 16));
    logoutButton->setToolTip("Abmelden");
    
    // Window Buttons
    minimizeButton = new QPushButton(this);
    minimizeButton->setObjectName("minimizeButton");
    minimizeButton->setFixedSize(30, 30);
    minimizeButton->setIcon(QIcon(":/icons/minimize.svg"));
    minimizeButton->setIconSize(QSize(12, 12));
    
    closeButton = new QPushButton(this);
    closeButton->setObjectName("closeButton");
    closeButton->setFixedSize(30, 30);
    closeButton->setIcon(QIcon(":/icons/close.svg"));
    closeButton->setIconSize(QSize(16, 16));

    titleBarLayout->addWidget(titleLabel);
    titleBarLayout->addStretch();
    titleBarLayout->addWidget(socketStatusLabel);
    titleBarLayout->addWidget(logoutButton);
    titleBarLayout->addWidget(minimizeButton);
    titleBarLayout->addWidget(closeButton);

    mainLayout->addWidget(titleBar);

    // Verbindungen
    connect(minimizeButton, &QPushButton::clicked, this, &MainWindow::onMinimizeClicked);
    connect(closeButton, &QPushButton::clicked, this, &MainWindow::onCloseClicked);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    
    // Socket Status Verbindungen
    auto& socket = SocketManager::instance();
    connect(&socket, &SocketManager::connected, this, [this]() {
        updateSocketStatus(true);
    });
    connect(&socket, &SocketManager::disconnected, this, [this]() {
        updateSocketStatus(false);
    });
    connect(&socket, &SocketManager::connectionStatusChanged, this, [this](SocketManager::ConnectionStatus status) {
        updateSocketStatus(status == SocketManager::ConnectionStatus::Connected);
    });
    
    // Initialen Status setzen
    updateSocketStatus(socket.isConnected());
}

void MainWindow::updateSocketStatus(bool connected)
{
    QString statusText = QString("Socket Status: <span style='color: %1;'>%2</span>")
        .arg(connected ? "#55aa55" : "#ff5555")  // Grün oder Rot
        .arg(connected ? "Verbunden" : "Getrennt");
    socketStatusLabel->setText(statusText);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && titleBar->rect().contains(event->pos())) {
        isDragging = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && isDragging) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
    }
}

void MainWindow::onMinimizeClicked()
{
    showMinimized();
}

void MainWindow::onCloseClicked()
{
    close();
}

void MainWindow::createSidebar()
{
    sidebar = new QWidget(this);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(60);
    sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setSpacing(15);
    sidebarLayout->setContentsMargins(10, 20, 10, 20);

    struct MenuButton {
        QString icon;
        QString tooltip;
    };

    MenuButton buttons[] = {
        {":/icons/home.svg", "Startseite"},
        {":/icons/chat.svg", "Chat"},
        {":/icons/keybinds.svg", "Tastenbelegungen"},
        {":/icons/textbinds.svg", "Textbelegungen"},
        {":/icons/commandbinds.svg", "Befehlsbelegungen"},
        {":/icons/posbinds.svg", "Positionsbelegungen"},
        {":/icons/frakbinds.svg", "Fraktionsbelegungen"},
        {":/icons/groupbinds.svg", "Gruppenbelegungen"},
        {":/icons/overlays.svg", "Overlays"},
        {":/icons/settings.svg", "Einstellungen"}
    };

    for (int i = 0; i < PageCount; ++i) {
        QPushButton *button = new QPushButton(this);
        button->setFixedSize(40, 40);
        button->setIcon(QIcon(buttons[i].icon));
        button->setIconSize(QSize(24, 24));
        button->setToolTip(buttons[i].tooltip);
        button->setToolTipDuration(2000);
        button->setCheckable(true);
        button->setProperty("class", "sidebar-button");
        
        connect(button, &QPushButton::clicked, this, [this, i]() {
            onSidebarButtonClicked(i);
        });

        sidebarLayout->addWidget(button, 0, Qt::AlignCenter);
        sidebarButtons.append(button);
    }

    sidebarLayout->addStretch();
    contentLayout->addWidget(sidebar);
}

void MainWindow::createPages()
{
    contentStack = new QStackedWidget(this);
    contentLayout->addWidget(contentStack);

    // Chat-Widget erstellen
    chatWidget = new ChatWidget(this);

    // Platzhalter-Seiten erstellen
    QStringList pageTitles = {
        "Startseite",
        "Chat",
        "Tastenbelegungen",
        "Textbelegungen",
        "Befehlsbelegungen",
        "Positionsbelegungen",
        "Fraktionsbelegungen",
        "Gruppenbelegungen",
        "Overlays",
        "Einstellungen"
    };

    for (int i = 0; i < pageTitles.size(); ++i) {
        QWidget* page;
        if (i == Chat) {
            page = chatWidget;
        } else {
            page = new QWidget(this);
            QVBoxLayout *layout = new QVBoxLayout(page);
            QLabel *label = new QLabel(pageTitles[i], this);
            label->setAlignment(Qt::AlignCenter);
            layout->addWidget(label);
        }
        contentStack->addWidget(page);
    }
}

void MainWindow::loadStyleSheet()
{
    QFile styleFile(":/styles/dark_theme.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        setStyleSheet(style);
        styleFile.close();
    }
}

void MainWindow::onSidebarButtonClicked(int index)
{
    contentStack->setCurrentIndex(index);
    
    // Titel aktualisieren
    QString pageTitle;
    switch (index) {
        case Home:
            pageTitle = "Startseite";
            break;
        case Chat:
            pageTitle = "Chat";
            break;
        case Keybinds:
            pageTitle = "Keybinds";
            break;
        case Textbinds:
            pageTitle = "Textbinds";
            break;
        case Commandbinds:
            pageTitle = "Commandbinds";
            break;
        case Posbinds:
            pageTitle = "Posbinds";
            break;
        case Frakbinds:
            pageTitle = "Frakbinds";
            break;
        case Groupbinds:
            pageTitle = "Groupbinds";
            break;
        case Overlays:
            pageTitle = "Overlays";
            break;
        case Settings:
            pageTitle = "Einstellungen";
            break;
    }
    titleLabel->setText("aBinder - " + pageTitle);
    
    // Socket Status aktualisieren
    updateSocketStatus(SocketManager::instance().isConnected());
    
    for (int i = 0; i < sidebarButtons.size(); ++i) {
        sidebarButtons[i]->setChecked(i == index);
    }
}

void MainWindow::onLogoutClicked()
{
    QApplication::quit();
} 