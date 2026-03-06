#include <raylib.h>
#include <stdio.h>
#include <math.h>

#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 600
#define BALL_SPEED 170
#define BALL_SIZE 20
#define PLAYER_SPEED 300
#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 70
#define BALL_ACCELERATION 10
#define INNER_PADDING 10

float f_clamp(float x, float min, float max) {
    const float r = x < min ? min : x;
    return r > max ? max : r;
}

int main(void)
{
    InitWindow(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Pong");
    InitAudioDevice();
    SetTargetFPS(120);

    Sound sfx_hit = LoadSound("sounds/hit.wav");
    Sound sfx_score = LoadSound("sounds/score.wav");

    int p1_score = 0;
    int p2_score = 0;
    Vector2 p1_pos = { INNER_PADDING,  DISPLAY_HEIGHT/2 };
    Vector2 p2_pos = { DISPLAY_WIDTH - PLAYER_WIDTH - INNER_PADDING, DISPLAY_HEIGHT/2 };
    Vector2 ball_pos = { DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2 };
    Vector2 ball_dir = { 1, 1 };
    float t = 0.0f;

    int target = 1;
    bool reset = false;
    bool paused = false;
    while (!WindowShouldClose())
    {
        if (reset) {
            p1_pos.x = INNER_PADDING;
            p1_pos.y = DISPLAY_HEIGHT/2;
            p2_pos.x = DISPLAY_WIDTH - PLAYER_WIDTH - INNER_PADDING;
            p2_pos.y = DISPLAY_HEIGHT/2;
            ball_pos.x = DISPLAY_WIDTH/2;
            ball_pos.y = DISPLAY_HEIGHT/2;
            ball_dir.x = -ball_dir.x;
            t = 0.0f;
            reset = false;
            target = ball_dir.x;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        float dt = GetFrameTime();
        if (IsKeyDown(KEY_W)) p1_pos.y    = f_clamp(p1_pos.y - PLAYER_SPEED * dt, INNER_PADDING, DISPLAY_HEIGHT - PLAYER_HEIGHT - INNER_PADDING);
        if (IsKeyDown(KEY_S)) p1_pos.y    = f_clamp(p1_pos.y + PLAYER_SPEED * dt, INNER_PADDING, DISPLAY_HEIGHT - PLAYER_HEIGHT - INNER_PADDING);
        if (IsKeyDown(KEY_UP)) p2_pos.y   = f_clamp(p2_pos.y - PLAYER_SPEED * dt, INNER_PADDING, DISPLAY_HEIGHT - PLAYER_HEIGHT - INNER_PADDING);
        if (IsKeyDown(KEY_DOWN)) p2_pos.y = f_clamp(p2_pos.y + PLAYER_SPEED * dt, INNER_PADDING, DISPLAY_HEIGHT - PLAYER_HEIGHT - INNER_PADDING);
        
        t += dt;
        ball_pos.x += (BALL_SPEED + (BALL_ACCELERATION * t)) * dt * ball_dir.x;
        ball_pos.y += (BALL_SPEED + (BALL_ACCELERATION * t)) * dt * ball_dir.y;

        if (ball_pos.y >= (DISPLAY_HEIGHT - BALL_SIZE)) ball_dir.y = -1;
        if (ball_pos.y <= 0) ball_dir.y = 1;
        
        // Left side (player1) detection
        int lAnchorX = p1_pos.x;
        if (
            (ball_pos.x >= lAnchorX) &&
            (ball_pos.x <= lAnchorX + BALL_SIZE) &&
            (ball_pos.y >= p1_pos.y - BALL_SIZE) &&
            (ball_pos.y <= p1_pos.y + PLAYER_HEIGHT + BALL_SIZE) &&
            target < 0
        ) {
            ball_dir.x = 1;
            PlaySound(sfx_hit);
            target = 1;
        }
        // Right side (player2) detection
        int rAnchorX = p2_pos.x;
        if (
            (ball_pos.x >= rAnchorX - BALL_SIZE) &&
            (ball_pos.x <= rAnchorX) &&
            (ball_pos.y >= p2_pos.y - BALL_SIZE) &&
            (ball_pos.y <= p2_pos.y + PLAYER_HEIGHT + BALL_SIZE) &&
            target > 0
        ) {
            ball_dir.x = -1;
            PlaySound(sfx_hit);
            target = -1;
        }

        // Score checks
        if (ball_pos.x >= DISPLAY_WIDTH + (BALL_SIZE/2)) {
            p1_score += 1;
            reset = true;
            PlaySound(sfx_score);
        } else if (ball_pos.x <= -(BALL_SIZE/2)) {
            p2_score += 1;
            reset = true;
            PlaySound(sfx_score);
        }

        DrawText(TextFormat("Player 1: %d", p1_score), INNER_PADDING, 10, 24, BLUE);
        DrawText(TextFormat("Player 2: %d", p2_score), INNER_PADDING, 36, 24, RED);
        
        DrawRectangle(p1_pos.x, p1_pos.y, PLAYER_WIDTH, PLAYER_HEIGHT, RAYWHITE);
        DrawRectangle(p2_pos.x, p2_pos.y, PLAYER_WIDTH, PLAYER_HEIGHT, RAYWHITE);
        DrawRectangle(ball_pos.x, ball_pos.y, BALL_SIZE, BALL_SIZE, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
