#include <raylib.h>

#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 600
#define BALL_SIZE 20
#define BALL_SPEED 200
#define BALL_ACCELERATION 25
#define PLAYER_SPEED 500
#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 70
#define PADDING 10
#define AUTO_PLAY false

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
    Sound sfx_meme = LoadSound("sounds/67.wav");

    int p1_score = 0;
    int p2_score = 0;
    int rally = 0;
    Vector2 p1_pos = { PADDING,  DISPLAY_HEIGHT/2 };
    Vector2 p2_pos = { DISPLAY_WIDTH - PLAYER_WIDTH - PADDING, DISPLAY_HEIGHT/2 };
    Vector2 ball_pos = { DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2 };
    Vector2 ball_dir = { 1, 1 };
    float t = 0.0f;

    int target = 1;
    bool reset = false;
    while (!WindowShouldClose())
    {
        if (reset) {
            p1_pos.x = PADDING;
            p1_pos.y = DISPLAY_HEIGHT/2;
            p2_pos.x = DISPLAY_WIDTH - PLAYER_WIDTH - PADDING;
            p2_pos.y = DISPLAY_HEIGHT/2;
            ball_pos.x = DISPLAY_WIDTH/2;
            ball_pos.y = DISPLAY_HEIGHT/2;
            ball_dir.x = -ball_dir.x;
            t = 0.0f;
            reset = false;
            target = ball_dir.x;
            rally = 0;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawLine(DISPLAY_WIDTH/2, 0, DISPLAY_WIDTH/2, DISPLAY_HEIGHT, RAYWHITE);

        // Clamping dt because of render pausing when dragging window! Don't want big jumps in frames.
        const float dt = f_clamp(GetFrameTime(), 0, 0.1);
        if (!AUTO_PLAY) {
            if (IsKeyDown(KEY_W)) p1_pos.y    = f_clamp(p1_pos.y - PLAYER_SPEED * dt, PADDING, DISPLAY_HEIGHT - PLAYER_HEIGHT - PADDING);
            if (IsKeyDown(KEY_S)) p1_pos.y    = f_clamp(p1_pos.y + PLAYER_SPEED * dt, PADDING, DISPLAY_HEIGHT - PLAYER_HEIGHT - PADDING);
            if (IsKeyDown(KEY_UP)) p2_pos.y   = f_clamp(p2_pos.y - PLAYER_SPEED * dt, PADDING, DISPLAY_HEIGHT - PLAYER_HEIGHT - PADDING);
            if (IsKeyDown(KEY_DOWN)) p2_pos.y = f_clamp(p2_pos.y + PLAYER_SPEED * dt, PADDING, DISPLAY_HEIGHT - PLAYER_HEIGHT - PADDING);
        } else { 
            float y = ball_pos.y - PLAYER_HEIGHT/2;
            
            // Unnecessary segmenting to make it look cooler when auto-playing
            if (ball_pos.x <= DISPLAY_WIDTH/2) {
                p1_pos.y = f_clamp(y, PADDING, DISPLAY_HEIGHT - PLAYER_HEIGHT - PADDING);
            } else {
                p2_pos.y = f_clamp(y, PADDING, DISPLAY_HEIGHT - PLAYER_HEIGHT - PADDING);
            }
        }

        t += dt;
        ball_pos.x += (BALL_SPEED + (BALL_ACCELERATION * t)) * dt * ball_dir.x;
        ball_pos.y += (BALL_SPEED + (BALL_ACCELERATION * t)) * dt * ball_dir.y;

        if (ball_pos.y >= (DISPLAY_HEIGHT - BALL_SIZE)) ball_dir.y = -1;
        if (ball_pos.y <= 0) ball_dir.y = 1;
        
        // Left side (player1) hit detection
        if ((ball_pos.x >= p1_pos.x) && (ball_pos.x <= p1_pos.x + BALL_SIZE) && (ball_pos.y >= p1_pos.y - BALL_SIZE) && (ball_pos.y <= p1_pos.y + PLAYER_HEIGHT + BALL_SIZE) && target == -1) {
            ball_dir.x = 1;
            PlaySound(sfx_hit);
            target = 1;
            rally++;
        }

        // Right side (player2) hit detection
        if ((ball_pos.x >= p2_pos.x - BALL_SIZE) && (ball_pos.x <= p2_pos.x) && (ball_pos.y >= p2_pos.y - BALL_SIZE) && (ball_pos.y <= p2_pos.y + PLAYER_HEIGHT + BALL_SIZE) && target == 1) {
            ball_dir.x = -1;
            PlaySound(sfx_hit);
            target = -1;
            rally++;
        }

        if (rally == 60) {
            PlaySound(sfx_meme);
        }

        // Score checks
        if (ball_pos.x >= DISPLAY_WIDTH + (BALL_SIZE/2)) {
            p1_score++;
            reset = true;
            PlaySound(sfx_score);
        } else if (ball_pos.x <= -(BALL_SIZE/2)) {
            p2_score++;
            reset = true;
            PlaySound(sfx_score);
        }

        DrawText(TextFormat("Player 1: %d", p1_score), 10, 10, 24, BLUE);
        DrawText(TextFormat("Player 2: %d", p2_score), 10, 36, 24, RED);
        DrawText(TextFormat("Rally: %d",    rally),    10, 62, 18, GREEN);
        DrawRectangle(p1_pos.x, p1_pos.y, PLAYER_WIDTH, PLAYER_HEIGHT, RAYWHITE);
        DrawRectangle(p2_pos.x, p2_pos.y, PLAYER_WIDTH, PLAYER_HEIGHT, RAYWHITE);
        DrawRectangle(ball_pos.x, ball_pos.y, BALL_SIZE, BALL_SIZE, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
