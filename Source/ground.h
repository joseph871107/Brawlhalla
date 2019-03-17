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
		void OnShow();											// 將圖形貼到畫面
		void LoadBitmap();										// 載入圖形
		bool Collision(ColArray*, double, int, int, int, int);
	private:
		CMovingBitmap bmp;
		int length;												// 長度
		double size;											// 大小
		ColArray array;											// 碰撞矩陣
		int bmpID;
	};
}

#endif //define GROUND_H