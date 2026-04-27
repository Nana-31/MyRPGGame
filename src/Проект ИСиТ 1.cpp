#include "raylib.h"
#include <iostream>
#include <string>
#include <cmath>
#include "map.h"
#include "Entity.h"
#include <fstream>
#include <vector>
#include <sstream>

#include "json.hpp"
using json = nlohmann::json;


float sceneWidth = 1280;
float sceneHight = 720;

class FontManager {

public:

    Color textColor = { 200, 200, 200, 255 };
    int count = 0;
    const char* kirr = u8" абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;=<>*";
    int* codecount = LoadCodepoints(kirr, &count);
    Font font = LoadFontEx("casio-fx-9860gii.ttf", 40, codecount, count);

    void DrawMyText(const char* text, float size, Vector2 pos, Color col) {

        DrawTextEx(font, text, pos, size, 2, col);

    }

};

class Button {

    FontManager font;
    Color buttonColor = { 61, 19, 19, 255 };
    

public:

    Rectangle start = { sceneWidth/2-300, sceneHight/2-90, 200, 40};
    Rectangle setting = { sceneWidth / 2 - 300, sceneHight / 2 - 30, 200, 40 };
    Rectangle exit = { sceneWidth / 2 - 300, sceneHight / 2 + 30, 200, 40 };


    void Draw() {

        DrawRectangleRec(start, buttonColor);
        DrawRectangleRec(setting, buttonColor);
        DrawRectangleRec(exit, buttonColor);
        font.DrawMyText(u8"ISiT", 40, {600, 75}, font.textColor);
        font.DrawMyText("Play!", 30, { start.x+55, start.y +8}, font.textColor);
        font.DrawMyText("Setting", 30, {setting.x+20, setting.y+5}, font.textColor);
        font.DrawMyText("Exit", 30, {exit.x+55, exit.y+8}, font.textColor);

    }


};

class Dialogues {
public:

    void initD(const char *put) {
        Image im = LoadImage(put);
        dialog = LoadTextureFromImage(im);
        UnloadImage(im);
    }
    void drawD(float x, float y, Color z) {
        DrawTexturePro(dialog, {0, 0, (float)dialog.width, (float)dialog.height}, {x, y, (float)dialog.width*3, (float)dialog.height*2}, {0, 0},0.0f, z );
    }
private:
    Texture2D dialog;
    Rectangle src;
    Rectangle dest;

};


class MainMenu {

public:

    Texture2D fonM;
    Rectangle rec2 = { 130, 65, 30, 40 };

    MainMenu() {

        Image image = LoadImage("assets/Image/FreeHorrorUi.png");
        fonM = LoadTextureFromImage(image);

    }

    void Draw() {

        Color fon = { 61, 30, 30 , 255 };
        ClearBackground(fon);

        Rectangle rec = {128, 0, 120, 65};
        Rectangle dest = {100, 10, 120*10, 65*10};
        DrawTexturePro(fonM, rec, dest, {0,0}, 0.0f, WHITE);

        Rectangle dest2 = {800, 270, 30*5, 40*5};
        DrawTexturePro(fonM, rec2, dest2, {0,0}, 0.0f, WHITE);

    }

    
};

class Game {

public:

    bool inMenu = true;
    bool close = false;
    bool isSoundMain = false;
    bool isSoundOpen = false;
    Camera2D camOsn;
    map myMap;
    Color gameBack = { 102, 157, 157, 255 };
    Sound soundMain;
    Sound soundOpen;
    Dialogues d1;
    json gameData;
    bool isDial = false;
    int phraseId = 0;
    std::vector<std::string> dial_With_NPC;
    std::vector<std::string> dial_ths;
    std::vector<std::string> thoughts;
    int pods = 0;
    NPC npc;

