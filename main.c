#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "raylib.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "Ball.h"
#include "Paddle.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
// Replace magic numbers with named constants
#define MAX_SCORE 3
#define MAX_INPUT_CHARS 9
#define TITLE_SIZE 65
#define TEXT_LARGE 40
#define TEXT_MEDIUM 30
#define TEXT_SMALL 20

// Globalne premenné
Paddle paddle1, paddle2;
int hrac1_x, hrac2_x;

// Struktúra pre parametre pre vlákno
typedef struct {
    SOCKET sock;
    Paddle* paddle1;
} ThreadParams;

// Function declarations
void drawMainMenu(int screenWidth, int screenHeight, int titleSize, int textLarge, int textSmall);
void drawGameMenu(int screenWidth, int screenHeight, int textMedium, int textLarge, Color* playerColor, char* name, int* letterCount);
int playGame(SOCKET sock, int screenWidth, int screenHeight, Color playerColor, char* name);
int connectToServer(char* serverIP, int port, SOCKET* sock);
int sendPlayerInfo(SOCKET sock, Color playerColor, char* name);
int receivePlayerInfo(SOCKET sock, Color* playerColor2, char* name2);
int sendPaddlePosition(SOCKET sock, float position);
float receivePaddlePosition(SOCKET sock);
int endClient(int returnCode, SOCKET sock);

DWORD WINAPI handle_paddle_position(LPVOID arg);

DWORD WINAPI handle_paddle_position(LPVOID arg) {
    SOCKET sock = *(SOCKET*)arg;
    while (1) {
        // Sending paddle position to Server
        if (sendPaddlePosition(sock, GetPaddlePosition(&paddle1).y) == SOCKET_ERROR) {
            break;
        }
        // Receiving other players paddle position from server
        float y = receivePaddlePosition(sock);
        if (y == SOCKET_ERROR) {
            break;
        }
        // Setting paddle position
        SetPaddlePosition(&paddle2, y);
    }
    return 0;
}

int currentMenu = 0;// 0: main menu, 1: start game, 2: leaderboard, 3: quit
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Klienta je nutne spustit s nasledujucimi argumentmi: adresa port.\n");
        return 1;
    }
    const int screenWidth = 800;
    const int screenHeight = 600;

    // Text Size Definitions
    int titleSize = 65;
    int textLarge = 40;
    int textMedium = 30;
    int textSmall = 20;

    InitWindow(screenWidth, screenHeight, "Pong Game Menu");

    Color playerColor = RAYWHITE;

    char name[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;

    SetTargetFPS(60);
    SOCKET sock;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (currentMenu == 0) {
            drawMainMenu(screenWidth, screenHeight, titleSize, textLarge, textSmall);
        } else if (currentMenu == 1) {
            drawGameMenu(screenWidth, screenHeight, textMedium, textLarge, &playerColor, name, &letterCount);

            // Check if the user pressed ENTER to start the game
            if (IsKeyPressed(KEY_ENTER)) {
                // Integrate the refactored game logic here
                if (connectToServer(argv[1], atoi(argv[2]), &sock) == 0) {
                    int gameResult = playGame(sock, screenWidth, screenHeight, playerColor, name);
                    return endClient(gameResult, sock);
                }
            }
        } else if (currentMenu == 2) {
            RLCloseWindow();
        }
        EndDrawing();
    }
    RLCloseWindow();

    return 0;
}


