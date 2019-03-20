#include "ground.h"

namespace game_framework
{
#ifndef PLAYER_H
#define PLAYER_H
class Player
{
    public:
        Player();			// Constructor
        ~Player();						// Deconstructor

        //Required for Game Framework
        void Initialize(Ground* groudPtrValue, int = 0);
        void LoadBitmap();
        void OnShow();
        void OnMove();
		void SetSize(double);
        void OnKeyDown(const UINT& nChar);
        void OnKeyUp(const UINT& nChar);
		void SetKeyMode(int = 0);

        //Functions getting coordinates
        int  GetX1();					// 玩家左上角 x 座標
        int  GetY1();					// 玩家左上角 y 座標
        int  GetX2();					// 玩家右下角 x 座標
        int  GetY2();					// 玩家右下角 y 座標

        //
        int ShowAnimationState();		// Return which CAnimation is playing

    private:
        //-----------------FUNCTIONS DECLARATIONS-----------------//
        //Movements
        void DoJump();
        void DoFall();
		void DoAttack();
        void DoOnGround();
        void DoOnEdge();
        bool IsOnGround();
        bool IsOnLeftEdge();
        bool IsOnRightEdge();
		void ResetJumpAnimations();
        void ResetJumpCount();
		void ResetAttackAnimations();

        //Animations
        void AddCAnimation(vector<int>*, double = 1.0, int = 5, bool = true, int = 1);
        // Push (bmps, (optional)size, (op)delay, (op)repeat, (op)repeat times) in vector of CAnimation

        void SetAnimationState(int);	// Set which CAnimation is going to play
        void ShowAnimation();			// Show CAnimation by currentAni


        //-----------------VARIABLES DECLARATIONS-----------------//
        //Required for Game Framework
        int _x, _y;
		double _size;
        vector<CAnimation> ani;			// vector of CAnimation
        int currentAni;					// current running CAnimation

        //Bitmaps
		vector<int> rl;	// bmps of running left
        vector<int> rr;	// bmps of running right
		vector<int> jl;	// bmps of jumping left
        vector<int> jr;	// bmps of jumping right
		vector<int> sl;	// bmps of standing left
        vector<int> sr;	// bmps of standing right
		vector<int> ll; //bmps of leaning left
		vector<int> lr; //bmps of leaning right
		vector<int> al; //bmps of attacking left
		vector<int> ar; //bmps of attacking right
        vector<vector<int>*> bmp_iter;

        //Required for "physical" existence in the game
        int _width, _height;
		int _keyMode;	//0 as test, 1 as P1, 2 as P3
		vector<bool> _keyModeBool;

        //[Attribute] Move left/ right
        bool _isMovingLeft, _isMovingRight;
        bool _dir; //false: player facing left, true: player facing right

        //[Attribute] Jump
        bool _isTriggerJump;
        int _jumpCount;
		//Wall Jump
		int _offsetVelocity;
		bool _isOffsetLeft, _isOffsetRight;
		//Attack
		bool _isAttacking;

        //Required for a jump simulating the physical world
        double _velocity;

        //Ground
        Ground* _ground;

};
#endif
}