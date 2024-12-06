#include "gobangai.h"

#include <algorithm>
#include <cstring>
#include <QPainter>
int GobangAI::CHESS_SIZE = 40;
QPoint GobangAI::START_POS = QPoint(6, 6);
std::vector<std::vector<int>> GobangAI::posValue = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
    {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

std::vector<Pattern> GobangAI::patterns = {
    {"11111", 100000},
    {"011110", 40000},
    {"01111-", 10000},
    {"-11110", 10000},
    {"10111", 10000},
    {"11101", 10000},
    {"11011", 10000},
    {"01110", 8000},
    {"1011", 7000},
    {"1101", 7000},
    {"00111-", 500},
    {"-11100", 500},
    {"01011-", 500},
    {"-11010", 500},
    {"01101-", 500},
    {"-10110", 500},
    {"10011", 500},
    {"11001", 500},
    {"10101", 500},
    {"-01110-", 500},
    {"001100", 50},
    {"01010", 50},
    {"1001", 50},
    {"00011-", 10},
    {"-11000", 10},
    {"00101-", 10},
    {"-10100", 10},
    {"01001-", 10},
    {"-10010", 10},
    {"10001", 10},
};


std::vector<std::pair<int, int> > GobangAI::fourDirections = {
    {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
};
std::vector<std::pair<int, int> > GobangAI::eightDirections = {
    {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
    {1, 0}, {1, -1}, {0, -1}, {-1, -1}
};
GobangAI::GobangAI(QWidget *parent) : QDialog(parent){
    acs = new AhoCorasick(patterns);
    QSize size = parent->size();
    int currentWidth = size.width() / 2;
    int currentHeight = size.height() / 2;
    this->setFixedSize(currentWidth * 0.8, currentHeight);
    int minOne = (currentWidth >= currentHeight) ? currentHeight : currentWidth;
    double halfblankPercentInHeight = 0.05;
    START_POS = QPoint( halfblankPercentInHeight * currentWidth, halfblankPercentInHeight * currentHeight);
    CHESS_SIZE = ((1 - halfblankPercentInHeight * 2) * minOne) / 14;
    setWindowIcon(QIcon(":/image/images/icon.png"));
    setWindowTitle("运行过程");
}
void GobangAI::drawChessBoard(QPainter& painter) {
    painter.setPen(QPen(QColor(Qt::black), 2));
    painter.setBrush(Qt::white);

    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 14; j++) {
            painter.drawRect(i * CHESS_SIZE + START_POS.x(),
                             j * CHESS_SIZE + START_POS.y(),
                             CHESS_SIZE, CHESS_SIZE);
        }
    }
}

void GobangAI::drawChessItem(QPainter& painter) {
    painter.setPen(QPen(QColor(Qt::black), 1));
    for (int i = 0; i < GRID_NUM; ++i) {
        for (int j = 0; j < GRID_NUM; ++j) {
            switch (localBoard.get(i, j)) {
            case BLACK: {
                painter.setBrush(Qt::black);
                drawChessAtPoint(painter, j, i);
                break;
            }
            case WHITE: {
                painter.setBrush(Qt::white);
                drawChessAtPoint(painter, j, i);
                break;
            }
            default: break;
            }
        }
    }
}

void GobangAI::drawChessAtPoint(QPainter& painter, int x, int y) {
    const QPoint pt(x * CHESS_SIZE + START_POS.x(),
                    y * CHESS_SIZE + START_POS.y());
    painter.drawEllipse(pt, CHESS_SIZE / 4, CHESS_SIZE / 4);
}
void GobangAI::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    drawChessBoard(painter);
    drawChessItem(painter);
    drawScore(painter);
}
void GobangAI::drawScore(QPainter& painter){
    // 设置绘图属性
    painter.setPen(Qt::black);
    painter.setFont(QFont("楷体", 10, QFont::Bold));
    // 绘制指定区域的数字
    int chessSize = START_POS.x() * 2 + CHESS_SIZE * 14;
    int areaWidth = (this -> size().width() - chessSize);
    int areaHeight = this -> size().height();
    QRect targetArea(chessSize, areaHeight * 0.45 , areaWidth * 0.8, areaHeight * 0.1);
    painter.drawText(targetArea, Qt::AlignCenter, QString::number(currentScore));
}
GobangAI::~GobangAI() {
    delete acs;
}

Point GobangAI::getStep(Board& state, bool isBlack, int depthmax) {
    localBoard = state;
    Point p = alphaBetaPlay(localBoard, isBlack, 0, -INF, INF, depthmax);
    p.isBlack = aiIsBlack;
    localBoard.push_back(p);
    repaint();
    return p;
}

int GobangAI::evluateState(Board& state) {
    int pointBLK = 0, pointWIT = 0;
    for (const auto& point : state.steps) {
        if (point.isBlack) {
            pointBLK += getScore(state, point.x, point.y, true);
        } else {
            pointWIT += getScore(state, point.x, point.y, false);
        }
    }
    return aiIsBlack ? pointBLK- pointWIT : pointWIT - pointBLK;
}


int GobangAI::getScoreInDir(Board& state, int x, int y, bool isBlack, const std::pair<int,int>& dir) {
    Chess color = isBlack ? BLACK : WHITE;
    std::string s = std::move(state.getChessStringInDir(x,y,color,dir));
    std::vector<int> matches = acs->search(s);
    int score = 0;
    for (int i : matches) {
        score += i;
    }
    return score;
}

int GobangAI::getScore(Board& state, int x, int y, bool isBlack) {
    int score = 0;
    const int weight = posValue[x][y];
    for (const auto & dir :fourDirections) {
        score += getScoreInDir(state, x, y, isBlack, dir);
    }
    return weight * score;
}


Point GobangAI::alphaBetaPlay(Board& state, bool isBlack, int depth, int alpha, int beta, int maxdepth) {
    if (depth == maxdepth) {
        Point nextpt = state.steps.back();
        nextpt.score = evluateState(state);
        currentScore = nextpt.score;
        repaint();
        return nextpt;
    }
    auto nextPossibleSteps = std::move(state.getNeighbours(isBlack));
    Point maxV = {-1, -1, isBlack, -INF}, minV = {-1, -1, isBlack, INF};
    for (auto tpstep : nextPossibleSteps) {
        state.push_back(tpstep);
        repaint();
        Point move = alphaBetaPlay(state, !isBlack, depth + 1, alpha, beta, maxdepth);
        if (depth % 2 == 0) {
            if (move.score >= maxV.score) {
                maxV = move;
                maxV.x = tpstep.x;
                maxV.y = tpstep.y;
            }
            alpha = std::max(alpha, maxV.score);
        } else {
            if (move.score <= minV.score) {
                minV.score = move.score;
                minV.x = move.x;
                minV.y = move.y;
            }
            beta = std::min(beta, minV.score);
        }
        state.pop_back();
        repaint();
        if (alpha >= beta) break;
    }
    if(depth % 2 == 0)
        return maxV;
    return minV;
}
