#pragma   once
#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H
#include "player.h"
#include <chrono>

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// 回合 class
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
        void OnMouseMove(UINT, CPoint);				// 滑鼠移動
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        CMovingBitmap	help;		// 說明圖
        CPoint mousePoint;
        UINT currentKeydown;
		chrono::time_point<chrono::steady_clock> start;
        Ground ground;
        Player player,enemy;
        Object background;
};

}

#endif //define BATTLESYSTEM_H