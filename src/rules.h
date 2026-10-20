/*
 * File: rules.h
 *
 * Rules struct and helper functions for applying presets
 */

#ifndef RULES_H
#define RULES_H

#include <stdbool.h>

#define N_NEIGHBORS 8

// Reserve 9 slots total for each possible neighbor + 0
typedef struct {
    bool birth[N_NEIGHBORS + 1];
    bool survival[N_NEIGHBORS + 1];
} Rules;

extern Rules rules;

void rules_init(void);

void preset_conway(void);
void preset_coral(void);
void preset_bacteria(void);
void preset_maze(void);
void preset_amoeba(void);
void preset_serviettes(void);
void preset_dance(void);

#endif

