#pragma   once
#ifndef GROUND_H
#define GROUND_H
#include "object.h"

namespace game_framework
{

	class GroundPARM {
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
		void SetLen(int len, int dimension = 0);				// �]�w���� (dimension == 0 : x, dimension == 1 : y)
        void OnShow();											// �N�ϧζK��e��
        int  GetCor(int);										// ����y�� 0:���WX, 1:���WY, 2:�k�UX, 3:�k�UY
		void SetID(int id);
		void SetLenOffset(int offset, int dimension = 0);
		void SetOffset(int i);
        void LoadBitmap();										// ���J�ϧ�
    private:
		void _UpdateWH();
		int _lengthX;												// ����
		int _lengthY;
        int _bmpID;
		int _lenOffsetX;
		int _lenOffsetY;
        COLORREF _color;
        int osX1, osY1, osX2, osY2;
};
}

#endif //define GROUND_H