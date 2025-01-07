#include "loginwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent), isDragging(false)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(400, 600);
    
    setupUi();
    loadStyleSheet();

    // Verbinde mit dem Server und versuche gespeicherte Anmeldedaten zu laden
    SocketManager::instance().connectToServer();
    if (SocketManager::instance().loadStoredAuth()) {
        emit loginSuccessful();
    }
}

void LoginWindow::setupUi()
{
    centralWidget = new QWidget(this);
    centralWidget->setObjectName("loginWidget");
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(1, 1, 1, 1);

    // Erstelle Titelleiste
    createTitleBar();

    // Erstelle Seiten-Stack
    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);

    // Erstelle Login- und Register-Seiten
    createLoginPage();
    createRegisterPage();

    // Zeige Login-Seite als Standard
    stackedWidget->setCurrentWidget(loginPage);

    // Verbinde mit dem Server
    SocketManager::instance().connectToServer();
}

void LoginWindow::createTitleBar()
{
    titleBar = new QWidget(this);
    titleBar->setObjectName("titleBar");
    titleBar->setFixedHeight(30);
    
    titleBarLayout = new QHBoxLayout(titleBar);
    titleBarLayout->setSpacing(0);
    titleBarLayout->setContentsMargins(10, 0, 0, 0);

    // Titel
    titleLabel = new QLabel("aBinder - Anmeldung", this);
    titleLabel->setObjectName("titleLabel");
    
    // Buttons
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
    titleBarLayout->addWidget(minimizeButton);
    titleBarLayout->addWidget(closeButton);

    mainLayout->addWidget(titleBar);

    // Verbindungen
    connect(minimizeButton, &QPushButton::clicked, this, &LoginWindow::onMinimizeClicked);
    connect(closeButton, &QPushButton::clicked, this, &LoginWindow::onCloseClicked);
}

void LoginWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && titleBar->rect().contains(event->pos())) {
        isDragging = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && isDragging) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
    }
}

void LoginWindow::onMinimizeClicked()
{
    showMinimized();
}

void LoginWindow::onCloseClicked()
{
    close();
}

void LoginWindow::onLoginClicked()
{
    loginButton->setEnabled(false);
    loginButton->setText("Anmeldung läuft...");

    // Verbinde die Socket.IO Signale
    connect(&SocketManager::instance(), &SocketManager::loginSuccess, this, [this](const QString& username) {
        loginButton->setEnabled(true);
        loginButton->setText("Anmelden");
        emit loginSuccessful();
    });

    connect(&SocketManager::instance(), &SocketManager::loginError, this, [this](const QString& message) {
        loginButton->setEnabled(true);
        loginButton->setText("Anmelden");
        QMessageBox::warning(this, "Fehler", "Anmeldung fehlgeschlagen: " + message);
    });

    // Sende Login-Anfrage
    SocketManager::instance().login(usernameEdit->text(), passwordEdit->text());
}

void LoginWindow::loadStyleSheet()
{
    QFile styleFile(":/styles/dark_theme.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        setStyleSheet(style);
        styleFile.close();
    }
}

