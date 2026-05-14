#include "map.h"
#include <fstream>
#include <string>

void map::Tex() {

    Image image = LoadImage("assets/Image/Tilemap.png");
    grassSprite = LoadTextureFromImage(image);
    UnloadImage(image);

    Image im = LoadImage("assets/Image/Tree2.png");
    treesSprite = LoadTextureFromImage(im);
    UnloadImage(im);

    valueGrassMap[3] = { 64, 64, 64, 64 };
    valueGrassMap[1] = { 64, 64, 64, 64 };
    valueGrassMap[2] = { 64, 64, 64, 64 };
    valueGrassMap[9] = { 64, 64, 64, 64 };
    valueGrassMap[10] = { 64, 64, 64, 64 };
    valueGrassMap[11] = { 64, 64, 64, 64 };
    valueGrassMap[16] = { 64, 64, 64, 64 };
    valueGrassMap[19] = { 64, 64, 64, 64 };
    valueGrassMap[18] = { 64, 64, 64, 64 };
    valueGrassMap[20] = { 64, 64, 64, 64 };
    valueGrassMap[33] = { 64, 64, 64, 64 };
    valueGrassMap[41] = { 64, 64, 64, 64 };
    valueGrassMap[42] = { 64, 64, 64, 64 };
    valueGrassMap[43] = { 64, 64, 64, 64 };

    valueTreesMap[1] = { 64, 0, 64, 64 };
    valueTreesMap[24] = { 0, 64, 64, 64 };
    valueTreesMap[25] = { 64, 64, 64, 64 };
    valueTreesMap[26] = { 128, 64, 64, 64 };
    valueTreesMap[48] = { 0, 128, 64, 64 };
    valueTreesMap[49] = { 64, 128, 64, 64 };
    valueTreesMap[50] = { 128, 128, 64, 64 };
    valueTreesMap[72] = { 0, 192, 64, 64 };
    valueTreesMap[73] = { 64, 192, 64, 64 };
    valueTreesMap[74] = { 128, 192, 64, 64 };

    SetTextureFilter(grassSprite, TEXTURE_FILTER_POINT);
    SetTextureFilter(treesSprite, TEXTURE_FILTER_POINT);

}

void map::loadMap(const std::string& mapFile, std::vector<int>& layer) {

    tileDest = { 0, 0, 64, 64 };
    std::ifstream file;
    file.open(mapFile);
    if (!file.is_open()) {
        std::cout << "Файл карты не открылся" << std::endl;
        return;
    }
    layer.clear();

    int value;
    while (file >> value) {
        layer.push_back(value);
        if (file.peek() == ',' || file.peek() == '\n') file.ignore();
    }
    file.close();

}

void map::drawLayer(const std::vector<int>& layer, Texture2D sprite, std::map<int, Rectangle>& tileMap, Vector2 playerPos) {

    int startX = (playerPos.x / Width) - 15;
    int startY = (playerPos.y / Height) - 10;
    int endX = startX + 30;
    int endY = startY + 20;

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            int idxX = x % mapW;
            int idxY = y % mapH;

            if (idxX < 0) idxX += mapW;
            if (idxY < 0) idxY += mapH;

            int index = idxY * mapW + idxX;
            if (index < 0 || index >= (int)layer.size()) continue;

            int tileValue = layer[index];
            if (tileValue <= 0) continue;

            tileSrc = tileMap.at(tileValue);
            tileDest.x = x * Width;
            tileDest.y = y * Height;
            DrawTexturePro(sprite, tileSrc, tileDest, { 0, 0 }, 0.0f, WHITE);
        }
    }

}