#pragma   once
#ifndef WEAPON_H
#define WEAPON_H
#include "ground.h"
#include "weapon.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Weapon class
/////////////////////////////////////////////////////////////////////////////

class Player;
class Weapon : public Object
{
    public:
        Weapon();
        void OnShow();
        void OnMove();
        void OnKeyDown(UINT);
        void LoadBitmap();
        void Initialize(vector<Ground*>, vector<Player*>);
		void Throw(bool, Player*);
        Player* HitPlayer();
		bool HasTaken();
		bool BeThrowen();
    private:
        CMovingBitmap bmp,tl, tr;
        int _bmpID, _throwLID, _throwRID, _flyingDistance;
        COLORREF _color;
        double _velocity;
        Ground* _ground;
        vector<Player*> _player;
        bool _isHolding, _isThrowing, _tDir;
		Player* _throwHost;
};
}

#endif //define GROUND_H