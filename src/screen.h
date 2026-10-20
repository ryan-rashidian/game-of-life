/*
 * File: screen.h
 */

#ifndef SCREEN_H
#define SCREEN_H

#include "raylib.h"
#include "raymath.h"

static const int SCREEN_WIDTH  = 720;
static const int SCREEN_HEIGHT = 720;

typedef enum GameScreen {
    SCREEN_UNKNOWN = -1,
    SCREEN_TITLE   =  0,
    SCREEN_GAME,
    SCREEN_MENU,
} GameScreen;

extern GameScreen screen_current;

void screen_title_update(void);
void screen_title_draw(void);

void screen_game_update(float dt);
void screen_game_draw(void);

void screen_menu_update(void);
void screen_menu_draw(void);

#endif

