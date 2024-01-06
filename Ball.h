#ifndef PONG_SEMPRACA_2_BALL_H
#define PONG_SEMPRACA_2_BALL_H

#include "raylib.h"

typedef struct Ball {
    Vector2 position;
    Vector2 velocity;
    int radius;
    Color color;
    int width;
    int height;
} Ball;

void InitBall(Ball* ball, Vector2 StartCoordinates, int widthInput, int height, int radius);
void MoveBall(Ball* ball, RLRectangle player1Bounds, RLRectangle player2Bounds, int* score1N, int* score2N);
Vector2 GetBallPosition(Ball* ball);
void DrawBall(Ball* ball);

#endif //PONG_SEMPRACA_2_BALL_H
