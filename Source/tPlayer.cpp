#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "tPlayer.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CGround : ground class
	/////////////////////////////////////////////////////////////////////////////

	Player::Player()
	{
		x = y = width = height = 0;
		isMovingLeft = isMovingRight = isMovingUp = dir = false;
		isMovingDown = true;
	}

	bool Player::HitObject(Ground *ground)
	{
		int x1 = ground->GetX1(), y1 = ground->GetY1(), x2 = ground->GetX2(), y2 = ground->GetY2();
		// 檢測玩家所構成的矩形是否碰到地面
		return HitRectangle(x1, y1, x2, y2);
	}

	bool Player::HitRectangle(int tx1, int ty1, int tx2, int ty2)
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

	int Player::GetX1()
	{
		return x;
	}

	int Player::GetY1()
	{
		return y;
	}

	int Player::GetX2()
	{
		return x + front.Width();
	}

	int Player::GetY2()
	{
		return y + front.Height();
	}

	void Player::LoadBitmap()
	{
		front.AddBitmap(IDB_PLAYER, RGB(0, 0, 0));			// 載入圖形
		back.AddBitmap(IDB_PLAYER_B, RGB(0, 0, 0));
		width = front.Width();
		height = front.Height();

	}

	void Player::OnMove(Ground *ground)
	{
		front.OnMove();
		back.OnMove();
		const int STEP_SIZE = 2;
		if (isMovingLeft)
			x -= STEP_SIZE;
		if (isMovingRight)
			x += STEP_SIZE;
		if (isMovingUp)
			y -= 4;// STEP_SIZE;
		if (isMovingDown && HitObject(ground)==false)
			y += STEP_SIZE;
	}

	void Player::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void Player::SetMovingLeft(bool flag)
	{
		dir = false;
		isMovingLeft = flag;
	}

	void Player::SetMovingRight(bool flag)
	{
		dir = true;
		isMovingRight = flag;
	}

	void Player::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void Player::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void Player::OnShow()
	{
		if (dir)
		{
			front.SetTopLeft(x, y);
			front.OnShow();
		}
		else
		{
			back.SetTopLeft(x, y);
			back.OnShow();
		}
	}
}