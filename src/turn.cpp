#include "turn.h"
#include "rand.h"
#include <cmath>
#include <unordered_set>

#define SUPPLY_RESPONSE_AGGRESSION 0.33
#define MAX_TREASURE_GOLD 300

void simulate_turn(Game& game, Assets& assets, MoveDirection move) {
    switch(move) {
    case MoveDirection::NORTH:
        if(game.caravan_y <= 0) {
            play_sound(assets.nosale);
            return;
        }
        game.caravan_y -= 1;
        break;
    case MoveDirection::WEST:
        if(game.caravan_x <= 0) {
            play_sound(assets.nosale);
            return;
        }
        game.caravan_x -= 1;
        break;
    case MoveDirection::SOUTH:
        if(game.caravan_y >= GAME_HEIGHT - 1) {
            play_sound(assets.nosale);
            return;
        }
        game.caravan_y += 1;
        break;
    case MoveDirection::EAST:
        if(game.caravan_x >= GAME_WIDTH - 1) {
            play_sound(assets.nosale);
            return;
        }
        game.caravan_x += 1;
        break;
    default:
        break;
    }

    bool ready_game_over = false; // only comes to fruition if ending in ready mode
    game.waters--;
    for(int i = 0; i < game.caravaneers; i++) {
        int cheapest_food = -1;
        for(int i = 0; i < game.inventory.size(); i++) {
            Item* item = game.inventory[i];

            if(item->type != ItemType::FOOD) {
                continue;
            }

            if(cheapest_food == -1 || item < game.inventory[cheapest_food]) {
                cheapest_food = i;
            }
        }
        if(cheapest_food != -1) { // if it does == -1, we are dead
            game.inventory.erase(game.inventory.begin() + cheapest_food);
        }

        if(game.waters < 0 || cheapest_food == -1) {
            ready_game_over = true;
        }
    }

    for(Market& market : game.markets) {
        if(market.x == game.caravan_x && market.y == game.caravan_y) {
            game.waters = game.caravaneers * WATER_PER_CARAVANEER;
            game.state = GameState::TRADE;
            game.cur_market = &market;
            play_sound(assets.trade_open);
        }

        std::unordered_set<Item*> encountered_items;
        for(Item& item : assets.items) {
            if(encountered_items.count(&item) == 1) {
                continue;
            }
            encountered_items.insert(&item);

			// Replenish market stock
			MarketPrice* price;
			for(MarketPrice& p : market.prices) {
				if(p.item == &item) {
					price = &p;
					break;
				}
			}

			float supply = market_supply(game, market, *price);
			int acc = 1000;
			int supply_acc = supply * acc;
			int goods_added = irand(supply_acc - acc, supply_acc) * item.average_movement;
			goods_added = goods_added / acc;

            if(goods_added > 0) {
                for(int i = 0; i < goods_added; i++) {
                    market.inventory.push_back(&item);
                }
            } else {
                for(int i = 0; i < abs(goods_added); i++) {
                    for(int j = 0; j < market.inventory.size(); j++) {
                        if(market.inventory[j] == &item) {
                            market.inventory.erase(market.inventory.begin() + j);
                        }
                    }
                }
            }
 

			/* Get count of item in market inventory
            int market_count = 0;
            for(Item* market_item : market.inventory) {
                if(market_item == &item) {
                    market_count++;
                }
            }
            */

            /* RESPOND TO SUPPLY
            // negative supply_delta values mean we have too many of an item
            int supply_delta = 0;
            for(MarketPrice& price : market.prices) {
                if(price.item != &item) {
                    continue;
                }

                if(market_count > 0) {
                    int supply_delta = item.average_movement - market_count;
                }

                int supply_delta_edge = 10;
                if(supply_delta < -supply_delta_edge) supply_delta = -supply_delta_edge;
                if(supply_delta < supply_delta_edge) supply_delta = supply_delta_edge;

                int supply_target = std::lerp(0, 1, (supply_delta + 10) / (supply_delta_edge * 2));
                price.supply = std::lerp(price.supply, supply_target, SUPPLY_RESPONSE_AGGRESSION);
            }
            */

            /* New code which moves qtys regardless of supply
            int max_movement = item.average_movement / 3;
            if(max_movement <= 0) {
                max_movement = 1;
            }
            int change_delta = irand(-max_movement, max_movement);
            if(market_count < item.average_movement && change_delta <= 0 && item.type == ItemType::FOOD) {
                change_delta += max_movement;
            }

            if(change_delta > 0) {
                for(int i = 0; i < change_delta; i++) {
                    market.inventory.push_back(&item);
                }
            } else {
                for(int i = 0; i < abs(change_delta); i++) {
                    for(int j = 0; j < market.inventory.size(); j++) {
                        if(market.inventory[j] == &item) {
                            market.inventory.erase(market.inventory.begin() + j);
                        }
                    }
                }
            }
            */

            /* Old code which moves qtys based on supply
            // the amount to be added or removed
            int change_count = (rand() % (abs(supply_delta) * 2)) - supply_delta;
            if(change_count >= 0) {
                // adding items
                for(int i = 0; i < change_count; i++) {
                    market.inventory.push_back(&item);
                }
            } else {
                // removing items
                for(int i = 0; i < change_count; i++) {
                    for(int j = 0; j < market.inventory.size(); j++) {
                        if(market.inventory[j] == &item) {
                            market.inventory.erase(market.inventory.begin() + j);
                        }
                    }
                }
            } 
            */
        }
    }

    for(Oasis& oasis : game.oases) {
        if(oasis.x == game.caravan_x && oasis.y == game.caravan_y) {
            game.waters = game.caravaneers * WATER_PER_CARAVANEER;
            play_sound(assets.oasis);
            game.state = GameState::OASIS_MSG;
            return;
        }
    }

    if(game.state == GameState::READY) {
        if(ready_game_over) {
            game.caravaneers--;
            if(game.caravaneers < 1) {
                ready_game_over = true;
                game.state = GameState::GAME_OVER;
                play_sound(assets.game_over);
                return;
            }
        }

        if(!try_bandit(game, assets)) {   
            play_sound(assets.step);
        }
    }

    // Treasure
    for(int i = 0; i < game.treasures.size(); i++) {
        Treasure& treasure = game.treasures[i];
        if(!treasure.active || treasure.x != game.caravan_x || treasure.y != game.caravan_y) {
            continue;
        }

        treasure.active = false;
        game.state = GameState::TREASURE;
        game.treasured_gold = rand() % MAX_TREASURE_GOLD;
        game.money += game.treasured_gold;
        play_sound(assets.treasure);
    }

    explore_tiles(game.explored_tiles, game.caravan_x, game.caravan_y);
}
