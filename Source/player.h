#include "ground.h"
#include "weapon.h"
#include "camera.h"
#include "Vector2.h"

#define _PLAYER_DEBUG false

namespace game_framework
{
#ifndef PLAYER_H
#define PLAYER_H
class Player
{
    public:
        //Essential functions for C++ class
        Player();						//Default constructor
        ~Player();						//Destructor

        //Required for Game Framework
        void Initialize(vector<Ground*> groundsValue, vector<Player*>* playersPtrValue, string nameValue, vector<long> keysValue);
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void OnKeyDown(const UINT& nChar);
        void OnKeyUp(const UINT& nChar);

        //Others - Joseph
        void SetHoldWeapon(bool);
        void BeenAttacked(Vector2 displaymentVector, bool beingAttackedDirection);
        bool GetHoldWeapon();
        bool GetDirection();
        int GetCor(int);				// 物件座標 0:左上X, 1:左上Y, 2:右下X, 3:右下Y
        int GetWidth();
        int GetHeight();
        void AddCamera(Camera* cam);	// Camera

        //Others - Bill
        const string& GetName() const;
        const int& GetLife() const;
        const bool IsOutOfLife() const;
        const long& GetAttackKey() const;
        void GenerateAndSetWeaponID();	// Called by 'Weapon::HitPlayer()' when the player picks up a weapon
        void ResetWeaponID();
        void PerformAttack(Player* targetPlayer, bool attackDirection);

    private:
        //-----------------FUNCTIONS DECLARATIONS-----------------//
        //Animations
        void AddCAnimation(vector<int>*, double = 1.0, int = 10, bool = true, int = 1); // Push (bmps, (optional)size, (op)delay, (op)repeat, (op)repeat times) in vector of CAnimation
        void ResetAnimations(int animationID);

        void SetAnimationStateLeftRight(int leftAnimationId);
        void SetAnimationState(int);	// Set which CAnimation is going to play
        void ShowCurrentAnimation();			// Show CAnimation by currentAni

