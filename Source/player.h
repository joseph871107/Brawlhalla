
#include "ground.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 地圖 class
	/////////////////////////////////////////////////////////////////////////////

	class Player
	{
	public:
		Player();
		bool HitObject(Ground *ground);							// 是否碰到地圖
		void LoadBitmap();										// 載入圖形
		void OnMove(Ground *ground);											// 移動
		void OnShow();											// 將圖形貼到畫面
		void SetXY(int nx, int ny);								// 設定玩家的座標
		int  GetX1();					// 玩家左上角 x 座標
		int  GetY1();					// 玩家左上角 y 座標
		int  GetX2();					// 玩家右下角 x 座標
		int  GetY2();					// 玩家右下角 y 座標
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
	protected:
		CAnimation front;		// 玩家的動畫	
		CAnimation back;
		int x, y;					// 玩家的座標
		int width, height;			// 玩家的長寬
		bool dir;					// 玩家方向 false向左 true向右
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
	private:
		bool* HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
}