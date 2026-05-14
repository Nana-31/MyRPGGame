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
    int* codecount = nullptr;
    Font font;

    FontManager() {
        codecount = LoadCodepoints(kirr, &count);
        font = LoadFontEx("casio-fx-9860gii.ttf", 40, codecount, count);
    }

    void DrawMyText(const char* text, float size, Vector2 pos, Color col, int a) {
        DrawTextEx(font, text, pos, size, a, col);
    }
};

class Button {
    FontManager font;
    Color textColor = { 102, 102, 75, 255 };
    Color textHoverColor = { 210, 210, 160, 255 };

    Texture2D buttonNormalTex;
    Texture2D buttonHoverTex;
    bool texturesLoaded = false;
    NPatchInfo patchInfo;

public:
    Rectangle start;
    Rectangle setting;
    Rectangle exit;
    Rectangle back; 

    Button() { texturesLoaded = false; }

    void InitTextures(const char* normalPath, const char* hoverPath) {
        buttonNormalTex = LoadTexture(normalPath);
        buttonHoverTex = LoadTexture(hoverPath);
        patchInfo.source = { 0.0f, 0.0f, (float)buttonNormalTex.width, (float)buttonNormalTex.height };
        patchInfo.left = 12; patchInfo.top = 12; patchInfo.right = 12; patchInfo.bottom = 12;
        patchInfo.layout = NPATCH_NINE_PATCH;
        texturesLoaded = true;
    }

    ~Button() {
        if (texturesLoaded) {
            UnloadTexture(buttonNormalTex);
            UnloadTexture(buttonHoverTex);
        }
    }

    void UpdateLayout() {
        float sw = (float)GetScreenWidth();
        float sh = (float)GetScreenHeight();
        float btnWidth = 340; float btnHeight = 45; float spacing = 15;
        float btnX = sw / 2 - btnWidth / 2;

        setting = { btnX, sh / 2 - btnHeight / 2, btnWidth, btnHeight };
        start = { btnX, setting.y - btnHeight - spacing, btnWidth, btnHeight };
        exit = { btnX, setting.y + btnHeight + spacing, btnWidth, btnHeight };
        back = { btnX, sh / 2 + 100, btnWidth, btnHeight };
    }

    void DrawSingleButton(Rectangle rect, const char* text, float fontSize, float spacingValue) {
        Vector2 mousePos = GetMousePosition();
        bool isHovered = CheckCollisionPointRec(mousePos, rect);

        if (texturesLoaded) {
            Texture2D currentTex = isHovered ? buttonHoverTex : buttonNormalTex;
            DrawTextureNPatch(currentTex, patchInfo, rect, { 0, 0 }, 0.0f, WHITE);
        }
        else {
            Color fallbackColor = isHovered ? Color{ 55, 55, 55, 255 } : Color{ 35, 35, 35, 255 };
            DrawRectangleRec(rect, fallbackColor);
        }
        Vector2 textSize = MeasureTextEx(font.font, text, fontSize, spacingValue);

        float textX = rect.x + (rect.width - textSize.x) / 2.0f;
        float textY = rect.y + (rect.height - textSize.y) / 2.0f;

        Color currentTextColor = isHovered ? textHoverColor : textColor;
        font.DrawMyText(text, fontSize, { textX, textY }, currentTextColor, spacingValue);
    }


    void DrawMainMenu() {
        UpdateLayout();
        DrawSingleButton(start, "Play!", 30, 2);
        DrawSingleButton(setting, "Setting", 30, 2);
        DrawSingleButton(exit, "Exit", 30, 2);
        Vector2 titleSize = MeasureTextEx(font.font, u8"ISiT", 50, 2);
        float titleX = (float)GetScreenWidth() / 2 - titleSize.x / 2;
        float titleY = start.y - 70;

    }

};




class Dialogues {
public:
    void initD(const char* put) {
        Image im = LoadImage(put);
        dialog = LoadTextureFromImage(im);
        UnloadImage(im);
    }
    void drawD(float x, float y, Color z) {
        DrawTexturePro(dialog, { 0, 0, (float)dialog.width, (float)dialog.height }, { x, y, (float)dialog.width * 3, (float)dialog.height * 2 }, { 0, 0 }, 0.0f, z);
    }
private:
    Texture2D dialog;
};

class MainMenu {
public:
    Texture2D fonM;

