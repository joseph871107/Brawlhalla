#pragma   once
#ifndef CAMERA_H
#define CAMERA_H

namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// Camera class
	/////////////////////////////////////////////////////////////////////////////

	class Camera
	{
	public:
		Camera();
		Camera(int tx, int ty);
		void SetSize(double);
		void SetCameraXY(int tx, int ty);
		void SetCameraOffsetXY(int tx, int ty);
		void SetGradual(bool);
		double GetSize();
		CPoint GetXY(int tx, int ty);
		CPoint GetCameraXY();
	private:
		int x, y, offsetX, offsetY;
		double size;
		bool gradual;
	};
}

#endif //define CAMERA_H