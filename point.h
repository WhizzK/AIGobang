//
// Created by whizzk on 2024/7/10.
//

#ifndef POINT_H
#define POINT_H
class Point {
public:
    Point();
    Point(int x, int y, bool isBlack, int score = 0);
    bool isBlack;
    int x, y,score;
    void print();
    bool operator<(const Point &other) const;
};



#endif //POINT_H
