//
// Created by $Will on 09.09.2025.
//
#include "DisappearingWall.h"
#include "../Config.h.in"
#include "CollisionResponse.h"
#include "ItemBase.h"

DisappearingWall::DisappearingWall(Vector2 position)
    : wall_texture(LoadTexture(game::Config::disappearing_wall_spritesheet)),
      disappearing_animation(
          game::Config::disappearing_wall_anim_size,
          game::Config::disappearing_wall_anim_path,
          game::Config::disappearing_wall_anim_frame_count,
          game::Config::disappearing_wall_anim_frame_count,
          game::Config::disappearing_wall_anim_speed
      ),
      sprite_position(position),
      is_disappearing(false),
      disappearing_timer(0.0f)
{
    this->hitbox = {
        position.x,
        position.y,
        game::Config::disappearing_wall_sprite_size.x,
        game::Config::disappearing_wall_sprite_size.y
    };

    float hitbox_center_x = this->hitbox.x + (this->hitbox.width / 2.0f);
    float hitbox_center_y = this->hitbox.y + (this->hitbox.height / 2.0f);

    this->wall_draw_position.x = hitbox_center_x - (this->wall_texture.width / 2.0f);
    this->wall_draw_position.y = hitbox_center_y - (this->wall_texture.height / 2.0f);

    this->animation_draw_position.x = hitbox_center_x - (game::Config::disappearing_wall_anim_size.x / 2.0f);
    this->animation_draw_position.y = hitbox_center_y - (game::Config::disappearing_wall_anim_size.y / 2.0f);
}

DisappearingWall::~DisappearingWall()
{
    UnloadTexture(wall_texture);
}

Collision_Type DisappearingWall::Get_Collision_Type() const
{
    return Collision_Type::DISAPPEARING_WALL;
}

void DisappearingWall::Tick(float delta_time)
{
    if (is_disappearing)
    {
        disappearing_animation.Update_Frame(delta_time);
        disappearing_timer -= delta_time;
        if (disappearing_timer <= 0.0f)
        {
            Mark_For_Destruction();
        }
    }
}

void DisappearingWall::Draw()
{
    if (is_disappearing)
    {
        disappearing_animation.Draw_Current_Frame(this->animation_draw_position);
    }
    else
    {
        DrawTextureV(this->wall_texture, this->wall_draw_position, WHITE);
    }
}

void DisappearingWall::On_Collision(Collidable* other)
{
    if (is_disappearing) return;

    if (other->Get_Collision_Type() == Collision_Type::PLAYER)
    {
        auto* player = static_cast<Player_Base_Class*>(other);

        if (player->HasItem() && player->GetHeldItem()->GetType() == ItemType::KEY)
        {
            player->GetHeldItem()->Activate(player);
            is_disappearing = true;
            float duration = 0.0f;
            if (game::Config::disappearing_wall_anim_speed > 0) {
                duration = static_cast<float>(game::Config::disappearing_wall_anim_frame_count) / game::Config::disappearing_wall_anim_speed;
            }
            disappearing_timer = duration;
        }
        else
        {
            CollisionResponse::Resolve_Overlap(player, this);
        }
    }
}

float DisappearingWall::GetYSortPosition() const
{
    return this->sprite_position.y + game::Config::disappearing_wall_sprite_size.y;
}
