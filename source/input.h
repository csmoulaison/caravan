#pragma once
#include <vector>
#include <SDL.h>

struct Button {
    bool held = false;
    bool just_pressed = false;
    bool just_released = false;
};

struct ScancodeButtonMap {
    SDL_Scancode scancode;
    Button* button;
};

struct Input {
    std::vector<ScancodeButtonMap> scancode_button_maps;
    std::vector<Button*> mapped_buttons;

    bool quit_application;
    int mouse_x;
    int mouse_y;
    bool mouse_down;

    Button quit;
    Button select;
    Button caravan_up;
    Button caravan_left;
    Button caravan_down;
    Button caravan_right;
    Button cam_up;
    Button cam_left;
    Button cam_down;
    Button cam_right;
};

void init_input(Input& input);
void input_poll(Input& input, int pixel_scalar);
void map_scancode_to_button(SDL_Scancode sc, Button* button, Input& input);
