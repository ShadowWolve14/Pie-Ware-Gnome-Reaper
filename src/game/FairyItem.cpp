//
// Created by $Will on 27.08.2025.
//

#include "FairyItem.h"
#include "PlayerBaseClass.h"
#include "Store.h"
#include "../Config.h.in"

FairyItem::FairyItem(Vector2 position, int current_level)
    : ItemBase(position, ItemType::FAIRY, "", false, {0,0}, 0, 0),
      aura_vfx(
          game::Config::fairy_Aura_VFX_Anim_Size,
          game::Config::kFairyAuraVFXAnim,
          game::Config::fairy_Aura_VFX_Frame_Count,
          game::Config::fairy_Aura_VFX_Frame_Count,
          game::Config::fairy_Aura_VFX_Anim_Speed
      ),
      collected_in_level(current_level)
{
    const char* anim_path;
    Vector2 item_size;

    switch (current_level)
    {
        case 1:
            anim_path = game::Config::kFairyAnimLvl1;
        item_size = game::Config::fairy_Anim_Size_Lvl1;
        break;
        case 2:
            anim_path = game::Config::kFairyAnimLvl2;
        item_size = game::Config::fairy_Anim_Size_Lvl2;
        break;
        case 3:
            anim_path = game::Config::kFairyAnimLvl3;
        item_size = game::Config::fairy_Anim_Size_Lvl3;
        break;
        default:
            anim_path = game::Config::kFairyAnimLvl1;
        item_size = game::Config::fairy_Anim_Size_Lvl1;
        break;
    }

    Vector2 centered_pos = {
        game::Config::fairy_Spawn_Position.x - item_size.x / 2.0f,
        game::Config::fairy_Spawn_Position.y - item_size.y / 2.0f
    };
    this->hitbox.x = centered_pos.x;
    this->hitbox.y = centered_pos.y;
    this->hitbox.width = item_size.x;
    this->hitbox.height = item_size.y;

    this->animation = RepeatAnimation(item_size, anim_path,
        game::Config::fairy_Frame_Count, game::Config::fairy_Frame_Count, game::Config::fairy_Anim_Speed);
}

void FairyItem::Tick(float delta_time)
{
    ItemBase::Tick(delta_time);
    aura_vfx.Update_Frame(delta_time);
}

void FairyItem::Draw()
{
    Vector2 item_center = {
        this->hitbox.x + this->hitbox.width / 2.0f,
        this->hitbox.y + this->hitbox.height / 2.0f
    };
    Vector2 vfx_pos = {
        item_center.x - game::Config::fairy_Aura_VFX_Anim_Size.x / 2.0f,
        item_center.y - game::Config::fairy_Aura_VFX_Anim_Size.y / 2.0f
    };
    Color vfx_tint = { 255, 255, 255, (unsigned char)game::Config::fairy_Aura_VFX_Transparency };
    aura_vfx.Draw_Current_Frame(vfx_pos, vfx_tint);

    ItemBase::Draw();
}
void FairyItem::Activate(Player_Base_Class* player)
{
    if (!player) return;

    PlaySound(LoadSound("assets/audio/sfx/Item_Obtained.wav"));

    player->SetHasFairy(true);
    game::core::Store::player_state->fairy_collected_in_level = this->collected_in_level;
    this->Mark_For_Destruction();
}
std::string FairyItem::GetName() const
{
    return "Fairy Item";
}