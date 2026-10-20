/*
 * File: button.h
 */

#include <stddef.h>

#include "button.h"

static void draw_button_rec(Button *button)
{
    Color inner_color = { 64, 81, 99, 127 };
    Color outer_color = { 64, 81, 99, 255 };
    DrawRectangleRec(button->rec, inner_color);
    DrawRectangleLinesEx(button->rec, 4, outer_color);
}

void button_draw(Button *button)
{
    if (button->text == NULL) {
        draw_button_rec(button);
        return;
    }

    int text_size = MeasureText(button->text, button->text_font);
    float x_offset = (button->rec.width - text_size) / 2.0f;
    float y_offset = (button->rec.height - button->text_font) / 2.0f;
    float pos_x = button->rec.x + x_offset;
    float pos_y = button->rec.y + y_offset;
    Color *color = &LIGHTGRAY;
    Color color_hl = (Color){ 125, 156, 224, 255 };
    if (button->highlight) color = &color_hl;
    draw_button_rec(button);
    DrawText(button->text, pos_x, pos_y, button->text_font, *color);
}

