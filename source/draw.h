#pragma once
#include <SDL.h>
#include <iostream>
#include <string>
#include "sprite.h"

#define FNAME_SPRITE_ATLAS "./assets/atlas.bmp"
#define FNAME_WINDOW_ICON "./assets/icon.bmp"

struct DrawContext {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* atlas;
    int bg_color[3];
    int screen_width;
    int screen_height;
    int logical_width = 352;
    int logical_height = 240;
    int pixel_scalar;
};

enum class Font {
    BIG,
    SMALL
};

enum class DialogStyle {
    SOFT,
    BOXY,
    LINE,
    HIGHLIGHT
};

void init_draw_context(DrawContext& context);
void draw_clear_screen(DrawContext& context);
void draw_present(DrawContext& context);
void draw_set_bg_black(DrawContext& context);
void draw_set_bg_tan(DrawContext& context);
void draw_sprite(DrawContext& context, Sprite& sprite, int pos_x, int pos_y);
void draw_sprite_cam(DrawContext& context, Sprite& sprite, int pos_x, int pos_y, int cam_x, int cam_y);
void draw_tile_cam(DrawContext& context, Sprite& sprite, int tile_x, int tile_y, int cam_x, int cam_y);
void draw_sprite_scaled(DrawContext& context, Sprite& sprite, int pos_x, int pos_y, int end_x, int end_y);
void draw_text(DrawContext& context, std::string string, Font font, int x, int y, int* out_x);
void draw_dialog(DrawContext& context, DialogStyle style, int x, int y, int w, int h);
SDL_Surface* load_surface(std::string fname);
