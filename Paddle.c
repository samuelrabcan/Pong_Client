#include "Paddle.h"

void InitPaddle(Paddle* paddle, int paddleCenterX, int paddleCenterY) {
    paddle->rect = (RLRectangle){paddleCenterX, paddleCenterY, 20, 140}; // Change paddle size
}

void MovePaddle(Paddle* paddle, int distance) {
    int position = paddle->rect.y += distance;
    paddle->rect.y  = CheckPaddlePosition(paddle, position);
}

Vector2 GetPaddlePosition(const Paddle* paddle) {
    return (Vector2){paddle->rect.x, paddle->rect.y};
}

RLRectangle GetPaddleRect(const Paddle* paddle) {
    return paddle->rect;
}

void SetPaddlePosition(Paddle* paddle, int position) {
    paddle->rect.y = position;
}

float CheckPaddlePosition(Paddle* paddle, float position) {
    if (position < 0){
        return 0;
    }
    if (position > GetScreenHeight() - paddle->rect.height) {
        return GetScreenHeight() - paddle->rect.height;
    }
    return position;
}
