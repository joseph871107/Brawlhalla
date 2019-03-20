#pragma   once
#ifndef GROUND_H
#define GROUND_H
#include "object.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// 地圖 class
/////////////////////////////////////////////////////////////////////////////

class Ground: public Object
{
    public:
        Ground();
        void SetLen(int len);									// 設定長度
        void OnShow();											// 將圖形貼到畫面
		int  GetCor(int);										// 物件座標 0:左上X, 1:左上Y, 2:右下X, 3:右下Y
        void LoadBitmap();										// 載入圖形
        bool Collision(ColArray*, double, int, int, int, int);
    private:
        CMovingBitmap bmp;
        int _length;												// 長度
        int _bmpID;
		int osX1,osY1,osX2,osY2;
		ColArray array;
};
}

#endif //define GROUND_H