/*
 * File: ui.h
 *
 * User interface
 */

#ifndef UI_H
#define UI_H

#include <stdbool.h>

void ui_init(void);
// ui_update: Returns true if mouse is interacting with UI
bool ui_update(void);
void ui_draw(void);

#endif

