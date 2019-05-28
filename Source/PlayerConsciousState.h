/* File description:
This is the header file 'PlayerConsciousState.h'. This is the API for the class 'PlayerConsciousState'. */

namespace game_framework
{
#ifndef PLAYER_CONSCIOUS_STATE_H
#define PLAYER_CONSCIOUS_STATE_H
class Player;

class PlayerConsciousState
{
    public:
        virtual ~PlayerConsciousState();
        //Destructor

        PlayerConsciousState();
        //Default constructor

        PlayerConsciousState(const PlayerConsciousState& objectValue);
        //Copy constructor

        PlayerConsciousState(Player* const playerPtrValue);
        //Full constructor

        PlayerConsciousState& operator=(const PlayerConsciousState& rightObject);
        //Operator overloading function of the assignment operator

        void OnMoveAnimationLogic();
        void OnMoveGameLogic();
		void SetCurrentNonTriggeredAnimationByWeapon();

    private:
        //-----------------FUNCTIONS DECLARATIONS-----------------//
        void DoRepositionAboutGround(int playerX1, int playerY1, int playerX2, int playerY2, Ground* groundPtr);
        void ModifyVerticalOffsetVariablesFunctions();
        void ProcessCurrentKeyCombinationGameLogic();
        bool IsFirstTimeOnEdge();
        void InitiateOnEdge();
        void DoOnEdge();
        void DoLeaveEdge();
        void DoOnGround();
        void InitiateTriggeredAction();
        void DoTriggeredAction();
        void DoNonTriggeredAction();
        void DoMoveRightWithAcceleration();
        void DoMoveLeftWithAcceleration();
        void DoMoveLeft(int movementUnit);
        void DoMoveRight(int movementUnit);
        void DoJump();
        void InitiateWallJump();
        //-----------------VARIABLES DECLARATIONS-----------------//
        Player* _playerPtr;

};
#endif
}