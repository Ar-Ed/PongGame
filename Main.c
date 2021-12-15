#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "winmm.lib")

typedef struct ball {
    Vector2 pos;
    Vector2 velocity;
    float radius;
} Ball;

typedef struct player {
    Rectangle rect;
    float speed;
    int score;
    Color color;

} Player;

void DrawSeperationLine(int screenWidth, int screenHeight, int seperation, int segmentHeight, int thickness);
void UpdateBallPos(Ball *ball, Vector2 ballVelocity, float deltaTime);
void ResetRound(Ball* ball, Player* player1, Player* player2);
void ResizeObjects(Ball* ball, Player* player1, Player* player2);

int screenWidth = 800;
int screenHeight = 450;

int main(void)
{
    const int fps = 60;

    Image icon = LoadImage("./dependencies/src/img/ping-pong.png");

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "The Mighty Pong Game");
    SetWindowIcon(icon);

    UnloadImage(icon);

    SetTargetFPS(fps);          


    InitAudioDevice();
    Wave colWav = LoadWave("./dependencies/src/sound/collision.wav");
    Wave loseWav = LoadWave("./dependencies/src/sound/lose.wav");
    Sound lose = LoadSoundFromWave(loseWav);
    Sound collision = LoadSoundFromWave(colWav);
    UnloadWave(colWav);
    UnloadWave(loseWav);



    Ball firstBall = { 50, 50, screenWidth / 10 , -screenHeight / 5, screenWidth / 60};

    Player player1 = { screenWidth / 20, screenHeight / 2, screenWidth / 100, screenHeight / 8,  screenHeight / 5, 0, BLACK};
    Player player2 = { screenWidth - screenWidth / 20, screenHeight / 2, screenWidth / 100, screenHeight / 8, screenHeight / 5, 0, BLACK };

    while (!WindowShouldClose())   
    {
        // update if resized
        if (IsWindowResized()) {
            ResizeObjects(&firstBall, &player1, &player2);
        }

        // update ball position
        UpdateBallPos(&firstBall, firstBall.velocity, 1. / fps);

        // update the ball velocity if collision with borders
        if (firstBall.pos.y > screenHeight - firstBall.radius || firstBall.pos.y < firstBall.radius) {
            firstBall.velocity.y = -firstBall.velocity.y;
            PlaySound(collision);
        }

        // check for end of round
        if (firstBall.pos.x > screenWidth - firstBall.radius) {
            player2.score += 1;
            ResetRound(&firstBall ,&player1, &player2);
            PlaySound(lose);
        }
        else if(firstBall.pos.x < firstBall.radius){
            player1.score += 1;
            ResetRound(&firstBall, &player1, &player2);
            PlaySound(lose);
        }

        // player move
        if (IsKeyDown(KEY_UP)) {
            player2.rect.y -= player1.speed / fps;
        }
        else if (IsKeyDown(KEY_DOWN)) {
            player2.rect.y += player1.speed / fps;
        }
        if (IsKeyDown(KEY_W)) {
            player1.rect.y -= player2.speed / fps;
        }
        else if (IsKeyDown(KEY_S)) {
            player1.rect.y += player2.speed / fps;
        }


        // Collision players and ball
        if (CheckCollisionCircleRec(firstBall.pos, firstBall.radius, player1.rect) || CheckCollisionCircleRec(firstBall.pos, firstBall.radius, player2.rect)) {
            firstBall.velocity.x = -firstBall.velocity.x;
            PlaySound(collision);
        }


        // Drawing
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawSeperationLine(screenWidth, screenHeight, screenWidth / 50, screenWidth / 18, screenWidth / 160);

        // convert scores to string
        char playerScore1[10], playerScore2[10];
        sprintf_s(playerScore1, 10, "%d", player1.score);
        sprintf_s(playerScore2, 10, "%d", player2.score);

        // Draw Scores
        DrawText(playerScore1,  3* screenWidth / 4, screenHeight / 2, screenWidth / 15, LIGHTGRAY);
        DrawText(playerScore2, screenWidth / 4, screenHeight / 2, screenWidth / 15, LIGHTGRAY);

        // Draw Ball
        DrawCircleGradient(firstBall.pos.x, firstBall.pos.y, firstBall.radius, YELLOW, RED);

        // Draw Players
        DrawRectangle(player1.rect.x, player1.rect.y, player1.rect.width, player1.rect.height, BLACK);
        DrawRectangle(player2.rect.x, player2.rect.y, player2.rect.width, player2.rect.height, BLACK);


        EndDrawing();
    }


    CloseWindow();        


    return 0;
}

void ResizeObjects(Ball* ball, Player* player1, Player* player2) {
    screenHeight = GetScreenHeight();
    screenWidth = GetScreenWidth();
    ball->radius = screenWidth / 60;
    ball->velocity.x = screenWidth / 10;
    ball->velocity.y = -screenHeight / 5;

    player1->rect.x = screenWidth / 20;
    player1->rect.y = screenHeight / 2;
    player1->rect.x = screenWidth / 100;
    player1->rect.y = screenHeight / 8;
    player1->speed = screenHeight / 5;

    player2->rect.x = screenWidth - screenWidth / 20;
    player2->rect.y = screenHeight / 2;
    player2->rect.x = screenWidth / 100;
    player2->rect.y = screenHeight / 8;
    player2->speed = screenHeight / 5;
}

void ResetRound(Ball* ball, Player *player1, Player* player2) {
    ball->pos.x = 50;
    ball->pos.y = 50;
    ball->velocity.x = screenWidth / 10;
    ball->velocity.y = -screenHeight / 5;
    ball->radius = screenWidth / 60;

    player1->rect.x = screenWidth / 20;
    player1->rect.y = screenHeight / 2;
    player1->speed = screenHeight / 5;

    player2->rect.x = screenWidth - screenWidth / 20;
    player2->rect.y = screenHeight / 2;
    player2->speed = screenHeight / 5;
}

void DrawSeperationLine(int screenWidth, int screenHeight, int seperation, int segmentHeight, int thickness) {
    Vector2 start, end;
    for (int i = 0; i < screenHeight; i += seperation + segmentHeight) {
        start.x = screenWidth / 2; start.y = i;
        end.x = screenWidth / 2; end.y = i + segmentHeight;
        DrawLineEx(start, end, thickness, LIGHTGRAY);
    }
}

void UpdateBallPos(Ball *ball, Vector2 ballVelocity, float deltaTime) {
    ball->pos.x += ballVelocity.x * deltaTime;
    ball->pos.y += ballVelocity.y * deltaTime;
}