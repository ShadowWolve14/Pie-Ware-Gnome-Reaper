//
// Created by $Will on 08.09.2025.
//

#ifndef MOVABLEWALL_H
#define MOVABLEWALL_H

#include <vector>
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
    Player_Base_Class* pushing_player = nullptr;
    Facing_Direction last_push_direction = FACING_NONE;

    Vector2 initial_start_position;

    static Texture2D smoke_spritesheet;
    int reset_vfx_counter = -1;
    Vector2 vfx_pos_disappear;
    Vector2 vfx_pos_respawn;

    Texture2D spr= LoadTexture("PieWare/assets/Spritesheets/VFX/Stone_Moving_Effect.png");
    Texture2D spl= LoadTexture("PieWare/assets/Spritesheets/VFX/Stone_Moving_Effect_Left.png");
    float vfxfc=0;
    bool moving= false;

    Sound pushs= LoadSound("assets/audio/sfx/Pushing_Stone.wav");

public:
    MovableWall(Vector2 position, Vector2 size, Vector2 target, const char* spritesheet_path, Object_Manager& obj_manager);
    ~MovableWall() override;
    Collision_Type Get_Collision_Type() const override;
    void Tick(float delta_time) override;
    void On_Collision(Collidable* other) override;
    void Draw() override;

    static void LoadAssets();
    static void UnloadAssets();
    void Reset();

    bool IsSolved() const { return is_solved; }
    void StopPushing();
};

#endif //MOVABLEWALL_H
