/* File description:
This is the header file 'PlayerImmuneState.h'. This is the API for the class 'PlayerImmuneState'. */

namespace game_framework
{
#ifndef PLAYER_IMMUNE_STATE_H
#define PLAYER_IMMUNE_STATE_H
class Player;

class PlayerImmuneState : public PlayerConsciousState
{
    public:
        virtual ~PlayerImmuneState();
        //Destructor

        PlayerImmuneState();
        //Default constructor

        PlayerImmuneState(const PlayerImmuneState& objectValue);
        //Copy constructor

        PlayerImmuneState(Player* const playerPtrValue, int frameCounterValue);
        //Full constructor

        PlayerImmuneState& operator=(const PlayerImmuneState& rightObject);
        //Operator overloading function of the assignment operator

        void OnMoveGameLogic();
        const int& GetFrameCounter() const;

    private:
        int _frameCounter;

};
#endif
}