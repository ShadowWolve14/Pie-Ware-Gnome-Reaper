#include "Cam.h"
#include <raylib.h>
#include "../config.h.in"
#include "raymath.h"

Cam::Cam(Player_Base_Class& mp) : pao(mp)
{
    this->cam = {0};
    this->cam.zoom = game::Config::cam_zoom;
    cam.target=pao.Get_Player_Center();
}

void Cam::Cam_Movement(double dtm, Vector2 mapDimensions)
{
   Vector2 mpp = pao.Get_Player_Center();


    Vector2 diff = Vector2Subtract(mpp, cam.target);


    float smoothness = 0.75f;
    cam.target = Vector2Add(cam.target, Vector2Scale(diff, smoothness * dtm));
    /*Vector2 mpp = pao.Get_Player_Center();

    Vector2 diff = Vector2Subtract(mpp, cam.target);
    float dist = Vector2Length(diff);


    float followStrength = 0.75f;


    float speed = dist * followStrength * dtm;

    if (speed > dist) speed = dist; // prevent overshoot


    Vector2 move = Vector2Scale(Vector2Normalize(diff), speed);
    cam.target = Vector2Add(cam.target, move);*/



    this->cam.offset = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };

    this->cam.zoom = game::Config::cam_zoom;

    if (mapDimensions.x > 0 && mapDimensions.y > 0)
    {
        float halfScreenWidthInWorld = (float)GetScreenWidth() / 2.0f / this->cam.zoom;
        float halfScreenHeightInWorld = (float)GetScreenHeight() / 2.0f / this->cam.zoom;

        this->cam.target.x = Clamp(this->cam.target.x, halfScreenWidthInWorld, mapDimensions.x - halfScreenWidthInWorld);
        this->cam.target.y = Clamp(this->cam.target.y, halfScreenHeightInWorld, mapDimensions.y - halfScreenHeightInWorld);
    }
    this->cam.target.x = roundf(this->cam.target.x);
    this->cam.target.y = roundf(this->cam.target.y);

    if (pao.Get_Health()!=prevh){
        ct=5;
    }
    if (ct>0){
        Camshake();
        ct--;
    }
    if (ct==0){
        this->cam.offset = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
    }
    prevh=pao.Get_Health();
}
void Cam::Camshake() {
    int x=GetRandomValue(-5,5);
    x=x*3;
    cam.offset.x=cam.offset.x+x;
    cam.offset.y=cam.offset.y+x;
}