#pragma   once
#ifndef GROUND_H
#define GROUND_H
#include "object.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Ground class
/////////////////////////////////////////////////////////////////////////////

class Ground: public Object
{
    public:
        Ground();
        void SetLen(int len);									// 設定長度
        void OnShow();											// 將圖形貼到畫面
        int  GetCor(int);										// 物件座標 0:左上X, 1:左上Y, 2:右下X, 3:右下Y
        void LoadBitmap();										// 載入圖形
    private:
        CMovingBitmap bmp;
        int _length;												// 長度
        int _bmpID;
        COLORREF _color;
        int osX1, osY1, osX2, osY2;
};
}

#endif //define GROUND_H