#ifndef UNIT_H
#define UNIT_H
struct Vec2{
    int e1;
    int e2;
    int& x(){ return e1;}
    int& y(){ return e2;}
    int& width(){return e1;}
    int& height(){return e2;}
};

using Position = Vec2;

enum Color {BLACK=0, RED, GREEN, YELLOW, BLUE, PINK, CYAN, WHITE, NOCHANGE};

enum Direction {UP=0, RIGHT, DOWN, LEFT};

enum State {WIN=0, LOSE, PLAYING};

#endif
