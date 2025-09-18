//
// Created by Kruse on 01/09/2025.
//

#include "HUD.h"
#include "Store.h"

HUD::HUD(Player_Class_One* mp):mp(mp) {
    prevhp=100;
    c=0;

    IP1 = LoadTexture(game::Config::kIconHealthPotion1);
    IP1U = LoadTexture(game::Config::kIconHealthPotion1_Use);
    IP2 = LoadTexture(game::Config::kIconHealthPotion2);
    IP2U = LoadTexture(game::Config::kIconHealthPotion2_Use);
    IP3 = LoadTexture(game::Config::kIconHealthPotion3);
    IP3U = LoadTexture(game::Config::kIconHealthPotion3_Use);
    IBomb = LoadTexture(game::Config::kIconIceBomb);
    IBombU = LoadTexture(game::Config::kIconIceBomb_Use);
    IAU = LoadTexture(game::Config::kIconAdrenalinNeedle_Use);
    IA = LoadTexture(game::Config::kIconAdrenalinNeedle);
    ILN = LoadTexture(game::Config::kIconLuckNeedle);
    ILNU = LoadTexture(game::Config::kIconLuckNeedle_Use);

    fairy_icon_lvl1 = LoadTexture("PieWare/assets/Items/FeenFluegel.png");
    fairy_icon_lvl2 = LoadTexture("PieWare/assets/Items/FeenHoerner.png");
    fairy_icon_lvl3 = LoadTexture("PieWare/assets/Items/Zepter.png");
    key_icon = LoadTexture("PieWare/assets/Items/Item_Schluessel.png");
}

