#pragma   once
#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H
#include "player.h"

namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// �^�X class
	/////////////////////////////////////////////////////////////////////////////

	extern map<string, int> idbList;
	extern map<int, string> fileList;
	extern map<int, CArray> cArray;

	class BattleSystem : public CGameState
	{
	public:
		BattleSystem(CGame *g) ;
		~BattleSystem();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT, UINT, UINT);				// ��L���U
		void OnKeyUp(UINT, UINT, UINT);					// ��L����
		void OnMouseMove(UINT , CPoint);				// �ƹ�����
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
		//GameComFunc func;
	private:
		//CMovingBitmap	background;	// �I����
		CMovingBitmap	help;		// ������
		CPoint mousePoint;
		UINT currentKeydown;
		Ground ground;
		Player player = Player(&ground);
		Object background;
	};
}

#endif //define BATTLESYSTEM_H