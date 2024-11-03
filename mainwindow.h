#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gameboard.h"
#include <QStackedWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onPlayerVsPlayerClicked();
    void onPlayerVsAIClicked();
    void onExitGameClicked();
    void showMainMenu();
private:
    Ui::MainWindow *ui;
    GameBoard *game;
    QStackedWidget *stackedWidget;
};

#endif // MAINWINDOW_H
