#include "loginwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent), isDragging(false)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(400, 600);
    
    setupUi();
    loadStyleSheet();
    setupConnections();
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

    // Status Label erstellen
    statusLabel = new QLabel(this);
    statusLabel->setObjectName("statusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);
    statusLabel->setMinimumHeight(40);
    statusLabel->hide();
    mainLayout->addWidget(statusLabel);

    // Erstelle Seiten-Stack
    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);

    // Erstelle Login- und Register-Seiten
    createLoginPage();
    createRegisterPage();

    // Zeige Login-Seite als Standard
    stackedWidget->setCurrentWidget(loginPage);

    // Prüfe Server-Verfügbarkeit
    checkServerAvailability();
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
    qDebug() << "\n=== Neuer Login-Versuch gestartet ===";
    
    if (usernameEdit->text().isEmpty() || passwordEdit->text().isEmpty()) {
        qDebug() << "Fehler: Leere Eingabefelder";
        setStatusMessage("Bitte fülle alle Felder aus", true);
        return;
    }

    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    
    qDebug() << "Versuche Login für Benutzer:" << username;
    
    // Deaktiviere UI während des Login-Versuchs
    enableLoginForm(false);
    setStatusMessage("Anmeldung läuft...");
    
    // Stelle sicher, dass die Verbindungen korrekt sind
    QObject::disconnect(&SocketManager::instance(), &SocketManager::loginSuccess, this, &LoginWindow::onLoginSuccess);
    QObject::disconnect(&SocketManager::instance(), &SocketManager::loginError, this, &LoginWindow::onLoginError);
    
    QObject::connect(&SocketManager::instance(), &SocketManager::loginSuccess, this, &LoginWindow::onLoginSuccess, Qt::UniqueConnection);
    QObject::connect(&SocketManager::instance(), &SocketManager::loginError, this, &LoginWindow::onLoginError, Qt::UniqueConnection);
    
    qDebug() << "Signal-Verbindungen hergestellt";
    
    try {
        SocketManager::instance().login(username, password);
        qDebug() << "Login-Anfrage gesendet";
    } catch (const std::exception& e) {
        qDebug() << "FEHLER beim Login-Versuch:" << e.what();
        setStatusMessage("Fehler bei der Anmeldung", true);
        enableLoginForm(true);
    }
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
    connect(usernameEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);

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
    if (regUsernameEdit->text().isEmpty() || regPasswordEdit->text().isEmpty() || regConfirmPasswordEdit->text().isEmpty()) {
        setStatusMessage("Bitte fülle alle Felder aus", true);
        return;
    }

    if (regPasswordEdit->text() != regConfirmPasswordEdit->text()) {
        setStatusMessage("Passwörter stimmen nicht überein", true);
        return;
    }

    if (regPasswordEdit->text().length() < 6) {
        setStatusMessage("Das Passwort muss mindestens 6 Zeichen lang sein", true);
        return;
    }

    if (regUsernameEdit->text().length() < 3) {
        setStatusMessage("Der Benutzername muss mindestens 3 Zeichen lang sein", true);
        return;
    }

    enableRegisterForm(false);
    setStatusMessage("Registrierung läuft...");
    
    // Direkt die Registrierung aufrufen, ohne neue Signal-Verbindungen
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

void LoginWindow::setupConnections()
{
    SocketManager& socketManager = SocketManager::instance();
    connect(&socketManager, &SocketManager::loginSuccess, this, &LoginWindow::onLoginSuccess);
    connect(&socketManager, &SocketManager::loginError, this, &LoginWindow::onLoginError);
    connect(&socketManager, &SocketManager::registerSuccess, this, &LoginWindow::onRegisterSuccess);
    connect(&socketManager, &SocketManager::registerError, this, &LoginWindow::onRegisterError);
    connect(&socketManager, &SocketManager::serverUnavailable, this, &LoginWindow::onServerError);
    
    // Nur aktivieren wenn vorher deaktiviert
    connect(&socketManager, &SocketManager::serverAvailable, this, [this]() {
        if (!usernameEdit->isEnabled()) {
            enableLoginForm(true);
        }
        if (!regUsernameEdit->isEnabled()) {
            enableRegisterForm(true);
        }
        statusLabel->hide();
    });
}

void LoginWindow::showLoginForm()
{
    stackedWidget->setCurrentWidget(loginPage);
    titleLabel->setText("aBinder - Anmeldung");
    
    // Felder zurücksetzen
    regUsernameEdit->clear();
    regPasswordEdit->clear();
    regConfirmPasswordEdit->clear();
}

