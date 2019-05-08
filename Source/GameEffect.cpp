/* File description:
This is the source code file 'TriggeredAnimation.cpp'. This is the implementation for the class 'TriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
//
#include "GameEffect.h"

namespace game_framework
{
//-----------------CONSTANTS DEFINITIONS-----------------//
const double BITMAP_SIZE = 0.5;

//-----------------FUNCTIONS DEFINITIONS-----------------//
GameEffect::~GameEffect()
{
    /* Body intentionally empty */
}

GameEffect::GameEffect() : GameEffect(int(), int(), vector<CAnimation>(), int(), nullptr)
{
    /* Body intentionally empty */
}

GameEffect::GameEffect(const GameEffect& objectValue) : GameEffect(objectValue._x, objectValue._y, objectValue._anis, objectValue._currentAni, objectValue._cameraPtr)
{
    /* Body intentionally empty */
}

GameEffect::GameEffect(const int& xValue, const int& yValue, const vector<CAnimation>& anisValue, const int& currentAniValue, Camera* const& cameraPtrValue) : _x(xValue), _y(yValue), _anis(anisValue), _currentAni(currentAniValue), _cameraPtr(cameraPtrValue)
{
    /* Body intentionally empty */
}

GameEffect& GameEffect::operator=(const GameEffect& rightObject)
{
    if (this != &rightObject)
    {
        _x = rightObject._x;
        _y = rightObject._y;
        _anis = rightObject._anis;
        _currentAni = rightObject._currentAni;
        _cameraPtr = rightObject._cameraPtr;
    }

    return (*this);
}

void GameEffect::Initialize()
{
    /* Do nothing */
}

void GameEffect::LoadBitmap()
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

void GameEffect::OnMove()
{
    _anis[_currentAni].OnMove();
}

void GameEffect::OnShow()
{
    CPoint cam = _cameraPtr->GetXY(_x, _y );
    _anis[_currentAni].SetSize(BITMAP_SIZE * _cameraPtr->GetSize());
    _anis[_currentAni].SetTopLeft(_x, _y);
    _anis[_currentAni].OnShow();
}

void GameEffect::SetXY(const int& newX, const int& newY)
{
    _x = newX;
    _y = newY;
}

void GameEffect::SetCurrentAni(const int& newCurrentAni)
{
    _currentAni = newCurrentAni;
}

void GameEffect::AddCAnimation(vector<int>* list, double size, int delay, bool repeat, int times)
//void AddCAnimation(vector<int>*, double = 1.0, int = 10, bool = true, int = 1);
{
    CAnimation temp(repeat, times);

    for (auto i = list->begin(); i != list->end(); i++)
        temp.AddBitmap(*i, RGB(0, 0, 0));

    temp.SetSize(size);
    temp.SetDelayCount(delay);
    _anis.push_back(temp);
}

bool GameEffect::IsCurrentAniFinalBitmap()
{
    return (_anis[_currentAni].IsFinalBitmap());
}

double GameEffect::GetCurrentAnimationHeight()
{
    return (_anis[_currentAni].Height() * BITMAP_SIZE);
}

double GameEffect::GetCurrentAnimationWidth()
{
    return (_anis[_currentAni].Width() * BITMAP_SIZE);
}

const int& GameEffect::GetCurrentAni() const
{
    return (_currentAni);
}

void GameEffect::AddCamera(Camera* camcameraPtrValue)
{
    _cameraPtr = camcameraPtrValue;
}

}