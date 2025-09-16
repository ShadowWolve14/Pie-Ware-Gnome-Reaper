//
// Created by $Will on 27.08.2025.
//

#include "HealthPotion.h"
#include "PlayerBaseClass.h"
#include "Store.h"
#include "../Config.h.in"

HealthPotion::HealthPotion(Vector2 position, ItemType type, const char* anim_path, int p_heal_amount)
    : ItemBase(position, type, anim_path, true,
               game::Config::health_Potion_Anim_Size, game::Config::health_Potion_Frame_Count, game::Config::health_Potion_Anim_Speed),
      heal_amount(p_heal_amount)  { }

HealthPotion::HealthPotion(Vector2 position)
: HealthPotion(position, ItemType::HEALTH_POTION, game::Config::kHealthPotionAnim, game::Config::health_Potion_Heal_Amount) {}

void HealthPotion::Activate(Player_Base_Class* player)
{
    PlaySound(us);
    SetSoundVolume(us, game::core::Store::volume / 10.0f);
    player->Take_Damage(-this->heal_amount);
    player->RemoveHeldItem();
}
std::string HealthPotion::GetName() const
{
    return "Health";
}