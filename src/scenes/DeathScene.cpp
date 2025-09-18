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
        // load fairy animation depending on level
        const std::string path = game::Config::fairy_spritesheet_path(level);
        if (path != "no sprite found") {
            const FairyAnimMeta meta = GetFairyMetaForLevel(level);
            fairyAnim = RepeatAnimation(
                    meta.frameSize,
                    path.c_str(),
                    meta.frames,
                    meta.columns,
                    meta.fps
            );
            fairyAnim.First_Frame();
        }



        if (has_fairy) {
            switch (current_level) {
                case 1: {
                    act_anim=&good1;
                    limx=10;
                    limy=33;
                    fc=330-10;
                    break;
                }
                case 2: {
                    //DrawTexturePro(good2, {tframe * 512 + 1, 1, 512, 208},
                                   //{1920 / 2 - 256 * 2, 1080 / 2 - sa + 150, 512 * 2, 208 * 2}, {0, 0}, 0, WHITE);
                    act_anim=&good2;
                    limx=30;
                    fc=9*30-12;
                    limy=9;
                    break;
                }
                case 3: {
                    act_anim=&good3;
                    limx=30;
                    limy=9;
                    fc=9*30-21;
                    break;
                }
            }
        } else {
            switch (current_level) {
                case 1: {
                    act_anim=&bad1;
                    limx=10;
                    limy=21;
                    fc=210-2;
                    break;
                }
                case 2: {
                    act_anim=&bad2;
                    limx=30;
                    limy=11;
                    fc=11*30-17;
                    break;
                }
                case 3: {
                    act_anim=&bad3;
                    limx=30;
                    limy=8;
                    fc=8*30-29;
                    break;
                }
            }
        }
    }

    DeathScene::~DeathScene() {}

    void DeathScene::Update() {
        SetSoundVolume(deaths,game::core::Store::volume*game::Config::Player_Death_Sound_Volume);
        SetSoundVolume(sound1,game::core::Store::volume*game::Config::Player_Death_Sound_Volume);
        fairyAnim.Update_Frame(GetFrameTime());
        if (frame>=13&&sa>=300){
            if (f<=fc){
                xframe++;
                if(xframe>=limx){
                    yframe++;
                    xframe=0;
                }
            }
            f++;


            if (IsGamepadButtonPressed(0,7)){
                PlaySound(sound1);
                if (!has_fairy&&current_level!=3){
                    auto upgradeScene = std::make_shared<ScoreboardScene>(game::core::Store::player_state->score);
                    game::core::Store::stage->SwitchToNewScene("MainMenu", upgradeScene);
                }
                if (has_fairy&&current_level!=3){
                    auto upgradeScene = std::make_shared<UpgradeScene>(final_souls, current_level);
                    game::core::Store::stage->SwitchToNewScene("MainMenu", upgradeScene);
                }
                if (current_level==3){
                    auto upgradeScene = std::make_shared<ScoreboardScene>(game::core::Store::player_state->score);
                    game::core::Store::stage->SwitchToNewScene("MainMenu", upgradeScene);
                }
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
                fairyAnim.Draw_Current_Frame_Pro(Vector2{ 140.0f, 400 });
                DrawTexturePro(*act_anim, {xframe * 512 + 1, 1+208*yframe, 512, 208},
                {1920 / 2 - 256 , 1080 / 2 - sa + 150, 512 * 2, 208 * 2}, {0, 0}, 0, WHITE);
            }
            if (f>fc+50){
                if (has_fairy&&current_level==3){
                    DrawTextEx(game::core::Store::font,"Drücke J um zum Main Menu zurück zu kehren",{1920 /2, 1080 / 2 - sa + 600},30,1,RED);
                }
                if (!has_fairy&&current_level==3){
                    DrawTextEx(game::core::Store::font,"Drücke J um zum Main Menu zurück zu kehren",{1920 /2, 1080 / 2 - sa + 600},30,1,RED);
                }
                if (has_fairy&&current_level!=3){
                    DrawTextEx(game::core::Store::font,"Drücke J um weiter zu kommen",{1920 /2, 1080 / 2 - sa + 600},30,1,RED);
                }
                if (!has_fairy&&current_level!=3){
                    DrawTextEx(game::core::Store::font,"Drücke J um zum Main Menu zurück zu kehren",{1920 /2, 1080 / 2 - sa + 600},30,1,RED);
                }

            }

        }
        de--;

    }
}