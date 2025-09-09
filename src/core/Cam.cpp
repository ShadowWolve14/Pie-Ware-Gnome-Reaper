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
    cam.target=mpp;
    /* int disx=mpp.x-cam.target.x;
     int disy=mpp.y-cam.target.y;
     int speed=game::Config::player_Class_One_Movement_Speed;
     if (disx<0){
         i=-1;
     } else{
         i=1;
     }
     if (disy<0){
         f=-1;
     } else{
         f=1;
     }
     disx= sqrt(disx*disx);
     disy= sqrt(disy*disy);
     if (c<1){
         c=10;
         //*speede=0;
         //*speedy=0;

         if (disx<game::Config::player_Class_One_Movement_Speed){
             speede= floor(speed*0);
         }
         if (disx>=game::Config::player_Class_One_Movement_Speed){
             speede= round(speed*0.5);
         }
         if (disx>game::Config::player_Class_One_Movement_Speed*3){
             speede= round(speed*1);
         }
         if (disx>game::Config::player_Class_One_Movement_Speed*5){
             speede= round(speed*1.5);
         }
         if (disx>game::Config::player_Class_One_Movement_Speed*7){
             speede= round(speed*2);
         }
         if (disx>game::Config::player_Class_One_Movement_Speed*9){
             speede= round(speed*2.5);
         }
         if (disy<game::Config::player_Class_One_Movement_Speed){
             speedy= floor(speed*0);
         }
         if (disy>=game::Config::player_Class_One_Movement_Speed){
             speedy= round(speed*0.5);
         }
         if (disy>game::Config::player_Class_One_Movement_Speed*3){
             speedy= round(speed*1);
         }
         if (disy>game::Config::player_Class_One_Movement_Speed*5){
             speedy= round(speed*1.5);
         }
         if (disy>game::Config::player_Class_One_Movement_Speed*7){
             speedy= round(speed*2);
         }
         if (disy>game::Config::player_Class_One_Movement_Speed*9){
             speedy= round(speed*2.5);
         }
     }
     c--;

     this->cam.target.x=this->cam.target.x+speede*i;
     this->cam.target.y=this->cam.target.y+speedy*f;
 */


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