//
// Created by Kruse on 26/08/2025.
//

#include "MainMenuScene.h"


MainMenuScene::MainMenuScene() {
    this->counter=0;
    state=main;
    song.looping= true;
    PlayMusicStream(song);

}
MainMenuScene::~MainMenuScene() { }

void MainMenuScene::Update()
{
    SetMusicVolume(song,game::core::Store::volume);
    UpdateMusicStream(song);


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
    if (IsKeyPressed(game::Config::key_Up)){
        this->counter= this->counter-1;
    }
    if (IsKeyPressed(game::Config::key_Down)){
        this->counter= this->counter+1;
    }
}
bool MainMenuScene::Input_Check_Sel() {
    if (IsKeyPressed(game::Config::key_Melee_Attack) || IsKeyPressed(KEY_ENTER)) {
        return true;
    } else {
        return false;
    }
}
void MainMenuScene::Input_Check_Back() {
    if (IsKeyPressed(game::Config::key_Ranged_Attack)){
        state=main;
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
                state=options;
                break;
            }
            case 2:{
                state=list;
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

    if (slider){
        if (IsKeyPressed(game::Config::key_Melee_Attack) || IsKeyPressed(KEY_ENTER)) {
            slider = !slider;
        }
        if (IsKeyPressed(game::Config::key_Left)){
            game::core::Store::volume=game::core::Store::volume-0.5;
            if (game::core::Store::volume<0){
                game::core::Store::volume=0;
            }
        }
        if (IsKeyPressed(game::Config::key_Right)){
            game::core::Store::volume=game::core::Store::volume+0.5;
            if (game::core::Store::volume>5){
                game::core::Store::volume=5;
            }
        }

    } else{
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
            } else{
                slider=true;
            }
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
    if (counter==1){
        DrawTexturePro(VolumeSlider,{128*10-128*game::core::Store::volume*2,1,128,48},{game::Config::kStageWidth/2-64*3,500,128*3,48*3},{0,0},0,WHITE);

    } else{
        DrawTexturePro(VolumeSlider,{128*10-128*game::core::Store::volume*2,1,128,48},{game::Config::kStageWidth/2-64*2,500,128*2,48*2},{0,0},0,WHITE);

    }

}
void MainMenuScene::list_Update() {
    Input_Check_Back();
}
void MainMenuScene::list_Draw() {

}
void MainMenuScene::credits_Update() {
    Input_Check_Back();
}
void MainMenuScene::credits_Draw() {

}
