//
// Created by whizzk on 2024/7/12.
//

#ifndef BOARD_H
#define BOARD_H
#include <string>
#include <vector>

#include "point.h"

enum Chess { NONE, BLACK, WHITE};
enum GameResult { DRAW, BLACK_WIN, WHITE_WIN, IN_PROGRESS };
static constexpr int GRID_NUM = 15;
class Board {
public:
    Board();
    Board(const Board& other);
    Board(Board && other) noexcept;
    Board& operator=(const Board& other);
    int countNearItem(const std::vector<std::vector<Chess> > &board, int row, int col);//判断棋子连接
    GameResult checkGameOver(int x, int y, bool isBlack);
    bool dropChess(int x,int y,Chess chess);
    void clear();
    bool empty() const;
    void repentance();

    Chess get(int i, int j) const;
    std::string getChessStringInDir(int x, int y, Chess color, const std::pair<int, int> & dir);
    std::vector<Point > getNeighbours(bool isBlack);
    void push_back(const Point &point);
    void pop_back();
    void emplace_back(int x,int y, bool isBlack);
    void print();
    static bool isValidPoint(int x, int y);

    std::vector<Point> steps;
private:
    std::vector<std::vector<Chess>> board;

};



#endif //BOARD_H
