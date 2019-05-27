#include "ground.h"
#include "weapon.h"
#include "camera.h"
#include "Vector2.h"
#include "RespawnCourier.h"

#define _PLAYER_DEBUG false

namespace game_framework
{
#ifndef PLAYER_H
#define PLAYER_H
class TriggeredAnimation;
class BattleSystem;
class ExplosionEffect;

class Player
{
    public:
        //-----------------FUNCTIONS DECLARATIONS-----------------//
        //Essential functions for C++ class
        Player();						//Default constructor
        ~Player();						//Destructor

        //Required for Game Framework
        void Initialize(BattleSystem* battleSystemValue, vector<Ground*> groundsValue, vector<Player*>* playersPtrValue, string nameValue, vector<long> keysValue, ExplosionEffect* const explosionEffectPtrValue);
        void LoadBitmap();
        virtual void OnMove();
        void OnShow();
        void OnKeyDown(const UINT& nChar);
        void OnKeyUp(const UINT& nChar);

        //Others - Joseph
        void SetName(string);
        void SetHoldWeapon(bool);
        void BeenAttacked(Vector2 displaymentVector, bool beingAttackedDirection);
        bool GetHoldWeapon();
        bool GetDirection();
        int GetCor(int);				// 物件座標 0:左上X, 1:左上Y, 2:右下X, 3:右下Y
        int GetWidth();
        int GetHeight();
        void AddCamera(Camera* cam);	// Camera
        void SetPlayer(bool tri);
        bool IsPlayer();
        void SetSize(double);
        double GetSize();
        vector<Weapon*>* weapons;

        //Others - Bill
        const string& GetName() const;
        const int& GetLife() const;
        const bool IsOutOfLife() const;
        const long& GetAttackKey() const;
        void GenerateAndSetWeaponID();	// Called by 'Weapon::HitPlayer()' when the player picks up a weapon
        void ResetWeaponID();
        void PerformAttack(Player* targetPlayer, bool attackDirection);
        ExplosionEffect* GetExplosionEffect();

        // Used by Triggered Animation classes
        void SetTriggeredAnimation(bool newIsTriggeredAni);
        void SetTriggeredAnimationKeyID(int newTriggeredAniKeyID);
        void SetTriggeredAnimationDir(bool newTriggeredAniDir);
        const bool& GetTriggeredAnimationDirection() const;
        void SetTriggeredAnimationAnimationID(const int& newTriggeredAniAnimationID);
        void DoAttack();
        void DoLand();
        void SetIsTriggerAttack(const bool& newIsTriggerAttack);
        void InitiateOffsetLeft(double initialOffsetVelocityMagnitude);
        void InitiateOffsetRight(double initialOffsetVelocityMagnitude);
        void SetIsDodging(const bool& newIsDodging);
        void SetIsTriggerDodge(const bool& newIsTriggerDodge);
        void SetIsTriggerDrawWeapon(const bool& newIsTriggerDrawWeapon);
        void EmptyHitTargetPlayers();
        int GetCurrentAniByWeaponNum();

        // Used by Ground
        void SetX(const int& newX);
        void SetY(const int& newY);

        //-----------------STATIC VARIABLES DECLARATIONS-----------------//
        // Animations ID of 'ani'

        // Animations ID of '_aniByWpn'
        static const int ANI_WPN_ID_STAND_LEFT = 0;
        static const int ANI_WPN_ID_STAND_RIGHT = 1;
        static const int ANI_WPN_ID_ATTACK_LEFT = 2;
        static const int ANI_WPN_ID_ATTACK_RIGHT = 3;
        static const int ANI_WPN_ID_GND_MOVE_ATTACK_LEFT = 4;
        static const int ANI_WPN_ID_GND_MOVE_ATTACK_RIGHT = 5;
        static const int ANI_WPN_ID_SLIDE_ATTACK_LEFT = 6;
        static const int ANI_WPN_ID_SLIDE_ATTACK_RIGHT = 7;
        static const int ANI_WPN_ID_AIR_ATTACK_LEFT = 8;
        static const int ANI_WPN_ID_AIR_ATTACK_RIGHT = 9;
        static const int ANI_WPN_ID_AIR_MOVE_ATTACK_LEFT = 10;
        static const int ANI_WPN_ID_AIR_MOVE_ATTACK_RIGHT = 11;
        static const int ANI_WPN_ID_AIR_DOWN_ATTACK_LEFT = 12;
        static const int ANI_WPN_ID_AIR_DOWN_ATTACK_RIGHT = 13;
        static const int ANI_WPN_ID_DRAW_SWORD_LEFT = 14;
        static const int ANI_WPN_ID_DRAW_SWORD_RIGHT = 15;
        static const int ANI_WPN_ID_MOVE_LEFT = 16;
        static const int ANI_WPN_ID_MOVE_RIGHT = 17;
        static const int ANI_WPN_ID_JUMP_LEFT = 18;
        static const int ANI_WPN_ID_JUMP_RIGHT = 19;
        static const int ANI_WPN_ID_LEAN_LEFT = 20;
        static const int ANI_WPN_ID_LEAN_RIGHT = 21;
        static const int ANI_WPN_ID_LAND_FALL_LEFT = 22;
        static const int ANI_WPN_ID_LAND_FALL_RIGHT = 23;
        static const int ANI_WPN_ID_UNCONSCIOUS_FLYING_LEFT = 24;
        static const int ANI_WPN_ID_UNCONSCIOUS_FLYING_RIGHT = 25;
        static const int ANI_WPN_ID_DODGE_LEFT = 26;
        static const int ANI_WPN_ID_DODGE_RIGHT = 27;
        // Others
        static const int OFFSET_INITIAL_VELOCITY = 20;

