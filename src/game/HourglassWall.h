//
// Created by $Will on 10.09.2025.
//

#ifndef HOURGLASSWALL_H
#define HOURGLASSWALL_H
#include "Collidable.h"

class HourglassWall : public Collidable
{
private:
    Texture2D spritesheet;
    Rectangle current_frame_rect;
    Vector2 draw_position;
    int total_frames;
    bool is_frozen = false;
    float freeze_timer = 0.0f;


public:
    HourglassWall(Vector2 position, int level);
    ~HourglassWall() override;
    void UpdateFrame(float wave_timer_remaining, float total_wave_time);
    void ApplyFreeze(float duration);
    void Tick(float delta_time) override;
    Collision_Type Get_Collision_Type() const override;
    void Draw() override;
    void On_Collision(Collidable* other) override;
    float GetYSortPosition() const override;
};
#endif //HOURGLASSWALL_H
