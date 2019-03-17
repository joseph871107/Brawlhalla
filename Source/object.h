#ifndef OBJECT_H
#define OBJECT_H
#include "gameComFunc.h"
#include <map>

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// 物件 class
/////////////////////////////////////////////////////////////////////////////

	class Object
	{
	public:
		Object();
		Object(double);
		Object(int,int,int,int);
		void OnShow(int);										// 將圖形貼到畫面
		void SetXY(int nx, int ny);								// 設定物件的座標
		void SetSize(double s);									// 設定物件大小
		void OnMove();											// 移動
		int  GetCor(int);										// 物件座標 0:左上X, 1:左上Y, 2:右下X, 3:右下Y
		void LoadBitmap(int, COLORREF);							// 載入圖形
		bool Collision(int, int, int, int, int);
		int ShowAnimationState();
	protected:
		CMovingBitmap bmp;
		int x, y;												// 物件的座標
		int width, height;										// 物件的長寬
		int osX1, osX2, osY1, osY2;								// 物件的Offset
		double size;											// 物件的大小
		ColArray array;											// 碰撞矩陣
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
}

#endif															//define OBJECT_H