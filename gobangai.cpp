#include "gobangai.h"

#include <algorithm>
#include <cstring>
#include <iostream>

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
GobangAI::GobangAI() {
    acs = new AhoCorasick(patterns);
}

GobangAI::~GobangAI() {
    delete acs;
}

Point GobangAI::getStep(Board& state, bool isBlack, int depthmax) {
    Board board = state;
    Point p = alphaBetaPlay(board, isBlack, 0, -INF, INF, depthmax);
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
    std::cout<<pointBLK<<" "<<pointWIT <<std::endl;
    return aiIsBlack ? pointBLK- pointWIT : pointWIT - pointBLK;
}


int GobangAI::getScoreInDir(Board& state, int x, int y, bool isBlack, const std::pair<int,int>& dir) {
    Chess color = isBlack ? BLACK : WHITE;
    std::string s = state.getChessStringInDir(x,y,color,dir);
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
        return nextpt;
    }
    auto nextPossibleSteps = state.getNeighbours(isBlack);
    Point maxV = {-1, -1, isBlack, -INF}, minV = {-1, -1, isBlack, INF};
    for (auto tpstep : nextPossibleSteps) {
        state.push_back(tpstep);
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
        if (alpha >= beta) break;
    }
    if(depth % 2 == 0)
        return maxV;
    return minV;
}
