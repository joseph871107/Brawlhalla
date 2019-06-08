#include "ground.h"
#include "weapon.h"
#include "camera.h"
#include "Vector2.h"
#include "RespawnCourier.h"
// Player states
#include "PlayerConsciousState.h"
#include "PlayerUnconsciousState.h"
#include "PlayerRespawnState.h"
#include "PlayerImmuneState.h"

#define _PLAYER_DEBUG false
//
#define PLAYER_MODE_PLAYER 0
#define PLAYER_MODE_ENEMY 1
#define PLAYER_MODE_BOSS 2

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
        /* Required for C++ class functions */
        ~Player(); // Destructor
        Player(); // Default constructor

        /* Required for the Game Framework functions */
        void Initialize(BattleSystem* battleSystemValue, vector<Ground*> groundsValue, vector<Player*>* playersPtrValue, string nameValue, vector<long> keysValue, ExplosionEffect* const explosionEffectPtrValue);
        void LoadBitmap();
        virtual void OnMove();
        virtual void OnShow();
        void OnKeyDown(const UINT& nChar);
        void OnKeyUp(const UINT& nChar);

        /* Get functions */
        bool GetHoldWeapon();
        bool GetDirection();
        int GetCor(int); // 物件座標 0:左上X, 1:左上Y, 2:右下X, 3:右下Y
        int GetWidth();
        int GetHeight();
        int GetPlayerMode();
        double GetSize();
        vector<long> GetKeys();
        const long& GetAttackKey() const;
        const int& GetState() const;
        const int& GetTakenDamage() const;
        const string& GetName() const;
        const int& GetLife() const;
        ExplosionEffect* GetExplosionEffect();
        virtual int GetSpecializedTakenDamage() const;
        const vector<Player*>& GetAttackListByGameMode() const;
        const bool& GetTriggeredAnimationDirection() const;
        int GetCurrentAniByWeaponNum();
        const double& GetVerticalVelocity() const;
        vector<Weapon*>*& GetWeaponsPtrByReference();

        /* Set functions */
        void SetName(string);
        void SetHoldWeapon(bool);
        void SetPlayer(int id);
        void SetSize(double);
        void SetRespawn(bool tri);
        void SetAttackList(vector<Player*> list);
        void SetTakenDmg(int dmg);
        void SetState(const int& newState);
        void SetAttacker(Player* const& newAttacker, const int& attackerAffectionFrameCountValue);
        void SetIsDodging(const bool& newIsDodging);
        void SetIsTriggerDodge(const bool& newIsTriggerDodge);
        void SetIsTriggerAttack(const bool& newIsTriggerAttack);
        void SetIsTriggerDrawWeapon(const bool& newIsTriggerDrawWeapon);
        void SetX(const int& newX);
        void SetY(const int& newY);
        // Triggered animations
        void SetTriggeredAnimationAnimationID(const int& newTriggeredAniAnimationID);
        void SetTriggeredAnimation(bool newIsTriggeredAni);
        void SetTriggeredAnimationKeyID(int newTriggeredAniKeyID);
        void SetTriggeredAnimationDir(bool newTriggeredAniDir);

        /* Other functions */
        void AddCamera(Camera* cam); // Camera
        const bool IsOutOfLife() const;
        void GenerateAndSetWeaponID();	// Called by 'Weapon::HitPlayer()' when the player picks up a weapon
        void ResetWeaponID();
        void PerformAttack(Player* targetPlayer, bool attackDirection);
        void DoLand();
        void DoAttack();
        void InitiateOffsetLeft(double initialOffsetVelocityMagnitude);
        void InitiateOffsetRight(double initialOffsetVelocityMagnitude);
        void EmptyHitTargetPlayers();
        void BeenAttacked(Vector2 displaymentVector, bool beingAttackedDirection);

        //-----------------STATIC VARIABLES DECLARATIONS-----------------//
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
        // Triggered Animation Key IDs
        static const int KEY_GND_ATTACK = 112;
        static const int KEY_GND_MOVE_RIGHT_ATTACK = 122;
        static const int KEY_GND_MOVE_LEFT_ATTACK = 132;
        static const int KEY_GND_LAND_DOWN_ATTACK = 142;
        static const int KEY_DRAW_SWORD = 113;
        static const int KEY_DODGE = 114;
        static const int KEY_AIR_ATTACK = 212;
        static const int KEY_AIR_MOVE_RIGHT_ATTACK = 222;
        static const int KEY_AIR_MOVE_LEFT_ATTACK = 232;
        static const int KEY_AIR_LAND_DOWN_ATTACK = 242;
        // Non-triggered Animation Key IDs
        static const int KEY_GND_IDLE = 111;
        static const int KEY_GND_MOVE_RIGHT = 121;
        static const int KEY_GND_MOVE_LEFT = 131;
        static const int KEY_GND_LAND_DOWN = 141;
        static const int KEY_AIR_IDLE = 211;
        static const int KEY_AIR_MOVE_RIGHT = 221;
        static const int KEY_AIR_MOVE_LEFT = 231;
        static const int KEY_AIR_LAND_DOWN = 241;
        // States
        static const int CONSCIOUS_STATE = 0;
        static const int UNCONSCIOUS_STATE = 1;
        static const int RESPAWN_STATE = 2;
        static const int IMMUNE_STATE = 3;
        // Velocities
        static const double INITIAL_VELOCITY;
        static const int OFFSET_INITIAL_VELOCITY = 20;
        static const double MAX_VERTICAL_VELOCITY;
        static const double MAX_MOVE_VELOCITY;
        // Accelerations
        static double INITIAL_ACCELERATION;
        static const double EDGE_SLIDING_ACCELERATION;
        static const double LANDING_ACCELERATION;
        static const double MOVE_ACCELERATION;
        static const double STOP_ACCELERATION;
        static const int INITIAL_MAX_CONSCIOUS_FRAME = 10;
        // Taken Damage
        static const int INCREMENT_AMOUNT_OF_TAKEN_DAMAGE = 3;
        static const int INITIAL_TAKEN_DAMAGE = INCREMENT_AMOUNT_OF_TAKEN_DAMAGE * 3;
        static const int TAKEN_DMG_DANGER_HIGH = 35;
        static const int TAKEN_DMG_DANGER_MEDIUM = 20;
        static const int TAKEN_DMG_DANGER_LOW = 0;
        // Others
        static const int MAX_IMMUNE_FRAMES = 60; // 2 secs
        static const int MOVEMENT_UNIT = 10;

    protected:
        //-----------------FRIEND CLASSES-----------------//
        friend class PlayerConsciousState;
        friend class PlayerUnconsciousState;
        friend class PlayerRespawnState;
        friend class PlayerImmuneState;

        //-----------------FUNCTIONS DECLARATIONS-----------------//
        /* Is functions */
        bool IsOnEdge();
        bool IsOnLeftEdge();
        bool IsOnRightEdge();
        bool IsOutMapBorder();
        bool IsBeingOffsetHorizontally();
        bool IsFinishedTriggeredAnimation();
        bool IsOnGround(); // Return 'true' if the player is on any ground of all grounds
        bool IsAttackable(Player* potentialTargetPlayer);

        /* Do functions */
        void DoThrowWeapon();
        void DoHorizontalOffset();
        void DoDead();
        virtual void DoRespawn();
        void DoReturnHomeRespawnCourier();
        void DoParseKeyPressed();

        /* Set functions */
        virtual void SetAnimation();
        void SetConscious();
        virtual void SetRespawnMovementVector(const int& startPosX, const int& startPosY, const int& destinationPosX, const int& destinationPosY);
        void SetAnimationStateByWeapon(int num);
        void SetTriggeredAnimationVariables(int keyCombInt);
        void SetCurrentAnimation();
        void SetCurrentNonTriggeredAnimationByWeapon();
        void SetCurrentTriggeredAnimationByWeapon();
        void SetCurrentAniByWeapon();

        /* Initiate functions */
        void InitiateOffsetUp(double initialOffsetVelocityMagnitude);
        void InitiateOffsetDown(double initialOffsetVelocityMagnitude);

        /* Initialize functions */
        void InitializeUnconsciousState(bool beingAttackedDirection);
        void InitializeTriggeredAnimations();
        virtual void InitializeOnRespawn();

        /* Reset functions */
        void ResetJumpCount();
        void ResetAnimations(int animationID);
        void ResetTriggeredAnimationVariables();
        void ResetMovementVelocity();

        /* Other functions */
        void AddCAnimationByWeapon(vector<CAnimation>& tempAniByWpn, vector<int>*, double = 1.0, int = 5, bool = true, int = 1); // Push (bmps, (optional)size, (op)delay, (op)repeat, (op)repeat times) in vector of CAnimation
        void AddCAnimationWithSprite(vector<CAnimation>*, vector< vector<CMovingBitmap>>*, vector<CPoint>*, double = 1.0, int = 5, bool = true, int = 1);
        void ShowCurrentAnimation(); // Show CAnimation by currentAni
        bool HitPlayer(Player* targetPlayer, bool attackDirection);
        void PlayAudioByState();
        bool StateChanged();
        bool WpnStateChanged();
        virtual void EvaluateDeadAndRespawn();
        Ground* OnGround();
        void FinishTriggeredAnimationAnimationLogic();
        void FinishTriggeredAnimationGameLogic();
        void MoveCurrentAnimation();
        void OnMoveAnimationLogic();
        void OnMoveGameLogic();
        int GetKeyCombination();

        //-----------------VARIABLES DECLARATIONS-----------------//
        /* Physics */
        int _x, _y;						// position of the collision's box
        double _size = 1;
        CMovingBitmap _collision_box;
        int _OFFSET_X = 20; // for the collision box
        int _OFFSET_Y = 10; // for the collision box
        int _width, _height;			// of the collision box

        /* Animations */
        vector<vector<CAnimation>> _aniByWpn;
        int _currentAniByWpn;

        /* Triggered Animations */
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
        // The index of the triggered animation in the 2-dimensional vector '_aniByWpn[_wpnID][---index---]',
        // Its value is one of the constants starting with 'ANI_WPN_ID_...'
        // Technically, the animation needed in 'OnShow()' is defined by '_aniByWpn[_wpnID][_triggeredAniAnimationID]'

        bool _isInitiatedTriggeredAni;

        int _lastTriggeredAniKeyID;

        int _lastTriggeredAniByWpnID;

        int _finishedTriggeredAniKeyID;

        vector<TriggeredAnimation*> _triggeredAnis;

        /* Movements */
        // Keys
        vector<long> _keys; // 0 - up, 1 - right, 2 - down, 3 - left, 4 - attack, 5 - dodge
        bool _isPressingLeft, _isPressingRight, _isPressingDown, _isTriggerPressingLeft;
        bool _dir; // false: player facing left, true: player facing right
        double _moveVelocity;

        // Offset left/ right
        bool _isOffsetLeft, _isOffsetRight;
        double _horizontalVelocity;
        // Jump
        bool _isTriggerJump;
        int _jumpCount;
        // Gravity
        double _verticalVelocity;
        double _verticalAcceleration;

        /* Respawn */
        Vector2 _vectorRespawnMovement;
        int _resDestPosX, _resDestPosY;
        double _preDistance; // used in combination with _vectorRespawnMovement
        RespawnCourier _respawnCourier;
        bool _allowRespawn;

        /* States */
        int _state;
        PlayerConsciousState _consciousState;
        PlayerUnconsciousState _unconsciousState;
        PlayerRespawnState _respawnState;
        PlayerImmuneState _immuneState;

        /* Others */
        vector<vector<int>*> bmp_iter;	// used to display current animation state in DEBUG mode
        bool _isTriggerDrawWeapon;
        bool _isHoldingWeapon;
        bool _isTriggerAttack;
        int MAX_LIFE = 3;
        vector<Player*> _attackList;
        vector<Player*>* _playersPtr;
        int _identifier;
        bool _isTriggerDodge;
        bool _isDodging;
        vector<Ground*> _grounds;
        int _life;
        string _name;
        Camera* camera;
        int _wpnID; // 0 - punch (default), 1 - sword 1, 2 - sword 2
        int _roundPrevPickedUpWpnID; // The previously picked up weapon in the round. Values: 1 - sword 1, 2 - sword 2 (initialized as 2)
        int _unconsciousFramesCount;
        bool _unconsciousAniDir;
        bool _isFirstTimeOnEdge;
        vector<Player*> _hitTargetPlayers;
        BattleSystem* _battleSystemPtr;
        ExplosionEffect* _explosionEffectPtr;
        Player* _attacker;
        int _attackerAffectionFrameCount;
        bool _isDead;
        vector<Weapon*>* _weaponsPtr;

        /* Others that require comments */
        int _takenDmg;
        // The taken damage will determine how far the target player would fly 'attackOffsetMagnitude', and
        // how long he would be in the unconscious state '_unconsciousFramesCount'

        int _currentKeyID;
        // The current "KeyID" (short for "ID of the combination of key pressed") being activated by the user
        // Its value is one of the constants starting with 'KEY_...'
        // Do note that this value is unique!!!

};
#endif
}