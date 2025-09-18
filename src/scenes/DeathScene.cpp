//
// Created by $Will on 01.09.2025.
//
#include "DeathScene.h"
#include <raylib.h>
#include <Store.h>
#include "../config.h.in"

namespace game::scenes {

static bool IsActionKeyPressed()
{
    return IsKeyPressed(game::Config::key_Melee_Attack) || IsKeyPressed(KEY_K) || IsKeyPressed(KEY_L) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE);
}

DeathScene::DeathScene(int score, int souls, bool fairy_status, int level)
    : textAnimation({0,0}, (Texture2D){0}, 0, 0, 1.0f),
      fairyAnim({0,0}, (Texture2D){0}, 0, 0, 1.0f)
{
    this->final_score = score;
    this->final_souls = souls;
    this->has_fairy = fairy_status;
    this->current_level = level;
    this->customFont = LoadFont("PieWare/assets/Font/GnomishGame.ttf");

    const std::string path = game::Config::fairy_spritesheet_path(level);
    if (path != "no sprite found") {
        const FairyAnimMeta meta = GetFairyMetaForLevel(level);
        fairyAnim = RepeatAnimation(meta.frameSize, path.c_str(), meta.frames, meta.columns, meta.fps);
        fairyAnim.First_Frame();
    }

    const Vector2 frameSize = {512.0f, 208.0f};
    const float animSpeed = 30.0f;

    if (has_fairy) {
        switch (current_level) {
            case 1: textAnimation = Animations(frameSize, good1, 321, 10, animSpeed); break;
            case 2: textAnimation = Animations(frameSize, good2, 259, 30, animSpeed); break;
            case 3: textAnimation = Animations(frameSize, good3, 250, 30, animSpeed); break;
        }
    } else {
        switch (current_level) {
            case 1: textAnimation = Animations(frameSize, bad1, 209, 10, animSpeed); break;
            case 2: textAnimation = Animations(frameSize, bad2, 314, 30, animSpeed); break;
            case 3: textAnimation = Animations(frameSize, bad3, 212, 30, animSpeed); break;
        }
    }
}

DeathScene::~DeathScene() {
    UnloadFont(customFont);
}

void DeathScene::Update() {
    SetSoundVolume(deaths, game::core::Store::volume * game::Config::Player_Death_Sound_Volume);
    SetSoundVolume(sound1, game::core::Store::volume * game::Config::Player_Death_Sound_Volume);
    fairyAnim.Update_Frame(GetFrameTime());

    if (frame >= 13 && sa < 300) { sa += 5; }
    if (de < 1 && frame < 17) { frame++; de = 8; }
    de--;

    if (frame >= 13 && sa >= 300) {
        text_anim_started = true;
    }

    if (text_anim_started && !textAnimation.IsFinished()) {
        float time_delta = GetFrameTime();

        if (is_skipping) {
            for(int i = 0; i < 20; ++i) {
                textAnimation.Update_Frame(time_delta);
            }
        } else {
            textAnimation.Update_Frame(time_delta);
        }
    }

    if (IsActionKeyPressed() || IsKeyPressed(KEY_ESCAPE)) {
        // Wenn Animation läuft -> Skippen aktivieren
        if (text_anim_started && !textAnimation.IsFinished()) {
            if (!is_skipping) {
                is_skipping = true;
                PlaySound(sound1);
            }
        }
        else if (text_anim_started && textAnimation.IsFinished()) {
            PlaySound(sound1);
            if (!has_fairy || current_level == 3) {
                auto mainMenuScene = std::make_shared<MainMenuScene>(final_score);
                game::core::Store::stage->SwitchToNewScene("MainMenu", mainMenuScene);
            } else {
                auto upgradeScene = std::make_shared<UpgradeScene>(final_souls, current_level);
                game::core::Store::stage->SwitchToNewScene("UpgradeScene", upgradeScene);
            }
        }
    }
}

void DeathScene::Draw() {
    float y = 9;
    if (frame < 6) y = frame + 3;
    if (frame == 6) { y = 8; PlaySound(deaths); }

    ClearBackground(BLACK);
    DrawTexturePro(dsprite, {frame * 16 + 28 * 16, 1, 16, 16},
                   {1920 / 2 - 8 * y, 1080 / 2 - 8 * y - sa, 16 * y, 16 * y}, {0, 0}, 0, WHITE);

    if (text_anim_started) {
        fairyAnim.Draw_Current_Frame_Pro(Vector2{ 140.0f, 400 });

        DrawTexturePro(
            textAnimation.GetSpritesheet(),
            textAnimation.GetCurrentFrameRec(),
            {1920 / 2 - 256.0f, 1080 / 2 - sa + 150, 512.0f * 2.0f, 208.0f * 2.0f},
            {0, 0}, 0, WHITE
        );

        const char* prompt_text = "";
        if (!textAnimation.IsFinished()) {
            prompt_text = "Druecke 'Angriff' um text zu skippen";
        } else {
            if (!has_fairy || current_level == 3) {
                prompt_text = "Druecke 'Angriff' um zur Bestenliste zu gelangen";
            } else {
                prompt_text = "Druecke 'Angriff' um weiter zu kommen";
            }
        }

        DrawTextEx(customFont, prompt_text, {1920 / 2 - 256.0f, 1080.0f / 2.0f - sa + 600}, 30, 1, RED);
    }
}

} // namespace game::scenes