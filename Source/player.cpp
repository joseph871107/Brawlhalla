#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "player.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CGround : ground class
	/////////////////////////////////////////////////////////////////////////////

	Player::Player(Ground *grd)
	{
		int rr[6] = { IDB_P1_RUN0, IDB_P1_RUN1, IDB_P1_RUN2, IDB_P1_RUN3, IDB_P1_RUN4, IDB_P1_RUN5 };
		int rl[6] = { IDB_P1_RUN0M, IDB_P1_RUN1M, IDB_P1_RUN2M, IDB_P1_RUN3M, IDB_P1_RUN4M, IDB_P1_RUN5M };
		int jr[6] = { IDB_P1_JUMP0, IDB_P1_JUMP1, IDB_P1_JUMP2, IDB_P1_JUMP3 };
		int jl[6] = { IDB_P1_JUMP0M, IDB_P1_JUMP1M, IDB_P1_JUMP2M, IDB_P1_JUMP3M };
		jumpL.SetRepeat(false);
		jumpR.SetRepeat(false);
		x = y = width = height = 0;
		jumpCount = 2;
		velocity = 0.0;
		isMovingLeft = isMovingRight = isMovingUp = dir = onGround = false;
		isMovingDown = true;
		ground = grd;
	}

	bool Player::HitObject()
	{
		int x1 = ground->GetCor(0), y1 = ground->GetCor(1), x2 = ground->GetCor(2), y2 = ground->GetCor(3);
		// 檢測玩家所構成的矩形是否碰到地
		bool flag = HitRectangle(x1, y1, x2, y2);
		if (flag)
			velocity = 0;
		return (flag);
	}

	bool Player::IsOnGround()
	{
		return ground->GetCor(1) <= GetY2();
	}

	bool Player::HitRectangle(int tx1, int ty1, int tx2, int ty2)
	{
		int x1 = GetX1(), y1 = GetY1(), x2 = GetX2(), y2 = GetY2();
		return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
	}

	void Player::Jump(double acc)
	{
		if (jumpCount > 0) {
			velocity = -acc;
			y -= (int)acc;
			jumpL.Reset();
			jumpR.Reset();
			jumpCount--;
		}
	}

	int Player::ShowAnimationState()
	{
		if (!isMovingUp && !isMovingDown)
		{
			if (dir) return rr[right.GetCurrentBitmapNumber()];
			else return rl[left.GetCurrentBitmapNumber()];
		}
		else {
			if (dir) return jr[jumpR.GetCurrentBitmapNumber()];
			else return jl[jumpL.GetCurrentBitmapNumber()];
		}
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
		return x + right.Width();
	}

	int Player::GetY2()
	{
		return y + right.Height();
	}

	void Player::LoadBitmap()
	{
		CAnimation temp;
		for (int i = 0;i < 6;i++)
			temp.AddBitmap(rl[i], RGB(0, 0, 0));
		temp.SetSize(1.0);
		temp.SetDelayCount(5);
		temp.Reset();
		ani.insert(ani.end(),temp);

		for (int i = 0;i < 6;i++)
			left.AddBitmap(rl[i], RGB(0, 0, 0));
		left.SetSize(2.5);
		left.SetDelayCount(5);

		for (int i = 0;i < 6;i++)
			right.AddBitmap(rr[i], RGB(0, 0, 0));
		right.SetSize(2.5);
		right.SetDelayCount(5);

		for (int i = 0;i < 4;i++)
			jumpL.AddBitmap(jl[i], RGB(0, 0, 0));
		jumpL.SetSize(2.5);
		jumpL.SetDelayCount(5);

		for (int i = 0;i < 4;i++)
			jumpR.AddBitmap(jr[i], RGB(0, 0, 0));
		jumpR.SetSize(2.5);
		jumpR.SetDelayCount(5);

		width = right.Width();
		height = right.Height();

	}

	void Player::OnMove()
	{
		ani_iter = ani.begin();
		ani_iter->OnMove();
		right.OnMove();
		left.OnMove();
		jumpL.OnMove();
		jumpR.OnMove();
		if (isMovingLeft)
			x -= 8;
		if (isMovingRight)
			x += 8;
		/*if (isMovingUp)
			y -= 8;
		if (isMovingDown)
			y += 8;
		*/
		if (!HitObject())
		{
			velocity += 2;
			y += (int)velocity;
		}
		if (HitObject())
		{
			if (ground->GetCor(1) <= GetY2() && ground->GetCor(3) > GetY1())
				y = ground->GetCor(1) - height;
			if (ground->GetCor(3) >= GetY1() && ground->GetCor(1) < GetY2())
				y = ground->GetCor(1) + height - 10;
			jumpCount = 2;
			//if (ground->GetX1() <= GetX2() && ground->GetX2() > GetX1())
				//x = ground->GetX1() + width - 10;
			//if (ground->GetX2() >= GetX1() && ground->GetX1() < GetX2())
				//x = ground->GetX1() - width + 10;
		}
		if (GetY1() > 1000)
		{
			y = -height;
			velocity = 0.0;
		}
		ani_iter = ani.begin();
	}

	void Player::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void Player::SetMovingLeft(bool flag)
	{
		if(flag)
			dir = false;
		isMovingLeft = flag;
		if (flag)
			left.SetPause(false);
		else
			left.SetPause(true);
	}

	void Player::SetMovingRight(bool flag)
	{
		if(flag)
			dir = true;
		isMovingRight = flag;
		if (flag)
			right.SetPause(false);
		else
			right.SetPause(true);
	}

	void Player::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
		jumpR.Reset();
		jumpR.Reset();
	}

	void Player::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void Player::OnShow()
	{
		ani_iter = ani.begin();
		//ani_iter->Reset();
		//ani_iter->SetTopLeft(x,y);
		ani_iter->OnShow();
		/*
		if (!isMovingUp && !isMovingDown)
		{
			if (dir)
			{
				right.SetTopLeft(x, y);
				right.OnShow();
			}
			else
			{
				left.SetTopLeft(x, y);
				left.OnShow();
			}
		}
		else {
			if (dir)
			{
				jumpR.SetTopLeft(x, y);
				jumpR.OnShow();
			}
			else
			{
				jumpL.SetTopLeft(x, y);
				jumpL.OnShow();
			}
		}*/
	}
}