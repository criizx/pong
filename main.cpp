#include <iostream>
#include <raylib.h>

using namespace std;

int Flag;
int playerScore = 0;
int aiScore = 0;

class Ball {
public:
    float x, y;
    int speedX, speedY;
    int radius;
    int pauseCounter = 0;

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update() {
        if (pauseCounter > 0) {
            pauseCounter--;
            return;
        }

        x += speedX;
        y += speedY;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speedY *= -1;
        }

        if (Flag == -1 || Flag == 1) {
            x = GetScreenWidth() / 2;
            y = GetScreenHeight() / 2;
            pauseCounter = 144;
            Flag = 0;

            speedX = 0;
            speedY = 0;
        }
    }

    void ResumeMovement(float initialSpeedX, float initialSpeedY) {
        if (pauseCounter == 0 && speedX == 0 && speedY == 0) {
            speedX = initialSpeedX;
            speedY = initialSpeedY;
        }
    }
};

class Paddle {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }
public:
    float x, y;
    int speed;
    float width, height;

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y += speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle {
public:
    void Update(int ballY) {
        if (y + height / 2 > ballY) {
            y -= speed;
        }
        if (y + height / 2 < ballY) {
            y += speed;
        }
        LimitMovement();
    }
};

void addPoints(Ball& ball) {
    if (ball.x >= GetScreenWidth()) {
        aiScore += 1;
        Flag = 1;
    }
    else if (ball.x <= 0) {
        playerScore += 1;
        Flag = -1;
    }
}

Ball ball;
Paddle player;
CpuPaddle ai;

int main() {
    cout << "starting the game" << endl;
    const int screenHeight = 1030, screenWidth = 1920;
    InitWindow(screenWidth, screenHeight, "pong ping");
    SetTargetFPS(144);

    ball.radius = screenHeight / 54;
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.speedX = screenWidth / 300;
    ball.speedY = ball.speedX;

    player.width = GetScreenHeight() / 54 * 1.2;
    player.height = GetScreenHeight() / 5;
    player.x = (player.width + screenWidth * 0.01) / 2;
    player.y = screenHeight / 2 - player.height / 2;
    player.speed = GetScreenHeight() / 216;

    ai.width = GetScreenHeight() / 54 * 1.2;
    ai.height = GetScreenHeight() / 5;
    ai.x = GetScreenWidth() - (player.width + screenWidth * 0.01) / 2;
    ai.y = screenHeight / 2 - player.height / 2;
    ai.speed = GetScreenHeight() / 216;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        ball.Update();
        player.Update();
        ai.Update(ball.y);

        addPoints(ball);
        ball.ResumeMovement(screenWidth / 300, screenWidth / 300);

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speedX *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{ai.x, ai.y, ai.width, ai.height})) {
            ball.speedX *= -1;
        }

        ball.Draw();
        ai.Draw();
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
        player.Draw();

        const int fontSize = 120;
        DrawText(TextFormat("%d", aiScore), screenWidth / 4 - MeasureText(TextFormat("%d", aiScore), fontSize) / 2, screenHeight * 0.1, fontSize, WHITE);
        DrawText(TextFormat("%d", playerScore), 3 * screenWidth / 4 - MeasureText(TextFormat("%d", playerScore), fontSize) / 2, screenHeight * 0.1, fontSize, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
