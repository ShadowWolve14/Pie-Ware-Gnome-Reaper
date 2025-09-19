//
// Created by Kruse on 26/08/2025.
//

#include "MainMenuScene.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <system_error>

namespace {

    std::string ReplaceUmlauts(std::string text) {
        size_t pos = 0;
        while ((pos = text.find("ä", pos)) != std::string::npos) { text.replace(pos, 2, "ae"); pos += 2; }
        pos = 0;
        while ((pos = text.find("ö", pos)) != std::string::npos) { text.replace(pos, 2, "oe"); pos += 2; }
        pos = 0;
        while ((pos = text.find("ü", pos)) != std::string::npos) { text.replace(pos, 2, "ue"); pos += 2; }
        pos = 0;
        while ((pos = text.find("Ä", pos)) != std::string::npos) { text.replace(pos, 2, "Ae"); pos += 2; }
        pos = 0;
        while ((pos = text.find("Ö", pos)) != std::string::npos) { text.replace(pos, 2, "Oe"); pos += 2; }
        pos = 0;
        while ((pos = text.find("Ü", pos)) != std::string::npos) { text.replace(pos, 2, "Ue"); pos += 2; }
        return text;
    }

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

    credits_page_index = 0;
    last_page_switch_time = (float)GetTime();

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
    Text[13]="Ein riesen Dank gilt";
    Text[14]="unseren externen Helfer*innen!";
    Text[15]="";
    Text[16]="Musik von Jana Schmidt";
    Text[17]="Arcade Automat ";
    Text[18]="von Thorsten Heilmann";
    Text[19]="";
    Text[20]="Sponsor Steffen Hever";
    Text[21]="Sponsor Tobias Wessbecher";
    Text[22]="";
    Text[23]="";
    Text[24]="Balancing und Excel Support";
    Text[25]="Oliver Pruchnicki";
    Text[26]="";
    Text[27]="Vielen Dank auch an unsere";
    Text[28]="vielen Tester!";
    Text[29]="";
    Text[30]="Natuerlich auch Danke an euch";
    Text[31]="fuers spielen!";
    Text[32]="";

