
#include "ground.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �a�� class
	/////////////////////////////////////////////////////////////////////////////

	class Player
	{
	public:
		Player();
		bool HitObject(Ground *ground);							// �O�_�I��a��
		void LoadBitmap();										// ���J�ϧ�
		void OnMove(Ground *ground);											// ����
		void OnShow();											// �N�ϧζK��e��
		void SetXY(int nx, int ny);								// �]�w���a���y��
		int  GetX1();					// ���a���W�� x �y��
		int  GetY1();					// ���a���W�� y �y��
		int  GetX2();					// ���a�k�U�� x �y��
		int  GetY2();					// ���a�k�U�� y �y��
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
	protected:
		CAnimation front;		// ���a���ʵe	
		CAnimation back;
		int x, y;					// ���a���y��
		int width, height;			// ���a�����e
		bool dir;					// ���a��V false�V�� true�V�k
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
	private:
		bool* HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
}