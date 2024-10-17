#include "game_ready_state.h"
#include <unordered_set>

#define CAM_SPEED 0.15

void handle_game_ready(Game& game, Assets& assets, Input& input) {
    game.selector_x = ((input.mouse_x + game.cam_x) / TILE_SIZE);
    game.selector_y = ((input.mouse_y + game.cam_y) / TILE_SIZE);

    MoveDirection move_direction = MoveDirection::NONE;
    if(input.caravan_up.just_pressed) {
        move_direction = MoveDirection::NORTH;
    } else if(input.caravan_left.just_pressed) {
        move_direction = MoveDirection::WEST;
    } else if(input.caravan_down.just_pressed) {
        move_direction = MoveDirection::SOUTH;
    } else if(input.caravan_right.just_pressed) {
        move_direction = MoveDirection::EAST;
    }

    // We might want to first switch to interstitial for the movement,
    // but should be easy enough to configure after the fact.
    // For now, we are just skipping right to simulate_turn, but this would
    // be handled by handle_game_interstitial or whatever in the future.
    if(move_direction != MoveDirection::NONE) {
        simulate_turn(game, assets, move_direction);
    }
}

void update_camera(Game& game, Input& input, bool& out_cam_moved) {
    int cam_move_x = 0;
    int cam_move_y = 0;
    if(input.cam_left.held) cam_move_x -= 1;
    if(input.cam_right.held) cam_move_x += 1;
    if(input.cam_up.held) cam_move_y -= 1;
    if(input.cam_down.held) cam_move_y += 1;

    game.cam_x += cam_move_x * CAM_SPEED;
    game.cam_y += cam_move_y * CAM_SPEED;

    out_cam_moved = false;
    if(cam_move_x != 0 || cam_move_y != 0) {
        out_cam_moved = true;
    }
}

void draw_game_ready(Game& game, Input& input, Assets& assets, DrawContext& context) {
    for(Market& market : game.markets) {
        if(game.explored_tiles.count(market.y * GAME_WIDTH + market.x) != 1) {
            continue;
        }
        Sprite market_sprite = assets.sprite_caravanserai;
        if(market.type == MarketType::CITY) {
            market_sprite = assets.sprite_city;
        }
        draw_tile_cam(context, market_sprite, market.x, market.y, game.cam_x, game.cam_y);
    }

    for(Oasis& oasis : game.oases) {
        if(game.explored_tiles.count(oasis.y * GAME_WIDTH + oasis.x) != 1) {
            continue;
        }
        draw_tile_cam(context, assets.sprite_oasis, oasis.x, oasis.y, game.cam_x, game.cam_y);
    }

    for(Rock& rock : game.rocks) {
        draw_tile_cam(context, *rock.sprite, rock.x, rock.y, game.cam_x, game.cam_y);
    }

    for(Treasure& treasure : game.treasures) {
        if(treasure.active) {
            draw_tile_cam(context, assets.sprite_treasure, treasure.x, treasure.y, game.cam_x, game.cam_y);
        }
    }

    draw_tile_cam(context, assets.sprite_camel, game.caravan_x, game.caravan_y, game.cam_x, game.cam_y);

    for(int y = 0; y < GAME_HEIGHT; y++) {
        for(int x = 0; x < GAME_WIDTH; x++) {
            if(game.explored_tiles.count(y * GAME_WIDTH + x) != 1) {
                Sprite sprite_fow = assets.sprite_fow;
                if(game.explored_tiles.count((y + 1) * GAME_WIDTH + x) == 1 && y + 1 < GAME_HEIGHT
                || game.explored_tiles.count(y * GAME_WIDTH + x + 1) == 1 && x + 1 < GAME_WIDTH
                || game.explored_tiles.count((y - 1) * GAME_WIDTH + x) == 1 && y - 1 > 0
                || game.explored_tiles.count(y * GAME_WIDTH + x - 1) == 1 && x - 1 > 0) {
                    sprite_fow = assets.sprite_fow_penumbra;
                }
                draw_tile_cam(context, sprite_fow, x, y, game.cam_x, game.cam_y);
            }
        }
    }

    draw_tile_cam(context, assets.sprite_selector, game.selector_x, game.selector_y, game.cam_x, game.cam_y);
}

void draw_player_header_ui(Game& game, Assets& assets, DrawContext& context) {
    // SURVIVAL STATUS
    draw_dialog(context, DialogStyle::LINE, 0, 0, 112, 17);
    // Water
    draw_sprite(context, assets.sprite_water, 0, 0);
    draw_text(context, "WATER ", Font::SMALL, 16, 2, nullptr);
    draw_text(context, std::to_string(game.waters), Font::BIG, 18, 7, nullptr);
    // Food
    int xoff = 38;
    draw_sprite(context, assets.sprite_bread, xoff, 0);
    draw_text(context, "FOOD ", Font::SMALL, 16 + xoff, 2, nullptr);
    draw_text(context, std::to_string(food_count(game)), Font::BIG, 18 + xoff, 7, nullptr);
    // Caravaneers
    xoff += 34;
    draw_sprite(context, assets.sprite_camel, xoff, 0);
    draw_text(context, "POP ", Font::SMALL, 16 + xoff, 2, nullptr);
    draw_text(context, std::to_string(game.caravaneers), Font::BIG, 18 + xoff, 7, nullptr);

    // ECON STATUS
    draw_dialog(context, DialogStyle::LINE, 0, 15, 112, 9);
    int curx = 2;
    draw_text(context, "GOLD:", Font::SMALL, curx, 17, &curx);
    draw_text(context, std::to_string(game.money), Font::SMALL, curx, 17, &curx);
    curx = 42;
    draw_text(context, "CAPACITY:", Font::SMALL, curx, 17, &curx);
    draw_text(context, std::to_string(weight_current(game)), Font::SMALL, curx, 17, &curx);
    draw_text(context, ";", Font::SMALL, curx, 17, &curx);
    draw_text(context, std::to_string(weight_capacity(game)), Font::SMALL, curx, 17, &curx);
    draw_text(context, "LB", Font::SMALL, curx, 17, &curx);
}
