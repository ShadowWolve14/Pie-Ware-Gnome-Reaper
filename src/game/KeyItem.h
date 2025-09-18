//
// Created by $Will on 27.08.2025.
//

#ifndef KEYITEM_H
#define KEYITEM_H
#include "ItemBase.h"


class KeyItem : public ItemBase
{
private:

public:
    KeyItem(Vector2 position);
    void Activate(Player_Base_Class* player) override;
    std::string GetName() const override;
};

#endif //KEYITEM_H
