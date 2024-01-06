#include "Paddle.h"

void InitPaddle(Paddle* paddle, int paddleCenterX, int paddleCenterY, int upKey, int downKey) {
    paddle->rect = (RLRectangle){paddleCenterX, paddleCenterY, 20, 140}; // Change paddle size
    paddle->controls.upKey = upKey;
    paddle->controls.downKey = downKey;
}

void MovePaddle(Paddle* paddle, int distance) {
    paddle->rect.y += distance;

    if (paddle->rect.y < 0) {
        paddle->rect.y = 0;
    }
    if (paddle->rect.y > GetScreenHeight() - paddle->rect.height) {
        paddle->rect.y = GetScreenHeight() - paddle->rect.height;
    }
}

Vector2 GetPaddlePosition(const Paddle* paddle) {
    return (Vector2){paddle->rect.x, paddle->rect.y};
}

RLRectangle GetPaddleRect(const Paddle* paddle) {
    return paddle->rect;
}

void SetPaddlePosition(Paddle* paddle, int newY) {
    paddle->rect.y = newY;
}