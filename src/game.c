/*
 * File: game.c
 */

#include <stddef.h>
#include <math.h>

#include "game.h"
#include "screen.h"
#include "menu.h"
#include "ui.h"

#define N_UPDATE_TIMES 19
#define INIT_UPDATE_INDEX 9

#define GRID_COLS 360
#define GRID_ROWS 360
#define GRID_SIZE (GRID_COLS * GRID_ROWS)
#define GRID_ID(r, c, w) ((r) * (w) + (c))

static const float ZOOM_SPEED  = 10.0f;
static const float PAN_SPEED  = 500.0f;

Camera2D game_cam = { 0 };
static RenderTexture2D grid_target = { 0 };

static float update_times[N_UPDATE_TIMES] = {
    1.00f, 0.90f, 0.80f, 0.70f, 0.60f, 0.50f, 0.40f, 0.30f, 0.20f,
    0.10f, 0.09f, 0.08f, 0.07f, 0.06f, 0.05f, 0.04f, 0.03f, 0.02f, 0.01f
};

static struct {
    bool running;
    size_t update_time_id;
    uint64_t gen;
} GAME;

static struct {
    bool curr[GRID_SIZE];
    bool next[GRID_SIZE];
    int pixel_spacing;
} cell_grid;

static void cell_grid_reset(void)
{
    for (size_t i = 0; i < GRID_SIZE; i++) {
        cell_grid.curr[i] = false;
        cell_grid.next[i] = false;
    }
    GAME.gen = 0;
}

// Generate texture for game world grid
static void render_grid_target(void)
{
    grid_target = LoadRenderTexture(GRID_WIDTH, GRID_HEIGHT);
    SetTextureFilter(grid_target.texture, TEXTURE_FILTER_BILINEAR);
    BeginTextureMode(grid_target);
    Color color_background = {  34,  45,  56, 255 };
    Color color_line       = {  49,  65,  71, 255 };
    Color color_bold_line  = {  66,  88,  96, 255 };
    ClearBackground(color_background);

    // Draw row lines
    for (int r = 0; r <= GRID_ROWS; r++) {
        Vector2 start = {
            0.0f,
            (float)(cell_grid.pixel_spacing * r) 
        };
        Vector2 end = {
            (float)(cell_grid.pixel_spacing * GRID_ROWS),
            (float)(cell_grid.pixel_spacing * r)
        };
        DrawLineEx(start, end, 2, color_line);
    }
    // Draw column lines
    for (int c = 0; c <= GRID_COLS; c++) {
        Vector2 start = {
            (float)(cell_grid.pixel_spacing * c),
            0.0f
        };
        Vector2 end = {
            (float)(cell_grid.pixel_spacing * c),
            (float)(cell_grid.pixel_spacing * GRID_COLS)
        };
        DrawLineEx(start, end, 2, color_line);
    }
    // Draw bold row lines
    for (int r = 0; r <= GRID_ROWS; r += 10) {
        Vector2 start = {
            0.0f,
            (float)(cell_grid.pixel_spacing * r) 
        };
        Vector2 end = {
            (float)(cell_grid.pixel_spacing * GRID_ROWS),
            (float)(cell_grid.pixel_spacing * r)
        };
        DrawLineEx(start, end, 2, color_bold_line);
    }
    // Draw bold column lines
    for (int c = 0; c <= GRID_COLS; c += 10) {
        Vector2 start = {
            (float)(cell_grid.pixel_spacing * c),
            0.0f
        };
        Vector2 end = {
            (float)(cell_grid.pixel_spacing * c),
            (float)(cell_grid.pixel_spacing * GRID_COLS)
        };
        DrawLineEx(start, end, 2, color_bold_line);
    }

    EndTextureMode();
}

static void camera_init(void)
{
    game_cam.target = (Vector2){ SCREEN_WIDTH, SCREEN_HEIGHT };
    game_cam.zoom = 1.0f;
    game_cam.offset.x = SCREEN_WIDTH / 2.0f;
    game_cam.offset.y = SCREEN_HEIGHT / 2.0f;
}

void game_init(void)
{
    menu_init();
    ui_init();
    rules_init();
    preset_conway();
    camera_init();
    cell_grid.pixel_spacing = GRID_WIDTH / GRID_COLS;
    render_grid_target();
    cell_grid_reset();
    GAME.running = false;
    GAME.update_time_id = INIT_UPDATE_INDEX;
    GAME.gen = 0;
}

void game_end(void)
{
    UnloadRenderTexture(grid_target);
}

static size_t count_neighbors(size_t row, size_t col)
{
    size_t neighbors = 0;

    for (int r_offset = -1; r_offset <= 1; r_offset++) {
        for (int c_offset = -1; c_offset <= 1; c_offset++) {
            // Skip cell being updated
            if (r_offset == 0 && c_offset == 0) continue;
            int cell_row = row + r_offset;
            int cell_col = col + c_offset;
            // Skip out of bounds for edge cells
            // Counts as a dead cell
            if (cell_row < 0 || cell_row >= GRID_COLS ||
                cell_col < 0 || cell_col >= GRID_ROWS) continue;
            size_t index = GRID_ID(cell_row, cell_col, GRID_COLS);
            if (cell_grid.curr[index]) neighbors++;
        }
    }

    return neighbors;
}

static bool apply_ruleset(bool is_alive, size_t n_neighbors)
{
    if (!is_alive) {
        if (rules.birth[n_neighbors]) return true;
        return false;
    } else {
        if (rules.survival[n_neighbors]) return true;
        return false;
    }
}

