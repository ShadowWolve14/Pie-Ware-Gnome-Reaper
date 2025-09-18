//
// Created by Kruse on 26/08/2025.
//

#ifndef RAYLIBSTARTER_MAINMENUSCENE_H
#define RAYLIBSTARTER_MAINMENUSCENE_H


#include "Scene.h"
#include "../config.h.in"
#include "store.h"
#include "GameScene.h"
struct HighscoreEntry {
    std::string name;
    int score;
};
class MainMenuScene : public game::core::Scene{
private:
    Texture2D start_button= LoadTexture(game::Config::start_button_asset);
    Texture2D options_button= LoadTexture(game::Config::option_button_asset);
    Texture2D ranking_button= LoadTexture(game::Config::ranking_button_asset);
    Texture2D credits_button= LoadTexture(game::Config::credits_button_asset);
    Texture2D quit_button= LoadTexture(game::Config::quit_button_asset);
    Texture2D sign_button= LoadTexture(game::Config::Back_Sign_asset);
    Texture2D scroll_button= LoadTexture(game::Config::Background_asset);
    Texture2D FullscreenButton= LoadTexture("PieWare/assets/UI/Allgemein/FensterlosButton.png");
    Texture2D VolumeSlider= LoadTexture("PieWare/assets/UI/Allgemein/Volume.png");
    Texture2D TB= LoadTexture("PieWare/assets/UI/Allgemein/Title_Banner.png");
    Texture2D SFXSlider;
    Texture2D BackButton;
    Font customFont;
    Rectangle src{1,1,140,32};

    enum menustate {main,options, credits ,end,list};
    menustate state;
int counter;

Music song = LoadMusicStream("assets/audio/tracks/MainMenuMusic.wav");
Sound sound1 = LoadSound("assets/audio/sfx/Item_Obtained.wav");
    Sound sound2 = LoadSound("assets/audio/sfx/Enemy_Hit.wav");
    Sound sound3 = LoadSound("assets/audio/sfx/Gnome_RangeAttack.wav");
    Sound sound4 = LoadSound("assets/audio/sfx/Willhelm Scream.wav");


    bool slider;
    bool prot;
    float vol = 5.0f;
    float sfx_volume = 5.0f;

    std::vector<HighscoreEntry> lines;
    bool loaded= false;
    std::vector<std::string> Text;
    int d=0;
    int s=0;

    void Input_Check_Mov();
    bool Input_Check_Sel();

    void main_Update();
    void main_Draw();
    void options_Update();
    void options_Draw();
    void credits_Update();
    void credits_Draw();
    void list_Update();
    void list_Draw();

    std::vector<HighscoreEntry> LoadHighscores(const std::string& filename);

public:
    MainMenuScene();
    ~MainMenuScene() override ;
    void Update() override;

    void Draw() override;

};


#endif //RAYLIBSTARTER_MAINMENUSCENE_H
