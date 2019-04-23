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
        void SetLen(int len);									// �]�w����
        void OnShow();											// �N�ϧζK��e��
        int  GetCor(int);										// ����y�� 0:���WX, 1:���WY, 2:�k�UX, 3:�k�UY
        void LoadBitmap();										// ���J�ϧ�
    private:
        int _length;												// ����
        int _bmpID;
        COLORREF _color;
        int osX1, osY1, osX2, osY2;
};
}

#endif //define GROUND_H