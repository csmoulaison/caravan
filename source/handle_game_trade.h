#pragma once
#include "game.h"
#include "game_ready_state.h"

void draw_game_trade(Game& game, Input& input, Assets& assets, DrawContext& context);
void draw_prospective_transaction(DrawContext& context, Item* item, bool buying, int stacked_index, int value);
int market_value(Game& game, Item* item);
