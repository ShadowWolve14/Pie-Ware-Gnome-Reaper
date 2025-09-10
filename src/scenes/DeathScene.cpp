//
// Created by $Will on 01.09.2025.
//

#include "DeathScene.h"
#include "UpgradeScene.h"
#include "ScoreboardScene.h"
#include <raylib.h>
#include <Store.h>
#include "../config.h.in"
namespace game::scenes {
    DeathScene::DeathScene(int score, int souls, bool fairy_status, int level) {
        this->final_score = score;
        this->final_souls = souls;
        this->has_fairy = fairy_status;
        this->current_level = level;
        this->de = 0;


        if (has_fairy) {
            switch (current_level) {
                case 1: {
                    act_anim=&good1;
                    limx=10;
                    limy=33;
                    fc=330-2;
                    break;
                }
                case 2: {
                    //DrawTexturePro(good2, {tframe * 512 + 1, 1, 512, 208},
                                   //{1920 / 2 - 256 * 2, 1080 / 2 - sa + 150, 512 * 2, 208 * 2}, {0, 0}, 0, WHITE);
                    act_anim=&good2;
                    limx=30;
                    fc=9*30-16;
                    limy=9;
                    break;
                }
                case 3: {
                    act_anim=&good3;
                    limx=30;
                    limy=9;
                    fc=9*30-20;
                    break;
                }
            }
        } else {
            switch (current_level) {
                case 1: {
                    act_anim=&bad1;
                    limx=10;
                    limy=21;
                    fc=210-9;
                    break;
                }
                case 2: {
                    act_anim=&bad2;
                    limx=30;
                    limy=11;
                    fc=11*30-11;
                    break;
                }
                case 3: {
                    act_anim=&bad3;
                    limx=30;
                    limy=8;
                    fc=8*30-28;
                    break;
                }
            }
        }
    }

    DeathScene::~DeathScene() {}

    void DeathScene::Update() {
        if (frame>=13&&sa>=300){
            if (f<=fc){
                xframe++;
                if(xframe==limx){
                    yframe++;
                    xframe=0;
                }
            }
            f++;

            if (f==fc+250){
                auto upgradeScene = std::make_shared<UpgradeScene>(final_souls, current_level);
                game::core::Store::stage->SwitchToNewScene("UpgradeScene", upgradeScene);
            }
        }

    }


    void DeathScene::Draw() {

        float y = 3;

        switch ((int) frame) {
            case 1: {
                y = 3;
                break;
            }
            case 2: {
                y = 4;
                break;
            }
            case 3: {
                y = 5;
                break;
            }
            case 4: {
                y = 6;
                break;
            }
            case 5: {
                y = 7;
                break;
            }
            case 6: {
                y = 8;
                PlaySound(deaths);
                break;
            }
            default: {
                y = 9;
            }
        }


        ClearBackground(BLACK);
        DrawTexturePro(dsprite, {frame * 16 + 28 * 16, 1, 16, 16},
                       {1920 / 2 - 8 * y, 1080 / 2 - 8 * y - sa, 16 * y, 16 * y}, {0, 0}, 0, WHITE);
        if (de < 1 && frame < 17) {
            frame = frame + 1;
            de = 8;
        }
        if (frame >= 13) {
            if (sa < 300) {
                sa = sa + 5;
            } else {
                DrawTexturePro(*act_anim, {xframe * 512 + 1, 1+208*yframe, 512, 208},
                {1920 / 2 - 256 * 2, 1080 / 2 - sa + 150, 512 * 2, 208 * 2}, {0, 0}, 0, WHITE);
            }
            

        }
        de--;

    }
}