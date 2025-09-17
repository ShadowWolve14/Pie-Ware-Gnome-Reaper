//
// Created by Manza on 7/4/2025.
//

#pragma once
#include "raylib.h"
#include "Collidable.h"
#include "AttackDirection.h"
#include "PlayerBaseClass.h"
#include "../core/RepeatAnimation.h"
#include "../core/Animations.h"

namespace game {
    class Player_Projectile : public Collidable {
    public:
        Vector2 velocity;
        bool is_active;
        int damage;
        RepeatAnimation animation;
        bool is_animated;
        static Texture2D normal_projectile_sprite;
        static Texture2D buffed_projectile_spritesheet;
        float rotation;

        Player_Projectile(Vector2 start_position, Vector2 direction, float projectile_speed, int final_damage,
            int pierce_count, float final_pierce_multiplier, bool is_buffed_and_animated);
        ~Player_Projectile() override;
        int pierce_count_remaining;
        float damage_falloff_multiplier;
        std::vector<Collidable*> hit_enemies;
        void Tick(float delta_time) override;
        void Draw() override;
        Collision_Type Get_Collision_Type() const override;
        void On_Collision(Collidable* other) override;
        static void LoadAssets();
        static void UnloadAssets();
    };
}
