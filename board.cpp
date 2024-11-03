//
// Created by whizzk on 2024/7/12.
//

#include "board.h"

#include <iostream>

Board::Board(): board(GRID_NUM), steps(0) {
    for (int i = 0; i < GRID_NUM; ++i) {
        board[i].resize(GRID_NUM);
        for (int j = 0; j < GRID_NUM; ++j) {
            board[i][j] = NONE;
        }
    }
}

Board::Board(const Board &other) {
    steps = other.steps;
    board = other.board;
}


Board::Board(Board &&other) noexcept {
    steps = std::move(other.steps);
    board = std::move(other.board);
}

int Board::countNearItem(const std::vector<std::vector<Chess> > &board, int x, int y) {
    auto chess = board[x][y];
    auto countLine = [&board, chess](int x, int y, int dr, int dc) -> int {
        int count = 0;
        for (int i = 1; i < 5; ++i) {
            if (!isValidPoint(x + dr * i,y + dc * i) ||
                board[x + dr * i][y + dc * i] != chess) {
                break;
                }
            ++count;
        }
        return count;
    };
    int maxCount = 0;
    // 检查四个方向
    maxCount = std::max(maxCount, countLine(x, y, 1, 0) + countLine(x, y, -1, 0) + 1);  // 垂直方向
    maxCount = std::max(maxCount, countLine(x, y, 0, 1) + countLine(x, y, 0, -1) + 1);  // 水平方向
    maxCount = std::max(maxCount, countLine(x, y, 1, 1) + countLine(x, y, -1, -1) + 1);  // 主对角线
    maxCount = std::max(maxCount, countLine(x, y, 1, -1) + countLine(x, y, -1, 1) + 1);  // 副对角线
    return maxCount;
}

GameResult Board::checkGameOver(int x, int y, bool isBlack) {
    bool isDraw = true;
    for (int i = 0; i<GRID_NUM; ++i ) {
        for (int j = 0; j < GRID_NUM; ++j) {
            if (board[i][j] == NONE) {
                isDraw = false;
                break;
            }
        }
    }
    if (isDraw) return DRAW;
    if (countNearItem(board,x, y) == 5) {
        return (isBlack) ? BLACK_WIN : WHITE_WIN;
    }
    return IN_PROGRESS;
}

bool Board::dropChess(int x, int y, Chess chess) {
    if (!isValidPoint(x, y)) return false;
    if (board[x][y] != NONE ) return false;
    steps.emplace_back(x, y, chess == BLACK);
    board[x][y]= chess;
    return true;
}

void Board::clear() {
    steps.clear();
    for (int i = 0; i < GRID_NUM; i++) {
        for (int j = 0; j < GRID_NUM; j++) {
            board[i][j] = NONE;
        }
    }
}

bool Board::empty() const {
    return steps.empty();
}

void Board::repentance() {
    const auto lastStep = steps.back();
    steps.pop_back();
    board[lastStep.x][lastStep.y] = NONE;
}

Chess Board::get(int i, int j) const {
    return board[i][j];
}

Board Board::beforeBoard() {
    Board temp;
    temp.steps = steps;
    temp.board = board;
    temp.repentance();
    return std::move(temp);
}

std::string Board::getChessStringInDir(int x, int y, Chess color, const std::pair<int, int> &dir) {
    std::string s;
    for (int i = -4; i <= 4; i++) {
        int r = x + i * dir.first, c = y + i * dir.second;
        if (isValidPoint(r, c)) {
            if (board[r][c] == color) s += "1";
            else if (board[r][c] == NONE) s += "0";
            else s += "-";
        } else
            s += "#";
    }
    return std::move(s);
}

std::vector<Point> Board::getNeighbours(bool isBlack) {
    auto existNeighbour = [this](int x, int y) -> int {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;
                if (isValidPoint(x + i, y + j) && get(x + i, y + j) != NONE) {
                    return true;
                }
            }
        }
        return false;
    };
    std::vector<Point> neighbours;
    for (int i = 0; i < GRID_NUM; ++i) {
        for (int j = 0; j < GRID_NUM; ++j) {
            if (board[i][j] == NONE && existNeighbour(i, j)) {
                neighbours.emplace_back(i, j, isBlack);
            }
        }
    }
    return std::move(neighbours);
}

void Board::push_back(const Point &point) {
    steps.push_back(point);
    board[point.x][point.y] = point.isBlack ? BLACK : WHITE;
}

void Board::pop_back() {
    auto p = steps.back();
    board[p.x][p.y] = NONE;
    steps.pop_back();
}

void Board::emplace_back(int x, int y, bool isBlack) {
    steps.emplace_back(x, y, isBlack);
    board[x][y] = isBlack ? BLACK : WHITE;
}

void Board::print() {
    for (int i = 0; i < GRID_NUM; i++) {
        for (int j = 0; j < GRID_NUM; j++) {
            std::cout << board[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

bool Board::isValidPoint(int x, int y) {
    return (x >= 0 && x < 15) && (y >= 0 && y < 15);
}
