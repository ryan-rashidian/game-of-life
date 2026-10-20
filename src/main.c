/*
 * File: main.c
 *
 * Game of Life
 */

#include "raylib.h"

#include "game.h"
#include "screen.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define TITLE "Game of Life"

GameScreen screen_current = SCREEN_TITLE;

static const int TARGET_FPS = 60;
static RenderTexture2D target = { 0 };

void update_draw_frame(void);

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);

    // Initialize a render texture target for the whole screen
    target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
    game_init();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update_draw_frame, TARGET_FPS, 1);
#else

    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose()) {
        update_draw_frame();
    }

#endif

    game_end();
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}

static void update_frame(void)
{
    float dt = GetFrameTime();

    switch (screen_current) {
        case SCREEN_TITLE: screen_title_update();  break;
        case SCREEN_GAME:  screen_game_update(dt); break;
        case SCREEN_MENU:  screen_menu_update(); break;
        default: break;
    }
}

static void draw_frame(void)
{
    // Render to target texture
    BeginTextureMode(target);
    Color color_background = { 34, 45, 56, 255 };
    ClearBackground(color_background);

    switch (screen_current) {
        case SCREEN_TITLE: screen_title_draw(); break;
        case SCREEN_GAME:  screen_game_draw();  break;
        case SCREEN_MENU:  screen_menu_draw();  break;
        default: break;
    }

    EndTextureMode();

    // Draw target texture
    BeginDrawing();
    ClearBackground(BLACK);

    Rectangle src_rec = {
        0, 0,
        (float)target.texture.width,
        -(float)target.texture.height
    };
    Rectangle dst_rec = {
        0, 0,
        (float)target.texture.width,
        (float)target.texture.height
    };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(target.texture, src_rec, dst_rec, origin, 0.0f, WHITE);

    EndDrawing();
}

void update_draw_frame(void)
{
    update_frame();
    draw_frame();
}

