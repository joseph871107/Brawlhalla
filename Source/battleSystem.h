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
class UIMessage;

extern map<string, int> idbList;
extern map<int, string> fileList;
extern map<int, ColArray> cArray;

extern vector< vector<CMovingBitmap>> louis_l0;
extern vector< vector<CMovingBitmap>> louis_r0;
extern vector< vector<CMovingBitmap>> louis_l1;
extern vector< vector<CMovingBitmap>> louis_r1;
extern vector< vector<CMovingBitmap>> louis_l2;
extern vector< vector<CMovingBitmap>> louis_r2;
extern vector< vector<CMovingBitmap>> louis_ex_l0;
extern vector< vector<CMovingBitmap>> louis_ex_r0;
extern vector< vector<CMovingBitmap>> louis_ex_l1;
extern vector< vector<CMovingBitmap>> louis_ex_r1;
extern vector< vector<CMovingBitmap>> louis_ex_l2;
extern vector< vector<CMovingBitmap>> louis_ex_r2;
extern vector< vector<CMovingBitmap>> louis_ex_l3;
extern vector< vector<CMovingBitmap>> louis_ex_r3;
extern vector< vector<CMovingBitmap>> julian_l0;
extern vector< vector<CMovingBitmap>> julian_r0;
extern vector< vector<CMovingBitmap>> julian_l1;
extern vector< vector<CMovingBitmap>> julian_r1;
extern vector< vector<CMovingBitmap>> julian_l2;
extern vector< vector<CMovingBitmap>> julian_r2;

class BattleSystem : public CGameState
{
    public:
        BattleSystem(CGame* g);
        BattleSystem(CGame* g, shared_ptr<Map> m);
        ~BattleSystem();
        void OnBeginState(int mode, int enemy, int diff);							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);				// ��L���U
        void OnKeyUp(UINT, UINT, UINT);					// ��L����
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnMouseMove(UINT, CPoint);					// �ƹ�����
        void TriggerDisplayMessage(const string& message, const int& posX, const int& posY, const int& durationByFrame);
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
        void ResizeCamera();
        void AddMap(shared_ptr<Map> m);
        bool IsGameOver();
        string GetGameResult();
        void TriggerExplosionEffect(Player* deadPlayer);
        shared_ptr<Map> GetReferenceMap();

    private:
        //-----------------FUNCTIONS DECLARATIONS-----------------//
        int GetNumberOfRemainingPlayers();
        bool IsFinishedPlayingAllEffects();
        void ClearPlayers();
        void ShowPlayerLife(const Player& player, int posXValue, int posYValue);
        int GetCurrentRemainTime();
        void LoadSoundOnInit();
        void GetExplosionEffectPosition(Player* deadPlayer, int* posXPtr, int* posYPtr);
        void ClearExplosionEffects();
        void InitializeExplosionEffectsOnBeginState();
        void InitializePlayersOnBeginState(int mode, int enemy, int diff);
        void RemoveFinishedDisplayMessages();
        void ClearUIMessages();
		void EasterEgg(int set = -1);
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
        vector<ExplosionEffect*> _explosionEffects;
        vector<UIMessage*> _uiMessages;
		CMovingBitmap takenDmgR, takenDmgY, takenDmgG;
		CAnimation chang;
		bool keyDebounce = true;
		int comboCounter = 0;
		int _mode;
};

}

#endif //define BATTLESYSTEM_H