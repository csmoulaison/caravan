#pragma once
#include <vector>
#include <unordered_set>
#include "draw.h"
#include "item.h"
#include "input.h"

struct InventoryAction {
    int inventory_index = -1;
    int stacked_index = -1;
    bool pressed = false;
};

InventoryAction draw_inventory_ui(DrawContext& context, Input& input, std::vector<Item*>& inventory, int x);
