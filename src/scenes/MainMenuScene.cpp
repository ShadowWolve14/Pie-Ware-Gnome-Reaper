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
    UpdateMusicStream(song);

Input_Check_Mov();
    switch (state) {
        case main:{
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
            CloseWindow();
            break;
        }
    }

}

void MainMenuScene::Draw()
{

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
    if (IsKeyPressed(game::Config::key_Melee_Attack)){
        return true;
    } else{
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
    Color grer{100,125,52,255};

    ClearBackground(grer);
    Rectangle dest{game::Config::kStageWidth/2-140,150,280,64};
    DrawTextureEx(scroll_button,{game::Config::kStageWidth/2-315,20+90*3-50},0,3,WHITE);
    DrawTextureEx(sign_button,{game::Config::kStageWidth/2-3*228,20},0,3,WHITE);
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
    if (counter>1){
        counter=counter-2;
    }

    if (Input_Check_Sel()){
        if (counter==0){
            ToggleFullscreen();
        } else{
            slider=true;
        }
    }
    if (slider){
        if (IsKeyPressed(game::Config::key_Ranged_Attack)){
            slider=!slider;
            prot=true;
        }
    }
    if (!slider&&prot){
        Input_Check_Back();
    }
    prot=false;

}
void MainMenuScene::options_Draw() {
    if (counter==0){
        // Draw Highlited Button
        DrawRectangle(game::Config::kStageWidth/2-100,100,200,50,RED);
    }
    else{
        //Draw Regular Button Asset
        DrawRectangle(game::Config::kStageWidth/2-100,100,200,50,GRAY);
    }
    if (counter==1){
// Draw Highlited Button
        DrawRectangle(game::Config::kStageWidth/2-100,170,200,50,RED);
    }
    else{
//Draw Regular Button Asset
        DrawRectangle(game::Config::kStageWidth/2-100,170,200,50,GRAY);
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
