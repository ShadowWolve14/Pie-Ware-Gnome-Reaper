//
// Created by Kruse on 26/08/2025.
//

#include "MainMenuScene.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <system_error>

namespace {
    void EnsureFileExists(const std::string& filename)
    {
        namespace fs = std::filesystem;
        std::error_code ec;
        fs::path p(filename);
        if (p.has_parent_path()) {
            fs::create_directories(p.parent_path(), ec);
        }
        if (!fs::exists(p)) {
            std::ofstream f(filename, std::ios::out);
        }
    }

    float ReadValue(const std::string& filename)
    {
        EnsureFileExists(filename);
        std::ifstream f(filename);
        if (!f.is_open()) return 5.0f;
        float value;
        if (!(f >> value)) {
            return 5.0f;
        }
        f.close();
        return value;
    }

    void SaveValue(const std::string& filename, float value)
    {
        EnsureFileExists(filename);
        std::ofstream f(filename, std::ios::out | std::ios::trunc);
        if (!f.is_open()) return;
        f << value << std::endl;
        f.close();
    }
}

MainMenuScene::MainMenuScene()
{
    this->state = main;
    Initialize();
}

MainMenuScene::MainMenuScene(int final_score)
{
    this->state = list;
    this->final_score = final_score;
    Initialize();
}

void MainMenuScene::Initialize() {
    this->counter = 0;
    song.looping = true;
    PlayMusicStream(song);

    game::core::Store::volume = ReadValue("MusicSettings.txt");
    sfx_volume = ReadValue("SFXSettings.txt");

    customFont = LoadFont("PieWare/assets/Font/GnomishGame.ttf");
    BackButton = LoadTexture("PieWare/assets/UI/Allgemein/Button_Back.png");
    ConfirmButton = LoadTexture("PieWare/assets/UI/Allgemein/Confirm.png");
    SFXSlider = LoadTexture("PieWare/assets/UI/Allgemein/Volume.png");

    LoadHighscores();

    if (final_score != -1) {
        CheckForNewHighscore();
    }

    Text.resize(33);
    Text[0]="Gnome Reaper von Pie Ware";
    Text[1]="";
    Text[2]="Game Designer Jonas Hever";
    Text[3]="";
    Text[4]="Artist Kira Kutzer";
    Text[5]="Artist Leonardo Castrucci";
    Text[6]="Artist Leon Schermann";
    Text[7]="";
    Text[8]="Lead Programmer Will Becker";
    Text[9]="Programmer Finn Kruse";
    Text[10]="Programmer Marcel Rende";
    Text[11]="";
    Text[12]="";
    Text[13]="Ein riesen Dank gilt unseren externen Helfer*innen!";
    Text[14]="";
    Text[15]="Musik von Jana Schmidt";
    Text[16]="";
    Text[17]="Arcade Automat von Thorsten Heilmann";
    Text[18]="";
    Text[19]="Sponsor Steffen Hever";
    Text[20]="Sponsor Tobias Wessbecher";
    Text[21]="";
    Text[22]="Balancing und Excel Support Oliver Pruchnicki";
    Text[23]="";
    Text[24]="";
    Text[25]="Vielen Dank auch an unsere vielen Tester!";
    Text[26]="";
    Text[27]="";
    Text[28]="Natürlich auch Danke an euch fürs spielen!";
    Text[29]="";
    Text[30]="";
    Text[31]="";
    Text[32]="";
}

MainMenuScene::~MainMenuScene()
{
    UnloadTexture(BackButton);
    UnloadTexture(ConfirmButton);
    UnloadTexture(SFXSlider);
    UnloadFont(customFont);
}

void MainMenuScene::Update()
{
    SetMusicVolume(song, game::core::Store::volume);
    UpdateMusicStream(song);
    SetSoundVolume(sound1, sfx_volume);
    SetSoundVolume(sound2, sfx_volume);
    SetSoundVolume(sound3, sfx_volume);

    switch (state)
    {
        case main:    main_Update();    break;
        case options: options_Update(); break;
        case list:    list_Update();    break;
        case credits: credits_Update(); break;
        case end:     game::core::Store::running = false; PlaySound(sound4); break;
    }
}

