#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "ground.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CGround : ground class
	/////////////////////////////////////////////////////////////////////////////

	Ground::Ground()
	{
		length = 1;
		size = 1.0;
		x = y =width = height = 0;
		osX1 = osY1 = osX2 = osY2 = (int)(10 * size);
	}

	int Ground::GetCor(int index)
	{
		switch (index) {
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

	bool Ground::HitRectangle(int tx1, int ty1, int tx2, int ty2)
	{
		int x1 = GetCor(0), y1 = GetCor(1), x2 = GetCor(2), y2 = GetCor(3);
		return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
	}

	void Ground::LoadBitmap()
	{
		bmp.LoadBitmap(IDB_GROUND, RGB(0, 0, 0));			// 載入地圖的圖形
		width = bmp.Width();
		height = bmp.Height();
		array = GetCArray(IDB_GROUND);
	}

	void Ground::SetSize(double s)
	{
		size = s;
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

	void Ground::OnMove()
	{
	}
	
	bool Ground::Collision(CArray* other, int tx, int ty, int ox, int oy) {
		/*
		CArray *min, *max;
		if (array.width*array.height < other->width*other->height) {
			min = &array;
			max = other;
		}
		else {
			min = other;
			max = &array;
		}*/
		
		int offsetX = (int)((ox - tx) / 4), offsetY = (int)((oy - ty) / 4);
		if ((tx + (int)(array.pixel.size() * 4) < ox)         // This checks if the sprites
			|| ty + (int)(array.pixel[0].size() * 4) < oy // aren't close to each other.
			|| ox + (int)(other->pixel.size() * 4) < tx
			|| oy + (int)(other->pixel[0].size() * 4) < ty)
			return false;
		/*
		if (offsetX > 0 && offsetY > 0) {
			if (offsetX < offsetY) {
				for (int i = 0; i < (int)(offsetX/2); i++) {
					for (int j = 0; j < offsetX; j++) {
						if (array.pixel[i+offsetX][j + offsetX] && other->pixel[i][j - offsetX + offsetY])
							return true;
					}
				}
			}
			else {

			}
		}*/
		return false;
	}
}