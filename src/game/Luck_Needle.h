//
// Created by Kruse on 17/09/2025.
//

#ifndef RAYLIBSTARTER_LUCK_NEEDLE_H
#define RAYLIBSTARTER_LUCK_NEEDLE_H
#include "ItemBase.h"

class Luck_Needle : public ItemBase{

private:
    int duration;
    Sound us= LoadSound("assets/audio/sfx/Healpotion_Used.wav");
public:
    Luck_Needle(Vector2 position, ItemType type, const char* anim_path, int duration);

    Luck_Needle(Vector2 position);

    void Activate(Player_Base_Class* player) override;
    std::string GetName() const override;
};


#endif //RAYLIBSTARTER_LUCK_NEEDLE_H
