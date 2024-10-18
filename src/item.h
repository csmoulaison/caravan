#pragma once
#include <string>
#include "sprite.h"

enum class ItemType {
    FOOD,
    WEAPON,
    ARMOR,
    TRADE,
    CAMEL
};

struct Item {
    ItemType type;
    int weight;
    int inherent_value;
    int average_movement;
    int producer_count;
    std::string name;
    std::string description;
    Sprite* sprite;   
};
