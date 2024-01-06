#include "Ball.h"

void InitBall(Ball* ball, Vector2 startCoordinates, int width, int height, int radius) {
    ball->position = startCoordinates;
    ball->velocity = (Vector2){2, 2}; // rychlosť lopty
    ball->radius = radius; // velkosť lopty
    ball->color = BLACK;
    ball->width = width;
    ball->height = height;
}

void MoveBall(Ball* ball, RLRectangle player1Bounds, RLRectangle player2Bounds, int* score1N, int* score2N) {
    ball->position.x += ball->velocity.x;
    ball->position.y += ball->velocity.y;

    if (ball->position.y < 0 || ball->position.y > ball->height - 2*ball->radius) {
        ball->velocity.y *= -1;
    }

    // Check if the ball was hit by player 1
    if (CheckCollisionCircleRec(ball->position, ball->radius, player1Bounds)) {
        ball->velocity.x *= -1.2;
    }

    // Check if the ball was hit by player 2
    if (CheckCollisionCircleRec(ball->position, ball->radius, player2Bounds)) {
        ball->velocity.x *= -1.2;
    }

    // Check if the ball scored
    if (ball->position.x < 0) {
        ball->position = (Vector2){ball->width / 2, ball->height / 2};
        (*score1N)++;
        ball->velocity.x = 2;
        ball->velocity.y = 2;
    }

    if (ball->position.x > ball->width) {
        ball->position = (Vector2){ball->width / 2, ball->height / 2};
        (*score2N)++;
        ball->velocity.x = 2;
        ball->velocity.y = 2;
    }
}

Vector2 GetBallPosition(Ball* ball) {
    return ball->position;
}

void DrawBall(Ball* ball) {
    DrawCircleV(ball->position, ball->radius, ball->color);
}
