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
        void SetLen(int len);									// �]�w����
        void OnShow();											// �N�ϧζK��e��
        int  GetCor(int);										// ����y�� 0:���WX, 1:���WY, 2:�k�UX, 3:�k�UY
        void LoadBitmap();										// ���J�ϧ�
    private:
        CMovingBitmap bmp;
        int _length;												// ����
        int _bmpID;
        COLORREF _color;
        int osX1, osY1, osX2, osY2;
};
}

#endif //define GROUND_H