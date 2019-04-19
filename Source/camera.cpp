#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "camera.h"

namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// Camera class
	/////////////////////////////////////////////////////////////////////////////

	Camera::Camera()
	{
		x = SIZE_X / 2; y = SIZE_Y / 2;
		size = 1;
	}
	Camera::Camera(int tx, int ty)
	{
		x = tx; y = ty;
		size = 1;
	}
	void Camera::SetSize(double s)
	{
		size = s;
	}
	void Camera::SetCameraXY(int tx, int ty)
	{
		x = tx;
		y = ty;
	}
	double Camera::GetSize()
	{
		return size;
	}
	CPoint Camera::GetXY(int tx, int ty)
	{
		CPoint temp;
		temp.x = (int)(x + (tx - x) * size);
		temp.y = (int)(y + (ty - y) * size);
		return temp;
	}
	CPoint Camera::GetCameraXY()
	{
		CPoint temp;
		temp.x = x;
		temp.y = y;
		return temp;
	}
}