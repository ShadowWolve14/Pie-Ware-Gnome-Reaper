//
// Created by $Will on 27.08.2025.
//

#ifndef BOMBEXPLOSIONHITBOX_H
#define BOMBEXPLOSIONHITBOX_H

#include "../game/Collidable.h"
#include "../core/Animations.h"

class BombExplosionHitbox : public Collidable
{
private:
    int damage;
    float lifetime;
    float damage_active_timer;
    Animations animation;
    static Texture2D explosion_texture;

public:
    BombExplosionHitbox(Rectangle rect, int dmg);
    ~BombExplosionHitbox() override = default;
    static void LoadAssets();
    static void UnloadAssets();
    void Tick(float delta_time) override;
    void On_Collision(Collidable* other) override;
    void Draw() override;

    Collision_Type Get_Collision_Type() const override { return Collision_Type::PLAYER_PROJECTILE; }
};

#endif //BOMBEXPLOSIONHITBOX_H
