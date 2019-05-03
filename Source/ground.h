#pragma   once
#ifndef GROUND_H
#define GROUND_H
#include "object.h"

namespace game_framework
{

class GroundPARM
{
    public:
        GroundPARM(int x = 0, int y = 0, double size = 1, int lengthX = 1, int lengthY = 1, int id = IDB_GROUND1, int offset = 2, int lenOffsetX = 100, int lenOffsetY = 100);
        CPoint point;
        double _size;
        int _lengthX, _lengthY, _id, _offset, _lenOffsetX, _lenOffsetY;

};

/////////////////////////////////////////////////////////////////////////////
// Ground class
/////////////////////////////////////////////////////////////////////////////

class Ground: public Object
{
    public:
        Ground();
        void SetLen(int len, int dimension = 0);				// 設定長度 (dimension == 0 : x, dimension == 1 : y)
        void OnShow();											// 將圖形貼到畫面
        int  GetCor(int);										// 物件座標 0:左上X, 1:左上Y, 2:右下X, 3:右下Y
        void SetID(int id);
        void SetLenOffset(int offset, int dimension = 0);
        void SetOffset(int i);
        void LoadBitmap();										// 載入圖形

        // Interaction with Ground
        bool IsIntersectGround(int objectX1, int objectY1, int objectX2, int objectY2);
        bool IsExplicitlyVerticallyIntersectGround(int objectX1, int objectY1, int objectX2, int objectY2);
        bool IsExplicitlyHorizontallyIntersectGround(int objectX1, int objectY1, int objectX2, int objectY2);

        bool IsOnGroundLeftEdge(int objectX1, int objectY1, int objectX2, int objectY2);
        bool IsOnGroundRightEdge(int objectX1, int objectY1, int objectX2, int objectY2);
        bool IsOnGroundUnderside(int objectX1, int objectY1, int objectX2, int objectY2);
        bool IsOnGround(int objectX1, int objectY1, int objectX2, int objectY2);

    private:
        void _UpdateWH();
        int _lengthX;												// 長度
        int _lengthY;
        int _bmpID;
        int _lenOffsetX;
        int _lenOffsetY;
        COLORREF _color;
        int osX1, osY1, osX2, osY2;
};
}

#endif //define GROUND_H