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

	valueGrassMap[3] = {0, 0, 64, 64};
	valueGrassMap[1] = {64, 0, 64, 64};
	valueGrassMap[2] = {128, 0, 64, 64 };
	valueGrassMap[9] = {0, 64,64, 64};
	valueGrassMap[10] = {64, 64, 64, 64};
	valueGrassMap[11] = {128, 64, 64, 64};
	valueGrassMap[16] = {128, 64, 64, 64}; 
	valueGrassMap[19] = {64, 128, 64, 64};
	valueGrassMap[18] = {0, 128, 64, 64};
	valueGrassMap[20] = {128, 128, 64, 64};
	valueGrassMap[33] = {64, 192, 64, 64};
	valueGrassMap[41] = {320, 256, 64, 64};
	valueGrassMap[42] = {320, 320, 64, 64};
	valueGrassMap[43] = {320, 256, 64, 64};

	valueTreesMap[1] = {64, 0, 64, 64};
	valueTreesMap[24] = {0, 64, 64, 64};
	valueTreesMap[25] = {64, 64, 64, 64};
	valueTreesMap[26] = {128, 64, 64, 64};
	valueTreesMap[48] = {0, 128, 64, 64};
	valueTreesMap[49] = {64, 128, 64, 64};
	valueTreesMap[50] = { 128, 128, 64, 64 };
	valueTreesMap[72] = {0, 192, 64, 64};
	valueTreesMap[73] = { 64, 192, 64, 64 };
	valueTreesMap[74] = { 128, 192, 64, 64 };

	SetTextureFilter(grassSprite, TEXTURE_FILTER_POINT);
	SetTextureFilter(treesSprite, TEXTURE_FILTER_POINT);

}

void map::loadMap(const std::string& mapFile, std::vector<int>& layer) {

	tileDest = {0, 0, 64, 64};
	std::ifstream file;
	file.open(mapFile);
	if (!file.is_open()) {
		std::cout << "Файл карты не открылся" << std::endl;
		return;
	}
	layer.clear();

	int value;
	while(file>>value){
		layer.push_back(value);
		if (file.peek() == ',' || file.peek() == '\n') file.ignore();
	}
	file.close();
	
}

void map::drawLayer(const std::vector<int>&layer, Texture2D sprite, std::map<int, Rectangle>&map) {

	for (int i = 0; i < layer.size(); i++) {
		if (layer[i] <= 0)continue;
		tileSrc = map.at(layer[i]);
		tileDest.x = tileDest.width * float(i % mapW);
		tileDest.y = tileDest.height * float(i / mapW);
		DrawTexturePro(sprite, tileSrc, tileDest, Vector2{ 0,0 }, 0.0f, WHITE);

	}

}