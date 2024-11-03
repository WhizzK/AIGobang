#include <QApplication>
#include <QFile>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QTextStream>
#include "mainwindow.h"
void applyStylesheet(QApplication &app) {
    QFile file(":/qss/styles/main.qss"); // 从资源中加载 QSS 文件
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        app.setStyleSheet(styleSheet); // 应用样式到整个应用程序
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    applyStylesheet(app);
    // 加载主窗口样式表
    MainWindow w;
    w.show();
    return app.exec();
}
