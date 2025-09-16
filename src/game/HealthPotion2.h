//
// Created by $Will on 16.09.2025.
//

#ifndef HEALTHPOTION2_H
#define HEALTHPOTION2_H

#include "HealthPotion.h"

class HealthPotion2 : public HealthPotion
{
public:
    HealthPotion2(Vector2 position);
    std::string GetName() const override;
};

#endif //HEALTHPOTION2_H
