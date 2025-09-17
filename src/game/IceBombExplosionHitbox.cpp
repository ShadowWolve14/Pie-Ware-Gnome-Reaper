//
// Created by $Will on 17.09.2025.
//
#include "IceBombExplosionHitbox.h"
#include "EnemyBaseClass.h"
#include "HourglassWall.h"
#include "../scenes/GameScene.h"
#include "../Config.h.in"

IceBombExplosionHitbox::IceBombExplosionHitbox(Rectangle rect, game::scenes::GameScene* game_scene)
    : BombExplosionHitbox(rect, 0), scene_ptr(game_scene) {}

void IceBombExplosionHitbox::On_Collision(Collidable* other)
{

    if (auto* enemy = dynamic_cast<enemy::Enemy_Base_Class*>(other))
    {
        enemy->ApplyFreeze(game::Config::kIceBombFreezeDuration);
    }
    if (auto* hourglass = dynamic_cast<HourglassWall*>(other))
    {
        if (!game::Config::kIceBombFreezesTimerGlobally && scene_ptr)
        {
            hourglass->ApplyFreeze(game::Config::kIceBombFreezeDuration);
            scene_ptr->FreezeWaveTimer(game::Config::kIceBombFreezeDuration);
        }
    }
}
