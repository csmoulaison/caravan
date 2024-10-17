#include "tile_exploration.h"

void explore_tiles(std::unordered_set<int>& explored_tiles, int x, int y) {
    explore_tile(explored_tiles, x - 1, y - 1);
    explore_tile(explored_tiles, x - 1, y);
    explore_tile(explored_tiles, x - 1, y + 1);

    explore_tile(explored_tiles, x, y - 1);
    explore_tile(explored_tiles, x, y);
    explore_tile(explored_tiles, x, y + 1);

    explore_tile(explored_tiles, x + 1, y - 1);
    explore_tile(explored_tiles, x + 1, y);
    explore_tile(explored_tiles, x + 1, y + 1);
}

void explore_tile(std::unordered_set<int>& explored_tiles, int x, int y) {
    if(x >= GAME_WIDTH || x < 0 || y >= GAME_HEIGHT || y < 0) return;
    explored_tiles.insert(y * GAME_WIDTH + x);
}
