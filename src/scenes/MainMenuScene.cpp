//
// Created by Kruse on 26/08/2025.
//

#include "MainMenuScene.h"

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

static void SaveValue(const std::string& filename, float value) {
    EnsureFileExists(filename);
    std::ofstream f(filename, std::ios::out | std::ios::trunc); // overwrite mode
    if (!f.is_open()) {
        TraceLog(LOG_ERROR, "SaveValue: failed to open file '%s'", filename.c_str());
        return;
    }
    f << value << std::endl;
    TraceLog(LOG_INFO, "SaveValue: wrote value to '%s'", filename.c_str());
    f.close();
}

static float ReadValue(const std::string& filename) {
    EnsureFileExists(filename);
    std::ifstream f(filename);
    if (!f.is_open()) {
        TraceLog(LOG_ERROR, "ReadValue: failed to open file '%s'", filename.c_str());
        return 5.0f; // fallback if cannot open
    }

    float value;
    if (!(f >> value)) {
        TraceLog(LOG_WARNING, "ReadValue: file '%s' empty or invalid, using default value 5.0", filename.c_str());
        value = 5.0f;
        SaveValue(filename, value); // make sure file contains the default
    } else {
        TraceLog(LOG_INFO, "ReadValue: read value from '%s'", filename.c_str());
    }

    f.close();
    return value;
}
MainMenuScene::MainMenuScene() {
    this->counter=0;
    state=main;
    song.looping= true;
    PlayMusicStream(song);

    game::core::Store::volume= ReadValue("AudioSettings.txt");

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
MainMenuScene::~MainMenuScene() { }

void MainMenuScene::Update()
{
    SetMusicVolume(song,game::core::Store::volume);
    UpdateMusicStream(song);
    SetSoundVolume(sound1,game::core::Store::volume);
    SetSoundVolume(sound2,game::core::Store::volume);
    SetSoundVolume(sound3,game::core::Store::volume);



    switch (state) {
        case main:{
            Input_Check_Mov();
            main_Update();
            break;
        }
        case options:{
            options_Update();
            break;
        }
        case list:{
            list_Update();
            break;
        }
        case credits:{
            credits_Update();
            break;
        }
        case end:{
            game::core::Store::running= false;
            PlaySound(sound4);

            break;
        }
    }

}

void MainMenuScene::Draw()
{
    Color grer{100,125,52,255};

    ClearBackground(grer);

    DrawTextureEx(scroll_button,{game::Config::kStageWidth/2-315,20+90*3-50},0,3,WHITE);


    switch (state) {
        case main:{
            main_Draw();
            break;
        }
        case options:{
            options_Draw();
            break;
        }
        case list:{
            list_Draw();
            break;
        }
        case credits:{
            credits_Draw();
            break;
        }
    }
}
void MainMenuScene::Input_Check_Mov() {
    if (GetGamepadAxisMovement(0, 1) < -0.5&&input_delay<1){
        this->counter= this->counter-1;
        input_delay=10;
        PlaySound(sound3);
    }
    if (GetGamepadAxisMovement(0, 1) > 0.5&&input_delay<1){
        this->counter= this->counter+1;
        input_delay=10;
        PlaySound(sound3);
    }
    input_delay--;
}
bool MainMenuScene::Input_Check_Sel() {
    if (IsGamepadButtonPressed(0,7)) {
        PlaySound(sound1);
        return true;

    } else {
        return false;
    }
}
void MainMenuScene::Input_Check_Back() {
    if (IsGamepadButtonPressed(0,6)){
        state=main;
        PlaySound(sound2);
    }
}
void MainMenuScene::main_Update() {

    if (counter>4){
        counter=counter-5;
    }
    if (counter<0){
        counter=4;
    }
    if (Input_Check_Sel()){

        switch (counter) {
            case 0:{
                game::core::Store::player_state = nullptr;
                game::core::upgrades = {};
                game::core::Store::stage->ReplaceWithNewScene("menu", "game", std::make_unique<game::scenes::GameScene>());
                break;
            }
            case 1:{
                counter=0;
                state=options;
                break;
            }
            case 2:{
                state=list;
                loaded= false;
                break;
            }
            case 3:{
                state=credits;
                break;
            }
            case 4:{
                state=end;
                break;
            }
        }
    }
}
void MainMenuScene::main_Draw() {

    DrawTextPro(game::core::Store::font,"Drücke Range Attack um ins Hauptmenü zurück zu kehren", {game::Config::kStageWidth/2-950,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);
    DrawTextPro(game::core::Store::font,"Drücke Meele Attack um den Aktuellen Button auszuwählen", {game::Config::kStageWidth/2+150,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);


    DrawTextureEx(sign_button,{game::Config::kStageWidth/2-3*228,20},0,3,WHITE);
    Rectangle dest{game::Config::kStageWidth/2-140,150,280,64};
    if (counter==0){
        // Draw Highlited Button
        src.x=145;
        dest.y=350;
        DrawTexturePro(start_button,src,dest,{0,0},0,WHITE);

    }
    else{
        src.x=1;
        dest.y=350;
        DrawTexturePro(start_button,src,dest,{0,0},0,WHITE);
    }
    if (counter==1){
// Draw Highlited Button
        src.x=145;
        dest.y=420;
        DrawTexturePro(options_button,src,dest,{0,0},0,WHITE);
    }
    else{
//Draw Regular Button Asset
        src.x=1;
        dest.y=420;
        DrawTexturePro(options_button,src,dest,{0,0},0,WHITE);
    }
    if (counter==2){
// Draw Highlited Button
        src.x=145;
        dest.y=490;
        DrawTexturePro(ranking_button,src,dest,{0,0},0,WHITE);
    }
    else{
//Draw Regular Button Asset
        src.x=1;
        dest.y=490;
        DrawTexturePro(ranking_button,src,dest,{0,0},0,WHITE);
    }
    if (counter==3){
// Draw Highlited Button
        src.x=145;
        dest.y=560;
        DrawTexturePro(credits_button,src,dest,{0,0},0,WHITE);
    }
    else{
//Draw Regular Button Asset
        src.x=1;
        dest.y=560;
        DrawTexturePro(credits_button,src,dest,{0,0},0,WHITE);
    }
    if (counter==4){
// Draw Highlited Button
        src.x=145;
        dest.y=630;
        DrawTexturePro(quit_button,src,dest,{0,0},0,WHITE);
    }
    else{
//Draw Regular Button Asset
        src.x=1;
        dest.y=630;
        DrawTexturePro(quit_button,src,dest,{0,0},0,WHITE);
    }
}
void MainMenuScene::options_Update() {
        Input_Check_Mov();
        if (counter>1){
            counter=counter-2;
        }
        if (counter<0){
            counter=0;
        }
        Input_Check_Back();

        if (Input_Check_Sel()){
            if (counter==0){
                ToggleFullscreen();
            }
        }
    if (counter==1){
        if (GetGamepadAxisMovement(0, 0) < -0.5&&input_delay<1){
            game::core::Store::volume=game::core::Store::volume-0.5;
            if (game::core::Store::volume<0){
                game::core::Store::volume=0;
            }
            input_delay = 10;
        }
        if (GetGamepadAxisMovement(0, 0) > 0.5&&input_delay<1){
            game::core::Store::volume=game::core::Store::volume+0.5;
            if (game::core::Store::volume>5){
                game::core::Store::volume=5;
            }
            input_delay = 10;
        }
        input_delay--;
        SaveValue("AudioSettings.txt",game::core::Store::volume);
    }

}
void MainMenuScene::options_Draw() {
    DrawTextureEx(scroll_button,{game::Config::kStageWidth/2-420,20+90*3-100},0,4,WHITE);
    DrawTexturePro(TB,{40+230*3,1,230,48},{game::Config::kStageWidth/2-460,20,230*4,48*4},{0,0},0,WHITE);
    DrawTextPro(game::core::Store::font,"Drücke Range Attack um ins Hauptmenü zurück zu kehren", {game::Config::kStageWidth/2-950,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);
    DrawTextPro(game::core::Store::font,"Drücke Meele Attack um den Aktuellen Button auszuwählen", {game::Config::kStageWidth/2+150,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);

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
    if (counter==1){
        DrawTexturePro(VolumeSlider,{128*10-128*game::core::Store::volume*2,1,128,48},{game::Config::kStageWidth/2-64*3,500,128*3,48*3},{0,0},0,WHITE);

    } else{
        DrawTexturePro(VolumeSlider,{128*10-128*game::core::Store::volume*2,1,128,48},{game::Config::kStageWidth/2-64*2,500,128*2,48*2},{0,0},0,WHITE);

    }

}
void MainMenuScene::list_Update() {
    Input_Check_Back();
    if (!loaded){
        lines=LoadHighscores("HighscoreList.txt");
        loaded= true;
    }



}
void MainMenuScene::list_Draw() {
    DrawTextureEx(scroll_button,{game::Config::kStageWidth/2-420-20,20+90*3-100},0,4,WHITE);
    DrawTexturePro(TB,{30+230*2,1,230,48},{game::Config::kStageWidth/2-460,20,230*4,48*4},{0,0},0,WHITE);
    DrawTextPro(game::core::Store::font,"Highscores:", {game::Config::kStageWidth/2-150,game::Config::kStageHeight/2-200}, {0,0}, 0, 50, 3, BLACK);
    DrawTextPro(game::core::Store::font,"Drücke Range Attack um ins Hauptmenü zurück zu kehren", {game::Config::kStageWidth/2-950,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);
    DrawTextPro(game::core::Store::font,"Drücke Meele Attack um den Aktuellen Button auszuwählen", {game::Config::kStageWidth/2+150,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);



    for (int s = 0; s < lines.size(); s++) {
        std::string output;
        output=std::to_string(s+1) +". "+lines[s].name+":";

        DrawTextPro(game::core::Store::font,std::to_string(lines[s].score).c_str(), {game::Config::kStageWidth/2+100,game::Config::kStageHeight/2-100+s*50}, {0,0}, 0, 50, 3, BLACK);

        DrawTextPro(game::core::Store::font,output.c_str(), {game::Config::kStageWidth/2-400+80,game::Config::kStageHeight/2-100+s*50}, {0,0}, 0, 50, 3, BLACK);
    }
}
void MainMenuScene::credits_Update() {
    Input_Check_Back();
}
void MainMenuScene::credits_Draw() {
    DrawTextureEx(scroll_button,{game::Config::kStageWidth/2-795,1500},-90,7,WHITE);
    DrawTexturePro(TB,{20+230*1,1,230,48},{game::Config::kStageWidth/2-460,20,230*4,48*4},{0,0},0,WHITE);
    DrawTextPro(game::core::Store::font,"Drücke Range Attack um ins Hauptmenü zurück zu kehren", {game::Config::kStageWidth/2-950,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);
    DrawTextPro(game::core::Store::font,"Drücke Meele Attack um den Aktuellen Button auszuwählen", {game::Config::kStageWidth/2+150,game::Config::kStageHeight/2+500}, {0,0}, 0, 25, 3, WHITE);


    for (int l = 0; l < 4; ++l) {
        for (int i = 0; i < 8; ++i) {
            if (i+8*s<=Text.size()-1){
                DrawTextPro(game::core::Store::font,Text[i+8*s].c_str(),{game::Config::kStageWidth/2-TextLength(Text[i+8*s].c_str())*11,game::Config::kStageHeight/2-150+50*i},{0,0},0,35,5,BLACK);
            } else{
                s=-1;
            }
      }
        d++;
        if (d==60*10){
            s++;
            d=0;
        }
    }

}

std::vector<HighscoreEntry> MainMenuScene::LoadHighscores(const std::string& filename) {
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
