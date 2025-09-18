//
// Created by Kruse on 17/09/2025.
//

#include "Luck_Needle.h"
//
// Created by $Will on 27.08.2025.
//
#include "PlayerBaseClass.h"
#include "Store.h"
#include "../Config.h.in"

Luck_Needle::Luck_Needle(Vector2 position, ItemType type, const char* anim_path, int p_heal_amount)
        : ItemBase(position, type, anim_path, true,
                   game::Config::Luck_Needle_Anim_Size, game::Config::Luck_Needle_Frame_Count, game::Config::Luck_Needle_Anim_Speed),
          duration(p_heal_amount)  { }

Luck_Needle::Luck_Needle(Vector2 position)
        : Luck_Needle(position, ItemType::LUCK_NEEDLE, game::Config::kLuck_Needle_Anim, game::Config::Luck_Needle_duration) {}

void Luck_Needle::Activate(Player_Base_Class* player)
{
    PlaySound(us);
    SetSoundVolume(us, game::core::Store::volume*game::Config::Luck_Sound_Volume);
    game::core::Store::drop_chance=100;
    game::core::Store::drop_chance_change_duration=duration;
    player->RemoveHeldItem();
}
std::string Luck_Needle::GetName() const
{
    return "Glück";
}