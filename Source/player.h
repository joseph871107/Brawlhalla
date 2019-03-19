#pragma   once
#ifndef PLAYER_H
#define PLAYER_H
#include "ground.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// 地圖 class
/////////////////////////////////////////////////////////////////////////////

class Player
{
    public:
        Player(Ground* grd);			// Constructor
        ~Player();						// Deconstructor
        bool HitObject();				// 是否碰到地圖
        bool IsOnGround();				//
        void LoadBitmap();				// 載入圖形
        void OnMove();					// 移動
        void OnShow();					// 將圖形貼到畫面
        void SetXY(int nx, int ny);		// 設定玩家的座標
        int  GetX1();					// 玩家左上角 x 座標
        int  GetY1();					// 玩家左上角 y 座標
        int  GetX2();					// 玩家右下角 x 座標
        int  GetY2();					// 玩家右下角 y 座標
        void SetMovingDown(bool flag);	// 設定是否正在往下移動
        void SetMovingLeft(bool flag);	// 設定是否正在往左移動
        void SetMovingRight(bool flag); // 設定是否正在往右移動
        void SetMovingUp(bool flag);	// 設定是否正在往上移動
        void Jump(double = 20);			// 跳起
        ColArray array;					// 碰撞矩陣
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
        int x, y;					// 玩家的座標
        int width, height;			// 玩家的長寬
        double velocity;			// 在空中停留時間
        bool dir;					// 玩家方向 false向左 true向右
        bool isMovingDown;			// 是否正在往下移動
        bool isMovingLeft;			// 是否正在往左移動
        bool isMovingRight;			// 是否正在往右移動
        bool isMovingUp;			// 是否正在往上移動
        int jumpCount;											// limit of jump times
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
        Ground* ground;
};
}

#endif //define PLAYER_H