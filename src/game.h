/*
 * File: game.h
 */

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#include "raylib.h"

#include "rules.h"

static const int GRID_WIDTH  = 1440;
static const int GRID_HEIGHT = 1440;

extern Camera2D game_cam;

void game_init(void);
void game_end(void);
void game_update(float dt);
void game_draw(void);

void game_toggle_pause(void);
bool game_is_paused(void);
void game_inc_update_time(void);
void game_dec_update_time(void);
float game_get_update_time(void);
uint64_t game_get_gen(void);

#endif

