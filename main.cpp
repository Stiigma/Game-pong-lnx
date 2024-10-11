#include <raylib.h>
#include "prueba.h"

int ShowMenu();
void GameSolo();
void Game();
void RandomWay(ball_t *ball, int random);
void CheckWallCollision(ball_t *ball);
int UpdateBallSolo(ball_t **ball, player_t *player);
int UpdateBall(ball_t **ball);
void movPlayer1(player_t *player1);
void movPlayer2(player_t *player2);
void movPlayerUP(player_t *player);
void movPlayerDown(player_t *player);
void CheckColisionBallToPyr(player_t *player, ball_t *ball);
Vector2 Direccion();
void puntos(player_t *player1, player_t *player2, int op);

int main()
{
    InitWindow(ScreenX, ScreenY, "PING MENU");
    int gameMode;

    while (!WindowShouldClose())
    {
        // Muestra el menú y espera una selección de modo
        gameMode = ShowMenu();

        if (gameMode == 1)
        {
            Game();
        }
        else
        {
            if (gameMode == 2)
            {
                GameSolo();
            }
        }
    }

    CloseWindow(); // Cerrar la ventana antes de salir del programa
    return 0;
}

int ShowMenu()
{
    int option = 0;
    Texture2D fondoMenu = LoadTexture("img/FondoMenu.png");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(fondoMenu,0,0,RAYWHITE);
        if (IsKeyPressed(KEY_ONE))
        {
            option = 1; // Juego de 2 jugadores
            break;
        }
        if (IsKeyPressed(KEY_TWO))
        {
            option = 2; // Juego Solitario
            break;
        }

        EndDrawing();
    }

    return option;
}
 
void GameSolo()
{
    ball_t *bola = initBall();
    player_t player1 = initplayer(0);

    bool gameRunning = true; // Bandera para controlar el ciclo del juego

    SetTargetFPS(60);

    while (!WindowShouldClose() && gameRunning)
    {
        // Verifica si se quiere pausar el juego
        if (IsKeyPressed(KEY_P))
        {
            if (PauseMenu() == 1) // Si elige salir, termina el juego
            {
                gameRunning = false; // Forzar salida del ciclo de juego
            }
        }

        movPlayer1(&player1);
        int status = UpdateBallSolo(&bola, &player1);
        if (status == 2)
            player1.pts++;

        if (IsKeyPressed(KEY_F11))
            ToggleFullscreen();

        BeginDrawing();
        DrawSolo(bola, &player1);
        EndDrawing();
    }

    free(bola);
    ClearBackground(RAYWHITE);
}

void Game()
{
    ball_t *bola = initBall();
    player_t player1, player2;
    initplayers(&player1, &player2);

    bool gameRunning = true; // Bandera para controlar el ciclo del juego

    SetTargetFPS(60);

    while (!WindowShouldClose() && gameRunning)
    {
        // Verifica si se quiere pausar el juego
        if (IsKeyPressed(KEY_P))
        {
            if (PauseMenu() == 1) // Si elige salir, termina el juego
            {
                gameRunning = false; // Forzar salida del ciclo de juego
            }
        }

        puntos(&player1, &player2, UpdateBall(&bola));
        movPlayer1(&player1);
        movPlayer2(&player2);
        CheckColisionBallToPyr(&player1, bola);
        CheckColisionBallToPyr(&player2, bola);

        if (IsKeyPressed(KEY_F11))
            ToggleFullscreen();

        BeginDrawing();
        Draw(bola, &player1, &player2);
        EndDrawing();
    }

    free(bola);
    ClearBackground(RAYWHITE);
}

void RandomWay(ball_t *ball, int random)
{
    switch (random)
    {
    case 1:
        ball->speed.x *= -1;
        break;
    case 2:
        ball->speed.y *= -1;
    case 3:
        ball->speed.x *= -1;
        ball->speed.y *= -1;
        break;
    default:
        break;
    }
}

int UpdateBall(ball_t **ball)
{
    if (!((*ball)->out))
    {
        RandomWay(*ball, GetRandomValue(1, 4));
        (*ball)->out = 1;
    }
    (*ball)->pos.x += (*ball)->speed.x;
    (*ball)->pos.y += (*ball)->speed.y;

    if ((*ball)->pos.y + (*ball)->radius >= GetScreenHeight() - 10)
    {
        (*ball)->speed.y *= -1;
    }
    if ((*ball)->pos.y - (*ball)->radius < 120)
    {
        (*ball)->speed.y *= -1;
    }

    if ((*ball)->pos.x + (*ball)->radius >= GetScreenWidth())
    {
        free(*ball);
        *ball = initBall();
        return 1; // player izquierda
    }
    if ((*ball)->pos.x - (*ball)->radius <= 0)
    {
        free(*ball);
        *ball = initBall();
        return 2;
    }
    return 0;
}

