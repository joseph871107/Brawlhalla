namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 地圖 class
	/////////////////////////////////////////////////////////////////////////////

	class Ground
	{
	public:
		Ground();
		int  GetX1();					// 地面左上角 x 座標
		int  GetY1();					// 地面左上角 y 座標
		int  GetX2();					// 地面右下角 x 座標
		int  GetY2();					// 地面右下角 y 座標
		void LoadBitmap();										// 載入圖形
		void OnMove();											// 移動
		void OnShow();											// 將圖形貼到畫面
		void SetXY(int nx, int ny);								// 設定地板的座標
		void SetLen(int len);
	protected:
		CMovingBitmap bmp;			// 地面的圖		
		int x, y;					// 地面的座標
		int width, height;			// 地面的長寬
		int length;					// 地面的長度
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
}