//
// Created by $Will on 16.09.2025.
//

#ifndef HEALTHPOTION3_H
#define HEALTHPOTION3_H

#include "HealthPotion.h"

class HealthPotion3 : public HealthPotion
{
public:
    HealthPotion3(Vector2 position);
    std::string GetName() const override;
};

#endif //HEALTHPOTION3_H
