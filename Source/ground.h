#pragma   once
#ifndef GROUND_H
#define GROUND_H
#include "object.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 地圖 class
	/////////////////////////////////////////////////////////////////////////////

	class Ground:public Object
	{
	public:
		Ground();
		int  GetCor(int);										// 物件座標 0:左上X, 1:左上Y, 2:右下X, 3:右下Y
		void SetLen(int len);									// 設定長度
		void OnMove();											// 移動
		void OnShow();											// 將圖形貼到畫面
		void LoadBitmap();										// 載入圖形
		void SetSize(double s);									// 設定物件大小
		bool Collision(CArray*, int, int, int, int);
	private:
		int length;												// 長度
		double size;											// 大小
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
		CArray array;											// 碰撞矩陣
	};
}

#endif //define GROUND_H