#pragma once
#include "game.h"
#include "turn.h"
#include "inventory_ui.h"

void handle_game_ready(Game& game, Assets& assets, Input& input);
void update_camera(Game& game, Input& input, bool& out_cam_moved);
void draw_game_ready(Game& game, Input& input, Assets& assets, DrawContext& context);
void draw_player_header_ui(Game& game, Assets& assets, DrawContext& context);
