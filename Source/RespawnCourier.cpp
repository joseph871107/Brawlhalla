/* File description:
This is the source code file 'RespawnCourier.cpp'. This is the implementation for the class 'RespawnCourier'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
//
#include "camera.h"
#include "RespawnCourier.h"

namespace game_framework
{
RespawnCourier::~RespawnCourier()
{
    /* Body intentionally empty */
}

RespawnCourier::RespawnCourier() : RespawnCourier(int(), int(), double(), CMovingBitmap(), CMovingBitmap(), nullptr, bool())
{
    /* Body intentionally empty */
}

RespawnCourier::RespawnCourier(const RespawnCourier& objectValue) : RespawnCourier(objectValue._x, objectValue._y, objectValue._size, objectValue._bmpLeft, objectValue._bmpRight, objectValue._cameraPtr, objectValue._dir)
{
    /* Body intentionally empty */
}

RespawnCourier::RespawnCourier(const int& xValue, const int& yValue, const double& sizeValue, const CMovingBitmap& bmpLeftValue, const CMovingBitmap& bmpRightValue, Camera* const cameraPtrValue, const bool& dirValue) : _x(xValue), _y(yValue), _size(sizeValue), _bmpLeft(bmpLeftValue), _bmpRight(bmpRightValue), _cameraPtr(cameraPtrValue), _dir(dirValue)
{
    /* Body intentionally empty */
}

RespawnCourier& RespawnCourier::operator=(const RespawnCourier& rightObject)
{
    if (this != &rightObject)
    {
        _x = rightObject._x;
        _y = rightObject._y;
        _size = rightObject._size;
        _bmpLeft = rightObject._bmpLeft;
        _bmpRight = rightObject._bmpRight;
        _cameraPtr = rightObject._cameraPtr;
        _dir = rightObject._dir;
    }

    return (*this);
}

void RespawnCourier::Initialize(Camera* const& cameraPtrValue)
{
    _x = -1000;
    _y = -1000;
    _size = 1.0;
    _cameraPtr = cameraPtrValue;
    _dir = false;
}

void RespawnCourier::LoadBitmap()
{
    _bmpLeft.LoadBitmap(IDB_RESPAWN_COURIER_LEFT, RGB(0, 0, 0));
    _bmpRight.LoadBitmap(IDB_RESPAWN_COURIER_RIGHT, RGB(0, 0, 0));
}

void RespawnCourier::OnShow()
{
    CPoint cam = _cameraPtr->GetXY(_x, _y);

    if (!_dir) // left
    {
        _bmpLeft.SetTopLeft(cam.x, cam.y);
        _bmpLeft.ShowBitmap(_size * _cameraPtr->GetSize());
    }
    else // right
    {
        _bmpRight.SetTopLeft(cam.x, cam.y);
        _bmpRight.ShowBitmap(_size * _cameraPtr->GetSize());
    }
}

void RespawnCourier::SetXY(const int& newX, const int& newY)
{
    _x = newX;
    _y = newY;
}

double RespawnCourier::GetWidth()
{
    return (_bmpLeft.Width());
}

const int& RespawnCourier::GetX() const
{
    return (_x);
}

const int& RespawnCourier::GetY() const
{
    return (_y);
}

void RespawnCourier::SetDir(const bool& newDir)
{
    _dir = newDir;
}

}