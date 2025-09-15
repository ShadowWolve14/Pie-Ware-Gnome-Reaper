//
// Created by Manza on 6/22/2025.
//

#include "MeleeEnemy.h"
#include "CollisionResponse.h"
#include "raymath.h"
#include "../Config.h.in"

namespace enemy
{
    std::map<std::string, Texture2D> Melee_Enemy::s_melee_textures;

    void Melee_Enemy::Load_All_Melee_Assets()
    {
        s_melee_textures["Bauer_Walk_Left"] = LoadTexture(game::Config::kMeleeEnemy1WalkLeftAnim);
        s_melee_textures["Bauer_Walk_Right"] = LoadTexture(game::Config::kMeleeEnemy1WalkRightAnim);
        s_melee_textures["Bauer_Attack_Left"] = LoadTexture(game::Config::kMeleeEnemy1AttackLeftAnim);
        s_melee_textures["Bauer_Attack_Right"] = LoadTexture(game::Config::kMeleeEnemy1AttackRightAnim);
        s_melee_textures["Bauer_Hit_Right"] = LoadTexture(game::Config::kMeleeEnemy1HitRight);
        s_melee_textures["Bauer_Hit_Left"] = LoadTexture(game::Config::kMeleeEnemy1HitLeft);

        s_melee_textures["Ritter_Walk_Left"] = LoadTexture(game::Config::kMeleeEnemy2WalkLeftAnim);
        s_melee_textures["Ritter_Walk_Right"] = LoadTexture(game::Config::kMeleeEnemy2WalkRightAnim);
        s_melee_textures["Ritter_Attack_Left"] = LoadTexture(game::Config::kMeleeEnemy2AttackLeftAnim);
        s_melee_textures["Ritter_Attack_Right"] = LoadTexture(game::Config::kMeleeEnemy2AttackRightAnim);
        s_melee_textures["Ritter_Hit_Right"] = LoadTexture(game::Config::kMeleeEnemy2HitRight);
        s_melee_textures["Ritter_Hit_Left"] = LoadTexture(game::Config::kMeleeEnemy2HitLeft);

        s_melee_textures["Demonenritter_Walk_Left"] = LoadTexture(game::Config::kMeleeEnemy3WalkLeftAnim);
        s_melee_textures["Demonenritter_Walk_Right"] = LoadTexture(game::Config::kMeleeEnemy3WalkRightAnim);
        s_melee_textures["Demonenritter_Attack_Left"] = LoadTexture(game::Config::kMeleeEnemy3AttackLeftAnim);
        s_melee_textures["Demonenritter_Attack_Right"] = LoadTexture(game::Config::kMeleeEnemy3AttackRightAnim);
        s_melee_textures["Demonenritter_Hit_Right"] = LoadTexture(game::Config::kMeleeEnemy3HitRight);
        s_melee_textures["Demonenritter_Hit_Left"] = LoadTexture(game::Config::kMeleeEnemy3HitLeft);
    }

    void Melee_Enemy::Unload_All_Melee_Assets()
    {
        for (auto const& [key, val] : s_melee_textures)
        {
            UnloadTexture(val);
        }
        s_melee_textures.clear();
    }

    Melee_Enemy::Melee_Enemy(Vector2 start_position, const std::string& name, int health, float speed, int damage,
                             int score, int souls, float cooldown, Vector2 hitbox_size, const char* walk_left_path,
                             const char* walk_right_path, const char* attack_left_path, const char* attack_right_path, Vector2 walk_anim_size,
                             int walk_frame_count, float walk_anim_speed, Vector2 attack_anim_size, int attack_frame_count, float attack_anim_speed,
                             Vector2 hit_anim_size, int hit_frame_count, float hit_anim_speed, float hit_anim_duration)

