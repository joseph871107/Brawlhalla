/* File description:
This is the header file 'PlayerUnconsciousState.h'. This is the API for the class 'PlayerUnconsciousState'. */

namespace game_framework
{
#ifndef PLAYER_UNCONSCIOUS_STATE_H
#define PLAYER_UNCONSCIOUS_STATE_H
class Player;

class PlayerUnconsciousState
{
    public:
        virtual ~PlayerUnconsciousState();
        //Destructor

        PlayerUnconsciousState();
        //Default constructor

        PlayerUnconsciousState(const PlayerUnconsciousState& objectValue);
        //Copy constructor

        PlayerUnconsciousState(Player* const playerPtrValue);
        //Full constructor

        PlayerUnconsciousState& operator=(const PlayerUnconsciousState& rightObject);
        //Operator overloading function of the assignment operator

		void OnMoveGameLogic();
		void DoBounceOffGround(int playerX1, int playerY1, int playerX2, int playerY2, Ground* groundPtr);
		void OnMoveAnimationLogic();

    private:
        Player* _playerPtr;
};
#endif
}