void CheckWallCollision(ball_t *ball)
{
    // Si la bola choca con el lado derecho de la pantalla
    if (ball->pos.x + ball->radius >= GetScreenWidth())
    {
        ball->speed.x *= -1;  // Rebotar
        ball->speed.x *= 1.2; // Aumentar la velocidad
        ball->speed.y *= 1.2;
    }
}

int UpdateBallSolo(ball_t **ball, player_t *player)
{
    // Actualiza la posición de la pelota
    (*ball)->pos.x += (*ball)->speed.x;
    (*ball)->pos.y += (*ball)->speed.y;

    // Verifica colisión con las paredes superior e inferior
    if ((*ball)->pos.y + (*ball)->radius >= GetScreenHeight() - 10 || (*ball)->pos.y - (*ball)->radius < 120)
    {
        (*ball)->speed.y *= -1;
    }

    // Verifica colisión con el jugador
    if (CheckCollisionCircleRec((*ball)->pos, (*ball)->radius, (Rectangle){player->pos.x, player->pos.y, player->dimen.x, player->dimen.y}))
    {
        Vector2 newDirection = Direccion();

        // Solo aumentar la velocidad si es menor que 12
        float speedMagnitude = sqrt((*ball)->speed.x * (*ball)->speed.x + (*ball)->speed.y * (*ball)->speed.y);

        if (speedMagnitude < 12.0f)
        {
            (*ball)->speed.x *= 1.2;
            (*ball)->speed.y *= 1.2;
            speedMagnitude = sqrt((*ball)->speed.x * (*ball)->speed.x + (*ball)->speed.y * (*ball)->speed.y); // Recalcular la magnitud
        }

        // Ajusta la velocidad con la nueva dirección sin modificar la magnitud
        (*ball)->speed.x = newDirection.x * speedMagnitude;
        (*ball)->speed.y = newDirection.y * speedMagnitude;

        return 2;
    }

    // Verifica colisión con la pared derecha
    CheckWallCollision(*ball);

    // Verifica colisión con la pared izquierda
    if ((*ball)->pos.x - (*ball)->radius <= 0)
    {
        free(*ball);        // Liberar la pelota actual
        *ball = initBall(); // Inicializar una nueva pelota
        return 1;           // Retorna 1 para indicar que la bola fue reiniciada
    }

    return 0;
}

void movPlayer1(player_t *player1)
{
    if (IsKeyDown(KEY_W))
    {
        movPlayerUP(player1);
    }

    if (IsKeyDown(KEY_S))
    {

        movPlayerDown(player1);
    }
}

void movPlayer2(player_t *player2)
{
    if (IsKeyDown(KEY_UP))
    {
        movPlayerUP(player2);
    }
    if (IsKeyDown(KEY_DOWN))
    {
        movPlayerDown(player2);
    }
}

void movPlayerUP(player_t *player)
{
    if (player->pos.y > 133)
        player->pos.y -= 7;
    else
        player->pos.y = 133;
}

void movPlayerDown(player_t *player)
{
    if (player->pos.y + player->dimen.y < GetScreenHeight() - 20)
        player->pos.y += 7;
    else
        player->pos.y = GetScreenHeight() - player->dimen.y - 20;
}

void CheckColisionBallToPyr(player_t *player, ball_t *ball)
{

    if (CheckCollisionCircleRec(ball->pos, ball->radius, (Rectangle){player->pos.x, player->pos.y, player->dimen.x, player->dimen.y}))
    {
        // Indica que la pelota ha colisionado y debe rebotar
        ball->back = 1;

        Vector2 newDirection = Direccion();
        ball->speed.x *= 1.2;
        ball->speed.y *= 1.2;
        if (player->plyr)
        {
            float speedMagnitude = sqrt(ball->speed.x * ball->speed.x + ball->speed.y * ball->speed.y);
            ball->speed.x = -newDirection.x * speedMagnitude;
            ball->speed.y = -newDirection.y * speedMagnitude;
        }
        else
        {
            // Establece la nueva velocidad de la pelota en la dirección generada
            float speedMagnitude = sqrt(ball->speed.x * ball->speed.x + ball->speed.y * ball->speed.y);
            ball->speed.x = newDirection.x * speedMagnitude;
            ball->speed.y = newDirection.y * speedMagnitude;
        }
    }
}

Vector2 Direccion()
{
    // Define el ángulo de rebote en radianes (60 grados)
    float angle = GetRandomValue(0, 60) * (PI / 180); // Convertir 60 grados a radianes
    // Calcula la nueva dirección en función del ángulo
    Vector2 newDirection = {cos(angle), sin(angle)};

    // Normaliza el nuevo vector de dirección
    float length = sqrt(newDirection.x * newDirection.x + newDirection.y * newDirection.y);
    newDirection.x /= length;
    newDirection.y /= length;

    return newDirection;
}

void puntos(player_t *player1, player_t *player2, int op)
{
    switch (op)
    {
    case 1:
        player1->pts++;
        break;
    case 2:
        player2->pts++;
        break;
    default:
        break;
    }
}