    for(auto& line : Text) {
        line = ReplaceUmlauts(line);
    }
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
        case credits: credits_Update(GetFrameTime()); break;
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
        PlaySound(sound1);
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

void MainMenuScene::main_Draw()
{
    Vector2 scroll_pos = {game::Config::kStageWidth / 2.0f - 440.0f, 190.0f};
    DrawTextureEx(scroll_button, scroll_pos, 0, 4, WHITE);

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
        if (counter == 0) { // Fullscreen-Button
            PlaySound(sound1); // Bestätigungssound
            ToggleFullscreen();
        } else if (counter == 3) { // Zurück-Button
            PlaySound(sound2); // Zurück-Sound
            state = main;
            counter = 1;
        }
    }
}
void MainMenuScene::options_Draw()
{
    Vector2 scroll_pos = {game::Config::kStageWidth / 2.0f - 440.0f, 190.0f};
    DrawTextureEx(scroll_button, scroll_pos, 0, 4, WHITE);

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
void MainMenuScene::list_Update()
{
    switch(list_state) {
        case VIEWING:
        {
            if (IsKeyPressed(KEY_ESCAPE) || Input_Check_Sel()) {
                state = main;
                counter = 2;
                PlaySound(sound2);
            }
            break;
        }

        case AWAITING_INPUT: {
            if (new_highscore_rank != -1) {
                Input_Check_Mov();
                if (counter > 1) counter = 0;
                if (counter < 0) counter = 1;
            } else {
                counter = 0;
            }
            if (Input_Check_Sel()) {
                PlaySound(sound1);
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
                        new_highscore_rank = -1;
                        final_score = -1;
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
    Vector2 scroll_pos = {game::Config::kStageWidth / 2.0f - 440.0f, 190.0f};
    DrawTextureEx(scroll_button, scroll_pos, 0, 4, WHITE);

    DrawTexturePro(TB, {30 + 230 * 2, 1, 230, 48}, {game::Config::kStageWidth / 2 - 460, 20, 230 * 4, 48 * 4}, {0, 0}, 0, WHITE);
    Vector2 titlePos = {game::Config::kStageWidth / 2 - 150, 298};
    DrawTextPro(customFont, "Highscores:", titlePos, {0, 0}, 0, 50, 3, BLACK);
    Vector2 titleSize = MeasureTextEx(customFont, "Highscores:", 50, 3);
    DrawRectangle((int)titlePos.x, (int)(titlePos.y + titleSize.y - 5), (int)titleSize.x, 4, BLACK);
    Color highlightColor = {228, 148, 59, 255};

    const float content_center_x = game::Config::kStageWidth / 2.0f;
    const float rank_x = content_center_x - 280;
    const float name_x = content_center_x - 210;
    const float score_x = content_center_x + 144;

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

        if (i == new_highscore_rank) {
            score_str = std::to_string(final_score);
            name_color = highlightColor;

            if (list_state == TYPING_NAME) {
                name_str = player_name_input;
                if ((int)(GetTime() * 2.0f) % 2 == 0) name_str += "____";
            } else if (list_state == AWAITING_INPUT) {
                name_str = player_name_input.empty() ? "Dein Name" : player_name_input;
            }
        }

        DrawTextEx(customFont, rank_str.c_str(), {rank_x, y_pos}, 40, 2, BLACK);
        DrawTextEx(customFont, name_str.c_str(), {name_x, y_pos}, 40, 2, name_color);
        DrawTextEx(customFont, score_str.c_str(), {score_x, y_pos}, 40, 2, BLACK);
    }

    const float button_y_pos = 840.0f;

    if (list_state == VIEWING) {
        float source_x = 160.0f;
        Rectangle source = {source_x, 0, 160, 48};
        Rectangle dest = {game::Config::kStageWidth / 2 - (160 * 2.0f / 2), button_y_pos, 160 * 2.0f, 48 * 2.0f};
        DrawTexturePro(BackButton, source, dest, {0, 0}, 0, WHITE);
    } else {

        bool confirm_selected = (new_highscore_rank == -1 || (counter == 1 && list_state == AWAITING_INPUT));
        float source_x = confirm_selected ? 160.0f : 0.0f;

        Rectangle source = {source_x, 0, 160, 48};
        Rectangle dest = {game::Config::kStageWidth / 2 - (160 * 2.0f / 2), button_y_pos, 160 * 2.0f, 48 * 2.0f};
        DrawTexturePro(ConfirmButton, source, dest, {0, 0}, 0, WHITE);
    }
}

void MainMenuScene::credits_Update(float delta_time)
{
    if (IsKeyPressed(KEY_ESCAPE) || Input_Check_Sel()) {
        PlaySound(sound2);
        state = main;
        counter = 3;
        credits_page_index = 0;
        last_page_switch_time = (float)GetTime();
        return;
    }

    const float page_hold_duration = 5.0f;
    const int lines_per_page = 11;
    const int total_pages = (Text.size() + lines_per_page - 1) / lines_per_page;

    if (GetTime() - last_page_switch_time > page_hold_duration) {
        last_page_switch_time = (float)GetTime();
        credits_page_index = (credits_page_index + 1) % total_pages;
    }
}

void MainMenuScene::credits_Draw()
{
    Vector2 scroll_pos = {game::Config::kStageWidth / 2.0f - 440.0f, 190.0f};
    DrawTextureEx(scroll_button, scroll_pos, 0, 4, WHITE);

    DrawTexturePro(TB,{20+230*1,1,230,48},{game::Config::kStageWidth/2-460,20,230*4,48*4},{0,0},0,WHITE);

    const int lines_per_page = 11;
    const float font_size = 28.0f;
    const float line_spacing = 36.0f;

    const float text_area_width = 750.0f;
    const float text_area_start_x = (game::Config::kStageWidth - text_area_width) / 2.0f;

    const float start_y = 340.0f;

    for (int i = 0; i < lines_per_page; ++i) {
        int text_index = i + lines_per_page * credits_page_index;
        if (text_index < Text.size() && !Text[text_index].empty()) {
            const std::string& current_text = Text[text_index];

            Vector2 text_size = MeasureTextEx(customFont, current_text.c_str(), font_size, 2);
            Vector2 text_pos = {
                text_area_start_x + (text_area_width - text_size.x) / 2.0f,
                start_y + (i * line_spacing)
            };
            DrawTextEx(customFont, current_text.c_str(), text_pos, font_size, 2, BLACK);
        }
    }

    float back_button_source_x = 160.0f;
    Rectangle back_source = { back_button_source_x, 0, 160, 48 };
    Rectangle back_dest = { game::Config::kStageWidth/2 - (160 * 2.0f / 2), 840, 160 * 2.0f, 48 * 2.0f };
    DrawTexturePro(BackButton, back_source, back_dest, {0,0}, 0, WHITE);
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
        list_state = VIEWING;
        counter = 0;
    }
}