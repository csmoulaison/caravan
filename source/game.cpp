#include "game.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "game_ready_state.h"
#include "game_over_state.h"
#include "handle_game_trade.h"
#include "rand.h"

#define MIN_DIST_MARKETS 8
#define MIN_DIST_OASES 6
#define DEBUG_EXPLORE_ALL true

void init_game(Game& game, Assets& assets) {
    game.state = GameState::READY;
    random_position(game.caravan_x, game.caravan_y);
    game.caravaneers = 1;
    game.waters = game.caravaneers * WATER_PER_CARAVANEER;
    game.money = 100;
    game.cur_market = nullptr;
    game.cam_x = -112;
    game.cam_y = 0;
    game.selector_x = 0;
    game.selector_y = 0;
    game.stolen_bandit_money = 0;

    for(int i = 0; i < 24; i++) {
		game.inventory.push_back(&assets.items[0]);
    }
    for(int i = 0; i < 3; i++) {
		game.inventory.push_back(&assets.items[2]);
    }

    game.markets.clear();
	for(int i = 0; i < 7; i++) {
	    generate_market(game, assets, MarketType::CITY);
    }
    for(int i = 0; i < 7; i++) {
	    generate_market(game, assets, MarketType::CARAVANSERAI);
    }
    game.oases.clear();
    for(int i = 0; i < 12; i++) {
	    Oasis oasis;
	    random_spawn(oasis.x, oasis.y, game, 3, 8, 2, 10000, false);
	    game.oases.push_back(oasis);
    }
    game.treasures.clear();
    for(int i = 0; i < 8; i++) {
	    Treasure treasure;
	    random_spawn(treasure.x, treasure.y, game, 3, 3, 8, 1000, true);
	    game.treasures.push_back(treasure);
    }
    game.rocks.clear();
    /* put in some rocks? */

	for(Item& item : assets.items) {
		for(int i = 0; i < item.producer_count; i++) {
			Producer producer;
			producer.item = &item;
			random_spawn(producer.x, producer.y, game, 1, 1, 1, 1000, false);
			game.producers.push_back(producer);
		}
	}
     
    game.explored_tiles.clear();
    explore_tiles(game.explored_tiles, game.caravan_x, game.caravan_y);
    if(DEBUG_EXPLORE_ALL) {
        for(int i = 0; i < GAME_WIDTH * GAME_HEIGHT; i++) {
            game.explored_tiles.insert(i);
        }
    }
}

void update_game(Game& game, Input& input, Assets& assets, DrawContext& draw_context) {
    bool bang_reset = false;
    int curx;
    int cury;

    switch(game.state) {
    case GameState::READY:
        handle_game_ready(game, assets, input);
        draw_set_bg_tan(draw_context);
        draw_game_ready(game, input, assets, draw_context);
        draw_inventory_ui(draw_context, input, game.inventory, 0);
        draw_player_header_ui(game, assets, draw_context);
        break;
    case GameState::TRADE:
        draw_game_trade(game, input, assets, draw_context);
        break;
    case GameState::OASIS_MSG:
        if(input.select.just_pressed) {
            game.state = GameState::READY;
            playSoundFromMemory(assets.step, VOLUME_MID);
        }
        draw_set_bg_tan(draw_context);
        draw_game_ready(game, input, assets, draw_context);
        draw_inventory_ui(draw_context, input, game.inventory, 0);
        draw_player_header_ui(game, assets, draw_context);

        draw_dialog(draw_context, DialogStyle::SOFT, 144, 80, 164, 19);
        draw_text(draw_context, "FOUND AN OASIS", Font::BIG, 148, 82, nullptr);
        draw_text(draw_context, "YOUR WATER SUPPLY HAS BEEN REPLENISHED", Font::SMALL, 148, 91, nullptr);
        break;
    case GameState::BANDITS:
        if(input.select.just_pressed) {
            game.state = GameState::READY;
            playSoundFromMemory(assets.step, VOLUME_MID);
        }
        draw_set_bg_tan(draw_context);
        draw_game_ready(game, input, assets, draw_context);
        draw_inventory_ui(draw_context, input, game.inventory, 0);
        draw_player_header_ui(game, assets, draw_context);
        draw_dialog(draw_context, DialogStyle::SOFT, 144, 80, 164, 25);
        draw_text(draw_context, "BANDIT ATTACK", Font::BIG, 148, 82, nullptr);
        draw_text(draw_context, "YOU HAVE BEEN ATTACKED BY BANDITS", Font::SMALL, 148, 91, nullptr);
        curx = 148;
        draw_text(draw_context, "THEY STOLE ", Font::SMALL, curx, 97, &curx);
        draw_text(draw_context, std::to_string(game.stolen_bandit_money), Font::SMALL, curx, 97, &curx);
        draw_text(draw_context, " COINS", Font::SMALL, curx, 97, &curx);
        break;
    case GameState::TREASURE:
        if(input.select.just_pressed) {
            game.state = GameState::READY;
            playSoundFromMemory(assets.step, VOLUME_MID);
        }
        draw_set_bg_tan(draw_context);
        draw_game_ready(game, input, assets, draw_context);
        draw_inventory_ui(draw_context, input, game.inventory, 0);
        draw_player_header_ui(game, assets, draw_context);

        draw_dialog(draw_context, DialogStyle::SOFT, 144, 80, 164, 19);
        draw_text(draw_context, "FOUND A TREASURE CHEST", Font::BIG, 148, 82, nullptr);
        curx = 148;
        draw_text(draw_context, "YOU RETRIEVED ", Font::SMALL, curx, 91, &curx);
        draw_text(draw_context, std::to_string(game.treasured_gold), Font::SMALL, curx, 91, &curx);
        draw_text(draw_context, " GOLD", Font::SMALL, curx, 91, &curx);
        break;
    case GameState::GAME_OVER:
        if(game.waters < 0) {
            game.waters = 0;
        }

        if(input.select.just_pressed) {
            init_game(game, assets);
            game.state = GameState::READY;
            playSoundFromMemory(assets.step, VOLUME_MID);
        }
        draw_set_bg_tan(draw_context);
        draw_game_ready(game, input, assets, draw_context);
        draw_inventory_ui(draw_context, input, game.inventory, 0);
        draw_player_header_ui(game, assets, draw_context);

        draw_dialog(draw_context, DialogStyle::SOFT, 144, 80, 64, 13);
        draw_text(draw_context, "GAME OVER", Font::BIG, 148, 82, nullptr);
        break;
    default:
        break;
    }
}

