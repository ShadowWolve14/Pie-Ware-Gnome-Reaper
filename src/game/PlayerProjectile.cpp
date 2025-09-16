#include "PlayerProjectile.h"
#include "../config.h.in"
#include "CollisionResponse.h"
#include "raymath.h"

namespace game
{
    Texture2D Player_Projectile::projectile_sprite;

    Player_Projectile::Player_Projectile(Vector2 start_position, Vector2 direction, float projectile_speed, int final_damage, int pierce_count)
        : is_active(true), damage(final_damage)
    {
        this->pierce_count_remaining = pierce_count;
        this->damage_falloff_multiplier = game::Config::projectile_pierce_damage_multiplier_percent / 100.0f;
        this->sprite = projectile_sprite;

        this->velocity = Vector2Scale(direction, projectile_speed);

        this->rotation = atan2(direction.y, direction.x) * RAD2DEG;

        this->hitbox = {
            start_position.x - game::Config::projectile_Hitbox_Size.x / 2.0f,
            start_position.y - game::Config::projectile_Hitbox_Size.y / 2.0f,
            game::Config::projectile_Hitbox_Size.x,
            game::Config::projectile_Hitbox_Size.y
        };
    }

    Player_Projectile::~Player_Projectile() {  }

    void Player_Projectile::Tick(float delta_time) {
        if (!is_active) return;
        hitbox.x += velocity.x * delta_time;
        hitbox.y += velocity.y * delta_time;
    }

    void Player_Projectile::Draw()
    {
        if (!is_active) return;

        Rectangle sourceRec = { 0.0f, 0.0f, (float)this->sprite.width, (float)this->sprite.height };
        Rectangle destRec = { hitbox.x + hitbox.width/2, hitbox.y + hitbox.height/2, (float)this->sprite.width, (float)this->sprite.height };
        Vector2 origin = { (float)this->sprite.width / 2, (float)this->sprite.height / 2 };

        destRec.x = roundf(destRec.x);
        destRec.y = roundf(destRec.y);

        DrawTexturePro(projectile_sprite, sourceRec, destRec, origin, this->rotation, WHITE);
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
        projectile_sprite = LoadTexture(game::Config::kProjectileSprite);
    }

    void Player_Projectile::UnloadAssets()
    {
        UnloadTexture(projectile_sprite);
    }

}