void HUD::HUD_update()
{
    sc=game::core::Store::player_state->souls;
    scc=game::core::Store::player_state->score;
    player_pos=mp->Get_Player_Center();

    if (!mp->HasItem()) {
        this->it = empty;
    } else {
        ItemType currentItemType = mp->GetHeldItem()->GetType();
        if (currentItemType == ItemType::BOMB) {
            this->it = bomb;
        } else if (currentItemType == ItemType::ICE_BOMB) {
            this->it = icebomb;
        } else if (currentItemType == ItemType::HEALTH_POTION) {
            this->it = potion1;
        } else if (currentItemType == ItemType::HEALTH_POTION_2) {
            this->it = potion2;
        } else if (currentItemType == ItemType::HEALTH_POTION_3) {
            this->it = potion3;
        } else if (currentItemType == ItemType::TESTO_NEEDLE) {
            this->it = testo;
        } else if (currentItemType == ItemType::ADRENALINE_NEEDLE) {
        this->it = adrenalin;
        } else if (currentItemType ==  ItemType::LUCK_NEEDLE){
            this->it = luck;
        }
    }


    if (IsKeyPressed(game::Config::key_Melee_Attack)){
        this->st=ma;
        UC=6;
    }
    if(IsKeyPressed(game::Config::key_Ranged_Attack)){
        this->st=ra;
        UC=6;
    }
    if (IsKeyPressed(game::Config::key_Use_Item)){
        this->st=iu;
        UC=6;
    }
    UC--;
    if (UC<1){
        this->st=id;
    }
    if (prevhp>mp->Get_Health()){
        c=11;
    }
    if (c>0){
        HUD_shake();
    }
    if (c==0){
        ofs=0;
        rot=0;
    }
    prevhp=mp->Get_Health();
    c--;
    cb--;
}
void HUD::HUD_shake() {
    if (ct<1){
        ofs=GetRandomValue(-5,5);
        //rot=GetRandomValue(-5,5);
        ct=3;
    }
    ct--;
}
void HUD::HUD_draw() {
    Vector2 v1{20,20};
    Vector2 v2{1700,20};
    Vector2 v3{1400,20};
    Vector2 v4{1650,950};
    Vector2 v5{1800,950};
    Vector2 v6{1800,800};
    Vector2 v7{1864,1014};
    Vector2 v8{1714,1014};
    Vector2 v9{1864,864};
    Vector2 v10{118,25};
    Vector2 v11{163,25};
    Vector2 v12{208,25};
    Vector2 v13{253,25};
    Vector2 v14{298,25};
    Vector2 v15{343,25};
    Vector2 v16{118,65};
    Vector2 v17{163,65};
    Vector2 v18{208,65};
    Vector2 v19{253,65};
    Vector2 v20{298,65};
    Vector2 v21{343,65};

    v1.x=v1.x+ofs;v1.y=v1.y+ofs;
    v2.x=v2.x+ofs;v2.y=v2.y+ofs;
    v3.x=v3.x+ofs;v3.y=v3.y+ofs;
    v4.x=v4.x+ofs;v4.y=v4.y+ofs;
    v5.x=v5.x+ofs;v5.y=v5.y+ofs;
    v6.x=v6.x+ofs;v6.y=v6.y+ofs;
    v7.x=v7.x+ofs;v7.y=v7.y+ofs;
    v8.x=v8.x+ofs;v8.y=v8.y+ofs;
    v9.x=v9.x+ofs;v9.y=v9.y+ofs;
    v10.x=v10.x+ofs;v10.y=v10.y+ofs;
    v11.x=v11.x+ofs;v11.y=v11.y+ofs;
    v12.x=v12.x+ofs;v12.y=v12.y+ofs;
    v13.x=v13.x+ofs;v13.y=v13.y+ofs;
    v14.x=v14.x+ofs;v14.y=v14.y+ofs;


    if (c>0){
        DrawTexturePro(damge,{1+(float)c*512,1,512,288},{0,0,512*3.8,288*3.8},{0,0},0,WHITE);
    }
    if (cb>0){
        DrawTexturePro(freeze,{1+(float)cb*512,1,512,288},{0,0,512*3.8,288*3.8},{0,0},0,WHITE);
    }

    DrawTextureEx(HS,v1,rot,3,WHITE);
    DrawTextureEx(SCC,v2,rot,3,WHITE);
    DrawTextureEx(SOC,v3,rot,3,WHITE);

    if (mp->IsBuffed())
    {
        DrawTextureEx(SA, v5, rot, 3, WHITE);
    }
    else
    {
        DrawTextureEx(AA, v5, rot, 3, WHITE);
    }

    if (mp->IsAdrenalinBuffed())
    {
        DrawTextureEx(AA, v4, rot, 3, WHITE);
    }
    else
    {
        DrawTextureEx(SA, v4, rot, 3, WHITE);
    }

    if (st==ma){
        if (UC>0)
        {
            if (mp->IsAdrenalinBuffed())
            {
                DrawTextureEx(AAU, v4, rot, 3, WHITE);
            }
            else
            {
                DrawTextureEx(SAU, v4, rot, 3, WHITE);
            }
        }
    }
    if (st==ra){
        if (UC>0){
            if (mp->IsBuffed())
            {
                DrawTextureEx(SAU, v5, rot, 3, WHITE);
            }
            else
            {
                DrawTextureEx(AAU, v5, rot, 3, WHITE);
            }
        }
    }

    if (mp->HasItem())
    {
        std::string itemName = mp->GetHeldItem()->GetName();
        float fontSize = 20.0f;
        float spacing = 1.0f;
        Vector2 textSize = MeasureTextEx(game::core::Store::font, itemName.c_str(), fontSize, spacing);
        Vector2 textPos;
        textPos.x = v6.x + (150 / 2) - (textSize.x / 2) - 64;
        textPos.y = v6.y - textSize.y - 26;
        DrawTextEx(game::core::Store::font, itemName.c_str(), textPos, fontSize, spacing, WHITE);
    }

    if (st == iu) {
        switch (it) {
            case bomb: {
                if (UC > 0) { DrawTextureEx(IBU, v6, rot, 3, WHITE); }
                break;
            }
            case icebomb: {
               if (UC > 0) { DrawTextureEx(IBombU, v6, rot, 3, WHITE);}
               cb=game::Config::kIceBombFreezeDuration*7;
                break;
            }
            case testo: {
                if (UC > 0) { DrawTextureEx(ITU, v6, rot, 3, WHITE); }
                break;
            }
            case adrenalin: {
                if (UC > 0) { DrawTextureEx(IAU, v6, rot, 3, WHITE); }
                break;
            }
            case potion1: {
                if (UC > 0) { DrawTextureEx(IP1U, v6, rot, 3, WHITE); }
                break;
            }
            case potion2: {
                if (UC > 0) { DrawTextureEx(IP2U, v6, rot, 3, WHITE); }
                break;
            }
            case potion3: {
                if (UC > 0) { DrawTextureEx(IP3U, v6, rot, 3, WHITE); }
                break;
            }
            case luck: {
                if (UC > 0) { DrawTextureEx(ILNU, v6, rot, 3, WHITE); }
            }
            default: {
                if (UC > 0) { DrawTextureEx(IEU, v6, rot, 3, WHITE); }
                break;
            }
        }
    } else {
        switch (it) {
            case bomb: {
                DrawTextureEx(IB, v6, rot, 3, WHITE);
                break;
            }
            case icebomb: {
                DrawTextureEx(IBomb, v6, rot, 3, WHITE);
                break;
            }
            case testo: {
                bool should_draw = true;
                if (mp->IsBuffed() && mp->GetBuffTimer() <= game::Config::testo_Needle_Blinking_Start_Time)
                {
                    if ((int)(GetTime() * 10) % 2 == 0)
                    {
                        should_draw = false;
                    }
                }

                if (should_draw)
                {
                    DrawTextureEx(IT, v6, rot, 3, WHITE);
                }
                else
                {
                    DrawTextureEx(IE, v6, rot, 3, WHITE);
                }
                break;
            }
            case adrenalin: {
                bool should_draw = true;
                if (mp->IsAdrenalinBuffed() && mp->GetAdrenalinBuffTimer() <= game::Config::adrenaline_Needle_Blinking_Start_Time)
                {
                    if ((int)(GetTime() * 10) % 2 == 0)
                    {
                        should_draw = false;
                    }
                }

                if (should_draw)
                {
                    DrawTextureEx(IA, v6, rot, 3, WHITE);
                }
                else
                {
                    DrawTextureEx(IE, v6, rot, 3, WHITE);
                }
                break;
            }
            case potion1: {
                DrawTextureEx(IP1, v6, rot, 3, WHITE);
                break;
            }
            case potion2: {
                DrawTextureEx(IP2, v6, rot, 3, WHITE);
                break;
            }
            case potion3: {
                DrawTextureEx(IP3, v6, rot, 3, WHITE);
                break;
            }
            case luck: {
                DrawTextureEx(ILN, v6, rot, 3, WHITE);
                break;
            }
            default: {
                DrawTextureEx(IE, v6, rot, 3, WHITE);
                break;
            }
        }
    }

    DrawTextureEx(SAI,v7,rot,3,WHITE);
    DrawTextureEx(AAI,v8,rot,3,WHITE);
    DrawTextureEx(II,v9,rot,3,WHITE);

    if (mp->Get_Health()>109){
        DrawTextureEx(HH,v15,rot,3,WHITE);
    }
    if (mp->Get_Health()>89){
        DrawTextureEx(HH,v14,rot,3,WHITE);
    }
    if (mp->Get_Health()>69){
        DrawTextureEx(HH,v13,rot,3,WHITE);
    }
    if (mp->Get_Health()>49){
        DrawTextureEx(HH,v12,rot,3,WHITE);
    }
    if (mp->Get_Health()>29){
        DrawTextureEx(HH,v11,rot,3,WHITE);
    }
    if (mp->Get_Health()>9){
        DrawTextureEx(HH,v10,rot,3,WHITE);
    }

    if (mp->Get_Health()>119){
        DrawTextureEx(H,v15,rot,3,WHITE);
    }
    if (mp->Get_Health()>99){
        DrawTextureEx(H,v14,rot,3,WHITE);
    }
    if (mp->Get_Health()>79){
        DrawTextureEx(H,v13,rot,3,WHITE);
    }
    if (mp->Get_Health()>59){
        DrawTextureEx(H,v12,rot,3,WHITE);
    }
    if (mp->Get_Health()>39){
        DrawTextureEx(H,v11,rot,3,WHITE);
    }
    if (mp->Get_Health()>19){
        DrawTextureEx(H,v10,rot,3,WHITE);

    }

    if (mp->Get_Health()>229){
        DrawTextureEx(HH,v21,rot,3,WHITE);
    }
    if (mp->Get_Health()>209){
        DrawTextureEx(HH,v20,rot,3,WHITE);
    }
    if (mp->Get_Health()>189){
        DrawTextureEx(HH,v19,rot,3,WHITE);
    }
    if (mp->Get_Health()>169){
        DrawTextureEx(HH,v18,rot,3,WHITE);
    }
    if (mp->Get_Health()>149){
        DrawTextureEx(HH,v17,rot,3,WHITE);
    }
    if (mp->Get_Health()>129){
        DrawTextureEx(HH,v16,rot,3,WHITE);
    }

    if (mp->Get_Health()>239){
        DrawTextureEx(H,v21,rot,3,WHITE);
    }
    if (mp->Get_Health()>219){
        DrawTextureEx(H,v20,rot,3,WHITE);
    }
    if (mp->Get_Health()>199){
        DrawTextureEx(H,v19,rot,3,WHITE);
    }
    if (mp->Get_Health()>179){
        DrawTextureEx(H,v18,rot,3,WHITE);
    }
    if (mp->Get_Health()>159){
        DrawTextureEx(H,v17,rot,3,WHITE);
    }
    if (mp->Get_Health()>139){
        DrawTextureEx(H,v16,rot,3,WHITE);

    }
    if (game::core::Store::player_state->fairy_collected_in_level > 0)
    {
        Texture2D current_icon;
        Rectangle destRec;

        switch (game::core::Store::player_state->fairy_collected_in_level)
        {
            case 1:
            {
                current_icon = fairy_icon_lvl1;
                const float scale = 4.0f;
                destRec = {
                    400.0f,
                    35.0f,
                    (float)current_icon.width * scale,
                    (float)current_icon.height * scale
                };
                break;
            }
            case 2:
            {
                current_icon = fairy_icon_lvl2;
                const float scale = 6.0f;
                destRec = {
                    405.0f,
                    18.0f,
                    (float)current_icon.width * scale,
                    (float)current_icon.height * scale
                };
                break;
            }
            case 3:
            {
                current_icon = fairy_icon_lvl3;
                const float scale = 3.0f;
                destRec = {
                    425.0f,
                    15.0f,
                    (float)current_icon.width * scale,
                    (float)current_icon.height * scale
                };
                break;
            }
            default:
                current_icon = fairy_icon_lvl1;
            destRec = { 400.0f, 30.0f, (float)current_icon.width * 3.0f, (float)current_icon.height * 3.0f };
            break;
        }

        Rectangle sourceRec = { 0.0f, 0.0f, (float)current_icon.width, (float)current_icon.height };
        Vector2 origin = { 0.0f, 0.0f };

        DrawTexturePro(current_icon, sourceRec, destRec, origin, 0.0f, WHITE);
    }
    if (mp->HasKey())
    {
        float fairy_end_x = 0.0f;
        if (game::core::Store::player_state->fairy_collected_in_level > 0)
        {
            Rectangle fairy_dest_rec;
            switch (game::core::Store::player_state->fairy_collected_in_level)
            {
                case 1: fairy_dest_rec = { 400.0f, 35.0f, (float)fairy_icon_lvl1.width * 4.0f, (float)fairy_icon_lvl1.height * 4.0f }; break;
                case 2: fairy_dest_rec = { 405.0f, 18.0f, (float)fairy_icon_lvl2.width * 6.0f, (float)fairy_icon_lvl2.height * 6.0f }; break;
                case 3: fairy_dest_rec = { 425.0f, 15.0f, (float)fairy_icon_lvl3.width * 3.0f, (float)fairy_icon_lvl3.height * 3.0f }; break;
                default: fairy_dest_rec = { 400.0f, 30.0f, (float)fairy_icon_lvl1.width * 3.0f, (float)fairy_icon_lvl1.height * 3.0f }; break;
            }
            fairy_end_x = fairy_dest_rec.x + fairy_dest_rec.width;
        }
        float key_icon_x = (fairy_end_x > 0.0f) ? fairy_end_x + 5.0f : 405.0f;
        const float scale = 4.0f;
        Rectangle source_rec = { 0.0f, 0.0f, (float)key_icon.width, (float)key_icon.height };
        Rectangle dest_rec = { key_icon_x, 35.0f, (float)key_icon.width * scale, (float)key_icon.height * scale };
        DrawTexturePro(key_icon, source_rec, dest_rec, {0,0}, 0.0f, WHITE);
    }
    std::string temp=std::to_string(game::core::Store::player_state->score);
    DrawTextEx(game::core::Store::font,temp.c_str(),{v2.x+85,v2.y+36},30,1,WHITE);
    temp=std::to_string(game::core::Store::player_state->souls);
    DrawTextEx(game::core::Store::font,temp.c_str(),{v3.x+85,v3.y+36},30,1,WHITE);
}