int food_count(Game& game) {
    int count = 0;
    for(Item* item : game.inventory) {
        if(item->type == ItemType::FOOD) {
            count++;
        }
    }
    return count;
}

int weight_capacity(Game& game) {
    return game.caravaneers * CAPACITY_PER_CARAVANEER;
}

int weight_current(Game& game) {
    int weight = 0;
    for(Item* item : game.inventory) {
        weight += item->weight;
    }
    return weight;
}

float market_supply(Game& game, Market& market, MarketPrice& price) {
	const int max_dist = 12;
	
	int producer_count = 0;
	for(Producer& producer : game.producers) {
		if(producer.item == price.item) {
			producer_count++;
		}
	}
	
	float supply = 0;
	for(Producer& producer : game.producers) {
		if(producer.item != price.item) {
			continue;
		}

		int dist = distance(producer.x, producer.y, market.x, market.y);
		if(dist > max_dist) {
			dist = max_dist;
		}
		// OLD VERSION: Why did I decide that demand should factor into supply/replenishment at all?
		// Demand ought to be an orthogonal factor determining price only.
		//supply += (((float)(max_dist - dist) / (float)max_dist) * (price.demand * 2)) / (float)producer_count;
		// Therefore:
		supply += ((max_dist - dist) / (float)max_dist) / (float)producer_count;
	}
	if(supply > 1) {
		supply = 1;
	}
	std::cout << "Supply: " <<  supply << std::endl;

	return supply;
}

void generate_market(Game& game, Assets& assets, MarketType type) {
    Market market;
    market.type = type;
    random_spawn(market.x, market.y, game, MIN_DIST_MARKETS, MIN_DIST_OASES, 1, 1000, true);

    for(Item& item : assets.items) {
	    int producer_count = 0;
	    int producer_total_dists = 0;
		for(Producer& producer : game.producers) {
			if(producer.item != &item) {
				continue;
			}

			producer_count++;
			producer_total_dists += distance(market.x, market.y, producer.x, producer.y);
		}
	    
        market.prices.push_back({
            &item,
            rand() / (float)RAND_MAX,
            0,
        });

		// Do an initial qty
	    if(market.prices[market.prices.size() - 1].demand > 0.33) {
	        int qty = 0;
	        for(int i = 0; i < (rand() % 20) * market.prices[market.prices.size() - 1].demand; i++) {
	            qty += rand() % item.average_movement;
	        }

	        for(int i = 0; i < qty; i++) {
	            market.inventory.push_back(&item);
	        }
	    }
    }

    market.name = &assets.market_names[rand() % assets.market_names.size() - 1];
    game.markets.push_back(market);
}

// Returns a random spawn point, attempting to find a point which fulfills the given criteria. Increasing iterations effectively increases the uniformity of the distribution, as the min_dist lowers throughout the course of the iterations.
void random_spawn(int& out_x, int& out_y, Game& game, int min_dist_market, int min_dist_oasis, int min_dist_treasure, int iterations, bool centrality_bias) {
    if(iterations < 1) {
        std::cout << "0 iterations given for random spawn. This has no effect" << std::endl;
        return;
    }

    // Poorly named. These are the i value in the iterations which makes the min distance iterate downward
    int min_dist_threshold_market   = iterations / (min_dist_market);
    int min_dist_threshold_oasis    = iterations / (min_dist_oasis);
    int min_dist_threshold_treasure = iterations / (min_dist_treasure);

    bool eligible_position = true;
    for(int i = 0; i < iterations; i++) {
        if(i % (iterations / min_dist_threshold_market)   == 0) min_dist_market--;
        if(i % (iterations / min_dist_threshold_oasis)    == 0) min_dist_oasis--;
        if(i % (iterations / min_dist_threshold_treasure) == 0) min_dist_treasure--;

        eligible_position = true;
        random_position(out_x, out_y);

        for(Market& other : game.markets) {
            if(distance(out_x, out_y, other.x, other.y) < min_dist_market) {
                eligible_position = false;
                break;
            }
        }
        for(Oasis& other : game.oases) {
            if(distance(out_x, out_y, other.x, other.y) < min_dist_oasis) {
                eligible_position = false;
                break;
            }
        }
        for(Treasure& other : game.treasures) {
            if(distance(out_x, out_y, other.x, other.y) < min_dist_treasure) {
                eligible_position = false;
                break;
            }
        }
        if(centrality_bias && distance(out_x, out_y, GAME_WIDTH / 2, GAME_HEIGHT / 2) > irand(0, GAME_WIDTH)) {
            eligible_position = false;
        }

        if(eligible_position) {
            break;
        }
    }

    if(!eligible_position) {
        out_x = -99;
        out_y = -99;
    }
}

// Returns a random position between two points
void random_position(int& out_x, int& out_y) {
    out_x = irand(0, GAME_WIDTH);
    out_y = irand(0, GAME_HEIGHT);
}

// Returns the traversal (paralell movement only) distance between two points
int distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

