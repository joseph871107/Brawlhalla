/* File description:
This is the source code file 'Vector2.cpp'. This is the implementation for the class 'Vector2'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Vector2.h"

namespace game_framework
{

Vector2::~Vector2()
{
    /* Body intentionally empty */
}

Vector2::Vector2() : Vector2(int(), int())
{
    /* Body intentionally empty */
}

Vector2::Vector2(const Vector2& objectValue) : Vector2(objectValue._x, objectValue._y)
{
    /* Body intentionally empty */
}

Vector2::Vector2(const int& xValue, const int& yValue) : _x(xValue), _y(yValue)
{
    /* Body intentionally empty */
}

Vector2& Vector2::operator=(const Vector2& rightObject)
{
    if (this != &rightObject)
    {
        _x = rightObject._x;
        _y = rightObject._y;
    }

    return (*this);
}

const double Vector2::GetLength() const
{
    return (sqrt(pow(_x, 2) + pow(_y, 2)));
}

const int& Vector2::GetX() const
{
    return (_x);
}

const int& Vector2::GetY() const
{
    return (_y);
}

void Vector2::SetXY(const int& x1, const int& y1, const int& x2, const int& y2)
{
    _x = x2 - x1;
    _y = y2 - y1;
}
}