void LoginWindow::showRegisterForm()
{
    stackedWidget->setCurrentWidget(registerPage);
    titleLabel->setText("aBinder - Registrierung");
    
    // Felder zurücksetzen
    usernameEdit->clear();
    passwordEdit->clear();
}

void LoginWindow::onServerError()
{
    statusLabel->setText("Fehler beim Verbinden mit dem Server");
    statusLabel->setStyleSheet("color: red;");
    statusLabel->show();
    enableLoginForm(true);
    enableRegisterForm(true);
}

void LoginWindow::onLoginSuccess(const QString& username)
{
    qDebug() << "=== Login erfolgreich für Benutzer:" << username << "===";
    setStatusMessage("Login erfolgreich!");
    
    // Deaktiviere weitere Login-Versuche
    enableLoginForm(false);
    
    // Verzögerung erhöhen und sicherstellen, dass das Fenster nicht geschlossen wird
    QTimer *successTimer = new QTimer(this);
    successTimer->setSingleShot(true);
    connect(successTimer, &QTimer::timeout, this, [this, username]() {
        qDebug() << "=== Starte Main-App für Benutzer:" << username << "===";
        emit loginSuccessful();
    });
    successTimer->start(1000);
}

void LoginWindow::onLoginError(const QString& error)
{
    qDebug() << "Login-Fehler:" << error;
    setStatusMessage(error, true);
    enableLoginForm(true);
}

void LoginWindow::onRegisterSuccess()
{
    setStatusMessage("Registrierung erfolgreich! Sie können sich jetzt anmelden.");
    
    // Felder zurücksetzen
    regUsernameEdit->clear();
    regPasswordEdit->clear();
    regConfirmPasswordEdit->clear();
    
    // Formular wieder aktivieren
    enableRegisterForm(true);
    
    // Zurück zur Login-Seite
    stackedWidget->setCurrentWidget(loginPage);
    titleLabel->setText("aBinder - Anmeldung");
}

void LoginWindow::onRegisterError(const QString& error)
{
    qDebug() << "Registrierungsfehler:" << error;
    setStatusMessage(error, true);
    enableRegisterForm(true);
}

void LoginWindow::setStatusMessage(const QString &message, bool isError)
{
    statusLabel->setText(message);
    
    if (isError) {
        statusLabel->setStyleSheet("QLabel { background-color: #ff5555; color: white; padding: 10px; border-radius: 4px; margin: 10px; }");
    } else {
        statusLabel->setStyleSheet("QLabel { background-color: #55aa55; color: white; padding: 10px; border-radius: 4px; margin: 10px; }");
    }
    
    statusLabel->show();
    
    // Nur bestimmte Meldungen automatisch ausblenden
    if (!isError || 
        (message.contains("erfolgreich") && !message.contains("nicht"))) {
        QTimer::singleShot(5000, this, [this, message]() {
            // Nur ausblenden, wenn sich die Meldung nicht geändert hat
            if (statusLabel->text() == message) {
                statusLabel->hide();
            }
        });
    }
}

void LoginWindow::enableLoginForm(bool enable)
{
    usernameEdit->setEnabled(enable);
    passwordEdit->setEnabled(enable);
    loginButton->setEnabled(enable);
    
    // Wenn aktiviert, Fokus auf Benutzernamen setzen
    if (enable && usernameEdit->text().isEmpty()) {
        usernameEdit->setFocus();
    }
}

void LoginWindow::enableRegisterForm(bool enable)
{
    regUsernameEdit->setEnabled(enable);
    regPasswordEdit->setEnabled(enable);
    regConfirmPasswordEdit->setEnabled(enable);
    registerButton->setEnabled(enable);
    
    // Wenn aktiviert, Fokus auf Benutzernamen setzen
    if (enable && regUsernameEdit->text().isEmpty()) {
        regUsernameEdit->setFocus();
    }
}

void LoginWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "\n=== Login-Fenster wird geschlossen ===";
    qDebug() << "Aktuelle UI-Status:";
    qDebug() << "- Login-Formular aktiviert:" << usernameEdit->isEnabled();
    qDebug() << "- Benutzername ausgefüllt:" << !usernameEdit->text().isEmpty();
    QMainWindow::closeEvent(event);
}

void LoginWindow::checkServerAvailability()
{
    SocketManager::instance().checkServerAvailability();
} 