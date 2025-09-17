//
// Created by $Will on 27.08.2025.
//

#include "FairyItem.h"
#include "PlayerBaseClass.h"
#include "../Config.h.in"

FairyItem::FairyItem(Vector2 position, int current_level)
    : ItemBase(position, ItemType::FAIRY, "", false, {0,0}, 0, 0)
{
    const char* anim_path;
    Vector2 item_size;

    switch (current_level)
    {
        case 1:
            anim_path = game::Config::kFairyAnimLvl1;
        item_size = game::Config::fairy_Anim_Size_Lvl1;
        break;
        case 2:
            anim_path = game::Config::kFairyAnimLvl2;
        item_size = game::Config::fairy_Anim_Size_Lvl2;
        break;
        case 3:
            anim_path = game::Config::kFairyAnimLvl3;
        item_size = game::Config::fairy_Anim_Size_Lvl3;
        break;
        default:
            anim_path = game::Config::kFairyAnimLvl1;
        item_size = game::Config::fairy_Anim_Size_Lvl1;
        break;
    }

    Vector2 centered_pos = {
        game::Config::fairy_Spawn_Position.x - item_size.x / 2.0f,
        game::Config::fairy_Spawn_Position.y - item_size.y / 2.0f
    };
    this->hitbox.x = centered_pos.x;
    this->hitbox.y = centered_pos.y;
    this->hitbox.width = item_size.x;
    this->hitbox.height = item_size.y;

    this->animation = RepeatAnimation(item_size, anim_path,
        game::Config::fairy_Frame_Count, game::Config::fairy_Frame_Count, game::Config::fairy_Anim_Speed);
}

std::string FairyItem::GetName() const
{
    return "Fairy Item";
}