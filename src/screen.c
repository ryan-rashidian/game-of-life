/*
 * File: screen.c
 */

#include "raylib.h"

#include "screen.h"
#include "game.h"
#include "menu.h"
#include "ui.h"

void screen_title_update(void)
{
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) ||
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT) ||
        IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) screen_current = SCREEN_GAME;
}

static void draw_message(const char *msg, int font, int y_offset, Color *color)
{
    int len = MeasureText(msg, font);
    int pos_x = (SCREEN_WIDTH  / 2) - (len / 2);
    int pos_y = (SCREEN_HEIGHT / 2) - (font / 2);
    pos_y += y_offset;
    DrawText(msg, pos_x, pos_y, font, *color);
}

void screen_title_draw(void)
{
    game_draw();
    draw_message("Game of Life", 90, -90, &YELLOW);
    draw_message("< Click to Start >", 50, 40, &LIGHTGRAY);
}

void screen_game_update(float dt)
{
    if (IsKeyPressed(KEY_TAB)) screen_current = SCREEN_MENU;
    game_update(dt);
}

void screen_game_draw(void)
{
    BeginMode2D(game_cam);
    game_draw();
    EndMode2D();
    ui_draw();
}

void screen_menu_update(void)
{
    if (IsKeyPressed(KEY_TAB)) screen_current = SCREEN_GAME;
    menu_update();
    ui_update();
}

void screen_menu_draw(void)
{
    screen_game_draw();
    menu_draw();
}

