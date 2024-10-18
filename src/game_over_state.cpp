#include "game_over_state.h"

// Returns true if ready to start over
bool handle_game_game_over(Game& game, Input& input) {
    if(input.select.just_pressed) {
        return true;
    }

    return false;
}

void draw_game_game_over(Game& game, DrawContext& context) {
    draw_text(context, "GAME OVER", Font::BIG, 48, 48, nullptr);
}
