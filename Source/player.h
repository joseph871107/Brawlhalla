#pragma   once
#ifndef PLAYER_H
#define PLAYER_H
#include "ground.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �a�� class
	/////////////////////////////////////////////////////////////////////////////

	class Player
	{
	public:
		Player(Ground *grd);
		bool HitObject();				// �O�_�I��a��
		bool IsOnGround();
		void LoadBitmap();				// ���J�ϧ�
		void OnMove();					// ����
		void OnShow();					// �N�ϧζK��e��
		void SetXY(int nx, int ny);		// �]�w���a���y��
		int  GetX1();					// ���a���W�� x �y��
		int  GetY1();					// ���a���W�� y �y��
		int  GetX2();					// ���a�k�U�� x �y��
		int  GetY2();					// ���a�k�U�� y �y��
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void Jump(double acc);
		ColArray array;					// �I���x�}
		int ShowAnimationState();
		list<CAnimation> ani;			// list of CMovingBitmap
		list<CAnimation>::iterator	ani_iter;
	protected:
		CAnimation left;			// ���a�V����
		CAnimation right;			// ���a�V�k��
		CAnimation jumpL;			// ���a���D
		CAnimation jumpR;			// ���a���D
		int rr[6] = { IDB_P1_RUN0, IDB_P1_RUN1, IDB_P1_RUN2, IDB_P1_RUN3, IDB_P1_RUN4, IDB_P1_RUN5 };
		int rl[6] = { IDB_P1_RUN0M, IDB_P1_RUN1M, IDB_P1_RUN2M, IDB_P1_RUN3M, IDB_P1_RUN4M, IDB_P1_RUN5M };
		int jr[6] = { IDB_P1_JUMP0, IDB_P1_JUMP1, IDB_P1_JUMP2, IDB_P1_JUMP3 };
		int jl[6] = { IDB_P1_JUMP0M, IDB_P1_JUMP1M, IDB_P1_JUMP2M, IDB_P1_JUMP3M };
		int x, y;					// ���a���y��
		int width, height;			// ���a�����e
		double velocity;			// �b�Ť����d�ɶ�
		bool onGround;
		bool dir;					// ���a��V false�V�� true�V�k
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
	private:
		int jumpCount;
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
		Ground *ground;
	};
}

#endif //define PLAYER_H