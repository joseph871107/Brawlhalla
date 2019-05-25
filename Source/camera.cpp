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
const double CAMERA_SIZING_OFFSET = 0.005;
const double CAMERA_ZOOMOUT = 1;
const double CAMERA_ZOOMIN = 0.75;

Camera::Camera()
{
    x = SIZE_X / 2;
    y = SIZE_Y / 2;
    offsetX = offsetY = 0;
    size = 1;
    gradual = false;
}
Camera::Camera(int tx, int ty)
{
    x = tx;
    y = ty;
    offsetX = offsetY = 0;
    size = 1;
    gradual = false;
}
void Camera::SetSize(double s)
{
    if (gradual)
    {
        static double gradient = s;

        if (abs(gradient - s) >= CAMERA_SIZING_OFFSET)
        {
            if (gradient > s)
                gradient -= CAMERA_SIZING_OFFSET * CAMERA_ZOOMOUT;
            else
                gradient += CAMERA_SIZING_OFFSET * CAMERA_ZOOMIN;
        }

        size = gradient;
    }
    else
        size = s;
}
void Camera::SetCameraXY(int tx, int ty)
{
	if (gradual)
	{
		static double gradientX = tx;

		if (abs(gradientX - tx) >= CAMERA_SIZING_OFFSET)
		{
			if (gradientX > tx)
				gradientX -= CAMERA_SIZING_OFFSET * CAMERA_ZOOMOUT;
			else
				gradientX += CAMERA_SIZING_OFFSET * CAMERA_ZOOMIN;
		}

		x = (int)gradientX;
	}
	else
		x = tx;
	if (gradual)
	{
		static double gradientY = ty;

		if (abs(gradientY - tx) >= CAMERA_SIZING_OFFSET)
		{
			if (gradientY > tx)
				gradientY -= CAMERA_SIZING_OFFSET * CAMERA_ZOOMOUT;
			else
				gradientY += CAMERA_SIZING_OFFSET * CAMERA_ZOOMIN;
		}

		y = (int)gradientY;
	}
	else
		y = ty;
}
void Camera::SetCameraOffsetXY(int tx, int ty)
{
    offsetX = tx;
    offsetY = ty;
}
void Camera::SetGradual(bool grad)
{
    gradual = grad;
}
double Camera::GetSize()
{
    return size;
}
CPoint Camera::GetXY(int tx, int ty)
{
    return CPoint ((int)((tx - x) * size + SIZE_X / 2 + offsetX), (int)((ty - y) * size + SIZE_Y / 2 + offsetY));
}
CPoint Camera::GetCameraXY()
{
    CPoint temp;
    temp.x = x;
    temp.y = y;
    return temp;
}
void Camera::Reset()
{
    x = SIZE_X / 2;
    y = SIZE_Y / 2;
    offsetX = offsetY = 0;
    size = 1;
    gradual = false;
}
}