            : Enemy_Base_Class(name, health, speed, damage, score, souls, start_position, hitbox_size.x, hitbox_size.y, cooldown,
                               game::Config::kAIBase_SeekWeight, game::Config::kAIBase_SeparationWeight, game::Config::kAIBase_PlayerSeparationWeight,
                               game::Config::kAIBase_DesiredSeparation, game::Config::kAIBase_Drag)
    {
        this->walk_texture_left = &s_melee_textures.at(name + "_Walk_Left");
        this->walk_texture_right = &s_melee_textures.at(name + "_Walk_Right");
        this->attack_texture_left = &s_melee_textures.at(name + "_Attack_Left");
        this->attack_texture_right = &s_melee_textures.at(name + "_Attack_Right");
        this->hit_texture_left = &s_melee_textures.at(name + "_Hit_Left");
        this->hit_texture_right = &s_melee_textures.at(name + "_Hit_Right");

        walk_animations.try_emplace(LEFT, walk_anim_size, *this->walk_texture_left,
                                    walk_frame_count, walk_frame_count, walk_anim_speed);
        walk_animations.try_emplace(RIGHT, walk_anim_size, *this->walk_texture_right,
                                    walk_frame_count, walk_frame_count, walk_anim_speed);

        attack_animations.try_emplace(LEFT, attack_anim_size, *this->attack_texture_left,
                                      attack_frame_count, attack_frame_count, attack_anim_speed);
        attack_animations.try_emplace(RIGHT, attack_anim_size, *this->attack_texture_right,
                                      attack_frame_count, attack_frame_count, attack_anim_speed);

        hit_animations.try_emplace(LEFT, hit_anim_size, *this->hit_texture_left,
                                   hit_frame_count, hit_frame_count, hit_anim_speed);
        hit_animations.try_emplace(RIGHT, hit_anim_size, *this->hit_texture_right,
                                   hit_frame_count, hit_frame_count, hit_anim_speed);

        this->hit_animation_duration = hit_anim_duration;

        hit_animations.try_emplace(LEFT, hit_anim_size, *this->hit_texture_left,
                                   hit_frame_count, hit_frame_count, hit_anim_speed);
        hit_animations.try_emplace(RIGHT, hit_anim_size, *this->hit_texture_right,
                                   hit_frame_count, hit_frame_count, hit_anim_speed);
    }

    void Melee_Enemy::Tick_Melee(float delta_time, Vector2 player_center)
{
    this->last_known_player_center = player_center;
    if (currentState == E_DYING)
    {
        death_timer -= delta_time;
        if (death_timer <= 0.0f) {
            this->Mark_For_Destruction();
        }
        hit_animations.at(facing_Direction).Update_Frame(delta_time);
        return;
    }
        if (currentState == E_KNOCKBACK)
        {
            hit_stun_timer -= delta_time;
            if (knockback_timer > 0.0f)
            {
                knockback_timer -= delta_time;
                this->hitbox.x += knockback_velocity.x * delta_time;
                this->hitbox.y += knockback_velocity.y * delta_time;
            }
            hit_animations.at(facing_Direction).Update_Frame(delta_time);
            if (hit_stun_timer <= 0.0f)
            {
                currentState = E_IDLE;
            }
            return;
        }

    Vector2 self_center = { this->hitbox.x + this->hitbox.width / 2.0f, this->hitbox.y + this->hitbox.height / 2.0f };
    if (player_center.x > self_center.x + 2.0f) {
        facing_Direction = RIGHT;
    } else if (player_center.x < self_center.x - 2.0f) {
        facing_Direction = LEFT;
    }
    if (currentState == E_ATTACKING) {
        if (attack_animations.at(attack_Direction).IsFinished()) {
            currentState = E_IDLE;
        }
    } else {
        float distance_to_target = Vector2Distance(self_center, player_center);
        float stopping_distance = (this->hitbox.width / 2.0f) + (game::Config::player_Hittbox.x / 2.0f);
        if (distance_to_target <= stopping_distance + 5.0f && attack_Cooldown_Timer <= 0) {
            Melee_Attack();
        } else {
            currentState = (Vector2LengthSqr(this->velocity) > 0.1f) ? E_WALKING : E_IDLE;
        }
    }

    if (currentState == E_ATTACKING) {
        attack_animations.at(attack_Direction).Update_Frame(delta_time);
    } else if (currentState == E_WALKING) {
        walk_animations.at(facing_Direction).Update_Frame(delta_time);
    }
}