    MainMenu() {
        Image image = LoadImage("assets/Image/main_menu.jpg");
        fonM = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    void Draw() {
        Rectangle srcRec = { 0.0f, 0.0f, (float)fonM.width, (float)fonM.height };
        Rectangle destRec = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
        DrawTexturePro(fonM, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
    }
};

class Game {
public:
    Vector2 worldOffset = { 0, 0 };
    bool inMenu = true;
    bool inSettings = false; 
    bool audioMuted = false; 
    bool close = false;
    bool isSoundMain = false;
    bool isSoundOpen = false;
    Camera2D camOsn;
    map myMap;
    Color gameBack = { 131, 154, 94, 255 };
    Sound soundMain;
    Sound soundOpen;
    Dialogues d1;
    json gameData;
    bool isDial = false;
    int phraseId = 0;
    std::vector<std::string> dial_With_NPC;
    std::vector<std::string> dial_ths;
    std::vector<std::string> thoughts;
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
        thoughts = gameData["intro_thoughts"];
        dial_ths = gameData["npcs"][0]["dialogues"];
    }

    Game() {
        camOsn.offset = { sceneWidth / 2, sceneHight / 2 };
        camOsn.zoom = 2.0;
        camOsn.rotation = 0.0;
        myMap.Tex();
        myMap.loadMap("data/мир_Слой тайлов 1.csv", myMap.grassLayer);
        myMap.loadMap("data/мир_деревья.csv", myMap.treesLayer);
        d1.initD("assets/Image/DialogueBoxSimple.png");
        button.InitTextures("assets/Image/one.png", "assets/Image/tue.png");
        soundMain = LoadSound("assets/sounds/Crying_moaning_ambience_2.wav");
        soundOpen = LoadSound("assets/sounds/Door_squeeky_2.wav");
        file_open();
        npc = NPC{ DOWN, {300, 200}, "", "assets/Image/c2_idle.png  (1).png" };
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
        if (audioMuted) {
            if (IsSoundPlaying(soundMain)) StopSound(soundMain);
            return;
        }

        if ((inMenu || inSettings) && !isSoundMain) {
            PlaySound(soundMain);
            isSoundMain = true;
            isSoundOpen = false;
        }
        else if (!inMenu && !inSettings && !isSoundOpen) {
            StopSound(soundMain);
            PlaySound(soundOpen);
            isSoundMain = false;
            isSoundOpen = true;
        }
    }

    void Update() {
        sceneWidth = (float)GetScreenWidth();
        sceneHight = (float)GetScreenHeight();

        camOsn.offset = { sceneWidth / 2, sceneHight / 2 };

        button.UpdateLayout();

        Vector2 mouse = GetMousePosition();
        camOsn.target.x = player.pos.x + (player.widthF * player.scale) / 2;
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
                    inMenu = false;    
                    inSettings = true;  
                }
                if (CheckCollisionPointRec(mouse, button.exit)) {
                    close = true;
                }
            }
        }
        else if (inSettings) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mouse, button.start)) {
                    audioMuted = !audioMuted;
                    if (audioMuted) SetMasterVolume(0.0f); 
                    else SetMasterVolume(1.0f);         
                }
                if (CheckCollisionPointRec(mouse, button.setting)) {
                    ToggleBorderlessWindowed();
                }
                if (CheckCollisionPointRec(mouse, button.back)) {
                    inSettings = false;
                    inMenu = true;
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
            button.DrawMainMenu();
        }
        else if (inSettings) {
            menu.Draw(); 

            std::string soundText = audioMuted ? "Sound: OFF" : "Sound: ON";
            std::string screenText = IsWindowFullscreen() ? "Screen: Full" : "Screen: Window";

            button.DrawSingleButton(button.start, soundText.c_str(), 30, 2);
            button.DrawSingleButton(button.setting, screenText.c_str(), 30, 2);
            button.DrawSingleButton(button.back, "Back", 30, 2);

            float titleTextWidth = MeasureText("SETTINGS", 40);
            font.DrawMyText("SETTINGS", 40, { (float)GetScreenWidth() / 2 - titleTextWidth / 2, button.start.y - 70 }, font.textColor, 2);
        }
        else {
            BeginMode2D(camOsn);
            DrawRectangleV(worldOffset, { (float)GetScreenWidth(), (float)GetScreenHeight() }, gameBack);
            myMap.drawLayer(myMap.grassLayer, myMap.grassSprite, myMap.valueGrassMap, player.pos);
            player.Draw();
            npc.Draw();
            myMap.drawLayer(myMap.treesLayer, myMap.treesSprite, myMap.valueTreesMap, player.pos);
            EndMode2D();

            float dialogX = sceneWidth / 2 - (384 * 3) / 2; 
            float dialogY = sceneHight - 180;

            if (npc.neighbour(&player) && !isDial) {
                d1.drawD(dialogX, dialogY, GRAY);
                font.DrawMyText(u8"Нажмите E, чтобы взаимодействовать \nс людьми и предметами", 25, { dialogX + 30, dialogY + 30 }, BLACK, 2);
            }
            if (isDial) {
                d1.drawD(dialogX, dialogY, GRAY);
                font.DrawMyText(dial_With_NPC[phraseId].c_str(), 25, { dialogX + 30, dialogY + 30 }, BLACK, 2);
            }
        }
        EndDrawing();
    }
};

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(sceneWidth, sceneHight, u8"Проект ИСиТ 1");
    SetTargetFPS(60);
    InitAudioDevice();

    Game game;

    while (!WindowShouldClose() && !game.close) {
        if (IsKeyPressed(KEY_F11)) {
            ToggleBorderlessWindowed();
        }

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