void MainMenuScene::Input_Check_Mov() {
    if (IsKeyPressed(game::Config::key_Up)){
        this->counter= this->counter-1;
        PlaySound(sound3);
    }
    if (IsKeyPressed(game::Config::key_Down)){
        this->counter= this->counter+1;
        PlaySound(sound3);
    }
}
bool MainMenuScene::Input_Check_Sel() {
    if (IsKeyPressed(game::Config::key_Melee_Attack) || IsKeyPressed(KEY_ENTER)) {
        PlaySound(sound1);
        return true;

    } else {
        return false;
    }
}

void MainMenuScene::main_Update() {
    Input_Check_Mov();

    if (counter > 4) {
        counter = 0;
    }
    if (counter < 0) {
        counter = 4;
    }
    if (Input_Check_Sel()) {
        switch (counter) {
            case 0: {
                game::core::Store::player_state = nullptr;
                game::core::upgrades = {};
                game::core::Store::stage->ReplaceWithNewScene("menu", "game", std::make_unique<game::scenes::GameScene>());
                break;
            }
            case 1: {
                counter = 0;
                state = options;
                break;
            }
            case 2: {
                state = list;
                list_state = VIEWING;
                counter = 0;
                break;
            }
            case 3: {
                state = credits;
                break;
            }
            case 4: {
                state = end;
                break;
            }
        }
    }
}

