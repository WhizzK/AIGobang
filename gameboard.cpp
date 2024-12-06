#include "gameboard.h"
#include <QMessageBox>
#include <csetjmp>
#include "ui_gameboard.h"
int GameBoard::CHESS_SIZE = 40;
QPoint GameBoard::START_POS = QPoint(40,40);
GameBoard::GameBoard(QMainWindow *parent,int gameMode) :
    ui(new Ui::GameBoard)
{
    ui->setupUi(this);
    setWindowTitle("五子棋");
    setWindowIcon(QIcon(":/image/images/icon.png"));
    setAttribute(Qt::WA_Hover); // 启用悬浮事件
    setMouseTracking(true);
    initGame(gameMode);
    menu = parent;
    //根据主界面的大小调整
    this ->resize(menu->size());
    ai = new GobangAI(this);
    ai->setAttribute(Qt::WA_DeleteOnClose, false);
    connect(ui->restartButton, &QPushButton::clicked, this, &GameBoard::onRestartClicked);
    connect(ui->repentanceButton, &QPushButton::clicked,this,&GameBoard::onRepentanceClicked);
    connect(ui->giveUpButton, &QPushButton::clicked, this, &GameBoard::onGiveUpClicked);
    connect(ui->restartNewGameButton, &QPushButton::clicked, this, &GameBoard::onRestartNewGameClicked);
}
void GameBoard::setStackedWidget(QStackedWidget* stackedWidget) {
    this->stackedWidget = stackedWidget;
}
void GameBoard::resizeEvent(QResizeEvent *event) {
    QSize currentSize = event->size();
    int currentWidth = currentSize.width();
    int currentHeight = currentSize.height();
    int minOne = (currentWidth >= currentHeight) ? currentHeight : currentWidth;

    double halfblankPercentInHeight = 0.05;
    START_POS = QPoint( halfblankPercentInHeight * currentWidth, halfblankPercentInHeight * currentHeight);
    CHESS_SIZE = ((1 - halfblankPercentInHeight * 2) * minOne) / 14;
    QWidget *layoutWidget = findChild<QWidget *>("layoutWidget");
    if (layoutWidget) {
        double halfblankPercentInButtonWidth = 0.05;
        double halfblankPercentInButtonHeight = 0.2;
        int chessBoardWidth = START_POS.x() * 2 + CHESS_SIZE * 14;
        int buttonAreaWidth = currentWidth - chessBoardWidth;
        int newX = chessBoardWidth + buttonAreaWidth * halfblankPercentInButtonWidth;
        int newY = halfblankPercentInButtonHeight * currentHeight;
        int newWidth = buttonAreaWidth * (1 - halfblankPercentInButtonWidth * 2);
        int newHeight = currentHeight * (1 - halfblankPercentInButtonHeight * 2);
        layoutWidget->setGeometry(newX, newY, newWidth, newHeight);
        layoutWidget->setContentsMargins(0, 0, 0, 0);
    }
}
GameBoard::~GameBoard()
{
    delete ui;
    delete ai;
}

void GameBoard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawChessBoard(painter);
    drawTrackPoint(painter);
    drawChessItem(painter);
    update();
}
void GameBoard::aiTerm(Board& board) {
    Point aistep = {0, 0, true, -INF};
    //IDA*（迭代加深）优化，从底层开始搜，如果有杀棋则提前返回
    if (!ai->isVisible()) {
        ai->show();
    }
    for (int i = 2; i <= 2; i += 2) {
        repaint();
        auto tmpstep = ai->getStep(board, false, i);
        //更新aistep
        if (tmpstep.score > aistep.score)
            aistep = tmpstep;
         if (aistep.score > 50000 * 2)
             break;
    }
    board.dropChess(aistep.x, aistep.y, isBlackTerm ? BLACK : WHITE);
    repaint();
    switch (board.checkGameOver(aistep.x,aistep.y, isBlackTerm)) {
        case BLACK_WIN: {
            board.clear();
            QMessageBox::information(this, "GAME OVER", "black win", QMessageBox::Yes);
            backToMainMenu();
            return;
        }
        case WHITE_WIN: {
            board.clear();
            QMessageBox::information(this, "GAME OVER", "white win", QMessageBox::Yes);
            backToMainMenu();
            return;
        }
        case DRAW: {
            board.clear();
            QMessageBox::information(this, "GAME OVER", "draw", QMessageBox::Yes);
            backToMainMenu();
            return;
        }
        default: ;
    }
}

