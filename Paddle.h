#ifndef PONG_SEMPRACA_2_PADDLE_H
#define PONG_SEMPRACA_2_PADDLE_H

#include "raylib.h"

typedef struct Controls {
    int upKey;
    int downKey;
} Controls;

typedef struct Paddle {
    RLRectangle rect;
    int width;
    int height;
    Controls controls; // New field for controls
} Paddle;

void InitPaddle(Paddle* paddle, int startPositionX, int startPositionY, int widthInput, int heightInput);
void MovePaddle(Paddle* paddle, int offsetX, int offsetY);
Vector2 GetPaddlePosition(Paddle* paddle);
RLRectangle GetPaddleRect(Paddle* paddle);
void SetPaddlePosition(Paddle* paddle, int newX, int newY);

#endif //PONG_SEMPRACA_2_PADDLE_H