        //Reposition about the grounds
        bool IsIntersectGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2);
        bool IsExplicitlyVerticallyIntersectGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2);
        bool IsExplicitlyHorizontallyIntersectGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2);

        bool IsOnGroundLeftEdge(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2);
        bool IsOnGroundRightEdge(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2);
        bool IsOnGroundUnderside(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2);
        bool IsOnParticularGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2);

        void DoRepositionAboutGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2);

        //Position
        bool IsOnGround();				// Return 'true' if the player is on any ground of all grounds
        bool IsOnLeftEdge();
        bool IsOnRightEdge();
        bool IsOutMapBorder();

        //Offsets
        void InitiateOffsetUp(double initialOffsetVelocityMagnitude);
        void InitiateOffsetDown(double initialOffsetVelocityMagnitude);
        void InitiateOffsetLeft(double initialOffsetVelocityMagnitude);
        void InitiateOffsetRight(double initialOffsetVelocityMagnitude);

        bool IsBeingOffsetHorizontally();
        void DoHorizontalOffset();

        //Movements
        void DoMoveLeft(int movementUnit);
        void DoMoveRight(int movementUnit);
        void DoLand();

        //Jump
        void DoJump();
        void ResetJumpCount();

        //Wall jump
        void InitiateWallJump();

        //Attack
        void DoAttack();
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

        //Bounce Off the Ground
        void DoBounceOffGround(int playerX1, int playerY1, int playerX2, int playerY2, int groundX1, int groundY1, int groundX2, int groundY2);

        //Others
        void DoDead();
        void DoRespawn();
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
        void SetFirstThreeTriggeredAnimationVariables(int keyCombInt);
		void SetTriggeredAnimation(bool newIsTriggeredAni);
		void SetTriggeredAnimationKeyID(int newTriggeredAniKeyID);
		void SetTriggeredAnimationDir(bool newTriggeredAniDir);
		void SetTriggeredAnimationVariables(int keyCombInt);

        void InitiateTriggeredAnimation();
        void DoTriggeredAnimation();
        bool IsFinishedTriggeredAnimation();

        void DoNonTriggeredAnimation();

        //Edges
        bool IsOnEdge();
        bool IsFirstTimeOnEdge();
        void InitiateOnEdge();
        void DoOnEdge();
        void DoLeaveEdge();

        //Ground
        void DoOnGround();

        //
        void SetAnimationSelector(bool newAniSelector);
        void SetTriggeredAnimationSelector();
        void SetNonTriggeredAnimationSelector();
        void SetCurrentAnimation();
		void UnconsciouslyOnMoveAnimationLogic();
		void FinishTriggeredAnimationAnimationLogic();
		void FinishTriggeredAnimationGameLogic();
		void ConsciouslyOnMoveAnimationLogic();
		void MoveCurrentAnimation();
		void OnMoveAnimationLogic();
		void OnMoveGameLogic();
        void SetCurrentNonTriggeredAnimationByWeapon();
        void SetCurrentTriggeredAnimationByWeapon();
        void SetCurrentTriggeredAnimation();
		void SetCurrentAniByWeapon();
		void SetCurrentAni();
        void SetCurrentNonTriggeredAnimation();

        //-----------------VARIABLES DECLARATIONS-----------------//
        //Required for Game Framework
        int _x, _y;						// position of the collision's box
        vector<CAnimation> ani;			// vector of CAnimation
        int currentAni;					// current running CAnimation
        //bool _beInterrupt;
        vector<vector<int>*> bmp_iter;	// used to display current animation state in DEBUG mode

        //Required for "physical" existence in the game
        CMovingBitmap _collision_box;
        int _width, _height;			// of the collision box

        //Keys
        vector<long> _keys; // 0 - up, 1 - right, 2 - down, 3 - left, 4 - attack, 5 - dodge

        //Offsets
        bool _isOffsetLeft, _isOffsetRight;
        double _horizontalVelocity;

        //Movements
        bool _isPressingLeft, _isPressingRight, _isPressingDown;
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

		//Trow weapon
		Weapon *_flyingWeapon;
		void DeleteFlyingWeapon();
        // The taken damage will determine how far the target player would fly 'attackOffsetMagnitude', and
        // how long he would be in the unconscious state '_unconsciousFramesCount'

        vector<Player*>* _playersPtr;

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
        // depending on the value of '_aniSelector'
        // Its value is one of the constants starting with 'ANI_ID_...' or 'ANI_WPN_ID_...'
        // Technically, the animation needed in 'OnShow()' is defined by 'ani[_triggeredAniAnimationID]' or '_aniByWpn[_wpnID][_triggeredAniAnimationID]',
        // depending on the value of '_aniSelector'

        bool _isInitiatedTriggeredAni;

        int _lastTriggeredAniKeyID;

        int _lastTriggeredAniByWpnID;

		int _finishedTriggeredAniKeyID;

        //Weapon
        int _wpnID; // 0 - punch (default), 1 - sword 1, 2 - sword 2
        int _roundPrevPickedUpWpnID; // The previously picked up weapon in the round. Values: 1 - sword 1, 2 - sword 2 (initialized as 2)

        //Animation By Weapons
        /// Remark: Technically, if '_aniSelector' is true, '_aniByWpn' is displayed instead of the traditional 'ani', and vice versa.
        vector<vector<CAnimation>> _aniByWpn;
        int _currentAniByWpn;
        bool _aniSelector;
        // False - choose 'ani' for showing the animation
        // True - choose '_aniByWpn' for showing the animation
        // This variable is determined in 'Player::OnMove()'

        //Unconscious state
        bool _isUnconscious;
        int _unconsciousFramesCount;
        bool _unconsciousAniDir;

        //Edges
        bool _isFirstTimeOnEdge;

		// Others
		int _currentKeyID;
		// The current "KeyID" (short for "ID of the combination of key pressed") being activated by the user
		// Its value is one of the constants starting with 'KEY_...'
		// Do note that this value is unique!!!
};
#endif
}