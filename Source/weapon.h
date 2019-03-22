#pragma   once
#ifndef WEAPON_H
#define WEAPON_H
#include "ground.h"
#include "player.h"
#include "weapon.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Weapon class
/////////////////////////////////////////////////////////////////////////////

class Weapon : public Object
{
    public:
        Weapon();
        void OnShow();
        void OnMove();
		void OnKeyDown(UINT);
        void LoadBitmap();
        void Initialize(vector<Ground*>, vector<Player*>);
		Player* HitPlayer();
    private:
        CMovingBitmap bmp;
        int _bmpID;
        COLORREF _color;
        double _velocity;
		Ground* _ground;
		vector<Player*> _player;
        bool _isHolding;
		int _flasingPtr;
};
}

#endif //define GROUND_H