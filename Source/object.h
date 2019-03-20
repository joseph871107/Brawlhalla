#ifndef OBJECT_H
#define OBJECT_H
#include "gameComFunc.h"
#include <map>

#define GENERATE_COLLISION_ARRAY false

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// ���� class
/////////////////////////////////////////////////////////////////////////////

class Object
{
    public:
        Object();
        Object(double);
		Object& operator=(const Object& rightObject);
		~Object();
        void OnShow();											// �N�ϧζK��e��
        void SetXY(int nx, int ny);								// �]�w���󪺮y��
        void SetSize(double s);									// �]�w����j�p
        void OnMove();											// ����
        int  GetCor(int);										// ����y�� 0:���WX, 1:���WY, 2:�k�UX, 3:�k�UY
        void LoadBitmap(vector<int>*, COLORREF);				// ���J�ϧ�
        bool Collision(int, int, int, int, int);
        int ShowAnimationState();		// Return which CAnimation is playing
        void SetAnimationState(int);	// Set which CAnimation is going to play
        void ShowAnimation();			// Show CAnimation by currentAni
        void AddCAnimation(vector<int>*, double = 1.0, int = 5, bool = true, int = 1);	// Push (bmps, (optional)size, (op)delay, (op)repeat, (op)repeat times) in vector of CAnimation
    protected:
        int x, y;												// ���󪺮y��
        int width, height;										// ���󪺪��e
        double _size;											// ���󪺤j�p
        ColArray array;											// �I���x�}
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	private:
		vector<CAnimation> ani;			// vector of CAnimation
		int currentAni;					// current running CAnimation
		vector<vector<int>*> bmp_iter;
};
}

#endif															//define OBJECT_H