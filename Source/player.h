#pragma   once
#ifndef PLAYER_H
#define PLAYER_H
#include "ground.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// �a�� class
/////////////////////////////////////////////////////////////////////////////

class Player
{
    public:
        Player(Ground* grd);			// Constructor
        ~Player();						// Deconstructor
        bool HitObject();				// �O�_�I��a��
        bool IsOnGround();				//
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
        void Jump(double = 20);			// ���_
        ColArray array;					// �I���x�}
        int ShowAnimationState();		// Return which CAnimation is playing
        void SetAnimationState(int);	// Set which CAnimation is going to play
        void ShowAnimation();			// Show CAnimation by currentAni
        void AddCAnimation(vector<int>*, double = 1.0, int = 10, bool = true, int = 1);	// Push (bmps, (optional)size, (op)delay, (op)repeat, (op)repeat times) in vector of CAnimation
    private:
        vector<CAnimation> ani;			// vector of CAnimation
        int currentAni;					// current running CAnimation
        vector<int> rr{ IDB_P1_RUN0, IDB_P1_RUN1, IDB_P1_RUN2, IDB_P1_RUN3, IDB_P1_RUN4, IDB_P1_RUN5 };			// bmps of running right
        vector<int> rl{ IDB_P1_RUN0M, IDB_P1_RUN1M, IDB_P1_RUN2M, IDB_P1_RUN3M, IDB_P1_RUN4M, IDB_P1_RUN5M };	// bmps of running left
        vector<int> jr{ IDB_P1_JUMP0, IDB_P1_JUMP1, IDB_P1_JUMP2, IDB_P1_JUMP3 };								// bmps of jumping right
        vector<int> jl{ IDB_P1_JUMP0M, IDB_P1_JUMP1M, IDB_P1_JUMP2M, IDB_P1_JUMP3M };							// bmps of jumping left
        vector<int> sr{ IDB_P1_IDLE0, IDB_P1_IDLE1, IDB_P1_IDLE2};												// bmps of standing right
        vector<int> sl{ IDB_P1_IDLE0M, IDB_P1_IDLE1M, IDB_P1_IDLE2M};											// bmps of standing left
        vector<vector<int>*> bmp_iter;
        int x, y;					// ���a���y��
        int width, height;			// ���a�����e
        double velocity;			// �b�Ť����d�ɶ�
        bool dir;					// ���a��V false�V�� true�V�k
        bool isMovingDown;			// �O�_���b���U����
        bool isMovingLeft;			// �O�_���b��������
        bool isMovingRight;			// �O�_���b���k����
        bool isMovingUp;			// �O�_���b���W����
        int jumpCount;											// limit of jump times
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
        Ground* ground;
};
}

#endif //define PLAYER_H