int playGame(SOCKET sock, int screenWidth, int screenHeight, Color playerColor, char* name) {
    int gameResult = 0;
    // Receive player information from the server
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    if (recv(sock, buffer, sizeof(buffer) - 1, 0) == SOCKET_ERROR) {
        printf("Chyba - recv.\n");
        return endClient(1, sock);
    }
    buffer[sizeof(buffer) - 1] = '\0';

    // Process player information
    int playerNumber;
    char name2[MAX_INPUT_CHARS];

    Color playerColor2;
    if (strcmp(buffer, "hrac1") == 0) {
        playerNumber = 1;
        hrac1_x = 50;
        hrac2_x = SCREEN_WIDTH - 60;
        if (sendPlayerInfo(sock, playerColor, name) == SOCKET_ERROR) {
            return endClient(1, sock);
        }
        if (receivePlayerInfo(sock, &playerColor2, name2) == SOCKET_ERROR) {
            return endClient(1, sock);
        }
    } else if (strcmp(buffer, "hrac2") == 0) {
        playerNumber = 2;
        hrac1_x = SCREEN_WIDTH - 60;
        hrac2_x = 50;
        if (receivePlayerInfo(sock, &playerColor2, name2) == SOCKET_ERROR) {
            return endClient(1, sock);
        }
        if (sendPlayerInfo(sock, playerColor, name) == SOCKET_ERROR) {
            return endClient(1, sock);
        }
    } else {
        printf("Chyba - vyber hraca.\n");
        return endClient(1, sock);
    }

    // Display the "Waiting for other player!" message

    BeginDrawing();
    RLDrawText("Waiting for other players!", SCREEN_WIDTH / 2 - MeasureText("Waiting for other players!", TEXT_LARGE) / 2, SCREEN_HEIGHT / 2, TEXT_LARGE, RAYWHITE);
    EndDrawing();
    Sleep(2000);

    // Initialize game elements
    Ball ball;
    InitBall(&ball, 20);

    InitPaddle(&paddle1, hrac1_x, SCREEN_HEIGHT / 2 - 25);
    InitPaddle(&paddle2, hrac2_x, SCREEN_HEIGHT / 2 - 25);
    HANDLE paddle_thread;

    paddle_thread = CreateThread(NULL, 0, handle_paddle_position, (void*)&sock, 0, NULL);
    if (paddle_thread == NULL) {
        printf("Error creating thread\n");
        return 1;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (IsKeyDown(KEY_W)) {
            MovePaddle(&paddle1,  -5);
        }
        if (IsKeyDown(KEY_S)) {
            MovePaddle(&paddle1, 5);
        }

        MoveBall(&ball, GetPaddleRect(&paddle1), GetPaddleRect(&paddle2));

        DrawRLRectangleRec(GetPaddleRect(&paddle1), playerColor);
        DrawRLRectangleRec(GetPaddleRect(&paddle2), playerColor2);
        DrawBall(&ball);

        // Draw name and score for each client
        if (playerNumber == 1){
            RLDrawText(TextFormat("%s: %i", name, ball.score.player1), 10, 10, 20, BLACK);
            RLDrawText(TextFormat("%s: %i", name2, ball.score.player2), SCREEN_WIDTH - 110, 10, 20, BLACK);
        } else {
            RLDrawText(TextFormat("%s: %i", name, ball.score.player2), SCREEN_WIDTH - 110, 10, 20, BLACK);
            RLDrawText(TextFormat("%s: %i", name2, ball.score.player1), 10, 10, 20, BLACK);
        }

        // Check for game end conditions
        if (ball.score.player1 >= MAX_SCORE) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            if (playerNumber == 1)
                RLDrawText(TextFormat("%s wins!", name), SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 +30, TEXT_MEDIUM, BLACK);
            if (playerNumber == 2) {
                RLDrawText(TextFormat("%s wins!", name2), SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 30, TEXT_MEDIUM,
                           BLACK);
            }
            EndDrawing();
            Sleep(3000); // Pauza na 3 sekundy
            RLCloseWindow();
            return endClient(0,sock);
        }

        if (ball.score.player2 >= MAX_SCORE) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            if (playerNumber == 1)
                RLDrawText(TextFormat("%s wins!", name2), SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 +30, TEXT_MEDIUM, BLACK);
            if (playerNumber == 2) {
                RLDrawText(TextFormat("%s wins!", name), SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 30, TEXT_MEDIUM,
                           BLACK);
            }
            EndDrawing();
            Sleep(3000); // Pauza na 3 sekundy
            RLCloseWindow();
            return endClient(0,sock);
        }
        EndDrawing();
    }

    WaitForSingleObject(paddle_thread, INFINITE);
    return gameResult;
}

void drawMainMenu(int screenWidth, int screenHeight, int titleSize, int textLarge, int textSmall) {
    RLDrawText("PONG", screenWidth / 2 - MeasureText("PONG", titleSize) / 2, 50, titleSize, RAYWHITE);
    RLDrawText("START", screenWidth / 2 - MeasureText("START", textLarge) / 2, 200, textLarge, RAYWHITE);
    RLDrawText("QUIT", screenWidth / 2 - MeasureText("QUIT", textLarge) / 2, 250, textLarge, RAYWHITE);
    RLDrawText("FULLSCREEN", screenWidth - MeasureText("FULLSCREEN", textSmall) - 10, screenHeight - 30, textSmall, RAYWHITE);
    RLDrawText("by TobakoLT & Crey", screenWidth / 2 - MeasureText("by TobakoLT & Crey", textSmall) / 2, screenHeight - 30, textSmall, RAYWHITE);

    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos, (RLRectangle){screenWidth / 2 - MeasureText("START", textLarge) / 2, 200, MeasureText("START", textLarge), textLarge})) {
            currentMenu = 1;
        } else if (CheckCollisionPointRec(mousePos, (RLRectangle){screenWidth / 2 - MeasureText("QUIT", textLarge) / 2, 300, MeasureText("QUIT", textLarge), textLarge})) {
            currentMenu = 2;
        } else if (CheckCollisionPointRec(mousePos, (RLRectangle){screenWidth - MeasureText("FULLSCREEN", textSmall) - 10, screenHeight - 30, MeasureText("FULLSCREEN", textSmall), textSmall})) {
            ToggleFullscreen();
        }
    }

    Color startColor = CheckCollisionPointRec(mousePos, (RLRectangle){screenWidth / 2 - MeasureText("START", textLarge) / 2, 200, MeasureText("START", textLarge), textLarge}) ? RED : RAYWHITE;
    Color quitColor = CheckCollisionPointRec(mousePos, (RLRectangle){screenWidth / 2 - MeasureText("QUIT", textLarge) / 2, 250, MeasureText("QUIT", textLarge), textLarge}) ? RED : RAYWHITE;

    RLDrawText("START", screenWidth / 2 - MeasureText("START", textLarge) / 2, 200, textLarge, startColor);
    RLDrawText("QUIT", screenWidth / 2 - MeasureText("QUIT", textLarge) / 2, 250, textLarge, quitColor);
}


