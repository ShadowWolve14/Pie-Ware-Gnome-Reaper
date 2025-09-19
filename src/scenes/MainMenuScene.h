//
// Created by Kruse on 26/08/2025.
//

#ifndef RAYLIBSTARTER_MAINMENUSCENE_H
#define RAYLIBSTARTER_MAINMENUSCENE_H


#include "Scene.h"
#include "../config.h.in"
#include "store.h"
#include "GameScene.h"

struct HighscoreEntry
{
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
    Texture2D ConfirmButton;
    Font customFont;

    std::vector<std::string> arcade_keyboard_layout;
    Vector2 arcade_keyboard_cursor = {0, 0};
    int input_delay = 0;
    bool is_keyboard_uppercase = true;
    Texture2D ranged_attack_icon;
    Texture2D item_use_icon;

    Rectangle src{1,1,140,32};

    enum menustate {main,options, credits ,end,list};
    menustate state;

    enum liststate { VIEWING, AWAITING_INPUT, TYPING_NAME };
    liststate list_state = VIEWING;
    int counter;

    Music song = LoadMusicStream("assets/audio/tracks/MainMenuMusic.wav");
    Sound sound1 = LoadSound("assets/audio/sfx/Item_Obtained.wav");
    Sound sound2 = LoadSound("assets/audio/sfx/Enemy_Hit.wav");
    Sound sound3 = LoadSound("assets/audio/sfx/Gnome_RangeAttack.wav");
    Sound sound4 = LoadSound("assets/audio/sfx/Willhelm Scream.wav");
    float sfx_volume = 5.0f;

    int credits_page_index = 0;
    float last_page_switch_time = 0.0f;

    void Input_Check_Mov();
    std::vector<HighscoreEntry> highscores;
    bool highscores_loaded = false;
    int final_score = -1;
    int new_highscore_rank = -1;
    std::string player_name_input;
    const int max_name_length = 10;

    std::vector<std::string> Text;
    int d=0;
    int s=0;

    bool Input_Check_Sel();
    bool Input_Check_Back(); // Neue Hilfsfunktion
    void Initialize();

    // --- Arcade Mode Additions ---
    void UpdateTyping();
    void DrawArcadeKeyboard();
    // -----------------------------

    void main_Update();
    void main_Draw();
    void options_Update();
    void options_Draw();
    void credits_Update(float delta_time);
    void credits_Draw();
    void list_Update();
    void list_Draw();

    void LoadHighscores(const std::string& filename = "HighscoreList.txt");
    void SaveHighscores(const std::string& filename = "HighscoreList.txt");
    void CheckForNewHighscore();


public:
    MainMenuScene();
    explicit MainMenuScene(int final_score);
    ~MainMenuScene() override;

    void Update() override;
    void Draw() override;
};


#endif //RAYLIBSTARTER_MAINMENUSCENE_H
