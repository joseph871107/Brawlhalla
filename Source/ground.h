#pragma   once
#ifndef GROUND_H
#define GROUND_H
#include "object.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �a�� class
	/////////////////////////////////////////////////////////////////////////////

	class Ground:public Object
	{
	public:
		Ground();
		int  GetCor(int);										// ����y�� 0:���WX, 1:���WY, 2:�k�UX, 3:�k�UY
		void SetLen(int len);									// �]�w����
		void OnShow();											// �N�ϧζK��e��
		void LoadBitmap();										// ���J�ϧ�
		bool Collision(ColArray*, double, int, int, int, int);
	private:
		CMovingBitmap bmp;
		int length;												// ����
		double size;											// �j�p
		ColArray array;											// �I���x�}
		int bmpID;
	};
}

#endif //define GROUND_H