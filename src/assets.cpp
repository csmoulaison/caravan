#include "assets.h"

void init_assets(Assets& assets) {
    //                          Type            lb,price,mvmt,prod
    assets.items.push_back(Item{ItemType::FOOD,   1, 10,  10, 10, "BREAD",       "FLUFFY AND DELICIOUS", &assets.sprite_bread});
    assets.items.push_back(Item{ItemType::WEAPON, 5, 75,  4,  3,  "SWORD",       "HELPFUL AGAINST BANDITS", &assets.sprite_sword});
    assets.items.push_back(Item{ItemType::TRADE,  2, 60,  9,  3,  "SILK",        "FROM THE FAR EAST", &assets.sprite_silk});
    assets.items.push_back(Item{ItemType::CAMEL,  0, 100, 2,  3,  "CAMEL",       "INCREASES POP", &assets.sprite_camel});
    assets.items.push_back(Item{ItemType::TRADE,  5, 200, 4,  1,  "RUG",         "HAND WOVEN IN TURKEY", &assets.sprite_rug});
    assets.items.push_back(Item{ItemType::TRADE,  1, 60,  9,  2,  "SPICES",      "MAKES ANY MEAL BETTER", &assets.sprite_spice});
    assets.items.push_back(Item{ItemType::FOOD,   1, 48,  8,  4,  "CURED HAM",   "NICE AND SALTY", &assets.sprite_ham});
    assets.items.push_back(Item{ItemType::FOOD,   1, 22,  8,  2,  "GRAPES",      "STRAIGHT FROM THE VINE", &assets.sprite_grapes});
    assets.items.push_back(Item{ItemType::TRADE,  2, 35,  3,  4,  "NECKLACE",    "GOLD AND STONE", &assets.sprite_necklace});
    assets.items.push_back(Item{ItemType::TRADE,  2, 175, 1,  2,  "SP NECKLACE", "GOLD AND SAPPHIRE", &assets.sprite_sapphire_necklace});
    assets.items.push_back(Item{ItemType::TRADE,  1, 100, 2,  1,  "SAFFRON",     "WORLDS FINEST SPICE", &assets.sprite_saffron});
    assets.items.push_back(Item{ItemType::TRADE,  4, 60,  3,  2,  "TEAPOT",      "HAND CARVED FROM JADE", &assets.sprite_teapot});
    assets.items.push_back(Item{ItemType::TRADE,  6, 80,  2,  2,  "LUTE",        "A BARDS TOOL", &assets.sprite_lute});

    // Sounds
    assets.step = Mix_LoadWAV("assets/sounds/step.wav");
	assets.game_over = Mix_LoadWAV("assets/sounds/gameover.wav");
	assets.trade_open = Mix_LoadWAV("assets/sounds/trade.wav");
	assets.trade_close = Mix_LoadWAV("assets/sounds/trade_close.wav");
	assets.buy = Mix_LoadWAV("assets/sounds/buy.wav");
	assets.sell = Mix_LoadWAV("assets/sounds/sale.wav");
	assets.nosale = Mix_LoadWAV("assets/sounds/nope.wav");
	assets.bandit = Mix_LoadWAV("assets/sounds/bandit.wav");
	assets.oasis = Mix_LoadWAV("assets/sounds/oasis.wav");
	assets.treasure= Mix_LoadWAV("assets/sounds/treasure.wav");
}
