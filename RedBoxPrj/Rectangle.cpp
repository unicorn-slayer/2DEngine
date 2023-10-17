#include "Rectangle.h"
#include "Level.h"

Rectangle::Rectangle() : _x(0), _y(0), _width(0), _height(0)
{
}

Rectangle::Rectangle(float x, float y, int width, int height) : _x(x), _y(y), _width(width), _height(height)
{
}

bool Rectangle::checkCollision(const Tile& tile)
{
    //The sides of the rectangles
    float leftA, leftB;
    float rightA, rightB;
    float topA, topB;
    float bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = _x;
    rightA = _x + _width;
    topA = _y;
    bottomA = _y + _height;

    //Calculate the sides of rect B
    leftB = tile.boundingBox._x;
    rightB = tile.boundingBox._x + tile.boundingBox._width;
    topB = tile.boundingBox._y;
    bottomB = tile.boundingBox._y + tile.boundingBox._height;

    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    return true;
}

sides::Side Rectangle::getCollisionSide(Rectangle& other)
{
    float amtRight, amtLeft, amtTop, amtBottom;
    amtRight = (_x + _width) - other._x;
    amtLeft = (other._x + other._width) - _x;
    amtTop = (other._y + other._height) - _x;
    amtBottom = (_y + _height) - other._y;

    float vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
    float lowest = vals[0];
    for (int i = 0; i < 4; i++) {
        if (vals[i] < lowest) {
            lowest = vals[i];
        }
    }

    return
        lowest == abs(amtRight) ? sides::RIGHT :
        lowest == abs(amtLeft) ? sides::LEFT :
        lowest == abs(amtTop) ? sides::TOP :
        lowest == abs(amtBottom) ? sides::BOTTOM :
        sides::NONE;
}
