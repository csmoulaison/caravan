#pragma once
#include <vector>
#include "sprite.h"
#include "item.h"
#include "audio.h"

#define VOLUME_MID 20

struct Assets {
    // Audio
    Audio* step;
    Audio* game_over;
    Audio* trade_open;
    Audio* trade_close;
    Audio* buy;
    Audio* sell;
    Audio* nosale;
    Audio* bandit;
    Audio* oasis;
    Audio* treasure;

    // Sprites
    Sprite sprite_selector = {0, 0, 16, 16};
    Sprite sprite_fow = {464, 0, 16, 16};
    Sprite sprite_fow_penumbra = {480, 0, 16, 16};
    Sprite sprite_camel = {16, 0, 16, 16};
    Sprite sprite_oasis = {32, 0, 16, 16};
    Sprite sprite_caravanserai = {80, 0, 16, 16};
    Sprite sprite_city = {96, 0, 16, 16};
    Sprite sprite_treasure = {528, 0, 16, 16};
    Sprite sprite_water = {48, 0, 16, 16};
    Sprite sprite_money = {64, 0, 16, 16};
    Sprite sprite_bread = {144, 0, 16, 16};
    Sprite sprite_silk = {112, 0, 16, 16};
    Sprite sprite_sword = {128, 0, 16, 16};

    Sprite sprite_rug = {336, 0, 16, 16};
    Sprite sprite_spice = {352, 0, 16, 16};
    Sprite sprite_ham = {368, 0, 16, 16};
    Sprite sprite_grapes = {512, 0, 16, 16};
    Sprite sprite_necklace = {384, 0, 16, 16};
    Sprite sprite_sapphire_necklace = {432, 0, 16, 16};
    Sprite sprite_saffron = {400, 0, 16, 16};
    Sprite sprite_teapot = {416, 0, 16, 16};
    Sprite sprite_lute = {448, 0, 16, 16};

    Sprite sprite_merchant = {208, 0, 16, 16};
    Sprite sprite_scribe = {496, 0, 16, 16};
    std::vector<Sprite> sprite_details = {
        {160, 0, 16, 16},
        {176, 0, 16, 16},
        {192, 0, 16, 16}
    };
    // Items
    std::vector<Item> items;
    // Names
    std::vector<std::string> market_names = {
        "BURSA",
        "KONYA",
        "ADANA",
        "ANTIOCH",
        "IZMIR",
        "BAKU",
        "BARDA",
        "HOMS",
        "MOSUL",
        "BAGHDAD",
        "ERBIL",
        "RASHT",
        "TUS",
        "QAZVIN",
        "NISA",
        "MERV",
        "AMUL",
        "BUKHARA",
        "OTRAR",
        "TARAZ",
        "OSH",
        "HERAT",
        "KABUL",
        "MULTAN",
        "LEH",
        "JAKAR",
        "PARO"
    };
};

void init_assets(Assets& assets);
