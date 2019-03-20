#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "object.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Basic object class
/////////////////////////////////////////////////////////////////////////////

Object::Object()
{
    x = y = width = height = currentAni = 0;
    size = 1.0;
}

Object::Object(double s)
{
    x = y = width = height = currentAni = 0;
    size = s;
}

Object::~Object()
{
	for (auto i = bmp_iter.begin();i != bmp_iter.end();i++)
		*i = NULL;
}

bool Object::HitRectangle(int tx1, int ty1, int tx2, int ty2)
{
    int x1 = GetCor(0), y1 = GetCor(1), x2 = GetCor(2), y2 = GetCor(3);
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

int Object::GetCor(int index)
{
    switch (index)
    {
        case 0:
            return x;

        case 1:
            return y;

        case 2:
            return x + (int)(width * size);

        case 3:
            return y + (int)(height * size);

        default:
            return NULL;
    }
}

void Object::LoadBitmap(vector<int>* resource, COLORREF color)
{
    AddCAnimation(resource);
}

void Object::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}

void Object::SetSize(double s)
{
    size = s;
}

void Object::OnShow()
{
    SetAnimationState(currentAni);
    ShowAnimation();
}

void Object::OnMove()
{
}

bool Object::Collision(int file, int tx, int ty, int ox, int oy)
{
    /*double s=1.0;
    ColArray other = cArray.find(file)->second;
    vector<vector<bool>> otherMask = other.pixel;
    if ((tx + ti(height,s) < ox)         // This checks if the sprites
    	|| ty + ti(width, s) < oy // aren't close to each other.
    	|| ox + ti(other.height,s) < tx
    	|| oy + ti(other.width,s) < ty)
    	return false;
    //This stuff isn't working.
    for (int i = 0; i < height; i++) {
    	for (int j = 0; j < width; j++) {
    		for (int k = 0; k < other.height; k++) {
    			for (int l = 0; l < other.width; l++) {
    				if ((tx + i * s) <= (ox + k * s + s) && (tx + i * s + s) >= (ox + k * s)
    					&& (ty + j * s) <= (oy + l * s + s) && (ty + j * s + s) >= (oy + l * s + s)
    					&& array.pixel[i][j] && other.pixel[k][l]) {
    					return true;
    				}
    			}
    		}
    	}
    }*/
    return false;
}

int Object::ShowAnimationState()
{
    return (*bmp_iter[currentAni])[ani[currentAni].GetCurrentBitmapNumber()];
}

void Object::AddCAnimation(vector<int>* list, double size, int delay, bool repeat, int times)
{
    CAnimation temp(repeat, times);

    for (auto i = list->begin(); i != list->end(); i++)
        temp.AddBitmap(*i, RGB(0, 0, 0));

    temp.SetSize(size);
    temp.SetDelayCount(delay);
    ani.push_back(temp);
    bmp_iter.push_back(list);
}

void Object::SetAnimationState(int num)
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

void Object::ShowAnimation()
{
    vector<CAnimation>::iterator ani_iter = ani.begin() + currentAni;
    ani_iter->SetTopLeft(x, y);
    ani_iter->OnShow();
}
}