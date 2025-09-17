//
// Created by Kruse on 01/09/2025.
//

#ifndef RAYLIBSTARTER_HUD_H
#define RAYLIBSTARTER_HUD_H


#include "PlayerClassOne.h"
#include "BombItem.h"
#include "ItemBase.h"

class HUD {
    enum item{bomb, icebomb, empty,key,potion1, potion2, potion3, testo, adrenalin, luck};
    enum state{ma,ra,id,iu};
    Player_Class_One* mp= nullptr;
    int UC=0;
    item it=empty;
    state st=id;
    int sc=0;
    int scc=0;
    Vector2 player_pos;
    int prevhp;
    int c;
    float rot;
    int ofs;
    int ct;
    int cb;

    Texture2D AA= LoadTexture(game::Config::Symbol_AA);
    Texture2D AAU= LoadTexture(game::Config::Symbol_AAU);
    Texture2D AAI= LoadTexture(game::Config::Symbol_AAI);
    Texture2D IB= LoadTexture(game::Config::Symbol_IB);
    Texture2D IBU= LoadTexture(game::Config::Symbol_IBU);
    Texture2D IE= LoadTexture(game::Config::Symbol_IE);
    Texture2D IEU= LoadTexture(game::Config::Symbol_IEU);
    Texture2D II= LoadTexture(game::Config::Symbol_II);
    Texture2D IK= LoadTexture(game::Config::Symbol_IK);
    Texture2D IKU= LoadTexture(game::Config::Symbol_IKU);
    Texture2D SA= LoadTexture(game::Config::Symbol_SA);
    Texture2D SAU= LoadTexture(game::Config::Symbol_SAU);
    Texture2D SAI= LoadTexture(game::Config::Symbol_SAI);
    Texture2D HS= LoadTexture(game::Config::Symbol_HS);
    Texture2D H= LoadTexture(game::Config::Symbol_H);
    Texture2D HH= LoadTexture(game::Config::Symbol_HH);
    Texture2D HU= LoadTexture(game::Config::Symbol_HU);
    Texture2D HUH= LoadTexture(game::Config::Symbol_HUH);
    Texture2D SCC= LoadTexture(game::Config::Symbol_SCC);
    Texture2D SOC= LoadTexture(game::Config::Symbol_SOC);
    Texture2D IP1;
    Texture2D IP1U;
    Texture2D IP2;
    Texture2D IP2U;
    Texture2D IP3;
    Texture2D IP3U;
    Texture2D IBomb;
    Texture2D IBombU;
    Texture2D IAU;
    Texture2D IA;
    Texture2D ILN;
    Texture2D ILNU;
    Texture2D IT= LoadTexture(game::Config::Symbol_IT);
    Texture2D ITU= LoadTexture(game::Config::Symbol_ITU);
    Texture2D damge=LoadTexture("PieWare/assets/Spritesheets/VFX/Damage_Taken_Overlay.png");
    Texture2D freeze=LoadTexture("PieWare/assets/Spritesheets/VFX/Frost_Overlay.png");



public:
    HUD(Player_Class_One* mp);
    void HUD_update();
    void HUD_draw();
    void HUD_shake();
};


#endif //RAYLIBSTARTER_HUD_H
