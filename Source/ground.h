#pragma   once
#ifndef GROUND_H
#define GROUND_H
#include "object.h"

namespace game_framework
{

	struct GroundPARM {
		GroundPARM(int x, int y, double size, int length)
		{
			point = CPoint(x, y);
			_size = size;
			_length = length;
		}
		CPoint point = CPoint(0, 0);
		double _size = 1.0;
		int _length = 1;

	};

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
        int _length;												// 長度
        int _bmpID;
        COLORREF _color;
        int osX1, osY1, osX2, osY2;
};
}

#endif //define GROUND_H