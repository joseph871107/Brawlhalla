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
        bool IsOutMapBorder();
        void LoadBitmap();
        void Initialize(vector<Ground*>, vector<Player*>);
        void Throw(bool, Player*);
        Player* HitPlayer();
        bool IsPickedUp();
        bool IsExpired();
        bool BeThrown();
    private:
        //-----------------FUNCTIONS DECLARATIONS-----------------//
        void DoBounceOffGround(int weaponX1, int weaponY1, int weaponX2, int weaponY2, Ground* groundPtr);
        void InitiateOffsetLeft(double initialOffsetVelocityMagnitude);
        void InitiateOffsetRight(double initialOffsetVelocityMagnitude);
        void InitiateOffsetUp(double initialOffsetVelocityMagnitude);
        void InitiateOffsetDown(double initialOffsetVelocityMagnitude);
        void DoHorizontalOffset();
        bool IsBeingOffsetHorizontally();
        bool IsOnGround();
        void DoWeaponBeingThrown();
        void DoWeaponDropbox();
        // Unused functions
        void SetState(long state);
        //-----------------VARIABLES DECLARATIONS-----------------//
        CRAnimation bmp, tl, tr, *sbmp; // Rotate animation
        clock_t start;
        vector<Ground*> _grounds;
        Ground* _ground;
        vector<Player*> _player;
        Player* _throwHost;
        bool _throwDir; // throw direction
        long _state;
        //
        int _width, _height; // true width and height
        // Since the bitmaps of weapon vary in size, we cannot use 'width' and 'height' given by the 'Object' class
        // Moreover, since 'GetCor(2)' and 'GetCor(3)' are dependent on 'width' and 'height', they are also deficient
        bool _isOffsetLeft, _isOffsetRight;
        double _horizontalVelocity;
        double _verticalVelocity;
};
}

#endif //define GROUND_H