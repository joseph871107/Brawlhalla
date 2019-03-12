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

	Player::Player()
	{
		x = y = width = height = 0;
		isMovingLeft = isMovingRight = isMovingUp = dir = false;
		isMovingDown = true;
	}

	bool Player::HitObject(Ground *ground)
	{
		int x1 = ground->GetX1(), y1 = ground->GetY1(), x2 = ground->GetX2(), y2 = ground->GetY2();
		// �˴����a�Һc�����x�άO�_�I��a��
		bool* flag;
		flag = HitRectangle(x1, y1, x2, y2);
		return (flag[0] && flag[1] && flag[2] && flag[3]);
	}

	bool* Player::HitRectangle(int tx1, int ty1, int tx2, int ty2)
	{
		int x1 = x;				// ���a�����W��x�y��
		int y1 = y;				// ���a�����W��y�y��
		int x2 = x + width;		// ���a���k�U��x�y��
		int y2 = y + height;	// ���a���k�U��y�y��
		bool flag[4] = { false };
		if (tx2 >= x1)			// ���a�I�쪫�饪��
			flag[0] = true;
		if (tx1 <= x2)			// ���a�I�쪫��k��
			flag[1] = true;
		if (ty2 >= y1)			// ���a�I�쪫��W��
			flag[2] = true;
		if (ty1 <= y2)			// ���a�I�쪫��U��
			flag[3] = true;
		return &(*flag);
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
		front.AddBitmap(IDB_PLAYER, RGB(0, 0, 0));			// ���J�ϧ�
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
		if (isMovingDown && !HitObject(ground))
			y += STEP_SIZE;
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
	}

	void Player::SetMovingRight(bool flag)
	{
		if(flag)
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