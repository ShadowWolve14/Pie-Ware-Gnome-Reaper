//
// Created by $Will on 27.08.2025.
//

#ifndef FAIRYITEM_H
#define FAIRYITEM_H
#include "ItemBase.h"

class FairyItem : public ItemBase
{
    RepeatAnimation aura_vfx;
public:
    FairyItem(Vector2 position, int current_level);

    void Tick(float delta_time) override;
    void Draw() override;

    void Activate(Player_Base_Class* player) override;
    std::string GetName() const override;
};


#endif //FAIRYITEM_H
