#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H
#include "player.h"
#include "weapon.h"
#include <chrono>

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// BattleSystem class
/////////////////////////////////////////////////////////////////////////////

extern map<string, int> idbList;
extern map<int, string> fileList;
extern map<int, ColArray> cArray;

struct Camera {
	Camera() {
		x = SIZE_X / 2; y = SIZE_Y / 2;
		size = 1;
	}
	Camera(int tx, int ty) {
		x = tx; y = ty;
		size = 1;
	}
	int x, y;
	double size;
	CPoint GetXY(int tx, int ty) {
		CPoint temp;
		temp.x = (int)( x + (tx - x) * size);
		temp.y = (int)( y + (ty - y) * size);
		return temp;
	}
};

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
        bool IsGameOver();
        string GetGameResult();
		void ResolveMemoryLeaksOnEndState();

    private:
        //-----------------FUNCTIONS DECLARATIONS-----------------//
        void BattleSystem::ShowPlayerLife(const Player& player, int posXValue, int posYValue);
        int GetCurrentRemainTime();
        //-----------------VARIABLES DECLARATIONS-----------------//
        int _secPerRound;
        CPoint mousePoint;
        UINT currentKeydown;
        clock_t start, lastTime;
        int nextTimeGenerateWeapon;
        Object background;
        vector<Ground*> _grounds;
        vector<Player*> _players;
		vector<Weapon*> _weapons;
		vector<Weapon*> _flyingWeapons;
		Camera camera;
};

}

#endif //define BATTLESYSTEM_H