void LoginWindow::createLoginPage()
{
    loginPage = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(loginPage);
    contentLayout->setSpacing(20);
    contentLayout->setContentsMargins(40, 60, 40, 40);

    // Logo
    logoLabel = new QLabel(this);
    logoLabel->setPixmap(QPixmap(":/icons/logo.svg").scaled(240, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(logoLabel);

    contentLayout->addSpacing(20);

    // Spielername
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Spielername");
    usernameEdit->setMinimumHeight(40);
    contentLayout->addWidget(usernameEdit);

    // Passwort
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Passwort");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumHeight(40);
    contentLayout->addWidget(passwordEdit);

    contentLayout->addSpacing(20);

    // Login Button
    loginButton = new QPushButton("Anmelden", this);
    loginButton->setFixedHeight(40);
    loginButton->setMinimumWidth(200);
    contentLayout->addWidget(loginButton, 0, Qt::AlignCenter);

    // Register Button
    QPushButton *toRegisterButton = new QPushButton("Registrieren", this);
    toRegisterButton->setObjectName("registerButton");
    toRegisterButton->setFixedHeight(40);
    toRegisterButton->setMinimumWidth(200);
    contentLayout->addWidget(toRegisterButton, 0, Qt::AlignCenter);

    contentLayout->addStretch();

    // Verbindungen
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(toRegisterButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(registerPage);
        titleLabel->setText("aBinder - Registrierung");
    });

    // Enter-Taste Verbindungen
    connect(usernameEdit, &QLineEdit::returnPressed, loginButton, &QPushButton::click);
    connect(passwordEdit, &QLineEdit::returnPressed, loginButton, &QPushButton::click);

    stackedWidget->addWidget(loginPage);
}

void LoginWindow::createRegisterPage()
{
    registerPage = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(registerPage);
    contentLayout->setSpacing(20);
    contentLayout->setContentsMargins(40, 60, 40, 40);

    // Logo
    QLabel *regLogoLabel = new QLabel(this);
    regLogoLabel->setPixmap(QPixmap(":/icons/logo.svg").scaled(240, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    regLogoLabel->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(regLogoLabel);

    contentLayout->addSpacing(20);

    // Spielername
    regUsernameEdit = new QLineEdit(this);
    regUsernameEdit->setPlaceholderText("Spielername");
    regUsernameEdit->setMinimumHeight(40);
    contentLayout->addWidget(regUsernameEdit);

    // Passwort
    regPasswordEdit = new QLineEdit(this);
    regPasswordEdit->setPlaceholderText("Passwort");
    regPasswordEdit->setEchoMode(QLineEdit::Password);
    regPasswordEdit->setMinimumHeight(40);
    contentLayout->addWidget(regPasswordEdit);

    // Passwort bestätigen
    regConfirmPasswordEdit = new QLineEdit(this);
    regConfirmPasswordEdit->setPlaceholderText("Passwort bestätigen");
    regConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    regConfirmPasswordEdit->setMinimumHeight(40);
    contentLayout->addWidget(regConfirmPasswordEdit);

    contentLayout->addSpacing(20);

    // Register Button
    registerButton = new QPushButton("Account erstellen", this);
    registerButton->setFixedHeight(40);
    registerButton->setMinimumWidth(200);
    contentLayout->addWidget(registerButton, 0, Qt::AlignCenter);

    // Zurück Button
    backToLoginButton = new QPushButton("Zurück zum Login", this);
    backToLoginButton->setObjectName("backButton");
    backToLoginButton->setFixedHeight(40);
    backToLoginButton->setMinimumWidth(200);
    contentLayout->addWidget(backToLoginButton, 0, Qt::AlignCenter);

    contentLayout->addStretch();

    // Verbindungen
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connect(backToLoginButton, &QPushButton::clicked, this, &LoginWindow::onBackToLoginClicked);

    // Enter-Taste Verbindungen
    connect(regUsernameEdit, &QLineEdit::returnPressed, registerButton, &QPushButton::click);
    connect(regPasswordEdit, &QLineEdit::returnPressed, registerButton, &QPushButton::click);
    connect(regConfirmPasswordEdit, &QLineEdit::returnPressed, registerButton, &QPushButton::click);

    stackedWidget->addWidget(registerPage);
}

void LoginWindow::onRegisterClicked()
{
    // Überprüfe, ob die Passwörter übereinstimmen
    if (regPasswordEdit->text() != regConfirmPasswordEdit->text()) {
        QMessageBox::warning(this, "Fehler", "Die Passwörter stimmen nicht überein!");
        return;
    }

    registerButton->setEnabled(false);
    registerButton->setText("Registrierung läuft...");

    // Verbinde die Socket.IO Signale
    connect(&SocketManager::instance(), &SocketManager::registerSuccess, this, [this]() {
        registerButton->setEnabled(true);
        registerButton->setText("Account erstellen");
        QMessageBox::information(this, "Erfolg", "Registrierung erfolgreich!\nBitte melden Sie sich jetzt an.");
        onBackToLoginClicked();
    });

    connect(&SocketManager::instance(), &SocketManager::registerError, this, [this](const QString& message) {
        registerButton->setEnabled(true);
        registerButton->setText("Account erstellen");
        QMessageBox::warning(this, "Fehler", "Registrierung fehlgeschlagen: " + message);
    });

    // Sende Registrierungs-Anfrage
    SocketManager::instance().registerUser(regUsernameEdit->text(), regPasswordEdit->text());
}

void LoginWindow::onBackToLoginClicked()
{
    stackedWidget->setCurrentWidget(loginPage);
    titleLabel->setText("aBinder - Anmeldung");
    
    // Felder zurücksetzen
    regUsernameEdit->clear();
    regPasswordEdit->clear();
    regConfirmPasswordEdit->clear();
} 