    void file_open() {
        std::ifstream file("data/npcs.json");
        if (!file.is_open()) {
            std::cout << "File is not open\n";
            return;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();
        gameData = json::parse(content);
        thoughts= gameData["intro_thoughts"];
        dial_ths = gameData["npcs"][2]["dialogues"];
    }
    
   
    Game()
    {
        camOsn.offset = {sceneWidth/2, sceneHight/2};
        camOsn.zoom = 2.0;
        camOsn.rotation = 0.0;
        myMap.Tex();
        myMap.loadMap("data/мир_Слой тайлов 1.csv", myMap.grassLayer);
        myMap.loadMap("data/мир_деревья.csv", myMap.treesLayer);
        d1.initD("assets/Image/DialogueBoxSimple.png");
        soundMain = LoadSound("assets/sounds/Crying_moaning_ambience_2.wav");
        soundOpen = LoadSound("assets/sounds/Door_squeeky_2.wav");
        file_open();
        npc= NPC{ DOWN, {300, 200}, "", "assets/Image/c2_idle.png  (1).png" };
    }

    ~Game() {
        UnloadSound(soundMain);
        UnloadSound(soundOpen);
    }
    
    MainMenu menu;
    Player player;
    Button button;
    FontManager font;

    void sound() {
        if (inMenu && !isSoundMain) {
            PlaySound(soundMain);
            isSoundMain = true;
            isSoundOpen = false;
        }
        else if(!inMenu && !isSoundOpen){
            StopSound(soundMain);
            PlaySound(soundOpen);
            isSoundMain = false;
            isSoundOpen = true;
        }
    }

    void Update() {

        Vector2 mouse = GetMousePosition();
        camOsn.target.x=player.pos.x + (player.widthF*player.scale)/2;
        camOsn.target.y = player.pos.y + (player.heighF * player.scale) / 2;

        if (inMenu) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

                if (CheckCollisionPointRec(mouse, button.start)) {
                    inMenu = false;
                    dial_With_NPC = thoughts;
                    isDial = true;
                    phraseId = 0;
                }

                if (CheckCollisionPointRec(mouse, button.setting)) {



                }

                if (CheckCollisionPointRec(mouse, button.exit)) {

                    close = true;

                }
            }
        }
        else {
          
            if (IsKeyPressed(KEY_E)) {
                if (!isDial) {
                    if (npc.neighbour(&player)) {
                        dial_With_NPC = dial_ths;
                        isDial = true;
                        phraseId = 0;
                    }
                }
                else {
                    phraseId++;
                    if (phraseId >= dial_With_NPC.size()) {
                        isDial = false;
                    }
                }
            }
            
            
        }
        sound();
    }


    void Draw() {

        BeginDrawing();
        ClearBackground(gameBack);
        if (inMenu) {
            menu.Draw();
            button.Draw();
        }
        else {
            BeginMode2D(camOsn);
            myMap.drawLayer(myMap.grassLayer, myMap.grassSprite, myMap.valueGrassMap);
            player.Draw();
            npc.Draw();
            myMap.drawLayer(myMap.treesLayer, myMap.treesSprite, myMap.valueTreesMap);
            //font.DrawMyText(u8"Привет !", 40, { 100, 100 });
            EndMode2D();
            if (npc.neighbour(&player) && !isDial ) {
                d1.drawD(160, 560, BLUE);
                font.DrawMyText(u8"Нажмите E, чтобы взаимодействовать \nс людьми и предметами", 25, {190, 590}, BLACK);
            }
            if (isDial) {
                d1.drawD(160, 560, GRAY);
                font.DrawMyText(dial_With_NPC[phraseId].c_str(), 25, { 190, 590 }, BLACK);

            }

        }

        EndDrawing();

    }
 

};

int main() {

    InitWindow(sceneWidth, sceneHight, u8"Проект ИСиТ 1");
    SetTargetFPS(60);
    InitAudioDevice();

    Game game;
 
    while (!WindowShouldClose() && !game.close) {

        if (!game.inMenu && !game.isDial) {

            game.player.Update();
            game.npc.textureUpdate();

        }
        game.Update();
        game.Draw();

    }

    CloseWindow();

    return 0;
}