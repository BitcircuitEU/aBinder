#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPoint>
#include <QStackedWidget>
#include <QMessageBox>
#include "../network/socketmanager.h"

class LoginWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccessful();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onMinimizeClicked();
    void onCloseClicked();
    void onBackToLoginClicked();

private:
    void setupUi();
    void loadStyleSheet();
    void createTitleBar();
    void createLoginPage();
    void createRegisterPage();

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    
    // Titelleiste
    QWidget *titleBar;
    QHBoxLayout *titleBarLayout;
    QLabel *titleLabel;
    QPushButton *minimizeButton;
    QPushButton *closeButton;
    
    // Seiten
    QStackedWidget *stackedWidget;
    QWidget *loginPage;
    QWidget *registerPage;
    
    // Login-Widgets
    QLabel *logoLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;

    // Register-Widgets
    QLineEdit *regUsernameEdit;
    QLineEdit *regPasswordEdit;
    QLineEdit *regConfirmPasswordEdit;
    QPushButton *registerButton;
    QPushButton *backToLoginButton;

    // Fenster-Bewegung
    bool isDragging;
    QPoint dragPosition;
}; 