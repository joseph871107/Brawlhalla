#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H
#include "player.h"
#include "weapon.h"
#include "map.h"
#include "ExplosionEffect.h"
#include <chrono>
#include "window.h"

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
        BattleSystem(CGame* g);
		BattleSystem(CGame* g, shared_ptr<Map> m);
        ~BattleSystem();
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);				// ��L���U
        void OnKeyUp(UINT, UINT, UINT);					// ��L����
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnMouseMove(UINT, CPoint);					// �ƹ�����
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
		void ResizeCamera();
		void AddMap(shared_ptr<Map> m);
        bool IsGameOver();
        string GetGameResult();
		void InitializeExplosionEffect(Player* deadPlayer);

    private:
        //-----------------FUNCTIONS DECLARATIONS-----------------//
		void ClearPlayers();
        void ShowPlayerLife(const Player& player, int posXValue, int posYValue);
        int GetCurrentRemainTime();
		void OnInitLoadSound();
		void GetExplosionEffectPosition(Player * deadPlayer, int * posXPtr, int * posYPtr);
		int DoubleToInteger(double mDouble);
        //-----------------VARIABLES DECLARATIONS-----------------//
        int _secPerRound;
        CPoint mousePoint;
        UINT currentKeydown;
        int nextTimeGenerateWeapon;
        Background* background;
        vector<Ground*>* _grounds;
        vector<Player*> _players;
		vector<Weapon*>* _weapons;
		Camera camera;
		shared_ptr<Map> map;
		Window settingWindow;
		ExplosionEffect _explosionEffect;
		bool _isShowingExplosionEffect;

};

}

#endif //define BATTLESYSTEM_H