//
// Created by $Will on 08.09.2025.
//

#ifndef MOVABLEWALL_H
#define MOVABLEWALL_H

#include "Walls.h"
class Player_Base_Class;
class Object_Manager;
class MovableWall : public Walls
{
private:
    Object_Manager& object_manager_ref;
    Vector2 target_position;
    bool is_solved = false;
    bool is_activating = false;
    float activation_timer = 0.0f;
    Texture2D spritesheet;
    Rectangle inactive_frame;
    Rectangle active_frame;
    bool player_is_colliding_this_frame = false;
    Player_Base_Class* pushing_player = nullptr;
    float push_timer = 0.0f;
    Facing_Direction last_push_direction = FACING_NONE;

public:
    MovableWall(Vector2 position, Vector2 size, Vector2 target, const char* spritesheet_path, Object_Manager& obj_manager);
    ~MovableWall() override;
    Collision_Type Get_Collision_Type() const override;
    void Tick(float delta_time) override;
    void On_Collision(Collidable* other) override;
    void Draw() override;

    bool IsSolved() const { return is_solved; }
    void StopPushing();
};

#endif //MOVABLEWALL_H