std::pair<int,int> GameBoard::getIndex(const QPoint& pt) {
    QPoint point = pt - START_POS;
    point.setX(point.x() + CHESS_SIZE);
    point.setY(point.y() + CHESS_SIZE);
    int x = -1, y = -1;
    if (point.x() % CHESS_SIZE <= CHESS_SIZE / 4) {
        x = point.x() / CHESS_SIZE;
    } else if (point.x() % CHESS_SIZE >= ((CHESS_SIZE * 3) / 4)) {
        x = (point.x() / CHESS_SIZE) + 1;
    } else {
        return {-1, -1};
    }
    if (point.y() % CHESS_SIZE <= CHESS_SIZE / 4) {
        y = point.y() / CHESS_SIZE;
    } else if (point.y() % CHESS_SIZE >= ((CHESS_SIZE * 3) / 4)) {
        y = (point.y() / CHESS_SIZE) + 1;
    } else {
        return {-1, -1};
    }
    if (x == 0 || y== 0) return {-1, -1};
    return {x - 1, y - 1};
}





void GameBoard::mousePressEvent(QMouseEvent *event)
{
    if(gameMode == 2 && !isBlackTerm) return;
    auto [x, y] = getIndex(event->pos());
    if (!board.dropChess(y, x, isBlackTerm ? BLACK : WHITE)) {
        return;
    }
    repaint();
    //统计四个点位是否连接
    switch (board.checkGameOver(y, x, isBlackTerm)) {
        case BLACK_WIN: {
            QMessageBox::information(this, "GAME OVER", "black win", QMessageBox::Yes);
            backToMainMenu();
            return;
        }
        case WHITE_WIN: {
            QMessageBox::information(this, "GAME OVER", "white win", QMessageBox::Yes);
            backToMainMenu();
            return;
        }
        case DRAW: {
            QMessageBox::information(this, "GAME OVER", "draw", QMessageBox::Yes);
            backToMainMenu();
            return;
        }
        default: ;
    }
    //换人下棋
    isBlackTerm = !isBlackTerm;
    if(gameMode == 2) {
        aiTerm(board);
        isBlackTerm = !isBlackTerm;
    }
}


bool GameBoard::event(QEvent *event) {
    if (event->type() == QEvent::HoverEnter || event->type() == QEvent::HoverMove) {
        const auto *hoverEvent = dynamic_cast<QHoverEvent*>(event);
        auto [x, y] = getIndex(hoverEvent->pos());
        if (x == -1 || y == -1) {
            mousePoint = {-1, -1};
        }else {
            mousePoint = {x * CHESS_SIZE, y * CHESS_SIZE};
            mousePoint += START_POS;
        }
        repaint();
    }
    return QWidget::event(event);
}


void GameBoard::setGameMode(int gameMode) {
    this->gameMode = gameMode;
}

void GameBoard::onRestartClicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "重置棋盘",
        "确定要重置棋盘吗?",
        QMessageBox::Yes | QMessageBox::No);
    if(reply != QMessageBox::Yes) return;
    board.clear();
    isBlackTerm = true;
    repaint();
}

void GameBoard::onRepentanceClicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(
    this,
    "撤销上一步",
    "确定要撤销上一步吗?",
    QMessageBox::Yes | QMessageBox::No);
    if(reply != QMessageBox::Yes) return;
    if (board.empty()) {
        QMessageBox::critical(this, "Error", "无可撤销操作！");
        return;
    }
    board.repentance();
    repaint();
}

