#ifndef OBJECT_H
#define OBJECT_H
#include "gameComFunc.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// ���� class
	/////////////////////////////////////////////////////////////////////////////

	class Object
	{
	public:
		Object();
		Object(double);
		Object(int,int,int,int);
		void OnShow();											// �N�ϧζK��e��
		void SetXY(int nx, int ny);								// �]�w���󪺮y��
		void SetSize(double s);									// �]�w����j�p
		virtual void OnMove();									// ����
		virtual int  GetCor(int);								// ����y�� 0:���WX, 1:���WY, 2:�k�UX, 3:�k�UY
		virtual void LoadBitmap(int, COLORREF);					// ���J�ϧ�
		virtual bool Collision(CArray, int, int, int, int);
	protected:
		CMovingBitmap bmp;										// ���󪺹�
		int x, y;												// ���󪺮y��
		int width, height;										// ���󪺪��e
		int osX1, osX2, osY1, osY2;								// ����Offset
		double size;											// ���󪺤j�p
		CArray array;											// �I���x�}
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
}

#endif															//define OBJECT_H