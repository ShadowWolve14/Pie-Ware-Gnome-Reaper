//
// Created by $Will on 27.08.2025.
//

#ifndef HEALTHPOTION_H
#define HEALTHPOTION_H

#include "ItemBase.h"

class HealthPotion : public ItemBase
{
private:
    int heal_amount;
    Sound us= LoadSound("assets/audio/sfx/Healpotion_Used.wav");
public:
    HealthPotion(Vector2 position, ItemType type, const char* anim_path, int heal_amount);

    HealthPotion(Vector2 position);

    void Activate(Player_Base_Class* player) override;
    std::string GetName() const override;
};



#endif //HEALTHPOTION_H
