// Paddle.h

#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"


typedef struct Paddle {
    RLRectangle rect;
} Paddle;

void InitPaddle(Paddle* paddle, int paddleCenterX, int paddleCenterY);
void MovePaddle(Paddle* paddle, int distance);
Vector2 GetPaddlePosition(const Paddle* paddle);
RLRectangle GetPaddleRect(const Paddle* paddle);
void SetPaddlePosition(Paddle* paddle, int position);
float CheckPaddlePosition(Paddle* paddle, float position);

#endif // PADDLE_H