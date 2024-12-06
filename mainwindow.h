#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gameboard.h"
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
private:
    Ui::MainWindow *ui;
    GameBoard *game;

    QScreen *screen;
};

#endif // MAINWINDOW_H
