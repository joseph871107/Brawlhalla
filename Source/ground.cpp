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
    _length = 1;
    osX1 = osY1 = osX2 = osY2 = 4;
    _bmpID = IDB_GROUND;
    _color = RGB(0, 0, 0);
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

void Ground::LoadBitmap()
{
    bmp.LoadBitmap(_bmpID, _color);	// 載入地圖的圖形
    width = bmp.Width() + (_length - 1) * 100;
    height = bmp.Height();
}

void Ground::SetLen(int len)
{
    _length = len;
    width = bmp.Width() + (_length - 1) * 100;
}

void Ground::OnShow()
{
    for (int i = 0; i < _length; i++)
    {
        bmp.SetTopLeft(x + (int)((i * 100 - osX1) * _size), y - (int)(osX2 * _size));
        bmp.ShowBitmap(_size);
    }
}
}