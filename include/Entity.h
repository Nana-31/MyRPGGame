#pragma once
#include "raylib.h"
#include <iostream>
#include <cmath>
#include <string>


enum position { UP, DOWN, RIGHT, LEFT, UPRIGHT, UPLEFT, DOWNRIGHT, DOWNLEFT };
class Entity {

public:

    Vector2 pos;
    Vector2 velocity;
    bool animated = false;
    float speed = 2;
    Texture2D moveTex;
    Texture2D idleTex;
    Texture2D* currentTex;
    float frameTimer = 0.0f;
    float frameSpeed = 0.1f;
    float frameTime = 0.3f;
    int CurrentFrame = 0;
    int maxFrame;
    int baseX = 0;
    int baseY = 0;
    float scale = 2;
    position p;
    position lastP;
    int widthF = 40;
    int heighF = 40;

    void Move();
    void UpdateDirection();
    void textureUpdate();
    void TextureDraw(const char* textMove, const char* textIdle);
    

};

class Player :public Entity {

public:

    Player();
    void Draw();
    void Update();
    
    

private:

    int cruelty;
    std::string name;

};

class NPC: public Entity{
public:
    NPC();
    NPC(position now, Vector2 Pos, const char* a, const char* b);
    bool neighbour(Entity *en);
    void Draw();
};