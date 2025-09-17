//
// Created by $Will on 17.09.2025.
//

#ifndef ICEBOMBITEM_H
#define ICEBOMBITEM_H
#include "ItemBase.h"


class IceBombItem : public ItemBase {
public:
    IceBombItem(Vector2 position);
    void Activate(Player_Base_Class* player) override;
    std::string GetName() const override;

private:
    Sound us = LoadSound("assets/audio/sfx/Bomb_detonation.wav");
};

#endif //ICEBOMBITEM_H
