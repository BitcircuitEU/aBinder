#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QPoint>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onMinimizeClicked();
    void onCloseClicked();
    void onLogoutClicked();

private:
    void setupUi();
    void createTitleBar();
    void createSidebar();
    void createPages();
    void loadStyleSheet();
    void onSidebarButtonClicked(int index);

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    // Titelleiste
    QWidget *titleBar;
    QHBoxLayout *titleBarLayout;
    QLabel *titleLabel;
    QPushButton *minimizeButton;
    QPushButton *closeButton;
    QPushButton *logoutButton;

    // Hauptbereich
    QHBoxLayout *contentLayout;
    QWidget *sidebar;
    QVBoxLayout *sidebarLayout;
    QStackedWidget *contentStack;
    QVector<QPushButton*> sidebarButtons;

    // Fenster-Bewegung
    bool isDragging;
    QPoint dragPosition;

    // Seiten-Indizes
    enum Pages {
        Home,
        Keybinds,
        Textbinds,
        Commandbinds,
        Posbinds,
        Frakbinds,
        Groupbinds,
        Overlays,
        Settings,
        PageCount
    };
}; 