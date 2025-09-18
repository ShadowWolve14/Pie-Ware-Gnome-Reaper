//
// Created by $Will on 16.09.2025.
//

#include "HealthPotion3.h"
#include "../Config.h.in"

HealthPotion3::HealthPotion3(Vector2 position)
    : HealthPotion(position, ItemType::HEALTH_POTION_3, game::Config::kHealthPotion3Anim, game::Config::health_Potion_3_Heal_Amount) { }
std::string HealthPotion3::GetName() const
{
    return "Heilung +2";
}