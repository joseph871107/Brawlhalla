/* File description:
This is the header file 'PlayerRespawnState.h'. This is the API for the class 'PlayerRespawnState'. */

namespace game_framework
{
#ifndef PLAYER_RESPAWN_STATE_H
#define PLAYER_RESPAWN_STATE_H
class Player;

class PlayerRespawnState
{
    public:
        virtual ~PlayerRespawnState();
        //Destructor

        PlayerRespawnState();
        //Default constructor

        PlayerRespawnState(const PlayerRespawnState& objectValue);
        //Copy constructor

        PlayerRespawnState(Player* const playerPtrValue);
        //Full constructor

        PlayerRespawnState& operator=(const PlayerRespawnState& rightObject);
        //Operator overloading function of the assignment operator

        void OnMoveGameLogic();
        void OnMoveAnimationLogic();

    private:
        Player* _playerPtr;

};
#endif
}