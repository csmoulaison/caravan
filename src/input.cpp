#include "input.h"

void init_input(Input& input) {
    input.quit_application = false;

    map_scancode_to_button(SDL_SCANCODE_ESCAPE, &input.quit, input);
    map_scancode_to_button(SDL_SCANCODE_SPACE, &input.select, input);
    map_scancode_to_button(SDL_SCANCODE_RETURN, &input.select, input);
    map_scancode_to_button(SDL_SCANCODE_W, &input.cam_up, input);
    map_scancode_to_button(SDL_SCANCODE_A, &input.cam_left, input);
    map_scancode_to_button(SDL_SCANCODE_S, &input.cam_down, input);
    map_scancode_to_button(SDL_SCANCODE_D, &input.cam_right, input);
    map_scancode_to_button(SDL_SCANCODE_UP, &input.caravan_up, input);
    map_scancode_to_button(SDL_SCANCODE_LEFT, &input.caravan_left, input);
    map_scancode_to_button(SDL_SCANCODE_DOWN, &input.caravan_down, input);
    map_scancode_to_button(SDL_SCANCODE_RIGHT, &input.caravan_right, input);
}

void input_poll(Input& input, int pixel_scalar) {
    input.mouse_down = false;

    for(Button* button : input.mapped_buttons) {
        button->just_pressed = false;
        button->just_released = false;
    }

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
        case SDL_QUIT:
            input.quit_application = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(e.button.button == SDL_BUTTON_LEFT) {
                input.mouse_down = true;
            }
        case SDL_KEYDOWN:
            for(ScancodeButtonMap& map : input.scancode_button_maps) {
                if(e.key.keysym.scancode == map.scancode && !map.button->held) {
                    map.button->held = true;
                    map.button->just_pressed = true;
                }
            }
            break;
        case SDL_KEYUP:
            for(ScancodeButtonMap& map : input.scancode_button_maps) {
                if(e.key.keysym.scancode == map.scancode) {
                    if(map.button->held) {
                        map.button->just_released = true;
                    }
                    map.button->held = false;
                }
            }
            break;
        case SDL_MOUSEMOTION:
            input.mouse_x = e.motion.x ;
            input.mouse_y = e.motion.y ;
            break;
        default:
            break;
        }
    }
}

void map_scancode_to_button(SDL_Scancode sc, Button* button, Input& input) {
    input.scancode_button_maps.push_back(ScancodeButtonMap{sc, button});

    bool matched_button = false;
    for(Button* mapped_button : input.mapped_buttons) {
        if(button == mapped_button) {
            matched_button = true;
        }
    }

    if(!matched_button) {
        input.mapped_buttons.push_back(button);
    }
}