    protected:
        //-----------------FUNCTIONS DECLARATIONS-----------------//
        //Animations
        void AddCAnimationWithSprite(vector<CAnimation>*, vector< vector<CMovingBitmap>>*, vector<CPoint>*, double = 1.0, int = 5, bool = true, int = 1);
        void ResetAnimations(int animationID);
        virtual void SetAnimation();

        void ShowCurrentAnimation();			// Show CAnimation by currentAni

        //Position
        bool IsOnLeftEdge();
        bool IsOnRightEdge();
        bool IsOutMapBorder();

        //Offsets
        void InitiateOffsetUp(double initialOffsetVelocityMagnitude);
        void InitiateOffsetDown(double initialOffsetVelocityMagnitude);

        bool IsBeingOffsetHorizontally();
        void DoHorizontalOffset();

        //Movements
        void DoMoveLeft(int movementUnit);
        void DoMoveRight(int movementUnit);

        //Jump
        void DoJump();
        void ResetJumpCount();

        //Wall jump
        void InitiateWallJump();

        //Attack
        bool HitPlayer(Player* targetPlayer, bool attackDirection);

        //Throw weapon
        void DoThrowingWeapon(); /// Unused function

        //Audio management
        void PlayAudioByState();

        //Unconscious state
        void InitializeUnconsciousState(bool beingAttackedDirection);
        void ConsciouslyOnMoveGameLogic();
        void UnconsciouslyOnMoveGameLogic();
        void SetConscious();

        //Others
        void DoDead();
        void SetRespawnMovementVector(const int& startPosX, const int& startPosY, const int& destinationPosX, const int& destinationPosY);
        void DoRespawn();
        void InitializeOnRespawn();
        int Round(double i);
        bool StateChanged();
        bool WpnStateChanged();
        void ModifyVerticalOffsetVariablesFunctions();

        /// Comment for future devs: Unorganized member functions are declared below. They should be cleaned up in the near future
        //Weapon
        void SetAnimationStateByWeapon(int num);

        //Animation By Weapon
        void AddCollectionOfAnimationsByWeapon(
            vector<int>& s2l, vector<int>& s2r, vector<int>& al, vector<int>& ar,
            vector<int>& gmal, vector<int>& gmar, vector<int>& sal, vector<int>& sar,
            vector<int>& aal, vector<int>& aar, vector<int>& amal, vector<int>& amar,
            vector<int>& adal, vector<int>& adar, vector<int>& sdl, vector<int>& sdr);
        void AddCAnimationByWeapon(vector<CAnimation>& tempAniByWpn, vector<int>*, double = 1.0, int = 5, bool = true, int = 1); // Push (bmps, (optional)size, (op)delay, (op)repeat, (op)repeat times) in vector of CAnimation

        //Key combination
        int GetKeyCombination();

        void ProcessCurrentKeyCombinationGameLogic();

        //Triggered animation concept
        void ResetTriggeredAnimationVariables();
        void SetTriggeredAnimationVariables(int keyCombInt);

        void InitiateTriggeredAction();
        void DoTriggeredAction();
        bool IsFinishedTriggeredAnimation();



        void DoNonTriggeredAction();

        //Edges
        bool IsOnEdge();
        bool IsFirstTimeOnEdge();
        void InitiateOnEdge();
        void DoOnEdge();
        void DoLeaveEdge();

        //Ground
        void DoOnGround();
        void DoRepositionAboutGround(int playerX1, int playerY1, int playerX2, int playerY2, Ground* groundPtr);
        bool IsOnGround();				// Return 'true' if the player is on any ground of all grounds
        void DoBounceOffGround(int playerX1, int playerY1, int playerX2, int playerY2, Ground* groundPtr);

        //
        void SetCurrentAnimation();
        void UnconsciouslyOnMoveAnimationLogic();
        void FinishTriggeredAnimationAnimationLogic();
        void FinishTriggeredAnimationGameLogic();
        void ConsciouslyOnMoveAnimationLogic();
        void MoveCurrentAnimation();
        void RespawnOnMoveAnimationLogic();
        void OnMoveAnimationLogic();
        void RespawnOnMoveGameLogic();
        void DoReturnHomeRespawnCourier();
        void OnMoveGameLogic();

        void SetCurrentNonTriggeredAnimationByWeapon();
        void SetCurrentTriggeredAnimationByWeapon();
        void SetCurrentAniByWeapon();

        void InitializeTriggeredAnimations();

        void SetAttacker(Player* const& newAttacker, const int& attackerAffectionFrameCountValue);

