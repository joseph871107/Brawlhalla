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
// CGround : ground class
/////////////////////////////////////////////////////////////////////////////

Ground::Ground(): bmpID(IDB_GROUND)
{
    length = 1;
    size = 1.0;
    x = y = width = height = 0;
    osX1 = osY1 = osX2 = osY2 = (int)(10 * size);
}

int Ground::GetCor(int index)
{
    switch (index)
    {
        case 0:
            return x + (int)(osX1 * size);

        case 1:
            return y + (int)(osY1 * size);

        case 2:
            return x + (int)((width + (length - 2) * 100 - osX2) * size);

        case 3:
            return y + (int)((height - osY2) * size);

        default:
            return 0;
    }
}

void Ground::LoadBitmap()
{
    bmp.LoadBitmap(bmpID, RGB(0, 0, 0));			// 載入地圖的圖形
    width = bmp.Width();
    height = bmp.Height();
    array = cArray.find(IDB_GROUND)->second;
}

void Ground::SetLen(int len)
{
    length = len;
}

void Ground::OnShow()
{
    for (int i = 0; i < length; i++)
    {
        bmp.SetTopLeft(x + (int)(i * 100 * size), y);
        bmp.ShowBitmap(size);
    }
}

bool GetInters(vector<vector<bool>> a, vector<vector<bool>> b, int aXS, int aYS, int bXS, int bYS, int width, int height)
{
    bool xb = width > height;
    int minV = (!xb ? width : height);
    int maxV = (xb ? width : height);

    for (int i = 0; i < (int)(minV / 2); i++)
    {
        if (xb)
        {
            for (int j = i; j < maxV - i; j++)
            {
                if ((a[aXS + j][aYS] && b[bXS + j][bYS]) || (a[aXS + j][aYS + height] && b[bXS + j][bYS + height]))
                    return true;
            }

            for (int j = i; j < minV - i; j++)
            {
                if ((a[aXS][aYS + j] && b[bXS][bYS + j]) || (a[aXS + width][aYS + j] && b[bXS + height][bYS + j]))
                    return true;
            }
        }
        else
        {
            for (int j = i; j < maxV - i; j++)
            {
                if ((a[aXS][aYS + j] && b[bXS][bYS + j]) || (a[aXS + height][aYS + j] && b[bXS + height][bYS + j]))
                    return true;
            }

            for (int j = i; j < minV - i; j++)
            {
                if ((a[aXS + j][aYS] && b[bXS + j][bYS]) || (a[aXS + j][aYS + width] && b[bXS + j][bYS + height]))
                    return true;
            }
        }
    }

    return false;
}

bool Ground::Collision(ColArray* other, double bSize, int tx, int ty, int ox, int oy)
{
    /*
    int offsetX = (int)((ox - tx) / 4), offsetY = (int)((oy - ty) / 4), aH = (int)(array.pixel.size() * size), aW = (int)(array.pixel[0].size() * size), bH = other->pixel.size(), bW = other->pixel[0].size();
    if ((tx + (int)(aH * 4) < ox)         // This checks if the sprites
    	|| ty + (int)(aW * 4) < oy // aren't close to each other.
    	|| ox + (int)(bH * 4 * bSize) < tx
    	|| oy + (int)(bW * 4 * bSize) < ty)
    	return false;
    int aXS, aYS, bXS, bYS, interW, interH;
    aXS = (offsetX > 0 ? offsetX - 1 : 0);
    aYS = (offsetY > 0 ? offsetY - 1 : 0);
    bXS = (offsetX > 0 ? 0 : -offsetX - 1);
    bYS = (offsetY > 0 ? 0 : -offsetY - 1);
    interW = (offsetX > 0 ? aW - offsetX : bW + offsetX);
    interH = (offsetY > 0 ? aH - offsetY : bH + offsetY);
    TRACE("aW : %d, aH : %d, bW : %d, bH : %d, aXS : %d, aYS : %d, bXS : %d, bYS : %d, interW : %d, interH : %d\n",aW,aH,bW,bH,aXS,aYS,bXS,bYS,interW,interH);
    return GetInters(array.pixel, other->pixel, aXS, aYS, bXS, bYS, interW, interH);*/
    return false;
}
}