void GameBoard::onGiveUpClicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(
    this,
    "放弃游戏",
    "确定要放弃游戏吗?",
    QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        board.clear();
        if (isBlackTerm) {
            QMessageBox::information(this, "GAME OVER", "white win", QMessageBox::Yes);
        } else {
            QMessageBox::information(this, "GAME OVER", "black win", QMessageBox::Yes);
        }
        backToMainMenu();
    }
}

void GameBoard::onRestartNewGameClicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(
    this,
    "重新开始新游戏",
    "确定要重新开始新游戏吗?",
    QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        board.clear();
        backToMainMenu();
    }
}

void GameBoard::initGame(int gameMode)
{
    isBlackTerm = true;
    this->gameMode = gameMode;
}
void GameBoard::drawChessBoard(QPainter& painter)
{
    painter.setPen(QPen(QColor(Qt::black), 2));
    painter.setBrush(Qt::white);

    // 棋盘格子
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 14; j++) {
            painter.drawRect(i * CHESS_SIZE + START_POS.x(), j * CHESS_SIZE + START_POS.y(), CHESS_SIZE, CHESS_SIZE);
        }
    }
}

void GameBoard::drawTrackPoint(QPainter& painter) const {
    if (mousePoint.x() == -1 ) return;
    painter.setPen(Qt::red);
    QPoint points[] = {
        mousePoint - QPoint(CHESS_SIZE / 4, CHESS_SIZE / 8),
        mousePoint - QPoint(CHESS_SIZE / 4, CHESS_SIZE / 4),
        mousePoint - QPoint(CHESS_SIZE / 8, CHESS_SIZE / 4),
        mousePoint - QPoint(-CHESS_SIZE / 4, CHESS_SIZE / 8),
        mousePoint - QPoint(-CHESS_SIZE / 4, CHESS_SIZE / 4),
        mousePoint - QPoint(-CHESS_SIZE / 8, CHESS_SIZE / 4),
        mousePoint - QPoint(CHESS_SIZE / 4, -CHESS_SIZE / 8),
        mousePoint - QPoint(CHESS_SIZE / 4, -CHESS_SIZE / 4),
        mousePoint - QPoint(CHESS_SIZE / 8, -CHESS_SIZE / 4),
        mousePoint + QPoint(CHESS_SIZE / 4, CHESS_SIZE / 8),
        mousePoint + QPoint(CHESS_SIZE / 4, CHESS_SIZE / 4),
        mousePoint + QPoint(CHESS_SIZE / 8, CHESS_SIZE / 4),
    };
    painter.drawPolyline(points, 3);
    painter.drawPolyline(points+3, 3);
    painter.drawPolyline(points+6, 3);
    painter.drawPolyline(points+9, 3);
}

void GameBoard::drawChessItem(QPainter& painter) const {
    painter.setPen(QPen(QColor(Qt::black),1));
    //遍历棋子，有就绘制，没有就不绘制
    for (int i = 0; i < GRID_NUM; ++i) {
        for (int j = 0; j< GRID_NUM; ++j) {
            switch (board.get(i, j)) {
                case BLACK: {
                    painter.setBrush(Qt::black);
                    drawChessAtPoint(painter, j , i);
                    break;
                }
                case WHITE: {
                    painter.setBrush(Qt::white);
                    drawChessAtPoint(painter, j , i);
                    break;
                }
                default:;
            }

        }
    }
}

void GameBoard::drawChessAtPoint(QPainter &painter, int x , int y)
{
    const QPoint pt(x * CHESS_SIZE + START_POS.x(),y * CHESS_SIZE + START_POS.y());
    painter.drawEllipse(pt, CHESS_SIZE/4, CHESS_SIZE/4);
}

void GameBoard::backToMainMenu() {
    this->close();
    menu->show();
}
