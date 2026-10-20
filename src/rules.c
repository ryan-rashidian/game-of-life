/*
 * File: rules.c
 *
 * Reference for presets:
 * - https://conwaylife.com/wiki/List_of_Life-like_rules
 */

#include <stddef.h>

#include "rules.h"

Rules rules;

static void rules_reset(void)
{
    for (size_t i = 0; i < N_NEIGHBORS + 1; i++) {
        rules.birth[i] = false;
        rules.survival[i] = false;
    }
}

void rules_init(void)
{
    rules_reset();
}

// === Preset Rules ===

// B3/S23
void preset_conway(void)
{
    rules_reset();
    rules.birth[3] = true;
    rules.survival[2] = true;
    rules.survival[3] = true;
}

// B3/S45678
void preset_coral(void)
{
    rules_reset();
    rules.birth[3] = true;
    for (size_t i = 4; i < 9; i++) rules.survival[i] = true;
}

// B34/S456
void preset_bacteria(void)
{
    rules_reset();
    rules.birth[3] = true;
    rules.birth[4] = true;
    for (size_t i = 4; i < 7; i++) rules.survival[i] = true;
}

// B3/S12345
void preset_maze(void)
{
    rules_reset();
    rules.birth[3] = true;
    for (size_t i = 1; i < 6; i++) rules.survival[i] = true;
}

// B357/S1358
void preset_amoeba(void)
{
    rules_reset();
    rules.birth[3] = true;
    rules.birth[5] = true;
    rules.birth[7] = true;
    rules.survival[1] = true;
    rules.survival[3] = true;
    rules.survival[5] = true;
    rules.survival[8] = true;
}

// B234/S
void preset_serviettes(void)
{
    rules_reset();
    for (size_t i = 2; i < 5; i++) rules.birth[i] = true;
}

// B34/S35
void preset_dance(void)
{
    rules_reset();
    rules.birth[3] = true;
    rules.birth[4] = true;
    rules.survival[3] = true;
    rules.survival[5] = true;
}

