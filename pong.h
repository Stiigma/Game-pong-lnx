#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define ScreenX 1024
#define ScreenY 640
#define TENNIS_GREEN \
    (Color) { 108, 147, 92, 255 }

typedef struct ball_s
{
    Vector2 pos;
    Vector2 speed;
    int radius;
    Color color;
    bool out;
    bool back;
} ball_t;

typedef struct player_s
{

    Vector2 pos;
    Vector2 dimen;
    Color color;
    bool plyr;
    int pts;
} player_t;

player_t initplayer(bool section);
ball_t *initBall();
void Draw(ball_t *ball, player_t *player1, player_t *player2);
void DrawSolo(ball_t *ball, player_t *player1);
void initplayers(player_t *player1, player_t *player2);
int PauseMenu();

player_t initplayer(bool section)
{
    player_t player;
    player.plyr = section;
    player.dimen = {20, 125};
    if (!section)
    {
        player.pos = {20, ScreenY / 2};
        player.color = YELLOW;
        player.plyr = 0;
    }
    else
    {
        player.pos = {ScreenX - player.dimen.x - 20, ScreenY / 2};
        player.color = BLUE;
        player.plyr = 1;
    }
    player.pts = 0;
    return player;
}

ball_t *initBall()
{
    ball_t *ball = (ball_t *)malloc(sizeof(ball_t));
    float angle = GetRandomValue(0, 60) * (PI / 180);
    float speedMagnitude = 4;

    Vector2 _speed = {speedMagnitude * cos(angle), speedMagnitude * sin(angle)};

    ball->pos = (Vector2){ScreenX / 2, ScreenY / 2};
    ball->speed = _speed;
    ball->color = RED;
    ball->radius = 15;
    ball->out = 0;
    ball->back = 0;

    return ball;
}

void Draw(ball_t *ball, player_t *player1, player_t *player2)
{

    char *pts1 = (char *)malloc(12 * sizeof(char));
    char *pts2 = (char *)malloc(12 * sizeof(char));

    sprintf(pts1, "%d", player1->pts);
    sprintf(pts2, "%d", player2->pts);

    //DrawLine(ScreenX / 2, 0, ScreenX / 2, ScreenY, Color{255, 255, 255, 120});
    DrawCircle(ScreenX / 2, ScreenY / 2 + 50, 100, Color{255, 255, 255, 120});
    DrawLineBezier({ScreenX / 2,  120}, {ScreenX / 2, ScreenY - 10}, 4.5, Color{255, 255, 255, 200});
    DrawCircleV((ball->pos), ball->radius, ball->color);
    DrawRectangleV(player1->pos, player1->dimen, player1->color);
    DrawRectangleV(player2->pos, player2->dimen, player2->color);
    DrawLineBezier({10, 120}, {10, ScreenY - 10}, 6.5, Color{255, 255, 255, 200});
    DrawLineBezier({6, 120}, {ScreenX - 10, 120}, 6.5, Color{255, 255, 255, 200});
    DrawLineBezier({ScreenX - 10, 120}, {ScreenX - 10, ScreenY - 10}, 6.5, Color{255, 255, 255, 200});

    DrawLineBezier({10, ScreenY - 10}, {ScreenX - 10, ScreenY - 10}, 6.5, Color{255, 255, 255, 200});
    DrawText(pts1, ScreenX / 4 - 25, 25, 100, WHITE);
    DrawText(pts2, ScreenX - ScreenX / 4, 25, 100, WHITE);
    ClearBackground(TENNIS_GREEN);
    free(pts1);
    free(pts2);
}

void DrawSolo(ball_t *ball, player_t *player1)
{

    //DrawLine(ScreenX / 2, 0, ScreenX / 2, ScreenY, Color{255, 255, 255, 120});
    DrawCircle(ScreenX / 2, ScreenY / 2 + 50, 100, Color{255, 255, 255, 120});
    DrawLineBezier({ScreenX / 2,  120}, {ScreenX / 2, ScreenY - 10}, 4.5, Color{255, 255, 255, 200});
    DrawCircleV((ball->pos), ball->radius, ball->color);
    DrawRectangleV(player1->pos, player1->dimen, player1->color);
    DrawLineBezier({10, 120}, {10, ScreenY - 10}, 6.5, Color{255, 255, 255, 200});
    DrawLineBezier({6, 120}, {ScreenX - 10, 120}, 6.5, Color{255, 255, 255, 200});
    DrawLineBezier({ScreenX - 10, 120}, {ScreenX - 10, ScreenY - 10}, 6.5, Color{255, 255, 255, 200});

    DrawLineBezier({10, ScreenY - 10}, {ScreenX - 10, ScreenY - 10}, 6.5, Color{255, 255, 255, 200});
    ClearBackground(TENNIS_GREEN);

}

void initplayers(player_t *player1, player_t *player2)
{
    *player1 = initplayer(1);
    *player2 = initplayer(0);
}


int PauseMenu()
{
    int opcion;
    Texture2D fondoMenu = LoadTexture("img/PausaMenu.png");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(fondoMenu,0,0,RAYWHITE);
        if (IsKeyDown(KEY_ONE))
        {
            opcion =  0;  // Seguir jugando
            break;
        }
        if (IsKeyDown(KEY_TWO))
        {
            opcion =  1;  // Salir del juego
            break;
        }

        EndDrawing();
    }

    return opcion;  // En caso de que se cierre la ventana, retorna 1 para salir
}