#include "bandits.h"

#define BANDIT_CHANCE_BASE 20
#define BANDIT_CHANCE_VALUE_MOD 0.9995 // every money does this

#define BANDIT_STEAL_BASE 25
#define BANDIT_STEAL_VALUE_MOD 1.0001
#define BANDIT_STEAL_SWORD_MOD 0.5

bool try_bandit(Game& game, Assets& assets) {
    int sword_count = 0;
    int total_value = 0;
    for(Item* item : game.inventory) {
        total_value += item->inherent_value;
        if(item->type == ItemType::WEAPON) {
            sword_count++;
        }
    }
    if(sword_count > game.caravaneers) {
        sword_count = game.caravaneers;
    }

    float bandit_chance = BANDIT_CHANCE_BASE;
    float max_steal_amount = BANDIT_STEAL_BASE;
    for(int i = 0; i < total_value; i++) {
        bandit_chance *= BANDIT_CHANCE_VALUE_MOD;
        max_steal_amount *= BANDIT_STEAL_VALUE_MOD;
    }
    for(int i = 0; i < sword_count; i++) {
        max_steal_amount *= BANDIT_STEAL_SWORD_MOD;
    }

    if(rand() % (int)bandit_chance == 0) {
        game.stolen_bandit_money = rand() % (int)max_steal_amount;
        game.money -= game.stolen_bandit_money; // just for display
        if(game.money < 0) {
            game.money = 0;
        }
        game.state = GameState::BANDITS;
        playSoundFromMemory(assets.bandit, VOLUME_MID);
        return true;
    }

    return false;
}
