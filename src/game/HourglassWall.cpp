//
// Created by $Will on 10.09.2025.
//
#include "HourglassWall.h"
#include "../Config.h.in"
#include "CollisionResponse.h"
#include <string>
#include <cmath>
#include <algorithm>

HourglassWall::HourglassWall(Vector2 position, int level)
{
    const char* spritesheet_path = nullptr;
    switch (level)
    {
        case 1:
            spritesheet_path = game::Config::hourglass_lvl1_spritesheet;
            break;
        case 2:
            spritesheet_path = game::Config::hourglass_lvl2_spritesheet;
            break;
        case 3:
            spritesheet_path = game::Config::hourglass_lvl3_spritesheet;
            break;
        default:
            spritesheet_path = game::Config::hourglass_lvl1_spritesheet;
            break;
    }
    this->spritesheet = LoadTexture(spritesheet_path);
    this->total_frames = game::Config::hourglass_frame_count;

    this->hitbox = {
        position.x,
        position.y + game::Config::hourglass_hitbox_y_offset,
        game::Config::hourglass_hitbox_size.x,
        game::Config::hourglass_hitbox_size.y
    };

    float hitbox_center_x = this->hitbox.x + (this->hitbox.width / 2.0f);
    this->draw_position.x = hitbox_center_x - (game::Config::hourglass_frame_size.x / 2.0f);
    this->draw_position.y = position.y;

    this->current_frame_rect = { 0.0f, 0.0f, game::Config::hourglass_frame_size.x, game::Config::hourglass_frame_size.y };
}

HourglassWall::~HourglassWall()
{
    UnloadTexture(this->spritesheet);
}

void HourglassWall::UpdateFrame(float wave_timer_remaining, float total_wave_time)
{
    if (total_wave_time <= 0) return;

    float time_elapsed_percent = 1.0f - (wave_timer_remaining / total_wave_time);

    int logical_index = static_cast<int>(floor(time_elapsed_percent * this->total_frames));
    logical_index = std::max(0, std::min(logical_index, this->total_frames - 1));

    const int frame_order_map[] = {2, 0, 1};

    int final_frame_index = frame_order_map[logical_index];

    this->current_frame_rect.x = static_cast<float>(final_frame_index) * game::Config::hourglass_frame_size.x;
}

Collision_Type HourglassWall::Get_Collision_Type() const
{
    return Collision_Type::WALL;
}

void HourglassWall::Tick(float delta_time)
{
    if (is_frozen)
    {
        freeze_timer -= delta_time;
        if (freeze_timer <= 0.0f)
        {
            is_frozen = false;
        }
    }
}


void HourglassWall::Draw()
{
    Color tint = is_frozen ? game::Config::kIceBombFreezeTint : WHITE;
    DrawTextureRec(this->spritesheet, this->current_frame_rect, this->draw_position, tint);
}

void HourglassWall::On_Collision(Collidable* other) { }

float HourglassWall::GetYSortPosition() const
{
    return this->hitbox.y + this->hitbox.height;
}

void HourglassWall::ApplyFreeze(float duration)
{
    this->is_frozen = true;
    this->freeze_timer = std::max(this->freeze_timer, duration);
}
bool HourglassWall::IsFrozen() const
{
    return is_frozen;
}