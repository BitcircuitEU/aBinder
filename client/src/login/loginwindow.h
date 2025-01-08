#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "../network/socketmanager.h"

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccessful();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onMinimizeClicked();
    void onCloseClicked();
    void onLoginClicked();
    void onRegisterClicked();
    void onBackToLoginClicked();
    void onLoginSuccess(const QString& username);
    void onLoginError(const QString& error);
    void onRegisterSuccess();
    void onRegisterError(const QString& error);
    void onServerError();
    void showLoginForm();
    void showRegisterForm();
    void setupConnections();
    void setStatusMessage(const QString &message, bool isError = false);
    void enableLoginForm(bool enabled);
    void enableRegisterForm(bool enabled);

private:
    void setupUi();
    void createTitleBar();
    void createLoginPage();
    void createRegisterPage();
    void loadStyleSheet();
    void checkServerAvailability();

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QWidget *titleBar;
    QHBoxLayout *titleBarLayout;
    QLabel *titleLabel;
    QPushButton *minimizeButton;
    QPushButton *closeButton;
    
    QStackedWidget *stackedWidget;
    QWidget *loginPage;
    QWidget *registerPage;
    
    // Login Page
    QLabel *logoLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    
    // Register Page
    QLineEdit *regUsernameEdit;
    QLineEdit *regPasswordEdit;
    QLineEdit *regConfirmPasswordEdit;
    QPushButton *registerButton;
    QPushButton *backToLoginButton;
    
    // Status
    QLabel *statusLabel;
    
    bool isDragging;
    QPoint dragPosition;
}; 