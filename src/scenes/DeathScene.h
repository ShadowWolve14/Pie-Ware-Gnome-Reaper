//
// Created by $Will on 01.09.2025.
//

#ifndef DEATHSCENE_H
#define DEATHSCENE_H

#include "Scene.h"
#include "Animations.h"
#include "MainMenuScene.h"
#include "FairyAnim.h"

namespace game::scenes
{
    class DeathScene final : public game::core::Scene
    {
    private:
        int final_score;
        int final_souls;
        bool has_fairy;
        int current_level;
        float frame=0;
        float xframe=-1;
        float yframe=0;
        int de;
        bool right;
        float sa=0;
        int limy=0;
        int limx=0;
        int fc=0;
        int f=0;
        int sk=0;

        Texture dsprite= LoadTexture("PieWare/assets/Spritesheets/Gnome_Dying.png");
        Texture good1= LoadTexture("PieWare/assets/UI/DeathScreem/Level_1_Upgradet.png");
        Texture good2= LoadTexture("PieWare/assets/UI/DeathScreem/Level_2_Upgradet.png");
        Texture good3= LoadTexture("PieWare/assets/UI/DeathScreem/Level_3_BadEndingt.png");
        Texture bad1= LoadTexture("PieWare/assets/UI/DeathScreem/Level_1_Deatht.png");
        Texture bad2= LoadTexture("PieWare/assets/UI/DeathScreem/Level_2_Deatht.png");
        Texture bad3= LoadTexture("PieWare/assets/UI/DeathScreem/Level_3_GoodEndingt.png");

        Sound deaths= LoadSound("assets/audio/sfx/Gnome_Death.wav");
        Texture* act_anim;

        RepeatAnimation fairyAnim{ Vector2{32.0f, 32.0f}, "", 1, 1, 1.0f };

        Sound sound1 = LoadSound("assets/audio/sfx/Item_Obtained.wav");



    public:
        DeathScene(int score, int souls, bool fairy_status, int level);
        ~DeathScene() override;
        void Update() override;
        void Draw() override;
    };
}

#endif //DEATHSCENE_H
