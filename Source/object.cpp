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
// CGround : ground class
/////////////////////////////////////////////////////////////////////////////

Object::Object()
{
    x = y = width = height = osX1 = osY1 = osX2 = osY2 = 0;
    size = 1.0;
}

Object::Object(double s)
{
    x = y = width = height = osX1 = osY1 = osX2 = osY2 = 0;
    size = s;
}

Object::Object(int x1, int y1, int x2, int y2)
{
    x = y = width = height = 0;
    size = 1.0;
    osX1 = x1;
    osY1 = y1;
    osX2 = x2;
    osY2 = y2;
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
            return x + (int)(osX1 * size);

        case 1:
            return y + (int)(osY1 * size);

        case 2:
            return x + (int)((width - osX2) * size);

        case 3:
            return y + (int)((height - osY2) * size);

        default:
            return NULL;
    }
}

void Object::LoadBitmap(int resource, COLORREF color)
{
    bmp.LoadBitmap(resource, color);		// 載入圖形
    width = bmp.Width();					// 載入寬度
    height = bmp.Height();					// 載入高度
    array = GetCArray(resource);
}

void Object::SetXY(int nx, int ny)
{
    x = nx - (int)(osX1 * size);
    y = ny - (int)(osY1 * size);
}

void Object::SetSize(double s)
{
    size = s;
}

void Object::OnShow()
{
    bmp.SetTopLeft(x - osX1, y - osY1);
    bmp.ShowBitmap(size);
}

void Object::OnMove()
{
}

bool Object::Collision(CArray other, int tx, int ty, int ox, int oy)
{
    double s = 1.0;
    vector<vector<bool>> otherMask = other.pixel;

    if ((tx + ti(height, s) < ox)        // This checks if the sprites
            || ty + ti(width, s) < oy // aren't close to each other.
            || ox + ti(other.height, s) < tx
            || oy + ti(other.width, s) < ty)
        return false;

    //This stuff isn't working.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < other.height; k++)
            {
                for (int l = 0; l < other.width; l++)
                {
                    if ((tx + i * s) <= (ox + k * s + s) && (tx + i * s + s) >= (ox + k * s)
                            && (ty + j * s) <= (oy + l * s + s) && (ty + j * s + s) >= (oy + l * s + s)
                            && array.pixel[i][j] && other.pixel[k][l])
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
}