// Ball.h

#ifndef BALL_H
#define BALL_H

#include "raylib.h"

typedef struct Score {
    int player1;
    int player2;
} Score;

typedef struct Ball {
    Vector2 position;
    Vector2 startPos;
    Vector2 velocity;
    int radius;
    Color color;
    Score score;  // Score structure
} Ball;

void InitBall(Ball* ball, int radius);
void MoveBall(Ball* ball, RLRectangle paddle1, RLRectangle paddle2);
void DrawBall(const Ball* ball);
_Bool CheckIfPlayer1Scored(Ball* ball);
_Bool CheckIfPlayer2Scored(Ball* ball);
void BounceBallOffWalls(Ball* ball);
void CheckIfBallWasHit(Ball* ball, double velocityMulti, RLRectangle paddle);
Vector2 GetBallPosition(Ball* ball);
Vector2 SetBallPosition(Ball* ball, int x, int y) ;

#endif // BALL_H
