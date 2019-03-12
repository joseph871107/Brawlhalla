namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �a�� class
	/////////////////////////////////////////////////////////////////////////////

	class Ground
	{
	public:
		Ground();
		int  GetX1();					// �a�����W�� x �y��
		int  GetY1();					// �a�����W�� y �y��
		int  GetX2();					// �a���k�U�� x �y��
		int  GetY2();					// �a���k�U�� y �y��
		void LoadBitmap();										// ���J�ϧ�
		void OnMove();											// ����
		void OnShow();											// �N�ϧζK��e��
		void SetXY(int nx, int ny);								// �]�w�a�O���y��
		void SetLen(int len);
	protected:
		CMovingBitmap bmp;			// �a������		
		int x, y;					// �a�����y��
		int width, height;			// �a�������e
		int length;					// �a��������
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
}