/*
 * File: ui.c
 *
 * User interface
 */

#include <stddef.h>
#include <stdio.h>

#include "raylib.h"

#include "game.h"
#include "ui.h"
#include "button.h"
#include "screen.h"

static struct {
    Button menu;
    Button pause;
    Button inc_speed;
    Button dec_speed;
} buttons = {
    .menu      = { .text_font = 30, .text = "Menu" },
    .pause     = { .text_font = 40, .text = NULL },
    .inc_speed = { .text_font = 28, .text = ">" },
    .dec_speed = { .text_font = 28, .text = "<" },
};

void ui_init(void)
{
    float pos_y = SCREEN_HEIGHT - (SCREEN_HEIGHT / 8.0f);
    buttons.menu.rec = (Rectangle){
        SCREEN_WIDTH / 4.0f - 60.0f, pos_y,
        120.0f, 60.0f
    };
    buttons.pause.rec = (Rectangle){
        SCREEN_WIDTH / 2.0f - 30.0f, pos_y,
        60.0f, 60.0f
    };
    buttons.inc_speed.rec = (Rectangle){
        SCREEN_WIDTH - (SCREEN_WIDTH / 4.0f), pos_y,
        60.0f, 30.0f
    };
    buttons.dec_speed.rec = (Rectangle){
        SCREEN_WIDTH - (SCREEN_WIDTH / 4.0f) - 60.0f, pos_y,
        60.0f, 30.0f
    };
}

bool ui_update(void)
{
    Vector2 mouse = GetMousePosition();
    Rectangle mouse_rec = { mouse.x, mouse.y, 1.0f, 1.0f };
    bool status = false;

    if (CheckCollisionRecs(buttons.pause.rec, mouse_rec)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) game_toggle_pause();
        buttons.pause.highlight = true;
        status = true;
    } else buttons.pause.highlight = false;
    if (CheckCollisionRecs(buttons.menu.rec, mouse_rec)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            switch (screen_current) {
                case SCREEN_MENU: screen_current = SCREEN_GAME; break;
                case SCREEN_GAME: screen_current = SCREEN_MENU; break;
                default: break;
            }
        }
        buttons.menu.highlight = true;
        status = true;
    } else buttons.menu.highlight = false;
    if (CheckCollisionRecs(buttons.inc_speed.rec, mouse_rec)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) game_dec_update_time();
        buttons.inc_speed.highlight = true;
        status = true;
    } else buttons.inc_speed.highlight = false;
    if (CheckCollisionRecs(buttons.dec_speed.rec, mouse_rec)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) game_inc_update_time();
        buttons.dec_speed.highlight = true;
        status = true;
    } else buttons.dec_speed.highlight = false;

    return status;
}

static void draw_pause_button(void)
{
    float pos_x = buttons.pause.rec.x;
    float pos_y = buttons.pause.rec.y;
    float width = buttons.pause.rec.width;
    float height = buttons.pause.rec.height;
    float pad = 20.0f;
    Color *color = &LIGHTGRAY;
    Color color_hl = (Color){ 125, 156, 224, 255 };
    if (buttons.pause.highlight) color = &color_hl;

    button_draw(&buttons.pause);
    if (game_is_paused()) {
        Vector2 v1 = { pos_x + pad, pos_y + pad };
        Vector2 v2 = { pos_x + pad, pos_y + height - pad };
        Vector2 v3 = { pos_x + width - pad, pos_y + height / 2.0f };
        DrawTriangle(v1, v2, v3, *color);
    } else {
        float symbol_height = height - pad * 2;
        Rectangle rec = {
            pos_x + pad, pos_y + pad,
            width / 16.0f, symbol_height 
        };
        DrawRectangleRec(rec, *color);
        rec.x += 16.0f;
        DrawRectangleRec(rec, *color);
    }
}

static void draw_speed_buttons(void)
{
    float update_time = game_get_update_time();
    char text[5];
    snprintf(text, 5, "%.2f", update_time);
    int text_size = MeasureText(text, 20);
    float x_offset = (buttons.dec_speed.rec.width - text_size) / 2.0f;
    x_offset += buttons.dec_speed.rec.width / 2.0f;
    float y_offset = (buttons.dec_speed.rec.height - 20) / 2.0f;
    float x_pos = buttons.dec_speed.rec.x + x_offset;
    float y_pos = buttons.dec_speed.rec.y + y_offset + 30.0f;

    button_draw(&buttons.inc_speed);
    button_draw(&buttons.dec_speed);
    DrawText(text, x_pos, y_pos, 20, LIGHTGRAY);
}

void ui_draw(void)
{
    button_draw(&buttons.menu);
    draw_pause_button();
    draw_speed_buttons();
}

