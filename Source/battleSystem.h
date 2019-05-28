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
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnKeyDown(UINT, UINT, UINT);				// 鍵盤按下
        void OnKeyUp(UINT, UINT, UINT);					// 鍵盤釋放
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnMouseMove(UINT, CPoint);					// 滑鼠移動
        void TriggerDisplayMessage(const string& message, const int& posX, const int& posY, const int& durationByFrame);
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
        void ResizeCamera();
        void AddMap(shared_ptr<Map> m);
        bool IsGameOver();
        string GetGameResult();
        void TriggerExplosionEffect(Player* deadPlayer);

    private:
        //-----------------FUNCTIONS DECLARATIONS-----------------//
        void ClearPlayers();
        void ShowPlayerLife(const Player& player, int posXValue, int posYValue);
        int GetCurrentRemainTime();
        void LoadSoundOnInit();
        void GetExplosionEffectPosition(Player* deadPlayer, int* posXPtr, int* posYPtr);
        void ClearExplosionEffects();
        void InitializeExplosionEffectsOnBeginState();
        void InitializePlayersOnBeginState();
        void RemoveFinishedDisplayMessages();
        void ClearUIMessages();
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
        vector<ExplosionEffect*> _explosionEffects;
        vector<UIMessage*> _uiMessages;

};

}

#endif //define BATTLESYSTEM_H