        bool IsAttackable(Player* potentialTargetPlayer);

        void DoMoveLeftWithAcceleration();

        void DoMoveRightWithAcceleration();

        void ResetMovementVelocity();

        void DoParseKeyPressed();

        void SetState(const int& newState);
        //-----------------VARIABLES DECLARATIONS-----------------//
        //Required for Game Framework
        int _x, _y;						// position of the collision's box
        double BITMAP_SIZE = 1;
        //bool _beInterrupt;
        vector<vector<int>*> bmp_iter;	// used to display current animation state in DEBUG mode
        int _OFFSET_X = 20;
        int _OFFSET_Y = 10;

        //Required for "physical" existence in the game
        CMovingBitmap _collision_box;
        int _width, _height;			// of the collision box

        //Keys
        vector<long> _keys; // 0 - up, 1 - right, 2 - down, 3 - left, 4 - attack, 5 - dodge

        //Offsets
        bool _isOffsetLeft, _isOffsetRight;
        double _horizontalVelocity;

        //Movements
        bool _isPressingLeft, _isPressingRight, _isPressingDown, _isTriggerPressingLeft;
        bool _dir; // false: player facing left, true: player facing right

        //Jump
        bool _isTriggerJump;
        int _jumpCount;

        //Gravity
        double _verticalVelocity;
        double _verticalAcceleration;

        //Draw Weapon
        bool _isTriggerDrawWeapon;

        //Attack
        bool _isHoldingWeapon;
        bool _isTriggerAttack;
        int _takenDmg;

        //Throw weapon
        Weapon* _flyingWeapon;
        void DeleteFlyingWeapon();
        // The taken damage will determine how far the target player would fly 'attackOffsetMagnitude', and
        // how long he would be in the unconscious state '_unconsciousFramesCount'

        vector<Player*>* _playersPtr;
        bool _isPlayer;

        //Dodge
        bool _isTriggerDodge;
        bool _isDodging;

        //Grounds
        vector<Ground*> _grounds;

        //Life
        int _life;

        //Name
        string _name;

        //Camera
        Camera* camera;

        ///Comment for future devs: Unorganized member variables are declared below. They should be cleaned up in the near future
        //Triggered animation concept
        bool _isTriggeredAni;
        // False - these is no triggered animation, the player can perform non-triggered animations
        // True - a triggered animation is activated, the player is constrained to perform the triggered animation

        int _triggeredAniKeyID;
        // The "KeyID" (short for "ID of the combination of key pressed") of the triggered animation
        // Its value is one of the constants starting with 'KEY_...'
        // Do note that this value is unique!!!

        bool _triggeredAniDir;
        // The direction the player is facing when the triggered animation is activated
        // Since the variable '_dir' is modified whenever a directional key is pressed, this variable is necessary for preserving the direction of the triggered animation
        // false: player facing left, true: player facing right

        int _triggeredAniAnimationID;
        // The index of the triggered animation in the 1-dimensional vector 'ani[---index---]' or 2-dimensional vector '_aniByWpn[_wpnID][---index---]',
        // Its value is one of the constants starting with 'ANI_ID_...' or 'ANI_WPN_ID_...'
        // Technically, the animation needed in 'OnShow()' is defined by 'ani[_triggeredAniAnimationID]' or '_aniByWpn[_wpnID][_triggeredAniAnimationID]'

        bool _isInitiatedTriggeredAni;

        int _lastTriggeredAniKeyID;

        int _lastTriggeredAniByWpnID;

        int _finishedTriggeredAniKeyID;

        //Weapon
        int _wpnID; // 0 - punch (default), 1 - sword 1, 2 - sword 2
        int _roundPrevPickedUpWpnID; // The previously picked up weapon in the round. Values: 1 - sword 1, 2 - sword 2 (initialized as 2)

        //Animation By Weapons
        vector<vector<CAnimation>> _aniByWpn;
        int _currentAniByWpn;
        // False - choose 'ani' for showing the animation
        // True - choose '_aniByWpn' for showing the animation
        // This variable is determined in 'Player::OnMove()'

        //Unconscious state
        int _unconsciousFramesCount;
        bool _unconsciousAniDir;

        //Edges
        bool _isFirstTimeOnEdge;

        // Others
        int _currentKeyID;
        // The current "KeyID" (short for "ID of the combination of key pressed") being activated by the user
        // Its value is one of the constants starting with 'KEY_...'
        // Do note that this value is unique!!!

        // Triggered animations
        vector<TriggeredAnimation*> _triggeredAnis;

        // Hit only once
        vector<Player*> _hitTargetPlayers;

        // Inertia
        double _moveVelocity;

        // Game Effect
        BattleSystem* _battleSystem;
        ExplosionEffect* _explosionEffectPtr;

        // Display killer
        Player* _attacker;
        int _attackerAffectionFrameCount;
        bool _isDead;

        // State
        int _state;

        // Repawn
        Vector2 _vectorRespawnMovement;
        int _resDestPosX, _resDestPosY;
        double _preDistance; // used in combination with _vectorRespawnMovement
        RespawnCourier _respawnCourier;
};
#endif
}