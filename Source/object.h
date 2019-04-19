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
        void SetXY(int nx, int ny);								// 設定物件的座標
        void SetSize(double s);									// 設定物件大小
        int  GetCor(int);										// 物件座標 0:左上X, 1:左上Y, 2:右下X, 3:右下Y
		int GetWidth();
		int GetHeight();
		double GetSize();
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
		void AddCamera(Camera *cam);
        virtual void OnShow();									// 將圖形貼到畫面
        virtual void OnMove();									// 移動
		virtual void LoadBitmap(int);					// 載入圖形
        virtual void LoadBitmap(int, COLORREF);					// 載入圖形
    protected:
        int x, y;												// 物件的座標
        int width, height;										// 物件的長寬
        double _size;											// 物件的大小
		Camera *camera;
		CMovingBitmap bmp;
};
}

#endif															//define OBJECT_H