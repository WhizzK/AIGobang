#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMainWindow>
#include "gobangai.h"
#include "board.h"
#include <QStackedWidget>

namespace Ui {
    class GameBoard;
}
class GameBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoard(QMainWindow *parent = nullptr,int gameMode = 0);
    void setGameMode(int gameMode);
    void setStackedWidget(QStackedWidget* stackedWidget);
    ~GameBoard() override;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    auto event(QEvent *event) -> bool override;
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void onRestartClicked();
    void onRepentanceClicked();
    void onGiveUpClicked();
    void onRestartNewGameClicked();
    void backToMainMenu();
private:
    void initGame(int gameMode);

    void drawChessBoard(QPainter& painter);
    void drawTrackPoint(QPainter& painter) const;
    void drawChessItem(QPainter& painter) const;//绘制棋盘上的棋子
    static void drawChessAtPoint(QPainter &painter , int x , int y); //画棋子的样式和位置
    void aiTerm(Board& board);
    std::pair<int,int> getIndex(const QPoint& point);
private:
    static int CHESS_SIZE;
    static QPoint START_POS;
private:
    Ui::GameBoard *ui;
    QMainWindow * menu;
    QStackedWidget *stackedWidget;
    GobangAI *ai;
    Board board;
    int gameMode;
    bool isBlackTerm;
    QPoint mousePoint;
};

#endif // GAMEBOARD_H
