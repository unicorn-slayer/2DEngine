#include "Level.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "tinyxml2.h"
#include "Globals.h"

Tile::Tile()
{
    sourceRect = {};
    destRect = {};
    boundingBox = Rectangle();
}

Tile::Tile(SDL_Rect source, SDL_Rect dest)
{
    sourceRect = source;
    destRect = dest;
    boundingBox = Rectangle((float)dest.x, (float)dest.y, dest.w, dest.h);
}


Level::Level() : m_numbers({})
{
    m_tileset = {};
    m_tilesetSourceRects = {};
    m_tileWidth = 0;
    m_tileHeight = 0;
    m_tileCount = 0;
    m_tilesetName = {};
    m_tiles = {};
    m_mapWidth = 0;
    m_collisionTiles = {};
}

void Level::loadMap(Graphics& graphics, const std::string& xml)
{
    //setting up xml parsing
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile("map.tmx") != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file." << std::endl;
        return;
    }

    tinyxml2::XMLElement* mapElement = doc.FirstChildElement("map");
    if (!mapElement) {
        std::cerr << "No 'map' element found." << std::endl;
        return;
    }

    tinyxml2::XMLElement* tilesetElement = mapElement->FirstChildElement("tileset");
    if (!tilesetElement) {
        std::cerr << "No 'tileset' element found." << std::endl;
        return;
    }

    tinyxml2::XMLElement* layerElement = mapElement->FirstChildElement("layer");
    if (!layerElement) {
        std::cerr << "No 'layer' element found." << std::endl;
        return;
    }

    tinyxml2::XMLElement* dataElement = layerElement->FirstChildElement("data");
    if (!dataElement) {
        std::cerr << "No 'data' element found." << std::endl;
        return;
    }

    tinyxml2::XMLElement* objectgroup = mapElement->FirstChildElement("objectgroup");
    if (!objectgroup) {
        std::cerr << "No 'objectgroup' element found." << std::endl;
        return;
    }

    //parsing out the tileset.tsx for png and width
    std::string tsxFileName;
    const char* sourceAttribute = tilesetElement->Attribute("source");
    if (sourceAttribute) {
        tsxFileName = sourceAttribute;
    }

    tinyxml2::XMLDocument tsxDoc;
    if (tsxDoc.LoadFile(tsxFileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file." << std::endl;
        return;
    }

    tinyxml2::XMLElement* tsxTilesetElement = tsxDoc.FirstChildElement("tileset");
    if (!tsxTilesetElement) {
        std::cerr << "No 'tileset' element found." << std::endl;
        return;
    }
    
    tinyxml2::XMLElement* imageElement = tsxTilesetElement->FirstChildElement("image");
    if (!imageElement) {
        std::cerr << "No 'image' element found." << std::endl;
        return;
    }

    if (tsxTilesetElement) {
        const char* tileCountAttribute = tsxTilesetElement->Attribute("tilecount");
        if (tileCountAttribute) {
            m_tileCount = std::stoi(tileCountAttribute);
        }
    }

    if (imageElement) {
        const char* sourceAttribute = imageElement->Attribute("source");
        if (sourceAttribute) {
            m_tileset = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(sourceAttribute));
        }
        const char* widthAttribute = imageElement->Attribute("width");
        if (widthAttribute) {
            m_tilesetWidth = std::stoi(widthAttribute);
        }
    }

    //defining tilewidth and tileheight
    if (mapElement) {
        const char* tileWidthAttribute = mapElement->Attribute("tilewidth");
        if (tileWidthAttribute) {
            m_tileWidth = std::stoi(tileWidthAttribute);
        }
        const char* tileHeightAttribute = mapElement->Attribute("tileheight");
        if (tileHeightAttribute) {
            m_tileHeight = std::stoi(tileHeightAttribute);
        }
        const char* mapWidthAttribute = mapElement->Attribute("width");
        if (mapWidthAttribute) {
            m_mapWidth = (std::stoi(mapWidthAttribute)) * m_tileWidth;
        }
    }

    //setting up the sourceRects
    int xCounter = 0;
    int yCounter = 0;
    m_tilesetSourceRects.resize(m_tileCount);
    for (SDL_Rect& rect : m_tilesetSourceRects)
    {
        rect.x = xCounter;
        rect.y = yCounter;
        rect.w = m_tileWidth;
        rect.h = m_tileHeight;

        xCounter += m_tileWidth;

        if (xCounter >= m_tilesetWidth)
        {
            xCounter = 0;
            yCounter += m_tileHeight;
        }
    }

    //parsing out the grid in order to set up the dest rects
    const char* csvData = dataElement->GetText();
    std::vector<int> numbers; //do I still need this?

    if (csvData) {
        std::istringstream csvStream(csvData);
        std::string token;

        int xCounter = 0;
        int yCounter = 0;

        while (std::getline(csvStream, token, ',')) {
            int number = std::stoi(token);
            numbers.push_back(number);

            Tile tile(m_tilesetSourceRects[number - 1], { xCounter, yCounter, m_tileWidth, m_tileHeight });

            xCounter += m_tileWidth;

            if (xCounter >= m_mapWidth)
            {
                xCounter = 0;
                yCounter += m_tileHeight;
            }

            m_tiles.push_back(tile);
        }
    }


    while (objectgroup) {
        int groupId;
        objectgroup->QueryIntAttribute("id", &groupId);

        if (groupId == 3) {
            // Iterate through objects within the objectgroup
            tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");
            while (object != nullptr) {
                Tile tile;
                object->QueryFloatAttribute("x", &tile.boundingBox._x);
                object->QueryFloatAttribute("y", &tile.boundingBox._y);
                object->QueryIntAttribute("width", &tile.boundingBox._width);
                object->QueryIntAttribute("height", &tile.boundingBox._height);
                m_collisionTiles.push_back(tile);
                object = object->NextSiblingElement("object");
            }

            objectgroup = objectgroup->NextSiblingElement("objectgroup");
            objectgroup->QueryIntAttribute("id", &groupId);

        }

        if (groupId == 4) {
            // Iterate through objects within the objectgroup
            tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");
            while (object != nullptr) {
                int x = 0;
                int y = 0;
                object->QueryIntAttribute("x", &x);
                object->QueryIntAttribute("y", &y);
                m_marioSpawnPoints.push_back({x, y});
                object = object->NextSiblingElement("object");
            }

            objectgroup = objectgroup->NextSiblingElement("objectgroup");
            objectgroup->QueryIntAttribute("id", &groupId);

        }

        if (groupId == 6) {
            // Iterate through objects within the objectgroup
            tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");
            while (object != nullptr) {
                int x = 0;
                int y = 0;
                object->QueryIntAttribute("x", &x);
                object->QueryIntAttribute("y", &y);
                m_jumpingMarioSpawnPoints.push_back({ x, y });
                object = object->NextSiblingElement("object");
            }

            objectgroup = objectgroup->NextSiblingElement("objectgroup");
            objectgroup->QueryIntAttribute("id", &groupId);

        }

        if (groupId == 5) {
            // Iterate through objects within the objectgroup
            tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");
            while (object != nullptr) {
                int x = 0;
                int y = 0;
                object->QueryIntAttribute("x", &x);
                object->QueryIntAttribute("y", &y);
                m_luigiSpawnPoints.push_back({ x, y });
                object = object->NextSiblingElement("object");
            }

            objectgroup = objectgroup->NextSiblingElement("objectgroup");
            objectgroup->QueryIntAttribute("id", &groupId);

        }

        break;

    }


}

void Level::draw(Graphics& graphics, Rectangle& camera)
{
    for (Tile tile : m_tiles)
    {
        if (camera.checkCollision(tile.boundingBox))
        {
            //as player moves, the tiles need to be offset by the camera x and y values
            tile.destRect.x = tile.destRect.x - (int)camera._x;
            tile.destRect.y = tile.destRect.y - (int)camera._y;

            graphics.blitSurface(m_tileset, &tile.sourceRect, &tile.destRect);
        }
    }
}

std::vector<Tile> Level::getCollisionTiles()
{
    return m_collisionTiles;
}

std::vector<std::pair<int, int>> Level::getMarioSpawnPoints()
{
    return m_marioSpawnPoints;
}

std::vector<std::pair<int, int>> Level::getLuigiSpawnPoints()
{
    return m_luigiSpawnPoints;
}

std::vector<std::pair<int, int>> Level::getJumpingMarioSpawnPoints()
{
    return m_jumpingMarioSpawnPoints;
}
