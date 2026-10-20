/*
 * File: button.h
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>

#include "raylib.h"

typedef struct {
    int text_font;
    bool highlight;
    const char *text;
    Rectangle rec;
} Button;

void button_draw(Button *button);

#endif

