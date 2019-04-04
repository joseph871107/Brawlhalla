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

class BattleSystem : public CGameState
{
    public:
        BattleSystem(CGame* g) ;
        ~BattleSystem();
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnKeyDown(UINT, UINT, UINT);				// 鍵盤按下
        void OnKeyUp(UINT, UINT, UINT);					// 鍵盤釋放
        void OnMouseMove(UINT, CPoint);					// 滑鼠移動
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
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
};

}

#endif //define BATTLESYSTEM_H