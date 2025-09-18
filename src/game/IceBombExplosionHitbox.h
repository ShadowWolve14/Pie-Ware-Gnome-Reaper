//
// Created by $Will on 17.09.2025.
//

#ifndef ICEBOMBEXPLOSIONHITBOX_H
#define ICEBOMBEXPLOSIONHITBOX_H
#include "BombExplosionHitbox.h"

namespace game::scenes { class GameScene; }

class IceBombExplosionHitbox : public BombExplosionHitbox {
public:
    IceBombExplosionHitbox(Rectangle rect, game::scenes::GameScene* game_scene);
    static void LoadAssets();
    static void UnloadAssets();
    void Tick(float delta_time) override;
    void Draw() override;

    void On_Collision(Collidable* other) override;

private:
    game::scenes::GameScene* scene_ptr;
    Animations ice_animation;
    static Texture2D ice_explosion_texture;
};
#endif //ICEBOMBEXPLOSIONHITBOX_H
