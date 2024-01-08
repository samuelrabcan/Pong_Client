// Ball.c


#include "Ball.h"

void InitBall(Ball* ball, int radius) {
    ball->startPos = (Vector2){GetScreenWidth() / 2, GetScreenHeight() / 2};
    ball->position = ball->startPos;
    ball->velocity = (Vector2){3, 3}; // Ball speed
    ball->radius = radius;            // Ball size
    ball->color = BLACK;
    ball->score.player1 = 0;  // Initialize scores
    ball->score.player2 = 0;
}

void MoveBall(Ball* ball, RLRectangle paddle1, RLRectangle paddle2) {
    ball->position.x += ball->velocity.x;
    ball->position.y += ball->velocity.y;
    // Bounce off the top and bottom walls
    BounceBallOffWalls(ball);

    // Check if the ball was hit by players
    CheckIfBallWasHit(ball, 1.1, paddle1);
    CheckIfBallWasHit(ball, 1.1, paddle2);

    // Check if the ball scored
    if (CheckIfPlayer1Scored(ball))
        ball->score.player1++;

    if (CheckIfPlayer2Scored(ball))
        ball->score.player2++;
}

void CheckIfBallWasHit(Ball* ball, double velocityMulti, RLRectangle paddle) {
    if (CheckCollisionCircleRec(ball->position, ball->radius, paddle)) {
        ball->velocity.x = ball->velocity.x * -velocityMulti;
        ball->velocity.y = ball->velocity.y * velocityMulti;
    }
}

void BounceBallOffWalls(Ball* ball) {
    if (ball->position.y < 0 || ball->position.y > GetScreenHeight() - ball->radius) {
        ball->velocity.y *= -1;
    }
}

_Bool CheckIfPlayer1Scored(Ball* ball) {
    if (ball->position.x > GetScreenWidth()) {
        ball->position = ball->startPos;
        ball->velocity = (Vector2){3, 3};
        return true;
    }
    return false;
}

_Bool CheckIfPlayer2Scored(Ball* ball) {
    if (ball->position.x < 0) {
        ball->position = ball->startPos;
        ball->velocity = (Vector2){3, 3};
        return true;
    }
    return false;
}

void DrawBall(const Ball* ball) {
    DrawCircleV(ball->position, ball->radius, ball->color);
}

