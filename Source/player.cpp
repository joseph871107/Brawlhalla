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
    x = y = width = height = currentAni = 0;
    jumpCount = 2;
    velocity = 0.0;
    isMovingLeft = isMovingRight = isMovingUp = dir = false;
    isMovingDown = true;
    ground = grd;
}

bool Player::HitObject()
{
    int x1 = ground->GetCor(0), y1 = ground->GetCor(1), x2 = ground->GetCor(2), y2 = ground->GetCor(3);
    // 檢測玩家所構成的矩形是否碰到地
    bool flag = HitRectangle(x1, y1, x2, y2);

    if (flag)
        velocity = 0.0;

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
    if (jumpCount > 0)
    {
        velocity = -acc;
        y -= (int)acc;
        jumpCount--;
        ani[2].Reset();
        ani[3].Reset();
    }
}

int Player::ShowAnimationState()
{
    return (*bmp_iter[currentAni])[ani[currentAni].GetCurrentBitmapNumber()];
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
    return x + width;
}

int Player::GetY2()
{
    return y + height;
}

void Player::AddCAnimation(vector<int>* list, double size, int delay, bool repeat, int times)
{
    CAnimation temp(repeat, times);

    for (auto i = list->begin(); i != list->end(); i++)
        temp.AddBitmap(*i, RGB(0, 0, 0));

    temp.SetSize(size);
    temp.SetDelayCount(delay);
    ani.push_back(temp);
    bmp_iter.push_back(list);
}

void Player::LoadBitmap()
{
    AddCAnimation(&rl, 2.5, 5);
    AddCAnimation(&rr, 2.5, 5);
    AddCAnimation(&jl, 2.5, 5, false);
    AddCAnimation(&jr, 2.5, 5, false);
    AddCAnimation(&sl, 2.5, 7);
    AddCAnimation(&sr, 2.5, 7);
    int sizeOfWhich = 0;
    width = ani[sizeOfWhich].Width();
    height = ani[sizeOfWhich].Height();
}

void Player::OnMove()
{
    for (auto i = ani.begin(); i != ani.end(); i++)
        i->OnMove();

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

        jumpCount = 2;
        ani[2].Reset();
        ani[3].Reset();
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
}

void Player::SetMovingRight(bool flag)
{
    if (flag)
        dir = true;

    isMovingRight = flag;
}

void Player::SetMovingUp(bool flag)
{
    isMovingUp = flag;
}

void Player::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}

void Player::SetAnimationState(int num)
{
    currentAni = num;

    for (auto i = ani.begin(); i != ani.end(); i++)
    {
        if (i == ani.begin() + num)
            i->SetPause(false);
        else
            i->SetPause(true);
    }
}

void Player::ShowAnimation()
{
    vector<CAnimation>::iterator ani_iter = ani.begin() + currentAni;
    ani_iter->SetTopLeft(x, y);
    ani_iter->OnShow();
}

void Player::OnShow()
{
    if (HitObject())
    {
        if (isMovingLeft || isMovingRight)
        {
            if (dir)
                SetAnimationState(1);
            else
                SetAnimationState(0);
        }
        else
        {
            if (dir)
                SetAnimationState(5);
            else
                SetAnimationState(4);
        }
    }
    else
    {
        if (dir)
            SetAnimationState(3);
        else
            SetAnimationState(2);
    }

    ShowAnimation();
}
}