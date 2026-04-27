#include "Entity.h"


void Entity::Move() {

    pos.x += velocity.x;
    pos.y += velocity.y;

}

void Entity::UpdateDirection() {

    if (animated) {
        switch (p) {
        case UP:        baseX = 0; baseY = 40; break;
        case DOWN:      baseX = 0; baseY = 0;  break;
        case LEFT:      baseX = 0; baseY = 120; break;
        case RIGHT:     baseX = 0; baseY = 80; break;
        case UPRIGHT:   baseX = 0; baseY = 240; break;
        case DOWNLEFT:  baseX = 0; baseY = 200; break;
        case UPLEFT:    baseX = 0; baseY = 280; break;
        case DOWNRIGHT: baseX = 0; baseY = 160; break;
        }
    }
    else {
        switch (lastP) {
        case UP:        baseX = 80; baseY = 0;   break;
        case DOWN:      baseX = 0;  baseY = 0;   break;
        case LEFT:      baseX = 80; baseY = 40;  break;
        case RIGHT:     baseX = 0;  baseY = 40;  break;
        case UPRIGHT:   baseX = 0;  baseY = 120; break;
        case DOWNLEFT:  baseX = 0;  baseY = 80;  break;
        case UPLEFT:    baseX = 80; baseY = 120; break;
        case DOWNRIGHT: baseX = 80; baseY = 80;  break;
        }
    }

}

void Entity::textureUpdate() {

    bool currentlyMoving = (velocity.x != 0 || velocity.y != 0);

    if (currentlyMoving != animated) {
        CurrentFrame = 0;
        frameTimer = 0;
    }

    animated = currentlyMoving;

    if (animated) {
        maxFrame = 4;
        currentTex = &moveTex;
    }
    else {
        maxFrame = 2;
        currentTex = &idleTex;
        p = lastP;
    }

    frameTimer += GetFrameTime();
    if (frameTimer >= frameTime) {
        frameTimer = 0;
        CurrentFrame++;
        if (CurrentFrame >= maxFrame) CurrentFrame = 0;
    }
}

void Entity::TextureDraw(const char* textMove, const char* textIdle) {

    Image im = LoadImage(textMove);
    moveTex = LoadTextureFromImage(im);
    UnloadImage(im);

    im = LoadImage(textIdle);
    idleTex = LoadTextureFromImage(im);
    UnloadImage(im);

    currentTex = &idleTex;

}

Player::Player() {
    p = DOWN;
    lastP = p;
    TextureDraw("assets/Image/движение.png", "assets/Image/дыхание.png");
}

void Player::Draw() {

    Rectangle frame = { baseX + CurrentFrame * widthF, baseY, widthF, heighF };
    Rectangle dst = { pos.x, pos.y, widthF * scale, heighF * scale };
    DrawTexturePro(*currentTex, frame, dst, { 0, 0 }, 0.0f, WHITE);

}

void Player::Update() {
    velocity = { 0, 0 };
    if (IsKeyDown(KEY_W)) velocity.y -= 1;
    if (IsKeyDown(KEY_S)) velocity.y += 1;
    if (IsKeyDown(KEY_A)) velocity.x -= 1;
    if (IsKeyDown(KEY_D)) velocity.x += 1;

    if (velocity.x != 0 || velocity.y != 0) {

        if (velocity.y < 0) p = UP;
        if (velocity.y > 0) p = DOWN;
        if (velocity.x < 0) p = LEFT;
        if (velocity.x > 0) p = RIGHT;

        if (velocity.y < 0 && velocity.x > 0) p = UPRIGHT;
        if (velocity.y < 0 && velocity.x < 0) p = UPLEFT;
        if (velocity.y > 0 && velocity.x < 0) p = DOWNRIGHT;
        if (velocity.y > 0 && velocity.x > 0) p = DOWNLEFT;

        lastP = p;
    }

    float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (length != 0) {
        velocity.x = (velocity.x / length) * speed;
        velocity.y = (velocity.y / length) * speed;
    }

    textureUpdate();
    UpdateDirection();
    Move();
}

bool NPC::neighbour(Entity *en){
    float dist = sqrt(pow(en->pos.x - this->pos.x, 2) + pow(en->pos.y - this->pos.y, 2));
    return dist <= 40;
}

NPC::NPC(position now, Vector2 Pos, const char* a, const char*b){
    this->p = now;
    this->pos = Pos;
    TextureDraw(a, b);
}

NPC::NPC() {
    this->p = DOWN;
    this->pos = { 0,0 };
}

void NPC::Draw() {
    Rectangle frame = { baseX + CurrentFrame * widthF, baseY, widthF, heighF };
    Rectangle dst = { pos.x, pos.y, widthF * scale, heighF * scale };
    DrawTexturePro(*currentTex, frame, dst, { 0, 0 }, 0.0f, WHITE);
}
