//
// Created by $Will on 17.09.2025.
//
#include "IceBombExplosionHitbox.h"
#include "EnemyBaseClass.h"
#include "HourglassWall.h"
#include "../scenes/GameScene.h"
#include "../Config.h.in"
Texture2D IceBombExplosionHitbox::ice_explosion_texture;

IceBombExplosionHitbox::IceBombExplosionHitbox(Rectangle rect, game::scenes::GameScene* game_scene)
    : BombExplosionHitbox(rect, 0),
      scene_ptr(game_scene),
      ice_animation(game::Config::bomb_Explosion_Tile_Size, ice_explosion_texture, game::Config::bomb_Explosion_Frame_Count,
                    game::Config::bomb_Explosion_Frame_Count, game::Config::bomb_Explosion_Anim_Speed) { }

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

void IceBombExplosionHitbox::LoadAssets()
{
    ice_explosion_texture = LoadTexture(game::Config::kIceBombExplosionAnim);
}

void IceBombExplosionHitbox::UnloadAssets()
{
    UnloadTexture(ice_explosion_texture);
}

void IceBombExplosionHitbox::Tick(float delta_time)
{
    BombExplosionHitbox::Tick(delta_time);
    ice_animation.Update_Frame(delta_time);
}

void IceBombExplosionHitbox::Draw()
{
    Texture2D sheet = ice_animation.GetSpritesheet();
    Rectangle source_rec = ice_animation.GetCurrentFrameRec();

    if (sheet.id <= 0 || source_rec.width == 0 || source_rec.height == 0) return;

    for (float y = hitbox.y; y < hitbox.y + hitbox.height; y += source_rec.height)
    {
        for (float x = hitbox.x; x < hitbox.x + hitbox.width; x += source_rec.width)
        {
            DrawTextureRec(sheet, source_rec, {x, y}, WHITE);
        }
    }
}
