/*
 * File: menu.c
 *
 * NOTE: I'm sure there's a better way to do this (same applies in ui.c).
 * Buttons are already abstracted into structs with a helper draw function,
 * but there are probably more ways to consolidate the design and make some
 * sort of 'UI mini-lib' using raylib. Also should design it to scale with
 * varying screen dimensions (fixed 720x720 is fine for demo/prototype).
 */

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include "raylib.h"

#include "menu.h"
#include "button.h"
#include "game.h"
#include "rules.h"
#include "screen.h"

enum {
    MENU_MAIN,
    MENU_PRESETS,
    MENU_COUNT
};

enum {
    TAB_STATS,
    TAB_RULES,
    TAB_CONTROLS,
    TAB_COUNT
};

enum {
    RULE_OPT_BIRTH,
    RULE_OPT_SURVIVAL,
    RULE_OPT_PRESETS,
    RULE_OPT_COUNT
};

enum {
    PRESET_OPT_BACK,
    PRESET_OPT_CONWAY,
    PRESET_OPT_CORAL,
    PRESET_OPT_BACTERIA,
    PRESET_OPT_MAZE,
    PRESET_OPT_AMOEBA,
    PRESET_OPT_SERVIETTES,
    PRESET_OPT_DANCE,
    PRESET_OPT_COUNT
};

// Button arrays
static Button tab_buttons[TAB_COUNT] = {
    [TAB_RULES].text             = "Rules",
    [TAB_STATS].text             = "Stats",
    [TAB_CONTROLS].text          = "Controls",
};
static Button rule_opt_buttons[RULE_OPT_COUNT] = {
    [RULE_OPT_BIRTH].text        = "Birth",
    [RULE_OPT_SURVIVAL].text     = "Survival",
    [RULE_OPT_PRESETS].text      = "Presets",
};
static Button preset_opt_buttons[PRESET_OPT_COUNT] = {
    [PRESET_OPT_BACK].text       = "< Back",
    [PRESET_OPT_CONWAY].text     = "Conway",
    [PRESET_OPT_CORAL].text      = "Coral",
    [PRESET_OPT_BACTERIA].text   = "Bacteria",
    [PRESET_OPT_MAZE].text       = "Maze",
    [PRESET_OPT_AMOEBA].text     = "Amoeba",
    [PRESET_OPT_SERVIETTES].text = "Serviettes",
    [PRESET_OPT_DANCE].text      = "Dance",
};
static Button birth_num_buttons[N_NEIGHBORS + 1] = {
    [0].text = "0", [1].text = "1", [2].text = "2",
    [3].text = "3", [4].text = "4", [5].text = "5",
    [6].text = "6", [7].text = "7", [8].text = "8"
};
static Button survival_num_buttons[N_NEIGHBORS + 1] = {
    [0].text = "0", [1].text = "1", [2].text = "2",
    [3].text = "3", [4].text = "4", [5].text = "5",
    [6].text = "6", [7].text = "7", [8].text = "8"
};

// Currently selected menu/tab/opt
static int curr_menu = MENU_MAIN;
static int curr_tab = TAB_RULES;
static int curr_rule_opt = RULE_OPT_BIRTH;
static int curr_preset_opt = PRESET_OPT_BACK;

// === Initialization functions ===
// Button recs are hard-coded for 720x720 currently.
// If these functions look messy/bad, thats because they are; but it works!
// Should be overhauled. But I only have the energy for a comment currently...
// raygui.h header library is probably a good solution for the future, or
// roll a custom mini-lib.

static void tab_buttons_init(void)
{
    float tab_button_w = 180.0f;
    float tab_button_h = 80.0f;
    float pos_y = SCREEN_HEIGHT / 16.0f;
    float pos_x = SCREEN_WIDTH / 4.0f;
    float spacing_x = pos_x;

    for (int i = 0; i < TAB_COUNT; i++) {
        tab_buttons[i].text_font = 30;
        tab_buttons[i].rec = (Rectangle){
            pos_x - (tab_button_w / 2.0f), pos_y,
            tab_button_w, tab_button_h
        };
        pos_x += spacing_x;
    }
}

