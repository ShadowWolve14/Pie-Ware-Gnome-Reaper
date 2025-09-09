//
// Created by $Will on 09.09.2025.
//

#ifndef DISAPPEARINGWALL_H
#define DISAPPEARINGWALL_H

#include "Collidable.h"
#include "../core/RepeatAnimation.h"
#include "PlayerBaseClass.h"

class DisappearingWall : public Collidable
{
private:
    Texture2D wall_texture;
    RepeatAnimation disappearing_animation;
    Vector2 animation_draw_position;
    Vector2 wall_draw_position;
    Vector2 sprite_position;
    bool is_disappearing = false;
    float disappearing_timer = 0.0f;

public:
    explicit DisappearingWall(Vector2 position);
    ~DisappearingWall() override;

    Collision_Type Get_Collision_Type() const override;
    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(Collidable* other) override;

    float GetYSortPosition() const override;
};

#endif //DISAPPEARINGWALL_H
