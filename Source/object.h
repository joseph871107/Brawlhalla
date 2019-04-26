#ifndef OBJECT_H
#define OBJECT_H
#include "gameComFunc.h"
#include "camera.h"
#include <map>

#define GENERATE_COLLISION_ARRAY false

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Basic object class
/////////////////////////////////////////////////////////////////////////////

class Object
{
    public:
        Object();
        Object(double);
		CPoint GetXY();
        void SetXY(int nx, int ny);								// �]�w���󪺮y��
        void SetSize(double s);									// �]�w����j�p
		virtual int  GetCor(int);								// ����y�� 0:���WX, 1:���WY, 2:�k�UX, 3:�k�UY
		virtual int GetWidth();
		virtual int GetHeight();
		double GetSize();
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
		void AddCamera(Camera *cam);
        virtual void OnShow(int offsetX = 0, int offsetY = 0);									// �N�ϧζK��e��
        virtual void OnMove();									// ����
		virtual void LoadBitmap(int);					// ���J�ϧ�
        virtual void LoadBitmap(int, COLORREF);					// ���J�ϧ�
    protected:
        int x, y;												// ���󪺮y��
        int width, height;										// ���󪺪��e
        double _size;											// ���󪺤j�p
		Camera *camera;
		CMovingBitmap bmp;
};

class Background : public Object
{
public:
	void OnShow(double offset)
	{
		if (camera != nullptr) {
			CPoint cam = camera->GetXY(x, y);
			bmp.SetTopLeft((int)(cam.x * offset), (int)(cam.y * offset));
			bmp.ShowBitmap(_size * (0.9 + 0.1 * camera->GetSize()));
		}
		else {
			bmp.SetTopLeft(x, y);
			bmp.ShowBitmap(_size);
		}
	}
};

}

#endif															//define OBJECT_H