void drawGameMenu(int screenWidth, int screenHeight, int textMedium, int textLarge, Color* playerColor, char* name, int* letterCount) {
    RLDrawText("Enter your name:", 10, 10, textMedium, RAYWHITE);
    RLDrawText(name, 10, 40, textLarge, *playerColor);

    if (*letterCount < MAX_INPUT_CHARS) {
        int key = GetKeyPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (*letterCount < MAX_INPUT_CHARS)) {
                name[*letterCount] = (char)key;
                (*letterCount)++;
            }
            key = GetKeyPressed();
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        (*letterCount)--;
        if (*letterCount < 0) *letterCount = 0;
        name[*letterCount] = '\0';
    }

    RLDrawText("Choose your color:", 10, 100, textMedium, RAYWHITE);
    DrawRectangle(10, 130, 100, 100, RED);
    DrawRectangle(110, 130, 100, 100, GREEN);
    DrawRectangle(210, 130, 100, 100, BLUE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, (RLRectangle){10, 130, 100, 100})) {
            *playerColor = RED;
        } else if (CheckCollisionPointRec(mousePos, (RLRectangle){110, 130, 100, 100})) {
            *playerColor = GREEN;
        } else if (CheckCollisionPointRec(mousePos, (RLRectangle){210, 130, 100, 100})) {
            *playerColor = BLUE;
        }
    }

    RLDrawText("Back", 10, screenHeight - 30, textMedium, RAYWHITE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, (RLRectangle){10, screenHeight - 30, MeasureText("Back", textMedium), 20})) {
            currentMenu = 0;
        }
    }

    RLDrawText("Press ENTER to start game", 10, 250, textMedium, RAYWHITE);
}


int connectToServer(char* serverIP, int port, SOCKET* sock) {
    // Initializing winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Error - WSAStartup.\n");
        return 1;
    }

    // Creating a socket
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == INVALID_SOCKET) {
        printf("Error - socket.\n");
        WSACleanup();
        return 1;
    }

    // Defining the server address
    struct sockaddr_in serverAddress;
    ZeroMemory(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (InetPton(AF_INET, serverIP, &serverAddress.sin_addr) <= 0) {
        printf("Error - InetPton.\n");
        return endClient(1, *sock);
    }

    // Connecting to the server
    if (connect(*sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("Error - server connect.\n");
        return endClient(1, *sock);
    }

    return 0;
}

int sendData(SOCKET sock, char* message) {
    if (send(sock, message, strlen(message), 0) == SOCKET_ERROR) {
        printf("Chyba - send.\n");
        return SOCKET_ERROR;
    }
    return 0;
}

int receiveData(SOCKET sock, char* buffer, int bufferSize) {
    memset(buffer, 0, bufferSize);
    if (recv(sock, buffer, bufferSize - 1, 0) == SOCKET_ERROR) {
        printf("Chyba - recv.\n");
        return SOCKET_ERROR;
    }
    buffer[bufferSize - 1] = '\0';
    return 0;
}

int sendPlayerInfo(SOCKET sock, Color playerColor, char* name) {
    char message[256];
    sprintf(message, "%d,%d,%d,%d,%s", playerColor.r, playerColor.g, playerColor.b, playerColor.a, name);
    return sendData(sock, message);
}

int receivePlayerInfo(SOCKET sock, Color* playerColor2, char* name2) {
    char buffer[256];
    if (receiveData(sock, buffer, sizeof(buffer)) == SOCKET_ERROR) {
        return SOCKET_ERROR;
    }
    int r, g, b, a;
    sscanf(buffer, "%d,%d,%d,%d,%s", &r, &g, &b, &a, name2);
    playerColor2->r = r;
    playerColor2->g = g;
    playerColor2->b = b;
    playerColor2->a = a;
    return 0;
}

int sendPaddlePosition(SOCKET sock, float position) {
    char message[256];
    sprintf(message, "%f", position);
    return sendData(sock, message);
}

float receivePaddlePosition(SOCKET sock) {
    char buffer[256];
    if (receiveData(sock, buffer, sizeof(buffer)) == SOCKET_ERROR) {
        return SOCKET_ERROR;
    }
    return strtof(buffer, NULL);
}


int endClient(int returnCode, SOCKET sock) {
    send(sock, "END", 3, 0);
    closesocket(sock);
    WSACleanup();
    return returnCode;
}