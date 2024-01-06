#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "raylib.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>


#include "Ball.h"
#include "Paddle.h"

//#include "raylib_wrapper.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_SCORE 5

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Klienta je nutne spustit s nasledujucimi argumentmi: adresa port.\n");
        return 1;
    }

    // ziskanie adresy a portu servera
    char *serverIP = argv[1];
    int port = atoi(argv[2]);
    if (port <= 0) {
        printf("Port musi byt cele cislo vacsie ako 0.\n");
        return 1;
    }

    // inicializacia winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Chyba - WSAStartup.\n");
        return 1;
    }

    // vytvorenie socketu
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Chyba - socket.\n");
        WSACleanup();
        return 1;
    }
    //
    int hrac1_x, hrac2_x;
    if (strcmp(argv[3], "hrac1") == 0) {
        hrac1_x = 50;
        hrac2_x = SCREEN_WIDTH - 60;
    } else {
        hrac1_x = SCREEN_WIDTH - 60;
        hrac2_x = 50;
    }

    // definovanie adresy servera
    struct sockaddr_in serverAddress;
    ZeroMemory(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (InetPton(AF_INET, serverIP, &serverAddress.sin_addr) <= 0) {
        printf("Chyba - InetPton.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // pripojenie na server
    if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("Chyba - server connect.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");

    Ball ball;
    InitBall(&ball, (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, SCREEN_WIDTH, SCREEN_HEIGHT, 20);

    Paddle paddle1, paddle2;
    InitPaddle(&paddle1, hrac1_x, SCREEN_HEIGHT / 2 - 25, SCREEN_WIDTH, SCREEN_HEIGHT);
    InitPaddle(&paddle2, hrac2_x, SCREEN_HEIGHT / 2 - 25, SCREEN_WIDTH, SCREEN_HEIGHT);

    int score1 = 0, score2 = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (IsKeyDown(KEY_W)) {
            MovePaddle(&paddle1, 0, -5);
        }
        if (IsKeyDown(KEY_S)) {
            MovePaddle(&paddle1, 0, 5);
        }

        MoveBall(&ball, GetPaddleRect(&paddle1), GetPaddleRect(&paddle2), &score1, &score2);

        DrawRLRectangleRec(GetPaddleRect(&paddle1), BLACK);
        DrawRLRectangleRec(GetPaddleRect(&paddle2), BLACK);
        DrawBall(&ball);

        RLDrawText(TextFormat("Player 1: %i", score1), 10, 10, 20, BLACK);
        RLDrawText(TextFormat("Player 2: %i", score2), SCREEN_WIDTH - 110, 10, 20, BLACK);

        if (score1 >= MAX_SCORE) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            RLDrawText("Player 1 wins!", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 20, BLACK);
            EndDrawing();
            Sleep(3000); // Pauza na 3 sekundy
            RLCloseWindow();
            return 0;
        }

        if (score2 >= MAX_SCORE) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            RLDrawText("Player 2 wins!", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 20, BLACK);
            EndDrawing();
            Sleep(3000); // Pauza na 3 sekundy
            RLCloseWindow();
            return 0;
        }

        // odoslanie pozicie pádla na server
        char message[256];
        sprintf(message, "%f",paddle1.rect.y);
        if (send(sock, message, strlen(message), 0) == SOCKET_ERROR) {
            printf("Chyba - send.\n");
            break;
        }

        // prijatie pozicie druhého pádla zo servera
        char buffer[256];
        if (recv(sock, buffer, sizeof(buffer) - 1, 0) == SOCKET_ERROR) {
            printf("Chyba - recv.\n");
            break;
        }
        buffer[sizeof(buffer) - 1] = '\0';

        // aktualizacia pozicie druhého pádla
        float y;
        y = strtof(buffer, NULL);
        SetPaddlePosition(&paddle2, hrac2_x, y);


        EndDrawing();
    }

    RLCloseWindow();

    closesocket(sock);
    WSACleanup();

    return 0;
}