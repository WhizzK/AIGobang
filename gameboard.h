#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include "gobangai.h"
#include "board.h"


namespace Ui {
    class GameBoard;
}
class GameBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoard(QWidget *parent = nullptr,int gameMode = 0);
    void setGameMode(int gameMode);
    ~GameBoard() override;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    auto event(QEvent *event) -> bool override;
signals:
    void backToMainMenu();
private slots:
    void onRestartClicked();
    void onRepentanceClicked();
    void onGiveUpClicked();
    void onRestartNewGameClicked();
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
    static int BUTTON_WIDTH;
    static QPoint START_POS;
private:
    Ui::GameBoard *ui;
    GobangAI *ai;
    Board board;
    int gameMode;
    bool isBlackTerm;
    QPoint mousePoint;
};

#endif // GAMEBOARD_H
