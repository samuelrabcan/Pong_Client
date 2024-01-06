// Paddle.h

#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

typedef struct Controls {
    int upKey;
    int downKey;
} Controls;

typedef struct Paddle {
    RLRectangle rect;
    Controls controls; // New field for controls
} Paddle;

void InitPaddle(Paddle* paddle, int paddleCenterX, int paddleCenterY, int upKey, int downKey);
void MovePaddle(Paddle* paddle, int distance);
Vector2 GetPaddlePosition(const Paddle* paddle);
RLRectangle GetPaddleRect(const Paddle* paddle);
void SetPaddlePosition(Paddle* paddle, int newY);

#endif // PADDLE_H