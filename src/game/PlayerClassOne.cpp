//
// Created by $Will on 22.06.2025.
//

#include "PlayerClassOne.h"
#include <math.h>
#include "../config.h.in"
#include "PlayerEffectiveStats.h"

Player_Class_One::Player_Class_One(Vector2 start_Position)
    : Player_Base_Class(/* maxHealth */ BuildEffectiveStats(game::core::upgrades).max_health,
                    /* moveSpeed */ BuildEffectiveStats(game::core::upgrades).movement_speed,
                    /* dmgMult   */ BuildEffectiveStats(game::core::upgrades).DMGxMult,
                    start_Position),
      adrenalin_vfx_animation(
          game::Config::adrenaline_VFX_Anim_Size,
          game::Config::kAdrenalineVFXAnim,
          game::Config::adrenaline_VFX_Frame_Count,
          game::Config::adrenaline_VFX_Frame_Count,
          game::Config::adrenaline_VFX_Anim_Speed
      )
{
    const float anim_speed = game::Config::player_Hit_Frame_Count / (game::Config::player_Hit_Anim_Duration / 10.0f);
    Vector2 anim_size = game::Config::player_Hit_Anim_Size;
    int frame_count = game::Config::player_Hit_Frame_Count;
    int spl = frame_count;

    hit_Animations.try_emplace(DOWN,  anim_size, game::Config::kPlayerHitDownAnim, frame_count, spl, anim_speed);
    hit_Animations.try_emplace(UP,    anim_size, game::Config::kPlayerHitUpAnim,   frame_count, spl, anim_speed);
    hit_Animations.try_emplace(LEFT,  anim_size, game::Config::kPlayerHitLeftAnim, frame_count, spl, anim_speed);
    hit_Animations.try_emplace(RIGHT, anim_size, game::Config::kPlayerHitRightAnim,frame_count, spl, anim_speed);

    int player_Walk_Anim_Speed = game::Config::player_Walk_Anim_Speed;
    Vector2 player_Walk_Anim_Size = game::Config::player_Walk_Anim_Size;
    int player_Walk_Frame_Count = game::Config::player_Walk_Frame_Count;
    int sprites_Per_Line_Walk = player_Walk_Frame_Count;
    walking_Animations.try_emplace(UP, player_Walk_Anim_Size, game::Config::kPlayerWalkUpAnim, player_Walk_Frame_Count, sprites_Per_Line_Walk,player_Walk_Anim_Speed);
    walking_Animations.try_emplace(DOWN, player_Walk_Anim_Size, game::Config::kPlayerWalkDownAnim, player_Walk_Frame_Count, sprites_Per_Line_Walk,player_Walk_Anim_Speed);
    walking_Animations.try_emplace(LEFT, player_Walk_Anim_Size, game::Config::kPlayerWalkLeftAnim, player_Walk_Frame_Count, sprites_Per_Line_Walk,player_Walk_Anim_Speed);
    walking_Animations.try_emplace(RIGHT, player_Walk_Anim_Size, game::Config::kPlayerWalkRightAnim, player_Walk_Frame_Count, sprites_Per_Line_Walk,player_Walk_Anim_Speed);

    int player_Idle_Anim_Speed = game::Config::player_Idle_Anim_Speed;
    Vector2 player_Idle_Anim_Size = game::Config::player_Idle_Anim_Size;
    int player_Idle_Frame_Count = game::Config::player_Idle_Frame_Count;
    int sprites_Per_Line_Idle = player_Idle_Frame_Count;
    idle_Animations.try_emplace(UP, player_Idle_Anim_Size, game::Config:: kPlayerIdleUpAnim, player_Idle_Frame_Count, sprites_Per_Line_Idle,player_Idle_Anim_Speed);
    idle_Animations.try_emplace(DOWN, player_Idle_Anim_Size, game::Config::kPlayerIdleDownAnim, player_Idle_Frame_Count, sprites_Per_Line_Idle,player_Idle_Anim_Speed);
    idle_Animations.try_emplace(LEFT, player_Idle_Anim_Size, game::Config::kPlayerIdleLeftAnim, player_Idle_Frame_Count, sprites_Per_Line_Idle,player_Idle_Anim_Speed);
    idle_Animations.try_emplace(RIGHT, player_Idle_Anim_Size, game::Config::kPlayerIdleRightAnim, player_Idle_Frame_Count, sprites_Per_Line_Idle,player_Idle_Anim_Speed);

    int player_Ranged_Attack_Anim_Speed = game::Config::player_Ranged_Attack_Anim_Speed;
    Vector2 player_Ranged_Attack_Anim_Size = game::Config::player_Ranged_Attack_Anim_Size;
    int player_Ranged_Attack_Frame_Count = game::Config::player_Ranged_Attack_Frame_Count;
    int sprites_Per_Line_Ranged_Attack =player_Ranged_Attack_Frame_Count;
    ranged_Attack_Animations.try_emplace(UP, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackUpAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(DOWN, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackDownAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(LEFT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackLeftAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(RIGHT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackRightAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(UP_LEFT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackUpLeftAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(UP_RIGHT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackUpRightAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(DOWN_LEFT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackDownLeftAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack, player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(DOWN_RIGHT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackDownRightAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);

    int player_Melee_Attack_Anim_Speed = game::Config::player_Melee_Attack_Anim_Speed;
    Vector2 player_Melee_Attack_Anim_Size = game::Config::player_Melee_Attack_Anim_Size;
    int player_Melee_Attack_Frame_Count = game::Config::player_Melee_Attack_Frame_Count;
    int sprites_Per_Line_Melee_Attack = player_Melee_Attack_Frame_Count;
    melee_Attack_Animations.try_emplace(UP, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackUpAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(DOWN, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackDownAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(LEFT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackLeftAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(RIGHT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackRightAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(UP_LEFT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackUpLeftAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(UP_RIGHT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackUpRightAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(DOWN_LEFT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackDownLeftAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack, player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(DOWN_RIGHT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackDownRightAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);

    int player_Buff_Walk_Anim_Speed = game::Config::player_Buff_Walk_Anim_Speed;
    Vector2 player_Buff_Walk_Anim_Size = game::Config::player_Buff_Walk_Anim_Size;
    int player_Buff_Walk_Frame_Count = game::Config::player_Buff_Walk_Frame_Count;
    int sprites_Per_Line_Buff_Walk = player_Buff_Walk_Frame_Count;
    buff_walking_Animations.try_emplace(UP, player_Buff_Walk_Anim_Size, game::Config::kPlayerBuffWalkUpAnim, player_Buff_Walk_Frame_Count, sprites_Per_Line_Buff_Walk, player_Buff_Walk_Anim_Speed);
    buff_walking_Animations.try_emplace(DOWN, player_Buff_Walk_Anim_Size, game::Config::kPlayerBuffWalkDownAnim, player_Buff_Walk_Frame_Count, sprites_Per_Line_Buff_Walk, player_Buff_Walk_Anim_Speed);
    buff_walking_Animations.try_emplace(LEFT, player_Buff_Walk_Anim_Size, game::Config::kPlayerBuffWalkLeftAnim, player_Buff_Walk_Frame_Count, sprites_Per_Line_Buff_Walk, player_Buff_Walk_Anim_Speed);
    buff_walking_Animations.try_emplace(RIGHT, player_Buff_Walk_Anim_Size, game::Config::kPlayerBuffWalkRightAnim, player_Buff_Walk_Frame_Count, sprites_Per_Line_Buff_Walk, player_Buff_Walk_Anim_Speed);

    int player_Buff_Idle_Anim_Speed = game::Config::player_Buff_Idle_Anim_Speed;
    Vector2 player_Buff_Idle_Anim_Size = game::Config::player_Buff_Idle_Anim_Size;
    int player_Buff_Idle_Frame_Count = game::Config::player_Buff_Idle_Frame_Count;
    int sprites_Per_Line_Buff_Idle = player_Buff_Idle_Frame_Count;
    buff_idle_Animations.try_emplace(UP, player_Buff_Idle_Anim_Size, game::Config::kPlayerBuffIdleUpAnim, player_Buff_Idle_Frame_Count, sprites_Per_Line_Buff_Idle, player_Buff_Idle_Anim_Speed);
    buff_idle_Animations.try_emplace(DOWN, player_Buff_Idle_Anim_Size, game::Config::kPlayerBuffIdleDownAnim, player_Buff_Idle_Frame_Count, sprites_Per_Line_Buff_Idle, player_Buff_Idle_Anim_Speed);
    buff_idle_Animations.try_emplace(LEFT, player_Buff_Idle_Anim_Size, game::Config::kPlayerBuffIdleLeftAnim, player_Buff_Idle_Frame_Count, sprites_Per_Line_Buff_Idle, player_Buff_Idle_Anim_Speed);
    buff_idle_Animations.try_emplace(RIGHT, player_Buff_Idle_Anim_Size, game::Config::kPlayerBuffIdleRightAnim, player_Buff_Idle_Frame_Count, sprites_Per_Line_Buff_Idle, player_Buff_Idle_Anim_Speed);

    int player_Buff_Melee_Attack_Anim_Speed = game::Config::player_Buff_Melee_Attack_Anim_Speed;
    Vector2 player_Buff_Melee_Attack_Anim_Size = game::Config::player_Buff_Melee_Attack_Anim_Size;
    int player_Buff_Melee_Attack_Frame_Count = game::Config::player_Buff_Melee_Attack_Frame_Count;
    int sprites_Per_Line_Buff_Melee_Attack = player_Buff_Melee_Attack_Frame_Count;
    buff_melee_Attack_Animations.try_emplace(UP, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeUpAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(DOWN, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeDownAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(LEFT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeLeftAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(RIGHT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeRightAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(UP_LEFT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeUpLeftAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(UP_RIGHT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeUpRightAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(DOWN_LEFT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeDownLeftAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(DOWN_RIGHT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeDownRightAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);

}



Player_Class_One::~Player_Class_One() {}

void Player_Class_One::Tick(float delta_time)
{
    Player_Base_Class::Tick(delta_time);
    if (IsAdrenalinBuffed())
    {
        adrenalin_vfx_animation.Update_Frame(delta_time);
    }

    if (itemvfx != nullptr)
    {
        vfx_timer += delta_time;
    }

    if (currentState == ATTACKING_MELEE)
    {
        auto* active_melee_map = IsBuffed() ? &buff_melee_Attack_Animations : &melee_Attack_Animations;
        if (!active_melee_map->count(attack_Direction) || active_melee_map->at(attack_Direction).IsFinished())
        {
            currentState = IDLE;
        }
    }
    else if (currentState == ATTACKING_RANGED)
    {
        if (!ranged_Attack_Animations.count(attack_Direction) || ranged_Attack_Animations.at(attack_Direction).IsFinished())
        {
            currentState = IDLE;
        }
    }

    if (currentState != ATTACKING_MELEE && currentState != ATTACKING_RANGED && currentState != DAMAGED)
    {
        currentState = is_Moving ? WALKING : IDLE;
    }

    auto* active_walking_map = IsBuffed() ? &buff_walking_Animations : &walking_Animations;
    auto* active_idle_map = IsBuffed() ? &buff_idle_Animations : &idle_Animations;
    auto* active_melee_map = IsBuffed() ? &buff_melee_Attack_Animations : &melee_Attack_Animations;

    Facing_Direction primaryDirection = facing_Direction;
    if (facing_Direction == UP_LEFT || facing_Direction == DOWN_LEFT) primaryDirection = LEFT;
    if (facing_Direction == UP_RIGHT || facing_Direction == DOWN_RIGHT) primaryDirection = RIGHT;

    switch (currentState)
    {
        case WALKING:
            if (active_walking_map->count(primaryDirection)) {
                active_walking_map->at(primaryDirection).Update_Frame(delta_time);
            }
            break;
        case IDLE:
            if (active_idle_map->count(primaryDirection)) {
                active_idle_map->at(primaryDirection).Update_Frame(delta_time);
            }
            break;
        case ATTACKING_MELEE:
            if (active_melee_map->count(attack_Direction)) {
                active_melee_map->at(attack_Direction).Update_Frame(delta_time);
            }
            break;
        case ATTACKING_RANGED:
            if (ranged_Attack_Animations.count(attack_Direction)) {
                ranged_Attack_Animations.at(attack_Direction).Update_Frame(delta_time);
            }
            break;
        case DAMAGED:
            if (hit_Animations.count(primaryDirection)) {
                hit_Animations.at(primaryDirection).Update_Frame(delta_time);
            }
        break;
    }
    item_pick_c--;
}

void Player_Class_One::Draw()
{
    if (item_pick_c>0){
        DrawTextPro(game::core::Store::font,std::to_string(item_val).c_str(),item_pick_pos,{0,0},0,4,1,WHITE);
    }
    if (IsAdrenalinBuffed())
    {
        Vector2 vfx_draw_pos = {
                this->hitbox.x - (adrenalin_vfx_animation.size.x - this->hitbox.width) / 2.0f,
                this->hitbox.y - (adrenalin_vfx_animation.size.y - this->hitbox.height) / 2.0f
        };
        Color vfx_tint = { 255, 255, 255, (unsigned char)game::Config::adrenaline_VFX_Transparency };
        adrenalin_vfx_animation.Draw_Current_Frame(vfx_draw_pos, vfx_tint);
    }
    if (game::core::Store::drop_chance_change_duration>=0){

        Vector2 vfx_draw_pos = {
                this->hitbox.x - (48 - this->hitbox.width) / 2.0f,
                this->hitbox.y - (32 - this->hitbox.height) / 2.0f+luck_offset
        };

        if (luck_vfx_c>1*5){
            luckc++;
            luck_vfx_c=0;

        }
        if (ofs>1*8){
            uu=uu*(-1);
            ofs=0;
        }
        if (ofs_d>5){
            luck_offset=luck_offset-uu;
            ofs++;
        }
        DrawTextureRec(luckvfx, { 1.0f + 48 * luckc, 1.0f, 48.0f, 32.0f }, vfx_draw_pos, WHITE);

        if (luckc>=14){
            luckc=0;
        }
        luck_vfx_c++;

        ofs_d++;
    }
    if (currentState == DAMAGED)
    {
        Facing_Direction primaryDirection = facing_Direction;
        if (facing_Direction == UP_LEFT || facing_Direction == DOWN_LEFT) primaryDirection = LEFT;
        if (facing_Direction == UP_RIGHT || facing_Direction == DOWN_RIGHT) primaryDirection = RIGHT;

        if (hit_Animations.count(primaryDirection))
        {
            Animations& hit_anim = hit_Animations.at(primaryDirection);
            Vector2 draw_pos;
            draw_pos.x = this->hitbox.x - (hit_anim.size.x - this->hitbox.width) / 2.0f;
            draw_pos.y = this->hitbox.y - (hit_anim.size.y - this->hitbox.height) / 2.0f;
            hit_anim.Draw_Current_Frame({roundf(draw_pos.x), roundf(draw_pos.y)});

            return;
        }
    }
    Animations* current_attack_anim = nullptr;
    RepeatAnimation* current_loop_anim = nullptr;
    Vector2 draw_pos;
    auto* active_walking_map = &walking_Animations;
    auto* active_idle_map = &idle_Animations;
    auto* active_melee_map = &melee_Attack_Animations;

    if (IsBuffed())
    {
        active_walking_map = &buff_walking_Animations;
        active_idle_map = &buff_idle_Animations;
        active_melee_map = &buff_melee_Attack_Animations;
    }

    if (currentState == ATTACKING_MELEE)
    {
        if (active_melee_map->count(attack_Direction))
        {
            current_attack_anim = &active_melee_map->at(attack_Direction);
        }
    }
    else if (currentState == ATTACKING_RANGED)
    {
        if (ranged_Attack_Animations.count(attack_Direction))
        {
            current_attack_anim = &ranged_Attack_Animations.at(attack_Direction);
        }
    }
    else
    {
        Facing_Direction primaryDirection = facing_Direction;
        switch (facing_Direction) {
            case UP_LEFT:    primaryDirection = LEFT;  break;
            case UP_RIGHT:   primaryDirection = RIGHT; break;
            case DOWN_LEFT:  primaryDirection = LEFT;  break;
            case DOWN_RIGHT: primaryDirection = RIGHT; break;
            default: break;
        }

        if (currentState == WALKING)
        {
            current_loop_anim = &active_walking_map->at(primaryDirection);
        }
        else
        {
            current_loop_anim = &active_idle_map->at(primaryDirection);
        }
    }

    if (current_attack_anim != nullptr) {
        draw_pos.x = this->hitbox.x - (current_attack_anim->size.x - this->hitbox.width) / 2.0f;
        draw_pos.y = this->hitbox.y - (current_attack_anim->size.y - this->hitbox.height) / 2.0f;
        current_attack_anim->Draw_Current_Frame({roundf(draw_pos.x), roundf(draw_pos.y)});
    } else if (current_loop_anim != nullptr) {
        draw_pos.x = this->hitbox.x - (current_loop_anim->size.x - this->hitbox.width) / 2.0f;
        draw_pos.y = this->hitbox.y - (current_loop_anim->size.y - this->hitbox.height) / 2.0f;
        current_loop_anim->Draw_Current_Frame({roundf(draw_pos.x), roundf(draw_pos.y)});
    }
    if (game::Config::visualize_Player_Hitbox)
    {
        DrawRectangleLinesEx(this->hitbox, 1.0f, BLUE);
    }

    if (itemvfx != nullptr)
    {
        int frame_limit = 0;
        int sprite_width = 0;
        float anim_speed = 1.0f;

        if (vfxtype == 1)
        {
            frame_limit = 4;
            sprite_width = 32;
            anim_speed = game::Config::kHealVFXAnimSpeed;
        }
        else if (vfxtype == 2)
        {
            frame_limit = 14;
            sprite_width = 16;
            anim_speed = game::Config::kSmokeVFXAnimSpeed;
        }


        int current_frame = static_cast<int>(vfx_timer * anim_speed);

        if (current_frame < frame_limit)
        {
            Rectangle source_rec = { 1.0f + sprite_width * current_frame, 1.0f, (float)sprite_width, 32.0f };
            Vector2 draw_pos = { hitbox.x - (sprite_width - hitbox.width)/2.0f, hitbox.y - 6 };
            DrawTextureRec(*itemvfx, source_rec, draw_pos, WHITE);
        }
        else
        {
            itemvfx = nullptr;
            vfx_timer = 0.0f;
        }
    }
}

void Player_Class_One::Ranged_Attack()
{
    this->attack_Direction = this->facing_Direction;
    PlaySound(s_ranged_attack_sound);
    Player_Base_Class::Ranged_Attack();
    if (ranged_Attack_Animations.count(this->attack_Direction)) {
        ranged_Attack_Animations.at(this->attack_Direction).First_Frame();
    }
}
void Player_Class_One::Melee_Attack()
{
    this->attack_Direction = this->facing_Direction;
    PlaySound(s_attack_sound);
    Player_Base_Class::Melee_Attack();
    auto* active_melee_map = IsBuffed() ? &buff_melee_Attack_Animations : &melee_Attack_Animations;
    if (active_melee_map->count(this->attack_Direction))
    {
        active_melee_map->at(this->attack_Direction).First_Frame();
    }
}

void Player_Class_One::ReapplyUpgrades()
{
    const auto eff = BuildEffectiveStats(game::core::upgrades);

    // Movement/health/dmg mult are stored in base; set what you need:
    SetMeleeDamage(eff.meleeDamage);
    SetRangedDamage(eff.rangedDamage);
    SetAttackCooldown(eff.meele_attack_cooldown);
    SetRangedCooldown(eff.ranged_attack_cooldown);
    SetMovementSpeed(eff.movement_speed);
    SetMaxHealth(eff.max_health);
    SetDMGMult(eff.DMGxMult);
}

void Player_Class_One::TriggerHitAnimation()
{
    Facing_Direction primaryDirection = facing_Direction;
    if (facing_Direction == UP_LEFT || facing_Direction == DOWN_LEFT || facing_Direction == UP_RIGHT || facing_Direction == DOWN_RIGHT)
    {
        if (facing_Direction == UP_LEFT || facing_Direction == DOWN_LEFT) primaryDirection = LEFT;
        if (facing_Direction == UP_RIGHT || facing_Direction == DOWN_RIGHT) primaryDirection = RIGHT;
    }

    if (hit_Animations.count(primaryDirection))
    {
        hit_Animations.at(primaryDirection).First_Frame();
    }
}