//
// Created by whizzk on 2024/7/10.
//

#include "point.h"

#include <iostream>

Point::Point() {}

Point::Point(int x, int y, bool isBlack, int score) {
    this->isBlack = isBlack;
    this->x = x;
    this->y = y;
    this->score = score;
}

void Point::print() {
    std::cout << x << ' ' << y << std::endl;
}

bool Point::operator<(const Point &other) const {
    return score < other.score;
}



