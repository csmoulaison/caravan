#pragma once
#include <unordered_set>
#include "game_width.h"

void explore_tiles(std::unordered_set<int>& explored_tiles, int x, int y);
void explore_tile(std::unordered_set<int>& explored_tiles, int x, int y);
