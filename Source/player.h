#pragma   once
#ifndef PLAYER_H
#define PLAYER_H
#include "ground.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 地圖 class
	/////////////////////////////////////////////////////////////////////////////

	class Player
	{
	public:
		Player(Ground *grd);
		bool HitObject();				// 是否碰到地圖
		bool IsOnGround();
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
		void Jump(double acc);
		CArray array;					// 碰撞矩陣
	protected:
		CAnimation left;			// 玩家向左走
		CAnimation right;			// 玩家向右走
		CAnimation jumpL;			// 玩家跳躍
		CAnimation jumpR;			// 玩家跳躍
		int x, y;					// 玩家的座標
		int width, height;			// 玩家的長寬
		double velocity;			// 在空中停留時間
		bool onGround;
		bool dir;					// 玩家方向 false向左 true向右
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
	private:
		bool* HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
		Ground *ground;
	};
}

#endif //define PLAYER_H