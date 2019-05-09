/* File description:
This is the source code file 'TriggeredAnimation.cpp'. This is the implementation for the class 'TriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
//
#include "ExplosionEffect.h"

namespace game_framework
{
//-----------------CONSTANTS DEFINITIONS-----------------//
const double BITMAP_SIZE = 0.5;

//-----------------FUNCTIONS DEFINITIONS-----------------//
ExplosionEffect::~ExplosionEffect()
{
    /* Body intentionally empty */
}

ExplosionEffect::ExplosionEffect() : ExplosionEffect(int(), int(), vector<CAnimation>(), int(), nullptr, bool())
{
    /* Body intentionally empty */
}

ExplosionEffect::ExplosionEffect(const ExplosionEffect& objectValue) : ExplosionEffect(objectValue._x, objectValue._y, objectValue._anis, objectValue._currentAni, objectValue._cameraPtr, objectValue._isTrigger)
{
    /* Body intentionally empty */
}

ExplosionEffect::ExplosionEffect(const int& xValue, const int& yValue, const vector<CAnimation>& anisValue, const int& currentAniValue, Camera* const& cameraPtrValue, const bool& isTriggerValue) : _x(xValue), _y(yValue), _anis(anisValue), _currentAni(currentAniValue), _cameraPtr(cameraPtrValue), _isTrigger(isTriggerValue)
{
    /* Body intentionally empty */
}

ExplosionEffect& ExplosionEffect::operator=(const ExplosionEffect& rightObject)
{
    if (this != &rightObject)
    {
        _x = rightObject._x;
        _y = rightObject._y;
        _anis = rightObject._anis;
        _currentAni = rightObject._currentAni;
        _cameraPtr = rightObject._cameraPtr;
		_isTrigger = rightObject._isTrigger;
    }

    return (*this);
}

void ExplosionEffect::LoadBitmap()
{
    vector<int> exploUp = vector<int> { IDB_FX_EXPLO_UP0, IDB_FX_EXPLO_UP1, IDB_FX_EXPLO_UP2, IDB_FX_EXPLO_UP3 };
    vector<int> exploRight = vector<int> { IDB_FX_EXPLO_RIGHT0, IDB_FX_EXPLO_RIGHT1, IDB_FX_EXPLO_RIGHT2, IDB_FX_EXPLO_RIGHT3 };
    vector<int> exploDown = vector<int> { IDB_FX_EXPLO_DOWN0, IDB_FX_EXPLO_DOWN1, IDB_FX_EXPLO_DOWN2, IDB_FX_EXPLO_DOWN3 };
    vector<int> exploLeft = vector<int> { IDB_FX_EXPLO_LEFT0, IDB_FX_EXPLO_LEFT1, IDB_FX_EXPLO_LEFT2, IDB_FX_EXPLO_LEFT3 };
    //
    AddCAnimation(&exploUp, BITMAP_SIZE); //_anis[0] Explosion Up
    AddCAnimation(&exploRight, BITMAP_SIZE); //_anis[1] Explosion Up
    AddCAnimation(&exploDown, BITMAP_SIZE); //_anis[2] Explosion Up
    AddCAnimation(&exploLeft, BITMAP_SIZE); //_anis[3] Explosion Up
}

void ExplosionEffect::OnMove()
{
    if (_isTrigger) // Animation only display when triggered
        _anis[_currentAni].OnMove();

    if (IsCurrentAniFinalBitmap()) // If the explosion effect finishes its showcase, then stop it
        SetIsTrigger(false);
}

void ExplosionEffect::OnShow()
{
    if (_isTrigger) // Animation only display when triggered
    {
        CPoint cam = _cameraPtr->GetXY(_x, _y);
        _anis[_currentAni].SetSize(BITMAP_SIZE * _cameraPtr->GetSize());
        _anis[_currentAni].SetTopLeft(_x, _y);
        _anis[_currentAni].OnShow();
    }
}

void ExplosionEffect::SetXY(const int& newX, const int& newY)
{
    _x = newX;
    _y = newY;
}

void ExplosionEffect::SetCurrentAni(const int& newCurrentAni)
{
    _currentAni = newCurrentAni;
}

void ExplosionEffect::AddCAnimation(vector<int>* list, double size, int delay, bool repeat, int times)
//void AddCAnimation(vector<int>*, double = 1.0, int = 10, bool = true, int = 1);
{
    CAnimation temp(repeat, times);

    for (auto i = list->begin(); i != list->end(); i++)
        temp.AddBitmap(*i, RGB(0, 0, 0));

    temp.SetSize(size);
    temp.SetDelayCount(delay);
    _anis.push_back(temp);
}

bool ExplosionEffect::IsCurrentAniFinalBitmap()
{
    return (_anis[_currentAni].IsFinalBitmap());
}

double ExplosionEffect::GetCurrentAnimationHeight()
{
    return (_anis[_currentAni].Height() * BITMAP_SIZE);
}

double ExplosionEffect::GetCurrentAnimationWidth()
{
    return (_anis[_currentAni].Width() * BITMAP_SIZE);
}

const int& ExplosionEffect::GetCurrentAni() const
{
    return (_currentAni);
}

void ExplosionEffect::AddCamera(Camera* camcameraPtrValue)
{
    _cameraPtr = camcameraPtrValue;
}

void ExplosionEffect::SetIsTrigger(bool newIsTrigger)
{
    _isTrigger = newIsTrigger;
}

const bool& ExplosionEffect::GetIsTrigger() const {
	return(_isTrigger);
}

}