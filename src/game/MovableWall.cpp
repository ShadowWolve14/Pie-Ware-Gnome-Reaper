//
// Created by $Will on 08.09.2025.
//
#include "MovableWall.h"
#include "PlayerBaseClass.h"
#include "../Config.h.in"
#include "raymath.h"
#include "CollisionResponse.h"
#include "Object_Manager.h"

MovableWall::MovableWall(Vector2 position, Vector2 size, Vector2 target, const char* spritesheet_path, Object_Manager& obj_manager)
    : Walls({position.x + game::Config::movable_wall_hitbox_offset.x, position.y + game::Config::movable_wall_hitbox_offset.y}, size),
    target_position(target), object_manager_ref(obj_manager)
{
    spritesheet = LoadTexture(spritesheet_path);
    inactive_frame = { 0.0f, 0.0f, game::Config::movable_wall_sprite_size.x, game::Config::movable_wall_sprite_size.y };
    active_frame = { game::Config::movable_wall_sprite_size.x, 0.0f, game::Config::movable_wall_sprite_size.x, game::Config::movable_wall_sprite_size.y };
}

MovableWall::~MovableWall()
{
    UnloadTexture(spritesheet);
}

void MovableWall::Tick(float delta_time)
{
    if (is_solved) return;

    if (pushing_player)
    {
        Facing_Direction current_push_direction = pushing_player->Get_Facing_Direction();

        if (!pushing_player->IsMoving() || (current_push_direction != UP && current_push_direction != DOWN && current_push_direction != LEFT && current_push_direction != RIGHT)) {
            StopPushing();
            return;
        }

        if (current_push_direction != last_push_direction) {
            push_timer = 0.0f;
            last_push_direction = current_push_direction;
        } else {
            push_timer += delta_time;
        }

        Vector2 move_vec = {0,0};
        if (last_push_direction == UP) move_vec.y = -1;
        else if (last_push_direction == DOWN) move_vec.y = 1;
        else if (last_push_direction == LEFT) move_vec.x = -1;
        else if (last_push_direction == RIGHT) move_vec.x = 1;

        Vector2 delta_pos = Vector2Scale(move_vec, game::Config::movable_wall_move_speed * delta_time);
        Rectangle original_hitbox = this->hitbox;

        // Bewege und prüfe die X-Achse
        this->hitbox.x += delta_pos.x;
        for (auto* obj : object_manager_ref.managed_objects) {
            if (obj == this) continue;
            Collision_Type type = obj->Get_Collision_Type();
            if (type == Collision_Type::WALL || type == Collision_Type::MOVABLE_WALL) {
                if (CheckCollisionRecs(this->hitbox, obj->Get_Hitbox())) {
                    this->hitbox.x = original_hitbox.x; // Setze bei Kollision zurück
                    break;
                }
            }
        }

        // Bewege und prüfe die Y-Achse
        this->hitbox.y += delta_pos.y;
        for (auto* obj : object_manager_ref.managed_objects) {
            if (obj == this) continue;
            Collision_Type type = obj->Get_Collision_Type();
            if (type == Collision_Type::WALL || type == Collision_Type::MOVABLE_WALL) {
                if (CheckCollisionRecs(this->hitbox, obj->Get_Hitbox())) {
                    this->hitbox.y = original_hitbox.y; // Setze bei Kollision zurück
                    break;
                }
            }
        }
    }

    if (Vector2Distance(Get_Position(), target_position) < game::Config::movable_wall_target_snap_radius)
    {
        if (!is_solved)
        {
            is_activating = true;
            activation_timer = 0.0f;
        }
        Set_Position(target_position);
        is_solved = true;
    }
}

void MovableWall::On_Collision(Collidable* other)
{
    if (is_solved)
    {
        CollisionResponse::Resolve_Overlap(other, this);
        return;
    }

    Collision_Type other_type = other->Get_Collision_Type();

    if (other_type == Collision_Type::PLAYER)
    {
        pushing_player = static_cast<Player_Base_Class*>(other);
        CollisionResponse::Resolve_Overlap(other, this);
    }
    else if (other_type == Collision_Type::ENEMY)
    {
        CollisionResponse::Resolve_Overlap(other, this);
    }
    else if (other_type == Collision_Type::WALL || other_type == Collision_Type::MOVABLE_WALL)
    {
        CollisionResponse::Resolve_Overlap(this, other);
    }
}


void MovableWall::StopPushing()
{
    pushing_player = nullptr;
    push_timer = 0.0f;
}

void MovableWall::Draw()
{
    Vector2 draw_pos =
    {
        this->hitbox.x - game::Config::movable_wall_hitbox_offset.x,
        this->hitbox.y - game::Config::movable_wall_hitbox_offset.y
    };
    if (is_activating)
    {
        activation_timer += GetFrameTime() * 10.0f;

        if ((int)(activation_timer) % 8 < 4)
        {
            DrawTextureRec(spritesheet, active_frame, draw_pos, WHITE);
        }
        else
        {
            DrawTextureRec(spritesheet, inactive_frame, draw_pos, WHITE);
        }

        if (activation_timer >= game::Config::movable_wall_activation_duration)
        {
            is_activating = false;
        }
    }
    else if (is_solved)
    {
        DrawTextureRec(spritesheet, active_frame, draw_pos, WHITE);
    }
    else
    {
        DrawTextureRec(spritesheet, inactive_frame, draw_pos, WHITE);
    }
}

Collision_Type MovableWall::Get_Collision_Type() const
{
    return Collision_Type::MOVABLE_WALL;
}

