#pragma once
#include <string>
#include <vector>
#include <SDL.h>
#include "Rectangle.h"
#include "Graphics.h"


struct Tile
{
	Tile();
	Tile(SDL_Rect source, SDL_Rect dest);
	SDL_Rect sourceRect;
	SDL_Rect destRect;
	Rectangle boundingBox;
};

class Level
{
public:
	Level();
	void loadMap(Graphics& graphics, const std::string& file);
	void draw(Graphics& graphics, Rectangle& camera);
	std::vector<Tile> getCollisionTiles();
	std::vector<std::pair<int, int>> getMarioSpawnPoints();
	std::vector<std::pair<int, int>> getLuigiSpawnPoints();
	std::vector<std::pair<int, int>> getJumpingMarioSpawnPoints();
	std::vector<std::pair<int, int>> getItemBoxes();

private:
	std::vector<int> m_numbers;
	SDL_Texture* m_tileset;
	std::vector<SDL_Rect> m_tilesetSourceRects;
	std::vector<Tile> m_tiles;
	std::vector<Tile> m_collisionTiles;
	std::vector<std::pair<int, int>> m_marioSpawnPoints;
	std::vector<std::pair<int, int>> m_luigiSpawnPoints;
	std::vector<std::pair<int, int>> m_jumpingMarioSpawnPoints;
	std::vector<std::pair<int, int>> m_itemBoxes;
	int m_tileCount;
	int m_tilesetWidth;
	int m_tileWidth;
	int m_tileHeight;
	std::string m_tilesetName;
	int m_mapWidth;
};