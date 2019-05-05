#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "ground.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Ground class
/////////////////////////////////////////////////////////////////////////////

Ground::Ground()
{
    _lengthX = _lengthY = 1;
    osX1 = osY1 = osX2 = osY2 = 4;
    _lenOffsetX = 100;
    _lenOffsetY = 100;
    _bmpID = IDB_GROUND1;
    _color = RGB(0, 255, 0);
}

int Ground::GetCor(int index)
{
    switch (index)
    {
        case 0:
            return x + (int)(osX1 * _size);

        case 1:
            return y + (int)(osY1 * _size);

        case 2:
            return x + (int)((width - osX2) * _size);

        case 3:
            return y + (int)((height - osY2) * _size);

        default:
            return NULL;
    }
}

void Ground::SetID(int id)
{
    _bmpID = id;
}

void Ground::SetLenOffset(int offset, int dimension)
{
    if (dimension)
        _lenOffsetY = offset;
    else
        _lenOffsetX = offset;

    _UpdateWH();
}

void Ground::SetOffset(int i)
{
    osX1 = osY1 = osX2 = osY2 = i;
    _UpdateWH();
}

void Ground::LoadBitmap()
{
    bmp.LoadBitmap(_bmpID, _color);	// 載入地圖的圖形
    _UpdateWH();
}

void Ground::_UpdateWH()
{
    width = bmp.Width() + (_lengthX - 1) * _lenOffsetX;
    height = bmp.Height() + (_lengthY - 1) * _lenOffsetY;
}

void Ground::SetLen(int len, int dimension)
{
    if (dimension)
        _lengthY = len;
    else
        _lengthX = len;

    _UpdateWH();
}

void Ground::OnShow()
{
    for (int i = 0; i < _lengthX; i++)
    {
        for (int j = 0; j < _lengthY; j++)
        {
            int _x = x + (int)((i * _lenOffsetX - osX1) * _size), _y = y + (int)((j * _lenOffsetY - osY1) * _size);
            double size = _size;

            if (camera != nullptr)
            {
                CPoint cam = camera->GetXY(_x, _y);
                _x = cam.x;
                _y = cam.y;
                size = _size * camera->GetSize();
            }

            bmp.SetTopLeft(_x, _y);
            bmp.ShowBitmap(size);
        }
    }
}
GroundPARM::GroundPARM(int x, int y, double size, int lengthX, int lengthY, int id, int offset, int lenOffsetX, int lenOffsetY)
{
    point = CPoint(x, y);
    _size = size;
    _lengthX = lengthX;
    _lengthY = lengthY;
    _id = id;
    _offset = offset;
    _lenOffsetX = lenOffsetX;
    _lenOffsetY = lenOffsetY;
}

bool Ground::IsIntersectGround(int objectX1, int objectY1, int objectX2, int objectY2)
{
    int groundX1 = GetCor(0);
    int groundY1 = GetCor(1);
    int groundX2 = GetCor(2);
    int groundY2 = GetCor(3);
    return (objectX2 >= groundX1 && objectX1 <= groundX2 && objectY2 >= groundY1 && objectY1 <= groundY2);
}

bool Ground::IsExplicitlyVerticallyIntersectGround(int objectX1, int objectY1, int objectX2, int objectY2)
{
    int groundX1 = GetCor(0);
    int groundY1 = GetCor(1);
    int groundX2 = GetCor(2);
    int groundY2 = GetCor(3);
    return (!(objectY2 <= groundY1 || objectY1 >= groundY2));
}

bool Ground::IsExplicitlyHorizontallyIntersectGround(int objectX1, int objectY1, int objectX2, int objectY2)
{
    int groundX1 = GetCor(0);
    int groundY1 = GetCor(1);
    int groundX2 = GetCor(2);
    int groundY2 = GetCor(3);
    return (!((objectX2 <= groundX1) || (objectX1 >= groundX2)));
}

bool Ground::IsOnGroundLeftEdge(int objectX1, int objectY1, int objectX2, int objectY2)
{
    int groundX1 = GetCor(0);
    int groundY1 = GetCor(1);
    int groundX2 = GetCor(2);
    int groundY2 = GetCor(3);
    return (IsExplicitlyVerticallyIntersectGround(objectX1, objectY1, objectX2, objectY2)
            &&
            (objectX1 < groundX1 && groundX1 < objectX2 && objectX2 < groundX2));
}

bool Ground::IsOnGroundRightEdge(int objectX1, int objectY1, int objectX2, int objectY2)
{
    int groundX1 = GetCor(0);
    int groundY1 = GetCor(1);
    int groundX2 = GetCor(2);
    int groundY2 = GetCor(3);
    return (IsExplicitlyVerticallyIntersectGround(objectX1, objectY1, objectX2, objectY2)
            &&
            (groundX1 < objectX1 && objectX1 < groundX2 && groundX2 < objectX2));
}

bool Ground::IsOnGroundUnderside(int objectX1, int objectY1, int objectX2, int objectY2)
{
    int groundX1 = GetCor(0);
    int groundY1 = GetCor(1);
    int groundX2 = GetCor(2);
    int groundY2 = GetCor(3);
    return (IsExplicitlyHorizontallyIntersectGround(objectX1, objectY1, objectX2, objectY2)
            &&
            (groundY1 < objectY1 && objectY1 <= groundY2 && groundY2 < objectY2));
}

bool Ground::IsOnGround(int objectX1, int objectY1, int objectX2, int objectY2)
{
    int groundX1 = GetCor(0);
    int groundY1 = GetCor(1);
    int groundX2 = GetCor(2);
    int groundY2 = GetCor(3);
    return (objectY1 < groundY1 && groundY1 <= objectY2 && objectY2 < groundY2
            &&
            groundX1 <= objectX1 && objectX2 <= groundX2);
}





}