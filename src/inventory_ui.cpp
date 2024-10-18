#include "inventory_ui.h"
#include <algorithm>

InventoryAction draw_inventory_ui(DrawContext& context, Input& input, std::vector<Item*>& inventory, int x) {
    draw_dialog(context, DialogStyle::BOXY, x, 0, 112, context.logical_height - 1);

    InventoryAction action;
    action.inventory_index = -1;
    action.stacked_index = -1;
    action.pressed = false;

    std::unordered_set<Item*> encountered_items;
    int yoff = 0;
    int stacked_count = 0;

    std::sort(inventory.begin(), inventory.end(), [](Item*& a, Item*& b) { 
        int i = 0;
        while(i < a->name.length() && i < b->name.length()) {
            if(a->name[i] != b->name[i]) {
                return (a->name[i] < b->name[i]);
            }
            i++;
        }
        return false;
    });
    for(int i = 0; i < inventory.size(); i++) {
        Item* item = inventory[i];
        if(encountered_items.count(item) == 1) {
            continue;
        }
        stacked_count++;
        encountered_items.insert(item);

        int count = 0;
        for(int j = i; j < inventory.size(); j++) {
            if(inventory[j] == item) {
                count++;
            }
        }

        draw_dialog(context, DialogStyle::LINE, x + 0, 22 + yoff, 112, 17);
        draw_sprite(context, *item->sprite, x + 0, 22 + yoff);
        if(count > 1) {
            draw_text(context, std::to_string(count), Font::SMALL, x + 2, 23 + 1 + yoff, nullptr);
        }
        int curx = x + 16;
        draw_text(context, item->name, Font::BIG, curx, 23 + yoff, &curx);
        curx = x + 94;
        draw_text(context, std::to_string(item->weight), Font::SMALL, curx, 23 + 3 + yoff, &curx);
        draw_text(context, "LB", Font::SMALL, curx, 23 + 3 + yoff, &curx);
        draw_text(context, item->description, Font::SMALL, x + 16, 23 + 9 + yoff, nullptr);
        if(input.mouse_x > x 
        && input.mouse_x < x + 112 
        && input.mouse_y > 22 + yoff 
        && input.mouse_y < 22 + 17 + yoff) {
            action.inventory_index = i;
            action.stacked_index = stacked_count - 1;
            if(input.mouse_down) {
                action.pressed = true;
            }
        }

        yoff += 15;
    }
    return action;
}
