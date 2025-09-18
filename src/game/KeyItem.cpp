//
// Created by $Will on 27.08.2025.
//

#include "KeyItem.h"
#include "PlayerBaseClass.h"
#include "../Config.h.in"
#include "Store.h"

KeyItem::KeyItem(Vector2 position)
        : ItemBase(position, ItemType::KEY, game::Config::kKeyAnim, false, // false -> hat keinen Despawn-Timer
                   game::Config::key_Anim_Size, game::Config::key_Frame_Count, game::Config::key_Anim_Speed)
{
    SetSoundVolume(us,game::core::Store::volume*game::Config::Key_Sound_Volume);
}

void KeyItem::Activate(Player_Base_Class* player)
{
    PlaySound(us);
    player->RemoveHeldItem();
}

std::string KeyItem::GetName() const
{
    return "Schlüssel";
}