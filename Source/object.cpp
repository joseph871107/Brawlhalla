#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "object.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Basic object class
/////////////////////////////////////////////////////////////////////////////

Object::Object()
{
    x = y = width = height = 0;
    _size = 1.0;
}

Object::Object(double s)
{
    x = y = width = height = 0;
    _size = s;
}

bool Object::HitRectangle(int tx1, int ty1, int tx2, int ty2)
{
    int x1 = GetCor(0), y1 = GetCor(1), x2 = GetCor(2), y2 = GetCor(3);
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

int Object::GetCor(int index)
{
    switch (index)
    {
        case 0:
            return x;

        case 1:
            return y;

        case 2:
            return x + (int)(width * _size);

        case 3:
            return y + (int)(height * _size);

        default:
            return NULL;
    }
}

int Object::GetWidth()
{
	return GetCor(2) - GetCor(0);
}

int Object::GetHeight()
{
	return GetCor(3) - GetCor(1);
}

double Object::GetSize()
{
	return _size;
}

void Object::LoadBitmap(int resource, COLORREF color)
{
    bmp.LoadBitmap(resource, color);
}

void Object::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}

void Object::SetSize(double s)
{
    _size = s;
}

void Object::OnShow()
{
	bmp.SetTopLeft(x, y);
    bmp.ShowBitmap(_size);
}

void Object::OnMove()
{
}
}