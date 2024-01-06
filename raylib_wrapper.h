/*//
// Created by luxer on 3. 1. 2024.
//

#ifndef PONG_SEMPRACA_2_RAYLIB_WRAPPER_H
#define PONG_SEMPRACA_2_RAYLIB_WRAPPER_H

// raylib_wrapper.h
#include "raylib.h"

#define RLDrawText Raylib_RLDrawText
#define RLRectangle Raylib_RLRectangle
#define RLCloseWindow Raylib_RLCloseWindow
#define RLShowCursor Raylib_RLShowCursor
#define RLLoadIMage Raylib_RLLoadIMage


typedef struct RLRectangle Raylib_RLRectangle;

void Raylib_RLDrawText(const char *text, int posX, int posY, int fontSize, Color color) {
    RLDrawText(text, posX, posY, fontSize, color);
}

void Raylib_RLCloseWindow(void) {
    RLCloseWindow();
}

void Raylib_RLShowCursor(void) {
    RLShowCursor();
}

Image Raylib_RLLoadIMage(const char *fileName) {
    return RLLoadIMage(fileName);
}


#endif //PONG_SEMPRACA_2_RAYLIB_WRAPPER_H
*/