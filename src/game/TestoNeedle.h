//
// Created by $Will on 27.08.2025.
//

#ifndef TESTONEEDLE_H
#define TESTONEEDLE_H


#include "ItemBase.h"

class TestoNeedle : public ItemBase
{
private:
    Sound us= LoadSound("assets/audio/sfx/Testoneadle_Used.wav");
public:
    TestoNeedle(Vector2 position, bool has_despawn_timer = true);
    void Activate(Player_Base_Class* player) override;
};

#endif //TESTONEEDLE_H