static void update_cell(size_t row, size_t col)
{
    size_t n_neighbors = count_neighbors(row, col);
    size_t index = GRID_ID(row, col, GRID_COLS);
    bool is_alive = cell_grid.curr[index];
    cell_grid.next[index] = apply_ruleset(is_alive, n_neighbors);
}

static void update_cells(void)
{
    for (size_t r = 0; r < GRID_ROWS; r++) {
        for (size_t c = 0; c < GRID_COLS; c++) {
            update_cell(r, c);
        }
    }

    for (size_t i = 0; i < GRID_SIZE; i++) {
        cell_grid.curr[i] = cell_grid.next[i];
    }
}

static void input_set_cell(Vector2 pos, bool alive)
{
    int col = floor(pos.x / cell_grid.pixel_spacing);
    int row = floor(pos.y / cell_grid.pixel_spacing);
    size_t index = GRID_ID(row, col, GRID_COLS);
    cell_grid.curr[index] = alive;
}

static void input_draw_cell(void)
{
    Vector2 mouse = GetMousePosition();
    Vector2 aim_point = GetScreenToWorld2D(mouse, game_cam);
    if (mouse.x < 0.0f || mouse.x > SCREEN_WIDTH ||
        mouse.y < 0.0f || mouse.y > SCREEN_HEIGHT) return;
    bool alive = true;
    if (IsKeyDown(KEY_LEFT_SHIFT) ||
        IsKeyDown(KEY_RIGHT_SHIFT)) alive = false;
    input_set_cell(aim_point, alive);
}

static void update_input(void)
{
    if (IsKeyPressed(KEY_SPACE)) GAME.running = !GAME.running;
    if (IsKeyPressed(KEY_R)) cell_grid_reset();
    if (IsKeyPressed(KEY_LEFT_BRACKET))  game_inc_update_time();
    if (IsKeyPressed(KEY_RIGHT_BRACKET)) game_dec_update_time();

    if (ui_update()) return;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) input_draw_cell();
}

static void camera_pan(void)
{
    Vector2 delta = GetMouseDelta();
    game_cam.target.x -= delta.x / game_cam.zoom;
    game_cam.target.y -= delta.y / game_cam.zoom;
}

static void camera_zoom(float dt)
{
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        game_cam.zoom += (wheel * ZOOM_SPEED) * dt;
        game_cam.zoom = Clamp(game_cam.zoom, 0.5f, 5.0f);
    }
}

static void camera_check_bounds(void)
{
    float pad_w = SCREEN_WIDTH  / 4.0f;
    float pad_h = SCREEN_HEIGHT / 4.0f;
    float min_x = -pad_w;
    float min_y = -pad_h;
    float max_x = GRID_WIDTH  + pad_w;
    float max_y = GRID_HEIGHT + pad_h;

    if (game_cam.target.x < min_x) game_cam.target.x = min_x;
    if (game_cam.target.y < min_y) game_cam.target.y = min_y;
    if (game_cam.target.x > max_x) game_cam.target.x = max_x;
    if (game_cam.target.y > max_y) game_cam.target.y = max_y;
}

static void camera_update(float dt)
{
    float speed = (PAN_SPEED / game_cam.zoom) * dt;

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    game_cam.target.y -= speed;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  game_cam.target.x -= speed;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  game_cam.target.y += speed;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) game_cam.target.x += speed;

    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) camera_pan();
    camera_zoom(dt);
    camera_check_bounds();
}

void game_update(float dt)
{
    static float timer = 0.0f;
    float update_time = update_times[GAME.update_time_id];

    camera_update(dt);
    update_input();
    if (GAME.running) {
        timer += dt;
        while (timer >= update_time) {
            GAME.gen++;
            update_cells();
            timer -= update_time;
        }
    }
}

static void draw_cell(size_t row, size_t col)
{
    float spacing = (float)cell_grid.pixel_spacing;
    Vector2 pos = (Vector2){ col * spacing, row * spacing };
    Vector2 size = (Vector2){ spacing, spacing };
    DrawRectangleV(pos, size, YELLOW);
}

static void draw_grid_lines(void)
{
    Rectangle srcrec = {
        0.0f, 0.0f,
        (float)grid_target.texture.width,
        -(float)grid_target.texture.height
    };
    Rectangle dstrec = {
        0.0f, 0.0f,
        (float)grid_target.texture.width,
        (float)grid_target.texture.height
    };
    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(grid_target.texture, srcrec, dstrec, origin, 0.0f, WHITE);
}

void game_draw(void)
{
    draw_grid_lines();
    for (size_t r = 0; r < GRID_ROWS; r++) {
        for (size_t c = 0; c < GRID_COLS; c++) {
            size_t index = GRID_ID(r, c, GRID_COLS);
            if (cell_grid.curr[index]) draw_cell(r, c);
        }
    }
}

void game_toggle_pause(void)
{
    GAME.running = !GAME.running;
}

bool game_is_paused(void)
{
    return !GAME.running;
}

void game_inc_update_time(void)
{
    if (GAME.update_time_id == 0) return;

    GAME.update_time_id--;
}

void game_dec_update_time(void)
{
    if (GAME.update_time_id == N_UPDATE_TIMES - 1) return;

    GAME.update_time_id++;
}

float game_get_update_time(void)
{
    return update_times[GAME.update_time_id];
}

uint64_t game_get_gen(void)
{
    return GAME.gen;
}

