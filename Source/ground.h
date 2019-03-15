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
		void OnMove();											// ����
		void OnShow();											// �N�ϧζK��e��
		void LoadBitmap();										// ���J�ϧ�
		void SetSize(double s);									// �]�w����j�p
		bool Collision(CArray*, int, int, int, int);
	private:
		int length;												// ����
		double size;											// �j�p
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
		CArray array;											// �I���x�}
	};
}

#endif //define GROUND_H