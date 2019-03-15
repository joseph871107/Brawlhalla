#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "player.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// CGround : ground class
/////////////////////////////////////////////////////////////////////////////

Player::Player(Ground* grd)
{
    jumpL.SetRepeat(false);
    jumpR.SetRepeat(false);
    x = y = width = height = 0;
    velocity = 0.0;
    isMovingLeft = isMovingRight = isMovingUp = dir = onGround = false;
    isMovingDown = true;
    ground = grd;
}

bool Player::HitObject()
{
    int x1 = ground->GetCor(0), y1 = ground->GetCor(1), x2 = ground->GetCor(2), y2 = ground->GetCor(3);
    // 檢測玩家所構成的矩形是否碰到地面
    bool* flag;
    flag = HitRectangle(x1, y1, x2, y2);
    bool Aflag = flag[0] && flag[1] && flag[2] && flag[3];

    if (Aflag)
        velocity = 0;

    return (Aflag);
}

bool Player::IsOnGround()
{
    return ground->GetCor(1) <= GetY2();
}

bool* Player::HitRectangle(int tx1, int ty1, int tx2, int ty2)
{
    int x1 = x, y1 = y, x2 = x + width, y2 = y + height;	// 玩家的座標
    bool flag[4] = { false };

    if (tx2 >= x1)			// 玩家碰到物體左邊
        flag[0] = true;

    if (tx1 <= x2)			// 玩家碰到物體右邊
        flag[1] = true;

    if (ty2 >= y1)			// 玩家碰到物體上面
        flag[2] = true;

    if (ty1 <= y2)			// 玩家碰到物體下面
        flag[3] = true;

    return &(*flag);
}

void Player::Jump(double acc)
{
    velocity = -acc;
    y -= (int)acc;
    jumpL.Reset();
    jumpR.Reset();
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
    array = GetCArray(IDB_P1_RUN0M);
    left.AddBitmap(IDB_P1_RUN0M, RGB(0, 0, 0));
    left.AddBitmap(IDB_P1_RUN1M, RGB(0, 0, 0));
    left.AddBitmap(IDB_P1_RUN2M, RGB(0, 0, 0));
    left.AddBitmap(IDB_P1_RUN3M, RGB(0, 0, 0));
    left.AddBitmap(IDB_P1_RUN4M, RGB(0, 0, 0));
    left.AddBitmap(IDB_P1_RUN5M, RGB(0, 0, 0));
    left.SetSize(2.5);
    left.SetDelayCount(5);
    right.AddBitmap(IDB_P1_RUN0, RGB(0, 0, 0));
    right.AddBitmap(IDB_P1_RUN1, RGB(0, 0, 0));
    right.AddBitmap(IDB_P1_RUN2, RGB(0, 0, 0));
    right.AddBitmap(IDB_P1_RUN3, RGB(0, 0, 0));
    right.AddBitmap(IDB_P1_RUN4, RGB(0, 0, 0));
    right.AddBitmap(IDB_P1_RUN5, RGB(0, 0, 0));
    right.SetSize(2.5);
    right.SetDelayCount(5);
    jumpL.AddBitmap(IDB_P1_JUMP0M, RGB(0, 0, 0));
    jumpL.AddBitmap(IDB_P1_JUMP1M, RGB(0, 0, 0));
    jumpL.AddBitmap(IDB_P1_JUMP2M, RGB(0, 0, 0));
    jumpL.AddBitmap(IDB_P1_JUMP3M, RGB(0, 0, 0));
    jumpL.SetSize(2.5);
    jumpL.SetDelayCount(5);
    jumpR.AddBitmap(IDB_P1_JUMP0, RGB(0, 0, 0));
    jumpR.AddBitmap(IDB_P1_JUMP1, RGB(0, 0, 0));
    jumpR.AddBitmap(IDB_P1_JUMP2, RGB(0, 0, 0));
    jumpR.AddBitmap(IDB_P1_JUMP3, RGB(0, 0, 0));
    jumpR.SetSize(2.5);
    jumpR.SetDelayCount(5);
    width = right.Width();
    height = right.Height();
}

void Player::OnMove()
{
    right.OnMove();
    left.OnMove();
    jumpL.OnMove();
    jumpR.OnMove();

    if (isMovingLeft)
        x -= 8;

    if (isMovingRight)
        x += 8;

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
}

void Player::SetMovingDown(bool flag)
{
    isMovingDown = flag;
}

void Player::SetMovingLeft(bool flag)
{
    if (flag)
        dir = false;

    isMovingLeft = flag;

    if (flag)
        left.SetPause(false);
    else
        left.SetPause(true);
}

void Player::SetMovingRight(bool flag)
{
    if (flag)
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
    x = nx;
    y = ny;
}

void Player::OnShow()
{
    if (dir && !isMovingUp && !isMovingDown)
    {
        right.SetTopLeft(x, y);
        right.OnShow();
    }
    else if (!dir && !isMovingUp && !isMovingDown)
    {
        left.SetTopLeft(x, y);
        left.OnShow();
    }

    if (isMovingUp || isMovingDown)
    {
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
    }
}
}