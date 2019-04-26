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
	camera = nullptr;
}

Object::Object(double s)
{
    x = y = width = height = 0;
    _size = s;
	camera = nullptr;
}

CPoint Object::GetXY()
{
	return CPoint(x, y);
}

bool Object::HitRectangle(int tx1, int ty1, int tx2, int ty2)
{
    int x1 = GetCor(0), y1 = GetCor(1), x2 = GetCor(2), y2 = GetCor(3);
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

void Object::AddCamera(Camera * cam)
{
	camera = cam;
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
            return x + GetWidth();

        case 3:
            return y + GetHeight();

        default:
            return NULL;
    }
}

int Object::GetWidth()
{
	return (int)(width * _size);
}

int Object::GetHeight()
{
	return (int)(height * _size);
}

double Object::GetSize()
{
	return _size;
}

void Object::LoadBitmap(int resource)
{
	bmp.LoadBitmap(resource);
	width = bmp.Width();
	height = bmp.Height();
}

void Object::LoadBitmap(int resource, COLORREF color)
{
    bmp.LoadBitmap(resource, color);
	width = bmp.Width();
	height = bmp.Height();
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

void Object::OnShow(int offsetX, int offsetY)
{
	int _x = x + offsetX, _y = y + offsetY;
	double size = _size;
	if (camera != nullptr) {
		CPoint cam = camera->GetXY(x, y);
		_x = cam.x;
		_y = cam.y;
		size = _size * camera->GetSize();
	}
	bmp.SetTopLeft(_x, _y);
	bmp.ShowBitmap(size);
}

void Object::OnMove()
{
}
}