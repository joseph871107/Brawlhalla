#pragma   once
#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H
#include "player.h"
#include <chrono>

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// BattleSystem class
/////////////////////////////////////////////////////////////////////////////

extern map<string, int> idbList;
extern map<int, string> fileList;
extern map<int, ColArray> cArray;

class BattleSystem : public CGameState
{
    public:
        BattleSystem(CGame* g) ;
        ~BattleSystem();
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);				// ��L���U
        void OnKeyUp(UINT, UINT, UINT);					// ��L����
        void OnMouseMove(UINT, CPoint);					// �ƹ�����
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
        int GetCurrenRemainTime();
    private:
        int _secPerRound;
        CPoint mousePoint;
        UINT currentKeydown;
        chrono::time_point<chrono::steady_clock> start;
        vector<Ground*> _ground;
		Object background;
        Player player1, player2;
};

}

#endif //define BATTLESYSTEM_H