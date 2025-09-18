//
// Created by $Will on 17.09.2025.
//
#include "IceBombItem.h"
#include "PlayerBaseClass.h"
#include "Object_Manager.h"
#include "IceBombExplosionHitbox.h"
#include "../scenes/GameScene.h"
#include "../Config.h.in"
#include "HourglassWall.h"
#include "Store.h"

IceBombItem::IceBombItem(Vector2 position)
    : ItemBase(position, ItemType::ICE_BOMB, game::Config::kIceBombItemAnim, true,
               game::Config::bomb_Item_Anim_Size, game::Config::bomb_Item_Frame_Count, game::Config::bomb_Item_Anim_Speed)
{
}

std::string IceBombItem::GetName() const
{
    return "Eisbombe";
}

void IceBombItem::Activate(Player_Base_Class* player)
{
    if (!player || !player->object_manager_ptr || !player->scene_ptr) return;
    PlaySound(us);
    SetSoundVolume(us, (game::core::Store::volume*game::Config::IceBomb_Sound_Volume));
    if (game::Config::kIceBombFreezesTimerGlobally)
    {
        player->scene_ptr->FreezeWaveTimer(game::Config::kIceBombFreezeDuration);
        for (auto* obj : player->object_manager_ptr->managed_objects) {
            if (auto* hourglass = dynamic_cast<HourglassWall*>(obj)) {
                hourglass->ApplyFreeze(game::Config::kIceBombFreezeDuration);
                break;
            }
        }
    }

    Vector2 player_center = player->Get_Player_Center();
    const float TILE_SIZE = 16.0f;
    const float radius_in_pixels = game::Config::bomb_Radius_Tiles * TILE_SIZE;
    const float FATNESS_IN_TILES = (game::Config::bomb_Radius_Tiles * 2.0f) - 3.0f;
    const float arm_width = FATNESS_IN_TILES * TILE_SIZE;
    const float arm_half_width = arm_width / 2.0f;

    Rectangle center_box = { player_center.x - arm_half_width, player_center.y - arm_half_width, arm_width, arm_width };
    const float arm_length = radius_in_pixels - arm_half_width;
    Rectangle top_box = { player_center.x - arm_half_width, player_center.y - radius_in_pixels, arm_width, arm_length };
    Rectangle bottom_box = { player_center.x - arm_half_width, player_center.y + arm_half_width, arm_width, arm_length };
    Rectangle left_box = { player_center.x - radius_in_pixels, player_center.y - arm_half_width, arm_length, arm_width };
    Rectangle right_box = { player_center.x + arm_half_width, player_center.y - arm_half_width, arm_length, arm_width };

    player->object_manager_ptr->AddObjectDeferred(new IceBombExplosionHitbox(center_box, player->scene_ptr));
    player->object_manager_ptr->AddObjectDeferred(new IceBombExplosionHitbox(top_box, player->scene_ptr));
    player->object_manager_ptr->AddObjectDeferred(new IceBombExplosionHitbox(bottom_box, player->scene_ptr));
    player->object_manager_ptr->AddObjectDeferred(new IceBombExplosionHitbox(left_box, player->scene_ptr));
    player->object_manager_ptr->AddObjectDeferred(new IceBombExplosionHitbox(right_box, player->scene_ptr));

    player->item_removal_timer = game::Config::bomb_Explosion_Damage_Lifetime + 0.1f;
    this->Mark_For_Destruction();
}