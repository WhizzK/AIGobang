#include "mainwindow.h"

#include <iostream>

#include "ui_mainwindow.h"
#include "gameboard.h"
#include <QMessageBox>
#include <QPixmap>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), game(new GameBoard), stackedWidget(new QStackedWidget(this)){
    ui->setupUi(this);
    // 设置窗口标题和图标
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowTitle("五子棋");
    setWindowIcon(QIcon(":/image/images/icon.png")); // 使用资源中的图标
    stackedWidget->addWidget(ui->centralWidget);
    stackedWidget->addWidget(game);

    // 设置 QStackedWidget 为中央窗口小部件
    setCentralWidget(stackedWidget);

    // 连接按钮的信号和槽
    connect(ui->playerVsPlayerButton, &QPushButton::clicked, this, &MainWindow::onPlayerVsPlayerClicked);
    connect(ui->playerVsAIButton, &QPushButton::clicked, this, &MainWindow::onPlayerVsAIClicked);
    connect(ui->exitGameButton, &QPushButton::clicked, this, &MainWindow::onExitGameClicked);
    connect(game,GameBoard::backToMainMenu,this,&MainWindow::showMainMenu);
}

MainWindow::~MainWindow() {
    delete ui;
    delete game;
}

void MainWindow::onPlayerVsPlayerClicked() {
    game->setGameMode(1);
    stackedWidget->setCurrentWidget(game);
}

void MainWindow::onPlayerVsAIClicked() {
    game->setGameMode(2);
    stackedWidget->setCurrentWidget(game);
}

void MainWindow::onExitGameClicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "退出游戏", "确定要退出游戏吗?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void MainWindow::showMainMenu() {
    stackedWidget->setCurrentWidget(ui->centralWidget);
}
