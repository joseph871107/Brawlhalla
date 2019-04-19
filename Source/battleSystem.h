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

class Background : public Object
{
public:
	void OnShow(double offset)
	{
		if (camera != nullptr) {
			CPoint cam = camera->GetXY(x, y);
			bmp.SetTopLeft((int)(cam.x * offset), (int)(cam.y * offset));
			bmp.ShowBitmap(_size * (0.9 + 0.1 * camera->GetSize()));
		}
		else {
			bmp.SetTopLeft(x, y);
			bmp.ShowBitmap(_size);
		}
	}
};

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
        Background background;
        vector<Ground*> _grounds;
        vector<Player*> _players;
		vector<Weapon*> _weapons;
		vector<Weapon*> _flyingWeapons;
		Camera camera;
};

}

#endif //define BATTLESYSTEM_H