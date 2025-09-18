//
// Created by $Will on 17.09.2025.
//
#include "AdrenalineNeedle.h"
#include "PlayerBaseClass.h"
#include "../Config.h.in"
#include "Store.h"

AdrenalineNeedle::AdrenalineNeedle(Vector2 position, bool has_despawn_timer)
    : ItemBase(position, ItemType::ADRENALINE_NEEDLE, game::Config::kAdrenalineNeedleAnim, has_despawn_timer,
               game::Config::adrenaline_Needle_Anim_Size, game::Config::adrenaline_Needle_Frame_Count, game::Config::adrenaline_Needle_Anim_Speed)
{
    SetSoundVolume(us, game::core::Store::volume*game::Config::Adrenalin_Sound_Volume);
    this->score_value=game::Config::adrenaline_Needle_value;
}

void AdrenalineNeedle::Activate(Player_Base_Class* player)
{
    if (!player) return;
    PlaySound(us);
    player->ApplyAdrenalineBuff();
}

std::string AdrenalineNeedle::GetName() const
{
    return "Adrenalin";
}
