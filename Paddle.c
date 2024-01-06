#include "Paddle.h"

void InitPaddle(Paddle* paddle, int startPositionX, int startPositionY, int widthInput, int heightInput,
                int upKey, int downKey) {
    paddle->rect = (RLRectangle){startPositionX, startPositionY, 20, 100}; // Change paddle size
    paddle->width = widthInput;
    paddle->height = heightInput;
}

void MovePaddle(Paddle* paddle, int offsetX, int offsetY) {
    paddle->rect.x += offsetX;
    paddle->rect.y += offsetY;

    if (paddle->rect.y < 0) {
        paddle->rect.y = 0;
    }
    if (paddle->rect.y > paddle->height - paddle->rect.height) {
        paddle->rect.y = paddle->height - paddle->rect.height;
    }
}

Vector2 GetPaddlePosition(Paddle* paddle) {
    return (Vector2){paddle->rect.x, paddle->rect.y};
}

RLRectangle GetPaddleRect(Paddle* paddle) {
    return paddle->rect;
}

void SetPaddlePosition(Paddle* paddle, int newX, int newY) {
    paddle->rect.x = newX;
    paddle->rect.y = newY;
}
