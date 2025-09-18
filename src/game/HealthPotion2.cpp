//
// Created by $Will on 16.09.2025.
//

#include "HealthPotion2.h"
#include "../Config.h.in"

HealthPotion2::HealthPotion2(Vector2 position)
    : HealthPotion(position, ItemType::HEALTH_POTION_2, game::Config::kHealthPotion2Anim, game::Config::health_Potion_2_Heal_Amount)
    {this->score_value=game::Config::health_Potion_2_value;}

std::string HealthPotion2::GetName() const
{
    return "Heilung +1";
}