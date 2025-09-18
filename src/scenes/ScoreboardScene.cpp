//
// Updated ScoreboardScene.cpp
// Ensures highscore file exists and logs problems for debugging.
//

#include "ScoreboardScene.h"
#include "MainMenuScene.h"
#include <raylib.h>
#include <Store.h>
#include "../config.h.in"

#include <filesystem>
#include <fstream>
#include <algorithm>
#include <system_error>

namespace game::scenes
{
    static std::vector<std::string> keyboardLayout = {
            "1234567890",
            "qwertyuiop",
            "asdfghjkl",
            "zxcvbnm"
    };

// Cursor state
    static int cursorRow = 0;
    static int cursorCol = 0;

    // Helper: ensure the directory exists and the file exists (create when missing)
    static void EnsureFileExists(const std::string& filename) {
        namespace fs = std::filesystem;
        std::error_code ec;

        fs::path p(filename);

        // Create parent directories if they don't exist
        if (p.has_parent_path()) {
            fs::create_directories(p.parent_path(), ec);
            if (ec) {
                TraceLog(LOG_WARNING, "EnsureFileExists: could not create directories for '%s' : %s",
                         filename.c_str(), ec.message().c_str());
            } else {
                TraceLog(LOG_INFO, "EnsureFileExists: parent directories exist/created for '%s'", filename.c_str());
            }
        }

        // Create the file if it doesn't exist
        if (!fs::exists(p)) {
            std::ofstream f(filename, std::ios::out);
            if (!f.is_open()) {
                TraceLog(LOG_ERROR, "EnsureFileExists: failed to create file '%s'", filename.c_str());
            } else {
                TraceLog(LOG_INFO, "EnsureFileExists: created new file '%s'", filename.c_str());
                f.close();
            }
        } else {
            TraceLog(LOG_INFO, "EnsureFileExists: file exists '%s'", filename.c_str());
        }
    }

    ScoreboardScene::ScoreboardScene(int score)
    {
        this->final_score = score;
        this->textmode = false;
        this->enterdname = false;

        // Use a 'friendly' default filename (you can still pass other names to AddHighscore)


        // Ensure the default file exists (useful if you later call AddHighscore without creating first)
        EnsureFileExists(defaultFile);

        // Helpful debug: print current working directory (useful if the file appears "missing")
        std::string cwd = std::filesystem::current_path().string();
        TraceLog(LOG_INFO, "ScoreboardScene initialized (CWD: %s)", cwd.c_str());

        lines= LoadHighscores(defaultFile);
    }

    ScoreboardScene::~ScoreboardScene() {}