    void Melee_Enemy::Melee_Attack()
    {
        this->attack_Direction = this->facing_Direction;
        currentState = E_ATTACKING;
        PlaySound(atS);
        attack_Cooldown_Timer = game::Config::melee_enemy_1_attack_cooldown;
        if (attack_animations.count(this->attack_Direction))
        {
            attack_animations.at(this->attack_Direction).First_Frame();
        }
        this->damage_applied_this_attack = false;
    }

    void Melee_Enemy::Draw()
    {
        Vector2 draw_pos;
        Animations* attack_anim = nullptr;
        Animations* hit_anim = nullptr;
        RepeatAnimation* walk_anim = nullptr;

        switch (currentState) {
            case E_ATTACKING:
                attack_anim = &attack_animations.at(attack_Direction);
            break;
            case E_DAMAGED:
                case E_DYING:
                case E_KNOCKBACK:
                hit_anim = &hit_animations.at(facing_Direction);
            break;
            case E_WALKING:
            case E_IDLE:
            default:
                walk_anim = &walk_animations.at(facing_Direction);
            break;
        }

        if (attack_anim != nullptr)
        {
            draw_pos.x = this->hitbox.x - (attack_anim->size.x - this->hitbox.width) / 2.0f;
            draw_pos.y = this->hitbox.y - (attack_anim->size.y - this->hitbox.height) / 2.0f;
            attack_anim->Draw_Current_Frame(draw_pos);
        }
        else if (hit_anim != nullptr)
        {
            draw_pos.x = this->hitbox.x - (hit_anim->size.x - this->hitbox.width) / 2.0f;
            draw_pos.y = this->hitbox.y - (hit_anim->size.y - this->hitbox.height) / 2.0f;
            hit_anim->Draw_Current_Frame(draw_pos);
        }
        else if (walk_anim != nullptr)
        {
            draw_pos.x = this->hitbox.x - (walk_anim->size.x - this->hitbox.width) / 2.0f;
            draw_pos.y = this->hitbox.y - (walk_anim->size.y - this->hitbox.height) / 2.0f;
            walk_anim->Draw_Current_Frame(draw_pos);
        }

        //DrawRectangleLinesEx(this->hitbox, 2.0f, RED);
    }

    void Melee_Enemy::On_Collision(Collidable* other)
    {

        if (other->Get_Collision_Type() == Collision_Type::PLAYER)
        {
            if (currentState == E_ATTACKING && !damage_applied_this_attack)
            {
                CollisionResponse::Apply_Damage(other, this->enemy_Damage);
                damage_applied_this_attack = true;
            }
        }
        Enemy_Base_Class::On_Collision(other);

    }
    void Melee_Enemy::Take_Damage_Check(int damage_amount)
    {
        if (currentState == E_DYING) return;
        if (this->enemy_Health <= 0)
        {
            currentState = E_DYING;
            death_timer = this->hit_animation_duration;
        }
        else
        {
            currentState = E_KNOCKBACK;
            knockback_timer = game::Config::kAIBase_Knockback_Duration;
            hit_stun_timer = this->hit_animation_duration;

            Vector2 self_center = { this->hitbox.x + hitbox.width / 2.0f, this->hitbox.y + hitbox.height / 2.0f };
            Vector2 direction_away_from_player = Vector2Normalize(Vector2Subtract(self_center, this->last_known_player_center));

            knockback_velocity = Vector2Scale(direction_away_from_player, game::Config::kAIBase_Knockback_Speed);
        }

        if (hit_animations.count(this->facing_Direction))
        {
            hit_animations.at(this->facing_Direction).First_Frame();
        }
    }
}