#include "mainwindow.h"


#include "ui_mainwindow.h"
#include "gameboard.h"
#include <QMessageBox>
#include <QPixmap>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), screen(QApplication::primaryScreen()){
    ui->setupUi(this);
    // 设置窗口标题和图标
    setWindowTitle("五子棋");
    setWindowIcon(QIcon(":/image/images/icon.png")); // 使用资源中的图标

    QRect screenGeometry = screen->geometry();
    int width = screenGeometry.width() *0.8;
    int height = screenGeometry.height() *0.8;
    this -> resize(width,height);
    // 设置 QStackedWidget 为中央窗口小部件
    setCentralWidget(ui->centralWidget);
    // 连接按钮的信号和槽
    connect(ui->playerVsPlayerButton, &QPushButton::clicked, this, &MainWindow::onPlayerVsPlayerClicked);
    connect(ui->playerVsAIButton, &QPushButton::clicked, this, &MainWindow::onPlayerVsAIClicked);
    connect(ui->exitGameButton, &QPushButton::clicked, this, &MainWindow::onExitGameClicked);
    game = new GameBoard(this);
    game->hide();
}

MainWindow::~MainWindow() {
    delete ui;
    delete game;
}

void MainWindow::onPlayerVsPlayerClicked() {
    game->setGameMode(1);
    this->hide();
    game->show();
}

void MainWindow::onPlayerVsAIClicked() {
    game->setGameMode(2);
    this->hide();
    game->show();
}

void MainWindow::onExitGameClicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "退出游戏", "确定要退出游戏吗?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}
