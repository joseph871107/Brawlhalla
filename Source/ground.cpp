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
		x = y =width = height = 0;
	}

	/*bool Ground::HitPlayer(Player player)
	{
		int x1=player.GetX1(), y1=player.GetY1(), x2=player.GetX2(), y2=player.GetY2();
		// 檢測玩家所構成的矩形是否碰到地面
		return HitRectangle(x1,y1,x2,y2);
	}*/

	bool Ground::HitRectangle(int tx1, int ty1, int tx2, int ty2)
	{
		int x1 = x;				// 球的左上角x座標
		int y1 = y;				// 球的左上角y座標
		int x2 = x + width;	// 球的右下角x座標
		int y2 = y + height;	// 球的右下角y座標
									//
									// 檢測球的矩形與參數矩形是否有交集
									//
		return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
	}

	int Ground::GetX1()
	{
		return x;
	}

	int Ground::GetY1()
	{
		return y;
	}

	int Ground::GetX2()
	{
		return x + (length - 1) * 100 + bmp.Width();
	}

	int Ground::GetY2()
	{
		return y + bmp.Height();
	}

	void Ground::LoadBitmap()
	{
		bmp.LoadBitmap(IDB_GROUND, RGB(0, 0, 0));			// 載入地圖的圖形
		width = bmp.Width();
		height = bmp.Height();

	}

	void Ground::OnMove()
	{
	}

	void Ground::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void Ground::SetLen(int len)
	{
		length = len;
	}

	void Ground::OnShow()
	{
		for (int i = 0; i < length; i++)
		{
			bmp.SetTopLeft(x + i * 100, y);
			bmp.ShowBitmap();
		}
	}
}