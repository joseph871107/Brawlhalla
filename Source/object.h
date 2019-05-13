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
        void SetXY(int nx, int ny);								// 設定物件的座標
        void SetSize(double s);									// 設定物件大小
		virtual int  GetCor(int);								// 物件座標 0:左上X, 1:左上Y, 2:右下X, 3:右下Y
		virtual int GetWidth();
		virtual int GetHeight();
		double GetSize();
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
		void AddCamera(Camera *cam);
        virtual void OnShow(int offsetX = 0, int offsetY = 0);	// 將圖形貼到畫面
        virtual void OnMove();									// 移動
		virtual void LoadBitmap(int);							// 載入圖形
		virtual void LoadBitmap(int, COLORREF);					// 載入圖形
		virtual void LoadBitmap(char*, RECT, COLORREF);					// 載入圖形
    protected:
        int x, y, loaded = 0;												// 物件的座標
        int width, height;										// 物件的長寬
        double _size;											// 物件的大小
		Camera *camera;
		CMovingBitmap bmp;
};

struct BkPARM {
	BkPARM(int x, int y, double size, double offset, int id)
	{
		point = CPoint(x, y);
		_size = size;
		_offset = offset;
		_id = id;
	}
	CPoint point = CPoint(0, 0);
	double _size = 1.0, _offset = 1;
	int _id = IDB_BACKGROUND1;

};

class Background : public Object
{
public:
	void OnShow()
	{
		if (loaded > 0) {
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
	}
	void SetOffset(double _offset) {
		offset = _offset;
	}
private:
	double offset = 1;
};

}

#endif															//define OBJECT_H