static void preset_opt_buttons_init(void)
{
    float preset_button_w = 240.0f;
    float preset_button_h = 40.0f;
    float pos_x = SCREEN_WIDTH / 2.0f;
    float pos_y = SCREEN_HEIGHT / 4.0f;

    for (int i = 0; i < PRESET_OPT_COUNT; i++) {
        preset_opt_buttons[i].text_font = 30;
        preset_opt_buttons[i].rec = (Rectangle){
            pos_x - (preset_button_w / 2.0f), pos_y,
            preset_button_w, preset_button_h
        };
        pos_y += preset_button_h + (preset_button_h / 4.0f);
    }
}

static void rule_opt_buttons_init(void)
{
    float rule_button_w = 240.0f;
    float rule_button_h = 40.0f;
    float pos_x = SCREEN_WIDTH / 2.0f;
    float pos_y = SCREEN_HEIGHT / 4.0f;

    for (int i = 0; i < RULE_OPT_COUNT; i++) {
        rule_opt_buttons[i].text_font = 30;
        rule_opt_buttons[i].rec = (Rectangle){
            pos_x - (rule_button_w / 2.0f), pos_y,
            rule_button_w, rule_button_h
        };
        pos_y += rule_button_h * 4;
    }
}

static void number_buttons_init(void)
{
    float num_button_w = 60.0f;
    float num_button_h = 60.0f;
    float pos_y = SCREEN_HEIGHT / 4.0f + num_button_h;
    float pos_x = SCREEN_WIDTH / 8.0f;
    float spacing_x = num_button_w + (num_button_w / 8.0f);

    for (size_t i = 0; i < N_NEIGHBORS + 1; i++) {
        birth_num_buttons[i].text_font = 30;
        birth_num_buttons[i].rec = (Rectangle){
            pos_x - (num_button_w / 2.0f), pos_y,
            num_button_w, num_button_h
        };
        survival_num_buttons[i].text_font = 30;
        survival_num_buttons[i].rec = (Rectangle){
            pos_x - (num_button_w / 2.0f), pos_y + 160.0f,
            num_button_w, num_button_h
        };
        pos_x += spacing_x;
    }
}

void menu_init(void)
{
    tab_buttons_init();
    preset_opt_buttons_init();
    rule_opt_buttons_init();
    number_buttons_init();
}

// === Rule tab update functions ===

static void update_rule_tab_highlights(void)
{
    for (int i = 0; i < RULE_OPT_COUNT; i++) {
        rule_opt_buttons[i].highlight = false;
    }
    rule_opt_buttons[curr_rule_opt].highlight = true;

    for (int i = 0; i < N_NEIGHBORS + 1; i++) {
        if (rules.birth[i]) birth_num_buttons[i].highlight = true;
        else birth_num_buttons[i].highlight = false;
        if (rules.survival[i]) survival_num_buttons[i].highlight = true;
        else survival_num_buttons[i].highlight = false;
    }
}

static void update_rule_tab_input(void)
{
    if (IsKeyPressed(KEY_ENTER)) {
        if (curr_rule_opt == RULE_OPT_PRESETS) {
            curr_preset_opt = PRESET_OPT_BACK;
            curr_menu = MENU_PRESETS;
        }
    }
    if (IsKeyPressed(KEY_DOWN)) {
        if (curr_rule_opt + 1 < RULE_OPT_COUNT) curr_rule_opt++;
        else curr_rule_opt = 0;
    }
    if (IsKeyPressed(KEY_UP)) {
        if (curr_rule_opt - 1 >= 0) curr_rule_opt--;
        else curr_rule_opt = RULE_OPT_COUNT - 1;
    }

    int toggle_rule = -1;
    switch (GetKeyPressed()) {
        case KEY_ZERO:  toggle_rule = 0; break;
        case KEY_ONE:   toggle_rule = 1; break;
        case KEY_TWO:   toggle_rule = 2; break;
        case KEY_THREE: toggle_rule = 3; break;
        case KEY_FOUR:  toggle_rule = 4; break;
        case KEY_FIVE:  toggle_rule = 5; break;
        case KEY_SIX:   toggle_rule = 6; break;
        case KEY_SEVEN: toggle_rule = 7; break;
        case KEY_EIGHT: toggle_rule = 8; break;
    }
    if (toggle_rule >= 0 && toggle_rule <= 8) {
        if (curr_rule_opt == RULE_OPT_BIRTH) {
            rules.birth[toggle_rule] = !rules.birth[toggle_rule];
        } else if (curr_rule_opt == RULE_OPT_SURVIVAL) {
            rules.survival[toggle_rule] = !rules.survival[toggle_rule];
        }
    }
}

