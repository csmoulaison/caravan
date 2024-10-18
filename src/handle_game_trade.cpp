#include "handle_game_trade.h"

void draw_game_trade(Game& game, Input& input, Assets& assets, DrawContext& context) {
    if(input.select.just_pressed) {
        game.state = GameState::READY;
        play_sound(assets.trade_close);
    }

    draw_set_bg_black(context);

    // SELL
    InventoryAction action = draw_inventory_ui(context, input, game.inventory, 0);
    if(action.inventory_index != -1) {
        Item* item = game.inventory[action.inventory_index];
        int value = market_value(game, item);
        draw_prospective_transaction(context, item, false, action.stacked_index, value);

        if(action.pressed) {
            game.money += value;
            game.cur_market->inventory.push_back(item);

            for(int i = 0; i < game.inventory.size(); i++) {
                if(game.inventory[i] == item) {
                    game.inventory.erase(game.inventory.begin() + i);
                    break;
                }
            }

            play_sound(assets.sell);
        }

    } 

    // BUY
    action = draw_inventory_ui(context, input, game.cur_market->inventory, 240);
    if(action.inventory_index != -1) {
        Item* item = game.cur_market->inventory[action.inventory_index];
        int value = market_value(game, item);
        draw_prospective_transaction(context, item, true, action.stacked_index, value);

        if(action.pressed) {
            if(value <= game.money && weight_current(game) + item->weight <= weight_capacity(game)) {
                game.money -= value;
                for(int i = 0; i < game.cur_market->inventory.size(); i++) {
                    if(game.cur_market->inventory[i] == item) {
                        game.cur_market->inventory.erase(game.cur_market->inventory.begin() + i);
                        break;
                    }
                }

                if(item->type == ItemType::CAMEL) {
                    game.caravaneers += 1;
                    game.waters = WATER_PER_CARAVANEER * game.caravaneers;
                } else {
                    game.inventory.push_back(item);
                }

                play_sound(assets.buy);
            } else {
                play_sound(assets.nosale);
            }
        }
    }

    draw_text(context, *game.cur_market->name, Font::BIG, 242, 4, nullptr);
    std::string type = "CITY";
    if(game.cur_market->type == MarketType::CARAVANSERAI) {
        type = "CARAVANSERAI";
    }
    draw_text(context, type, Font::SMALL, 242, 13, nullptr);
    draw_player_header_ui(game, assets, context);
}

void draw_prospective_transaction(DrawContext& context, Item* item, bool buying, int stacked_index, int value) {
    int dialog_x = 0;
    if(buying) {
        dialog_x = 240;
    }
    draw_dialog(context, DialogStyle::HIGHLIGHT, dialog_x, 22 + 15 * stacked_index, 112, 17);

    draw_dialog(context, DialogStyle::SOFT, 128, 48, 96, 68);
    draw_sprite(context, *item->sprite, 168, 56);
    std::string transact_type = "SELL";
    int transact_type_xoff = 8;
    if(buying) {
        transact_type = "BUY";
        transact_type_xoff = 6;
    }
    draw_text(context, transact_type, Font::SMALL, context.logical_width / 2 - transact_type_xoff, 74, nullptr);
    draw_text(context, item->name, Font::BIG, 136, 80, nullptr);
    draw_text(context, "FOR", Font::SMALL, context.logical_width / 2 - 6, 90, nullptr);
    draw_text(context, std::to_string(value), Font::BIG, 136, 95, nullptr);
}

int market_value(Game& game, Item* item) {
    int base_value = item->inherent_value;
    float supply;
    float demand;

    for(MarketPrice& price : game.cur_market->prices) {
        if(price.item == item) {
            supply = market_supply(game, *game.cur_market, price);
            demand = price.demand;
            break;
        }
    }

    int market_val = base_value - base_value * supply + base_value * demand;
    if(market_val < 1) {
        market_val = 1;
    }

    return market_val;
}
