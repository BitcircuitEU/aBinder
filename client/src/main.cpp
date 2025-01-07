#include <QApplication>
#include "login/loginwindow.h"
#include "mainapp/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Dark Theme als Standard
    app.setStyle("Fusion");

    LoginWindow *loginWindow = new LoginWindow();
    MainWindow *mainWindow = new MainWindow();

    // Verbinde Login-Erfolg mit dem Öffnen des Hauptfensters
    QObject::connect(loginWindow, &LoginWindow::loginSuccessful, [loginWindow, mainWindow]() {
        loginWindow->hide();
        mainWindow->show();
    });

    loginWindow->show();

    return app.exec();
} 