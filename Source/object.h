#ifndef OBJECT_H
#define OBJECT_H
#include "gameComFunc.h"
#include <map>

#define GENERATE_COLLISION_ARRAY false

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// 物件 class
/////////////////////////////////////////////////////////////////////////////

class Object
{
    public:
        Object();
        Object(double);
		Object& operator=(const Object& rightObject);
		~Object();
        void OnShow();											// 將圖形貼到畫面
        void SetXY(int nx, int ny);								// 設定物件的座標
        void SetSize(double s);									// 設定物件大小
        void OnMove();											// 移動
        int  GetCor(int);										// 物件座標 0:左上X, 1:左上Y, 2:右下X, 3:右下Y
        void LoadBitmap(vector<int>*, COLORREF);				// 載入圖形
        bool Collision(int, int, int, int, int);
        int ShowAnimationState();		// Return which CAnimation is playing
        void SetAnimationState(int);	// Set which CAnimation is going to play
        void ShowAnimation();			// Show CAnimation by currentAni
        void AddCAnimation(vector<int>*, double = 1.0, int = 5, bool = true, int = 1);	// Push (bmps, (optional)size, (op)delay, (op)repeat, (op)repeat times) in vector of CAnimation
    protected:
        int x, y;												// 物件的座標
        int width, height;										// 物件的長寬
        double _size;											// 物件的大小
        ColArray array;											// 碰撞矩陣
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	private:
		vector<CAnimation> ani;			// vector of CAnimation
		int currentAni;					// current running CAnimation
		vector<vector<int>*> bmp_iter;
};
}

#endif															//define OBJECT_H