void MainMenuScene::main_Draw() {
    DrawTextureEx(scroll_button, {game::Config::kStageWidth / 2 - 420 - 20, 20 + 90 * 3 - 100}, 0, 4, WHITE);
    DrawTextPro(game::core::Store::font,"Drücke Range Attack um ins Hauptmenü zurück zu kehren", {game::Config::kStageWidth/2-950,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);
    DrawTextPro(game::core::Store::font,"Drücke Meele Attack um den Aktuellen Button auszuwählen", {game::Config::kStageWidth/2+150,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);
    DrawTextureEx(sign_button, {game::Config::kStageWidth / 2 - 3 * 228, 20}, 0, 3, WHITE);
    Rectangle dest{game::Config::kStageWidth / 2 - 148, 0, 280, 64};

    if (counter == 0) {
        src.x = 145;
        dest.y = 350;
        DrawTexturePro(start_button, src, dest, {0, 0}, 0, WHITE);
    } else {
        src.x = 1;
        dest.y = 350;
        DrawTexturePro(start_button, src, dest, {0, 0}, 0, WHITE);
    }
    if (counter == 1) {
        src.x = 145;
        dest.y = 440;
        DrawTexturePro(options_button, src, dest, {0, 0}, 0, WHITE);
    } else {
        src.x = 1;
        dest.y = 440;
        DrawTexturePro(options_button, src, dest, {0, 0}, 0, WHITE);
    }
    if (counter == 2) {
        src.x = 145;
        dest.y = 530;
        DrawTexturePro(ranking_button, src, dest, {0, 0}, 0, WHITE);
    } else {
        src.x = 1;
        dest.y = 530;
        DrawTexturePro(ranking_button, src, dest, {0, 0}, 0, WHITE);
    }
    if (counter == 3) {
        src.x = 145;
        dest.y = 620;
        DrawTexturePro(credits_button, src, dest, {0, 0}, 0, WHITE);
    } else {
        src.x = 1;
        dest.y = 620;
        DrawTexturePro(credits_button, src, dest, {0, 0}, 0, WHITE);
    }
    if (counter == 4) {
        src.x = 145;
        dest.y = 710;
        DrawTexturePro(quit_button, src, dest, {0, 0}, 0, WHITE);
    } else {
        src.x = 1;
        dest.y = 710;
        DrawTexturePro(quit_button, src, dest, {0, 0}, 0, WHITE);
    }
}

void MainMenuScene::Draw() {
    Color grer{100,125,52,255};
    ClearBackground(grer);

    switch (state) {
        case main:    main_Draw();    break;
        case options: options_Draw(); break;
        case list:    list_Draw();    break;
        case credits: credits_Draw(); break;
    }
}
void MainMenuScene::options_Update() {
    Input_Check_Mov();
    if (counter > 3) {
        counter = 0;
    }
    if (counter < 0) {
        counter = 3;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        state = main;
        counter = 1;
        PlaySound(sound2);
        return;
    }

    if (counter == 1) {
        if (IsKeyPressed(game::Config::key_Left)) {
            game::core::Store::volume -= 0.5;
            if (game::core::Store::volume < 0) game::core::Store::volume = 0;
        }
        if (IsKeyPressed(game::Config::key_Right)) {
            game::core::Store::volume += 0.5;
            if (game::core::Store::volume > 5) game::core::Store::volume = 5;
        }
        SaveValue("MusicSettings.txt", game::core::Store::volume);

    } else if (counter == 2) {
        if (IsKeyPressed(game::Config::key_Left)) {
            sfx_volume -= 0.5;
            if (sfx_volume < 0) sfx_volume = 0;
            PlaySound(sound1);
        }
        if (IsKeyPressed(game::Config::key_Right)) {
            sfx_volume += 0.5;
            if (sfx_volume > 5) sfx_volume = 5;
            PlaySound(sound1);
        }
        SaveValue("SFXSettings.txt", sfx_volume);
    }

    if (Input_Check_Sel()) {
        if (counter == 0) {
            ToggleFullscreen();
        } else if (counter == 3) {
            state = main;
            counter = 1;
        }
    }
}
void MainMenuScene::options_Draw() {
    DrawTextureEx(scroll_button,{game::Config::kStageWidth/2-420,20+90*3-100},0,4,WHITE);
    DrawTexturePro(TB,{40+230*3,1,230,48},{game::Config::kStageWidth/2-460,20,230*4,48*4},{0,0},0,WHITE);

    if (counter==0){
        if (IsWindowFullscreen()){
            DrawTexturePro(FullscreenButton,{150-6,1,144,64},{game::Config::kStageWidth/2-72*2,350,144*2,64*2},{0,0},0,WHITE);
        } else{
            DrawTexturePro(FullscreenButton,{150*3-16,1,144,64},{game::Config::kStageWidth/2-72*2,350,144*2,64*2},{0,0},0,WHITE);
        }
    }
    else{
        if (IsWindowFullscreen()){
            DrawTexturePro(FullscreenButton,{1,1,144,64},{game::Config::kStageWidth/2-72*2,350,144*2,64*2},{0,0},0,WHITE);
        } else{
            DrawTexturePro(FullscreenButton,{150*2-10,1,144,64},{game::Config::kStageWidth/2-72*2,350,144*2,64*2},{0,0},0,WHITE);
        }
    }
    DrawTextEx(customFont, "Musik", {game::Config::kStageWidth/2 - 50, 480}, 40, 2, BLACK);
    int music_volume_step = 10 - (int)(game::core::Store::volume * 2);
    int music_frame_offset = (counter == 1) ? 11 : 0;
    float music_source_x = (float)((music_volume_step + music_frame_offset) * 128);
    Rectangle music_source_rect = { music_source_x, 1, 128, 48 };
    Rectangle music_dest_rect = { game::Config::kStageWidth/2 - (128 * 2.4f / 2), 520, 128 * 2.4f, 48 * 2.4f };
    DrawTexturePro(VolumeSlider, music_source_rect, music_dest_rect, {0,0}, 0, WHITE);

    DrawTextEx(customFont, "SFX", {game::Config::kStageWidth/2 - 30, 640}, 40, 2, BLACK);
    int sfx_volume_step = 10 - (int)(sfx_volume * 2);
    int sfx_frame_offset = (counter == 2) ? 11 : 0;
    float sfx_source_x = (float)((sfx_volume_step + sfx_frame_offset) * 128);
    Rectangle sfx_source_rect = { sfx_source_x, 1, 128, 48 };
    Rectangle sfx_dest_rect = { game::Config::kStageWidth/2 - (128 * 2.4f / 2), 680, 128 * 2.4f, 48 * 2.4f };
    DrawTexturePro(SFXSlider, sfx_source_rect, sfx_dest_rect, {0,0}, 0, WHITE);

    float back_button_source_x = (counter == 3) ? 160.0f : 0.0f;
    Rectangle back_source = { back_button_source_x, 0, 160, 48 };
    Rectangle back_dest = { game::Config::kStageWidth/2 - (160 * 2.0f / 2), 840, 160 * 2.0f, 48 * 2.0f };
    DrawTexturePro(BackButton, back_source, back_dest, {0,0}, 0, WHITE);
}
void MainMenuScene::list_Update() {
    switch(list_state) {
        case VIEWING: {
            if (Input_Check_Sel() || IsKeyPressed(KEY_ESCAPE)) {
                state = main;
                counter = 2;
                PlaySound(sound2);
            }
            break;
        }

        case AWAITING_INPUT: {
            Input_Check_Mov();
            if (counter > 1) counter = 0;
            if (counter < 0) counter = 1;

            if (Input_Check_Sel()) {
                if (counter == 0) {
                    list_state = TYPING_NAME;
                } else if (counter == 1) {
                    if (new_highscore_rank != -1) {
                        if (player_name_input.empty()) {
                            highscores[new_highscore_rank].name = "________";
                        } else {
                            highscores[new_highscore_rank].name = player_name_input;
                        }
                        SaveHighscores();
                    }
                    state = main;
                    counter = 2;
                }
            }
            break;
        }

        case TYPING_NAME: {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (player_name_input.length() < max_name_length)) {
                    player_name_input.push_back((char)key);
                    PlaySound(sound1);
                } else if (player_name_input.length() >= max_name_length) {
                    PlaySound(sound2);
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !player_name_input.empty()) {
                player_name_input.pop_back();
                PlaySound(sound2);
            }

            if (IsKeyPressed(KEY_ENTER)) {
                PlaySound(sound1);
                list_state = AWAITING_INPUT;
                counter = 0;
            }
            break;
        }
    }
}
void MainMenuScene::list_Draw() {
    DrawTextureEx(scroll_button, {game::Config::kStageWidth / 2 - 420 - 20, 20 + 90 * 3 - 100}, 0, 4, WHITE);
    DrawTexturePro(TB, {30 + 230 * 2, 1, 230, 48}, {game::Config::kStageWidth / 2 - 460, 20, 230 * 4, 48 * 4}, {0, 0}, 0, WHITE);
    Vector2 titlePos = {game::Config::kStageWidth / 2 - 150, 282};
    DrawTextPro(customFont, "Highscores:", titlePos, {0, 0}, 0, 50, 3, BLACK);
    Vector2 titleSize = MeasureTextEx(customFont, "Highscores:", 50, 3);
    DrawRectangle((int)titlePos.x, (int)(titlePos.y + titleSize.y - 5), (int)titleSize.x, 4, BLACK);
    Color highlightColor = {228, 148, 59, 255};

    for (int i = 0; i < 10; ++i) {
        float y_pos = 350 + i * 50;
        std::string rank_str = std::to_string(i + 1) + ".";
        std::string name_str = "----------------";
        std::string score_str = "0";
        Color name_color = BLACK;

        if (i < highscores.size()) {
            if (highscores[i].name != "________") { name_str = highscores[i].name; }
            score_str = std::to_string(highscores[i].score);
        }

        if (i == new_highscore_rank && final_score != -1) {
            score_str = std::to_string(final_score);
            name_color = highlightColor;

            if (player_name_input.empty()) {
                if (list_state == TYPING_NAME) {
                    if ((int)(GetTime() * 2.5f) % 2 == 0) name_str = "Dein Name";
                    else name_str = " ";
                } else {
                     name_str = "Dein Name";
                }
            } else {
                name_str = player_name_input;
            }

            if (list_state == TYPING_NAME && (int)(GetTime() * 2.0f) % 2 == 0) {
                name_str += "_";
            }
        }

        DrawTextEx(customFont, rank_str.c_str(), {680, y_pos}, 40, 2, BLACK);
        DrawTextEx(customFont, name_str.c_str(), {750, y_pos}, 40, 2, name_color);
        DrawTextEx(customFont, score_str.c_str(), {1104, y_pos}, 40, 2, BLACK);
    }

    const float button_y_pos = 840.0f;

    if (list_state == VIEWING) {
        float source_x = 160.0f;
        Rectangle source = {source_x, 0, 160, 48};
        Rectangle dest = {game::Config::kStageWidth / 2 - (160 * 2.0f / 2), button_y_pos, 160 * 2.0f, 48 * 2.0f};
        DrawTexturePro(BackButton, source, dest, {0, 0}, 0, WHITE);
    } else {
        float source_x = (counter == 1 && list_state == AWAITING_INPUT) ? 160.0f : 0.0f;
        Rectangle source = {source_x, 0, 160, 48};
        Rectangle dest = {game::Config::kStageWidth / 2 - (160 * 2.0f / 2), button_y_pos, 160 * 2.0f, 48 * 2.0f};
        DrawTexturePro(ConfirmButton, source, dest, {0, 0}, 0, WHITE);
    }
}

void MainMenuScene::credits_Update()
{

}
void MainMenuScene::credits_Draw()
{
    DrawTextureEx(scroll_button,{game::Config::kStageWidth/2-795,1500},-90,7,WHITE);
    DrawTexturePro(TB,{20+230*1,1,230,48},{game::Config::kStageWidth/2-460,20,230*4,48*4},{0,0},0,WHITE);
    DrawTextPro(game::core::Store::font,"Drücke Range Attack um ins Hauptmenü zurück zu kehren", {game::Config::kStageWidth/2-950,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);
    DrawTextPro(game::core::Store::font,"Drücke Meele Attack um den Aktuellen Button auszuwählen", {game::Config::kStageWidth/2+150,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);

    for (int l = 0; l < 4; ++l) {
        for (int i = 0; i < 8; ++i) {
            if (i+8*s <= Text.size()-1) {

                Vector2 textPos = {
                    (float)(game::Config::kStageWidth/2 - TextLength(Text[i+8*s].c_str())*11),
                    (float)(game::Config::kStageHeight/2 - 150 + 50*i)
                };
                DrawTextPro(game::core::Store::font, Text[i+8*s].c_str(), textPos, {0,0}, 0, 35, 5, BLACK);
            } else {
                s = -1;
            }
        }
        d++;
        if (d == 60 * game::Config::credits_anim_speed * 10) {
            s++;
            d = 0;
        }
    }
}

void MainMenuScene::LoadHighscores(const std::string& filename) {
    EnsureFileExists(filename);
    highscores.clear();
    std::ifstream infile(filename);
    if (!infile.is_open()) return;

    std::string name;
    int score;
    while (infile >> name >> score) {
        highscores.push_back({name, score});
    }
    infile.close();

    std::sort(highscores.begin(), highscores.end(), [](const HighscoreEntry& a, const HighscoreEntry& b) {
        return a.score > b.score;
    });

    highscores_loaded = true;
}

void MainMenuScene::SaveHighscores(const std::string& filename) {
    EnsureFileExists(filename);
    std::ofstream outfile(filename, std::ios::trunc);
    if (!outfile.is_open()) return;

    for (const auto& entry : highscores) {
        outfile << entry.name << " " << entry.score << "\n";
    }
    outfile.close();
}

void MainMenuScene::CheckForNewHighscore() {
    int rank = -1;
    for (int i = 0; i < 10; ++i) {
        if (i >= highscores.size() || final_score > highscores[i].score) {
            rank = i;
            break;
        }
    }

    if (rank != -1) {
        new_highscore_rank = rank;
        player_name_input = "";
        HighscoreEntry new_entry = {"________", final_score};
        highscores.insert(highscores.begin() + rank, new_entry);
        if (highscores.size() > 10) {
            highscores.resize(10);
        }
        list_state = TYPING_NAME;
    } else {
        new_highscore_rank = -1;
        list_state = AWAITING_INPUT;
        counter = 0;
    }
}