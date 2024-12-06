#ifndef GOBANGAI_H
#define GOBANGAI_H

#include <vector>
#include <QDialog>
#include <queue>
#include "point.h"
#include "ahocorasick.h"
#include "board.h"
constexpr int INF = 0x3f3f3f3f;


class GobangAI : public QDialog {
public:
    GobangAI(QWidget *parent = nullptr);
    ~GobangAI();

    // 获取AI的走法
    Point getStep(Board& state, bool isBlack, int depthmax);
private:
    // Alpha-Beta剪枝的Max-Min博弈树搜索
    Point alphaBetaPlay(Board& state, bool isBlack, int depth, int alpha, int beta, int maxdepth);

    // 局面评估
    int evluateState(Board& state);

    // 获取某点在局面为g某点的8联通方向分数
    int getScore(Board& state, int x, int y, bool isBlack);

    // 获取某点的dir联通方向分数
    int getScoreInDir(Board& state, int x, int y, bool isBlack, const std::pair<int,int>& dir);

    // 获得当前层最优可落点并返回按分值从大到小排序后的节点
    std::priority_queue<Point> getPossibleNextStep(Board& state, bool isBlack);

private:
    bool aiIsBlack = false;
    // 模式匹配
    AhoCorasick* acs;
    // Key-Value分值表
    static std::vector<Pattern> patterns;
    static std::vector<std::pair<int, int>> fourDirections;
    static std::vector<std::pair<int,int>> eightDirections;
    static std::vector<std::vector<int>> posValue;

    //弹窗用于显示α-β计算过程
    Board localBoard;
    int currentScore = 0;
    static int CHESS_SIZE;
    static QPoint START_POS;
    void drawChessBoard(QPainter& painter);
    void drawChessItem(QPainter& painter);
    void drawChessAtPoint(QPainter& painter, int x, int y);
    void drawScore(QPainter& painter);
protected:
    void paintEvent(QPaintEvent* event) override ;
};

#endif // GOBANGAI_H
