#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <map>

class map {

public:

	Texture2D grassSprite;
	Texture2D treesSprite;

	Rectangle tileSrc;
	Rectangle tileDest;
	std::vector<int> tileMap;
	std::string srcMap;
	int mapW = 10;
	int mapH = 10;
	int Width = 64;
	int Height = 64;
	void Tex();
	void loadMap(const std::string& mapFile, std::vector<int>&layer);
	void drawLayer(const std::vector<int>&layer, Texture2D sprite, std::map<int, Rectangle>&map);
	std::map<int, Rectangle> valueGrassMap;
	std::map<int, Rectangle> valueTreesMap;
	std::vector<int> grassLayer;
	std::vector<int> treesLayer;

};