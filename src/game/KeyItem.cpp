//
// Created by $Will on 27.08.2025.
//

#include "KeyItem.h"
#include "PlayerBaseClass.h"
#include "../Config.h.in"
#include "Store.h"

KeyItem::KeyItem(Vector2 position)
        : ItemBase(position, ItemType::KEY, game::Config::kKeyAnim, false,
                   game::Config::key_Anim_Size, game::Config::key_Frame_Count, game::Config::key_Anim_Speed)
                   {this->score_value=game::Config::key_value; }

void KeyItem::Activate(Player_Base_Class* player)
{
    if (player)
    {
        player->SetHasKey(true);
    }
}

std::string KeyItem::GetName() const
{
    return "Schlüssel";
}