//
// Created by $Will on 17.09.2025.
//

#ifndef ADRENALINENEEDLE_H
#define ADRENALINENEEDLE_H

#include "ItemBase.h"

class AdrenalineNeedle : public ItemBase
{
private:
    Sound us = LoadSound("assets/audio/sfx/Testoneadle_Used.wav");

public:
    AdrenalineNeedle(Vector2 position, bool has_despawn_timer = true);
    void Activate(Player_Base_Class* player) override;
    std::string GetName() const override;
};

#endif //ADRENALINENEEDLE_H
