//
// Created by $Will on 01.09.2025.
//

#ifndef SCOREBOARDSCENE_H
#define SCOREBOARDSCENE_H
#include "Scene.h"
#include <fstream>
#include <vector>

namespace game::scenes
{
    struct HighscoreEntry {
        std::string name;
        int score;
    };
    class ScoreboardScene final : public game::core::Scene
    {
    private:
        int final_score;
        std::ifstream file;
        std::vector<HighscoreEntry> lines;
        bool textmode;
        bool enterdname;
        std::string inputText;
        const int maxLength = 20;
        const std::string defaultFile = "HighscoreList.txt";


    public:
        ScoreboardScene(int score);
        ~ScoreboardScene() override;
        void Update() override;
        void Draw() override;
        void AddHighscore(const std::string& filename, const std::string& playerName, int playerScore);
        std::vector<HighscoreEntry> LoadHighscores(const std::string& filename);
    };
}


#endif //SCOREBOARDSCENE_H
