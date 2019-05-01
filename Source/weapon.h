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
		void SetState(long state);
		bool HasTaken();
		bool BeThrown();
    private:
		CRAnimation bmp, tl, tr, *sbmp;
		clock_t start;
        double _velocityX, _velocityY;
        Ground* _ground;
        vector<Player*> _player;
		Player* _throwHost;
        bool _tDir;
		long _state;
};
}

#endif //define GROUND_H