static void update_rule_tab_collisions(void)
{
    Vector2 mouse = GetMousePosition();
    Rectangle mouse_rec = { mouse.x, mouse.y, 1.0f, 1.0f };

    // Rule option buttons
    for (int i = 0; i < RULE_OPT_COUNT; i++) {
        if (CheckCollisionRecs(rule_opt_buttons[i].rec, mouse_rec)) {
            rule_opt_buttons[i].highlight = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                curr_rule_opt = i;
                if (i == RULE_OPT_PRESETS) {
                    curr_preset_opt = PRESET_OPT_BACK;
                    curr_menu = MENU_PRESETS;
                }
            }
        }
    }

    // Rule number buttons
    for (int i = 0; i < N_NEIGHBORS + 1; i++) {
        if (CheckCollisionRecs(birth_num_buttons[i].rec, mouse_rec)) {
            birth_num_buttons[i].highlight = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                rules.birth[i] = !rules.birth[i];
            }
        }
        if (CheckCollisionRecs(survival_num_buttons[i].rec, mouse_rec)) {
            survival_num_buttons[i].highlight = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                rules.survival[i] = !rules.survival[i];
            }
        }
    }
}

static void update_rules_tab(void)
{
    update_rule_tab_input();
    update_rule_tab_highlights();
    update_rule_tab_collisions();
}

// === Menu update functions ===

static void update_presets_menu_highlights(void)
{
    for (int i = 0; i < PRESET_OPT_COUNT; i++) {
        preset_opt_buttons[i].highlight = false;
    }
    preset_opt_buttons[curr_preset_opt].highlight = true;
}

static void select_preset(void)
{
    switch (curr_preset_opt) {
        case PRESET_OPT_CONWAY:     preset_conway(); break;
        case PRESET_OPT_CORAL:      preset_coral(); break;
        case PRESET_OPT_BACTERIA:   preset_bacteria(); break;
        case PRESET_OPT_MAZE:       preset_maze(); break;
        case PRESET_OPT_AMOEBA:     preset_amoeba(); break;
        case PRESET_OPT_SERVIETTES: preset_serviettes(); break;
        case PRESET_OPT_DANCE:      preset_dance(); break;
        default: break;
    }
}

static void update_presets_menu_input(void)
{
    if (IsKeyPressed(KEY_DOWN)) {
        if (curr_preset_opt + 1 < PRESET_OPT_COUNT) curr_preset_opt++;
        else curr_preset_opt = 0;
    }
    if (IsKeyPressed(KEY_UP)) {
        if (curr_preset_opt - 1 >= 0) curr_preset_opt--;
        else curr_preset_opt = PRESET_OPT_COUNT - 1;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        curr_menu = MENU_MAIN;
        select_preset();
    }
}

static void update_presets_menu_collisions(void)
{
    Vector2 mouse = GetMousePosition();
    Rectangle mouse_rec = { mouse.x, mouse.y, 1.0f, 1.0f };

    for (int i = 0; i < PRESET_OPT_COUNT; i++) {
        if (CheckCollisionRecs(preset_opt_buttons[i].rec, mouse_rec)) {
            curr_preset_opt = i;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                curr_menu = MENU_MAIN;
                select_preset();
            }
        }
    }
}

static void update_presets_menu(void)
{
    update_presets_menu_input();
    update_presets_menu_collisions();
    update_presets_menu_highlights();
}

static void update_main_menu_highlights(void)
{
    for (int i = 0; i < TAB_COUNT; i++) {
        tab_buttons[i].highlight = false;
    }
    tab_buttons[curr_tab].highlight = true;
}

static void update_main_menu_input(void)
{
    if (IsKeyPressed(KEY_RIGHT)) {
        if (curr_tab + 1 < TAB_COUNT) curr_tab++;
        else curr_tab = 0;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        if (curr_tab - 1 >= 0) curr_tab--;
        else curr_tab = TAB_COUNT - 1;
    }
}