    void ScoreboardScene::Update() {
        SetSoundVolume(sound1,game::core::Store::volume);
        SetSoundVolume(sound2,game::core::Store::volume);
        SetSoundVolume(sound3,game::core::Store::volume);

        if (textmode && !enterdname) {
            // Navigation
            if (IsKeyPressed(game::Config::key_Up) && cursorRow > 0) cursorRow--;
            if (IsKeyPressed(game::Config::key_Down) && cursorRow < (int)keyboardLayout.size()-1) cursorRow++;
            if (IsKeyPressed(game::Config::key_Left) && cursorCol > 0) cursorCol--;
            if (IsKeyPressed(game::Config::key_Right) && cursorCol < (int)keyboardLayout[cursorRow].size()-1) cursorCol++;

            // Select key
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if ((int)inputText.length() < maxLength) {
                    inputText.push_back(keyboardLayout[cursorRow][cursorCol]);
                    PlaySound(sound1);
                }
            }

            // Backspace
            if (IsKeyPressed(KEY_BACKSPACE) && !inputText.empty()) {
                inputText.pop_back();
                PlaySound(sound2);
            }

            // Finalize name
            if (IsKeyPressed(KEY_TAB)) { // e.g. TAB to finish input
                PlaySound(sound1);
                enterdname = true;
                textmode = false;

                const std::string filename = "HighscoreList.txt";
                AddHighscore(filename, inputText, final_score);
                lines= LoadHighscores(defaultFile);
            }
        }
        else {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(game::Config::key_Melee_Attack)) {
                textmode = true;
            }
            if (IsKeyPressed(game::Config::key_Ranged_Attack)) {
                auto mainMenu = std::make_shared<MainMenuScene>();
                game::core::Store::stage->SwitchToNewScene("MainMenu", mainMenu);
            }
        }
    }


        void ScoreboardScene::Draw() {
        float y=0;
            std::string scoret = "Score: " + std::to_string(final_score);
            DrawTextPro(game::core::Store::font, scoret.c_str(),
                        {game::Config::kStageWidth / 2 - 200, game::Config::kStageHeight / 2 - 400}, {0, 0}, 0, 50, 3,
                        WHITE);

            ClearBackground(BLACK);

            if (textmode) {
                y=600;
                // Show typed name
                std::string namefield = "Name: " + inputText;
                DrawTextPro(game::core::Store::font, namefield.c_str(),
                            {game::Config::kStageWidth / 2 - 200, game::Config::kStageHeight / 2 - 300}, {0, 0}, 0, 50,
                            3, WHITE);

                // Draw keyboard
                int keySize = 80;
                int spacing = 10;
                int startX = game::Config::kStageWidth / 2 - 400;
                int startY = game::Config::kStageHeight / 2 - 160;

                for (int r = 0; r < keyboardLayout.size(); r++) {
                    for (int c = 0; c < keyboardLayout[r].size(); c++) {
                        int x = startX + c * (keySize + spacing);
                        int y = startY + r * (keySize + spacing);

                        Rectangle keyRect = {(float) x, (float) y, (float) keySize, (float) keySize};
                        if (r == cursorRow && c == cursorCol) {
                            DrawRectangleRec(keyRect, SKYBLUE);
                        } else {
                            DrawRectangleLinesEx(keyRect, 2, DARKGRAY);
                        }

                        char keyChar = keyboardLayout[r][c];
                        DrawText(TextFormat("%c", keyChar),
                                 x + keySize / 2 - 5,
                                 y + keySize / 2 - 10,
                                 30, WHITE);
                    }
                }
            }
        DrawTextPro(game::core::Store::font,"Highscores:", {game::Config::kStageWidth/2-200,game::Config::kStageHeight/2-300+y}, {0,0}, 0, 50, 3, YELLOW);


        for (int s = 0; s < lines.size(); s++) {
            std::string output;
            output=std::to_string(s+1) +". "+lines[s].name+":";

            DrawTextPro(game::core::Store::font,std::to_string(lines[s].score).c_str(), {game::Config::kStageWidth/2+200,game::Config::kStageHeight/2-200+y+s*50}, {0,0}, 0, 50, 3, YELLOW);

            DrawTextPro(game::core::Store::font,output.c_str(), {game::Config::kStageWidth/2-400,game::Config::kStageHeight/2-200+y+s*50}, {0,0}, 0, 50, 3, YELLOW);
        }

        DrawTextPro(game::core::Store::font,"Drücke Range Attack um ins Hauptmenü zurück zu kehren", {game::Config::kStageWidth/2-950,game::Config::kStageHeight/2+500}, {0,0}, 0, 30, 3, RED);
        if (!textmode){
            DrawTextPro(game::core::Store::font,"Drücke Meele Attack um deinen Namen einzugeben", {game::Config::kStageWidth/2+250,game::Config::kStageHeight/2+500}, {0,0}, 0, 30, 3, GREEN);
        }


    }

    void ScoreboardScene::AddHighscore(const std::string& filename, const std::string& playerName, int playerScore) {
        // Ensure file and parent directories exist first
        EnsureFileExists(filename);

        std::vector<HighscoreEntry> highscores;

        // Read existing highscores (if any)
        std::ifstream infile(filename);
        if (!infile.is_open()) {
            TraceLog(LOG_WARNING, "AddHighscore: could not open '%s' for reading; starting with empty list.", filename.c_str());
        } else {
            std::string name;
            int score;
            while (infile >> name >> score) {
                highscores.push_back({name, score});
            }
            infile.close();
            TraceLog(LOG_INFO, "AddHighscore: loaded %d entries from '%s'", (int)highscores.size(), filename.c_str());
        }

        // Add new entry
        highscores.push_back({playerName, playerScore});
        TraceLog(LOG_INFO, "AddHighscore: adding '%s' score=%d", playerName.c_str(), playerScore);

        // Sort descending and keep top 10
        std::sort(highscores.begin(), highscores.end(),
                  [](const HighscoreEntry& a, const HighscoreEntry& b) {
                      return a.score > b.score;
                  });

        if (highscores.size() > 10) highscores.resize(10);

        // Write back to file
        std::ofstream outfile(filename, std::ios::trunc);
        if (!outfile.is_open()) {
            TraceLog(LOG_ERROR, "AddHighscore: failed to open '%s' for writing.", filename.c_str());
            return;
        }

        for (const auto& entry : highscores) {
            outfile << entry.name << " " << entry.score << "\n";
        }
        outfile.close();

        TraceLog(LOG_INFO, "AddHighscore: saved %d entries to '%s'", (int)highscores.size(), filename.c_str());
    }

    std::vector<HighscoreEntry> ScoreboardScene::LoadHighscores(const std::string& filename) {
        EnsureFileExists(filename);

        std::vector<HighscoreEntry> highscores;
        std::ifstream infile(filename);
        if (!infile.is_open()) {
            TraceLog(LOG_WARNING, "LoadHighscores: could not open '%s' for reading (returning empty).", filename.c_str());
            return highscores;
        }

        std::string name;
        int score;
        while (infile >> name >> score) {
            highscores.push_back({name, score});
        }
        infile.close();

        std::sort(highscores.begin(), highscores.end(),
                  [](const HighscoreEntry& a, const HighscoreEntry& b) {
                      return a.score > b.score;
                  });

        if (highscores.size() > 10) highscores.resize(10);

        TraceLog(LOG_INFO, "LoadHighscores: returning %d entries from '%s'", (int)highscores.size(), filename.c_str());
        return highscores;
    }
} // namespace game::scenes
