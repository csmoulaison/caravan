#include "draw.h"
#include "tile_size.h"

// SDL must be initialized
void init_draw_context(DrawContext& context) {
    SDL_DisplayMode display_mode;
    SDL_GetDesktopDisplayMode(0, &display_mode);
    
    context.screen_width = display_mode.w;
    context.screen_height = display_mode.h;

    context.window = SDL_CreateWindow("Caravan", 0, 0, context.screen_width, context.screen_height, SDL_WINDOW_BORDERLESS);
    context.renderer = SDL_CreateRenderer(context.window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(context.renderer, context.logical_width, context.logical_height);

    SDL_Surface* atlas_surface = load_surface(FNAME_SPRITE_ATLAS);
    context.atlas = SDL_CreateTextureFromSurface(context.renderer, atlas_surface);

    SDL_Surface* icon_surface = load_surface(FNAME_WINDOW_ICON);
    SDL_SetWindowIcon(context.window, icon_surface);

    draw_set_bg_tan(context);
}

void draw_clear_screen(DrawContext& context) {
    SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(context.renderer);

    SDL_SetRenderDrawColor(context.renderer, context.bg_color[0], context.bg_color[1], context.bg_color[2], SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(context.renderer, NULL);
}

void draw_present(DrawContext& context) {
    SDL_RenderPresent(context.renderer);
}

void draw_set_bg_black(DrawContext& context) {
    context.bg_color[0] = 0;
    context.bg_color[1] = 0;
    context.bg_color[2] = 0;
}

void draw_set_bg_tan(DrawContext& context) {
    context.bg_color[0] = 251;
    context.bg_color[1] = 208;
    context.bg_color[2] = 101;
}

// VOLATILE some redundancy here with draw_sprite_scaled
void draw_sprite(DrawContext& context, Sprite& sprite, int pos_x, int pos_y) {
    context.pixel_scalar = context.screen_height / context.logical_height;
    int pixel_scalar = context.pixel_scalar;
    pixel_scalar = 1;
    SDL_Rect src {sprite.x, sprite.y, sprite.w, sprite.h};
    SDL_Rect dst {pos_x * pixel_scalar, pos_y * pixel_scalar, sprite.w * pixel_scalar, sprite.h * pixel_scalar};
    SDL_RenderCopyEx(context.renderer, context.atlas, &src, &dst, 0, nullptr, SDL_FLIP_NONE);
}

void draw_sprite_cam(DrawContext& context, Sprite& sprite, int pos_x, int pos_y, int cam_x, int cam_y) {
    draw_sprite(context, sprite, pos_x - cam_x, pos_y - cam_y);
}

void draw_tile_cam(DrawContext& context, Sprite& sprite, int tile_x, int tile_y, int cam_x, int cam_y) {
    draw_sprite_cam(context, sprite, tile_x * TILE_SIZE, tile_y * TILE_SIZE, cam_x, cam_y);
}

// VOLATILE some redundancy here with draw_sprite
// TODO could just make draw_sprite a caller of draw_sprite_scaled?
void draw_sprite_scaled(DrawContext& context, Sprite& sprite, int pos_x, int pos_y, int end_x, int end_y) {
    context.pixel_scalar = context.screen_height / context.logical_height;
    int pixel_scalar = context.pixel_scalar;
    pixel_scalar = 1;
    SDL_Rect src {sprite.x, sprite.y, sprite.w, sprite.h};
    SDL_Rect dst {pos_x * pixel_scalar, pos_y * pixel_scalar, (end_x - pos_x) * pixel_scalar, (end_y - pos_y) * pixel_scalar};
    SDL_RenderCopyEx(context.renderer, context.atlas, &src, &dst, 0, nullptr, SDL_FLIP_NONE);
}

SDL_Surface* load_surface(std::string fname) {
    SDL_Surface* surface = SDL_LoadBMP(fname.c_str());
    if(!surface) {
        std::cout << "Error loading surface. Does file exist at '" << fname << "'?" << std::endl;
    }
    Uint32 background_color_key = SDL_MapRGB(surface->format, 255, 0, 255);
    SDL_SetColorKey(surface, 1, background_color_key);
    return surface;
}

void draw_text(DrawContext& context, std::string string, Font font, int x, int y, int* out_x) {
    int font_w = 6;
    int font_h = 8;
    int font_y = 16;
    if(font == Font::SMALL) {
        font_w = 4;
        font_h = 4;
        font_y = 24;
    }

    int cur_x = x;
    for(char c : string) {
        Sprite sprite {((int)c - 48) * font_w, font_y, font_w, font_h};
        draw_sprite(context, sprite, cur_x, y);
        cur_x += font_w;
    }

    if(out_x != nullptr) {
        *out_x = cur_x;
    }
}

void draw_dialog(DrawContext& context, DialogStyle style, int x, int y, int w, int h) {
    int offx = 0;
    int depthy = 1;
    if(style == DialogStyle::BOXY) {
        offx = 3;
    }
    if(style == DialogStyle::LINE) {
        offx = 3;
        depthy = 0;
    }
    if(style == DialogStyle::HIGHLIGHT) {
        offx = 6;
        depthy = 0;
    }
    Sprite nw {272 + offx, 16, 2, 2};
    Sprite ne {273 + offx, 16, 2, 2};
    Sprite sw {272 + offx, 18, 2, 2 + depthy};
    Sprite se {273 + offx, 18, 2, 2 + depthy};
    Sprite no {273 + offx, 16, 1, 2};
    Sprite we {272 + offx, 18, 1, 1};
    Sprite so {273 + offx, 19, 1, 1 + depthy};
    Sprite ea {274 + offx, 18, 1, 1};
    Sprite ce {273 + offx, 18, 1, 1};

    draw_sprite(context, nw, x, y);
    draw_sprite(context, ne, x + w - 2, y);
    draw_sprite(context, sw, x, y + h - 3);
    draw_sprite(context, se, x + w - 2, y + h - 3);
    draw_sprite_scaled(context, no, x + 2, y, x + w - 2, y + 2);
    draw_sprite_scaled(context, we, x, y + 2, x + 1, y + h - 2);
    draw_sprite_scaled(context, so, x + 2, y + h - 2, x + w - 2, y + h - 1 + depthy);
    draw_sprite_scaled(context, ea, x + w - 1, y + 2, x + w, y + h - 2);
    draw_sprite_scaled(context, ce, x + 1, y + 1, x + w - 1, y + h - 2);
}