static void update_main_menu_collisions(void)
{
    Vector2 mouse = GetMousePosition();
    Rectangle mouse_rec = { mouse.x, mouse.y, 1.0f, 1.0f };

    for (int i = 0; i < TAB_COUNT; i++) {
        if (CheckCollisionRecs(tab_buttons[i].rec, mouse_rec)) {
            tab_buttons[i].highlight = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) curr_tab = i;
        }
    }
}

static void update_main_menu(void)
{
    switch (curr_tab) {
        case TAB_RULES: update_rules_tab(); break;
        case TAB_STATS: break;
        case TAB_CONTROLS: break;
    }
    update_main_menu_input();
    update_main_menu_highlights();
    update_main_menu_collisions();
}

void menu_update(void)
{
    switch (curr_menu) {
        case MENU_MAIN:    update_main_menu();   break;
        case MENU_PRESETS: update_presets_menu(); break;
    }
}

// === Drawing functions ===

static void draw_message(const char *msg, int font, int y_offset, Color *color)
{
    int len = MeasureText(msg, font);
    int pos_x = (SCREEN_WIDTH  / 2) - (len / 2);
    int pos_y = (SCREEN_HEIGHT / 2) - (font / 2);
    pos_y += y_offset;
    DrawText(msg, pos_x, pos_y, font, *color);
}

static void draw_rules_tab(void)
{
    for (int i = 0; i < RULE_OPT_COUNT; i++) {
        button_draw(&rule_opt_buttons[i]);
    }
    for (int i = 0; i < N_NEIGHBORS + 1; i++) {
        button_draw(&birth_num_buttons[i]);
        button_draw(&survival_num_buttons[i]);
    }
}

static void draw_stats_tab(void)
{
    char gen_chars[20];
    uint64_t gen = game_get_gen();
    snprintf(gen_chars, 20, "%lu", gen);
    int text_size = MeasureText(gen_chars, 30);
    float pos_x = SCREEN_WIDTH / 2.0f - (text_size / 2.0f);
    float pos_y = SCREEN_HEIGHT / 2.0f;
    DrawText(gen_chars, pos_x, pos_y + text_size, 30, LIGHTGRAY);

    text_size = MeasureText("Generations:", 30);
    pos_x = SCREEN_WIDTH / 2.0f - (text_size / 2.0f);
    pos_y -= 30;
    DrawText("Generations:", pos_x, pos_y, 30, LIGHTGRAY);
}

static void draw_controls_tab(void)
{
    draw_message("Controls:",                  30, -150, &LIGHTGRAY);
    draw_message("Left-Click - Draw",          25, -100, &LIGHTGRAY);
    draw_message("Shift + Left-Click - Erase", 25,  -60, &LIGHTGRAY);
    draw_message("Space - Pause",              25,  -20, &LIGHTGRAY);
    draw_message("r - Reset Cells",            25,   20, &LIGHTGRAY);
    draw_message("[ - Decrease Speed",         25,   60, &LIGHTGRAY);
    draw_message("] - Increase Speed",         25,  100, &LIGHTGRAY);
    draw_message("Tab - Menu",                 25,  140, &LIGHTGRAY);
}

static void draw_main_menu(void)
{
    for (size_t i = 0; i < TAB_COUNT; i++) {
        button_draw(&tab_buttons[i]);
    }

    switch (curr_tab) {
        case TAB_RULES:    draw_rules_tab();    break;
        case TAB_STATS:    draw_stats_tab();    break;
        case TAB_CONTROLS: draw_controls_tab(); break;
    }
}

static void draw_presets_menu(void)
{
    int text_size = MeasureText("Presets:", 30);
    float pos_x = SCREEN_WIDTH / 2.0f - (text_size / 2.0f);
    float pos_y = SCREEN_HEIGHT / 6.0f;
    DrawText("Presets:", pos_x, pos_y, 30, LIGHTGRAY);
    for (size_t i = 0; i < PRESET_OPT_COUNT; i++) {
        button_draw(&preset_opt_buttons[i]);
    }
}

void menu_draw(void)
{
    switch (curr_menu) {
        case MENU_MAIN:    draw_main_menu();    break;
        case MENU_PRESETS: draw_presets_menu(); break;
    }
}

