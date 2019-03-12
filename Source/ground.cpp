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
		// �˴����a�Һc�����x�άO�_�I��a��
		return HitRectangle(x1,y1,x2,y2);
	}*/

	bool Ground::HitRectangle(int tx1, int ty1, int tx2, int ty2)
	{
		int x1 = x;				// �y�����W��x�y��
		int y1 = y;				// �y�����W��y�y��
		int x2 = x + width;	// �y���k�U��x�y��
		int y2 = y + height;	// �y���k�U��y�y��
									//
									// �˴��y���x�λP�ѼƯx�άO�_���涰
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
		bmp.LoadBitmap(IDB_GROUND, RGB(0, 0, 0));			// ���J�a�Ϫ��ϧ�
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