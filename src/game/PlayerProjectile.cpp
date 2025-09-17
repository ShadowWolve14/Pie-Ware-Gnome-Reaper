#include "PlayerProjectile.h"
#include "../config.h.in"
#include "CollisionResponse.h"
#include "raymath.h"

namespace game
{
    Texture2D Player_Projectile::normal_projectile_sprite;
    Texture2D Player_Projectile::buffed_projectile_spritesheet;
    Player_Projectile::Player_Projectile(Vector2 start_position, Vector2 direction, float projectile_speed,
    int final_damage, int pierce_count, float final_pierce_multiplier, bool is_buffed_and_animated)
    : is_active(true),
      damage(final_damage),
      is_animated(is_buffed_and_animated),
      animation(
          is_buffed_and_animated ? game::Config::adrenaline_Projectile_Anim_Size : Vector2{0, 0},
          is_buffed_and_animated ? buffed_projectile_spritesheet : Texture2D{0},
          is_buffed_and_animated ? game::Config::adrenaline_Projectile_Frame_Count : 0,
          is_buffed_and_animated ? game::Config::adrenaline_Projectile_Frame_Count : 1,
          is_buffed_and_animated ? game::Config::adrenaline_Projectile_Anim_Speed : 0.0f
      )
    {
        this->pierce_count_remaining = pierce_count;
        this->damage_falloff_multiplier = final_pierce_multiplier;
        this->velocity = Vector2Scale(direction, projectile_speed);
        this->rotation = atan2(direction.y, direction.x) * RAD2DEG;

        this->hitbox = {
            start_position.x - game::Config::projectile_Hitbox_Size.x / 2.0f,
            start_position.y - game::Config::projectile_Hitbox_Size.y / 2.0f,
            game::Config::projectile_Hitbox_Size.x,
            game::Config::projectile_Hitbox_Size.y
        };
    }

    Player_Projectile::~Player_Projectile() { }

    void Player_Projectile::Tick(float delta_time) {
        if (!is_active) return;
        hitbox.x += velocity.x * delta_time;
        hitbox.y += velocity.y * delta_time;
        if (is_animated) {
            animation.Update_Frame(delta_time);
        }
    }

    void Player_Projectile::Draw()
    {
        if (!is_active) return;

        if (is_animated)
        {
            Texture2D sheet = animation.GetSpritesheet();
            Rectangle sourceRec = animation.GetCurrentFrameRec();
            Rectangle destRec = { hitbox.x + hitbox.width / 2, hitbox.y + hitbox.height / 2, sourceRec.width, sourceRec.height };
            Vector2 origin = { sourceRec.width / 2, sourceRec.height / 2 };
            DrawTexturePro(sheet, sourceRec, destRec, origin, this->rotation, WHITE);
        }
        else
        {
            Rectangle sourceRec = { 0.0f, 0.0f, (float)normal_projectile_sprite.width, (float)normal_projectile_sprite.height };
            Rectangle destRec = { hitbox.x + hitbox.width/2, hitbox.y + hitbox.height/2, (float)normal_projectile_sprite.width,
                (float)normal_projectile_sprite.height }; Vector2 origin = { (float)normal_projectile_sprite.width
                    / 2, (float)normal_projectile_sprite.height / 2 };
            DrawTexturePro(normal_projectile_sprite, sourceRec, destRec, origin, this->rotation, WHITE);
        }

        if (game::Config::visualize_Attack_Hitboxes)
        {
            DrawRectangleLinesEx(this->hitbox, 1.0f, RED);
        }
    }

    Collision_Type Player_Projectile::Get_Collision_Type() const { return Collision_Type::PLAYER_PROJECTILE; }

    void Player_Projectile::On_Collision(Collidable* other)
    {
        Collision_Type other_type = other->Get_Collision_Type();
        if (other_type == Collision_Type::WALL || other_type == Collision_Type::ENEMY_SPAWNER) {
            this->Mark_For_Destruction();
            return;
        }
        if (other_type == Collision_Type::ENEMY)
        {
            if (std::find(hit_enemies.begin(), hit_enemies.end(), other) != hit_enemies.end()) {
                return;
            }
            CollisionResponse::Apply_Damage(other, this->damage);
            hit_enemies.push_back(other);
            this->pierce_count_remaining--;
            this->damage = static_cast<int>(this->damage * this->damage_falloff_multiplier);
            if (this->pierce_count_remaining <= 0) {
                this->Mark_For_Destruction();
            }
        }
    }
    void Player_Projectile::LoadAssets()
    {
        normal_projectile_sprite = LoadTexture(game::Config::kProjectileSprite);
        buffed_projectile_spritesheet = LoadTexture(game::Config::kAdrenalineProjectileAnim);
    }

    void Player_Projectile::UnloadAssets()
    {
        UnloadTexture(normal_projectile_sprite);
        UnloadTexture(buffed_projectile_spritesheet);
    }
}
