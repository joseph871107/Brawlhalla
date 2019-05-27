/* File description:
This is the header file 'AirMoveRightAttackTriggeredAnimation.h'. This is the API for the class 'AirMoveRightAttackTriggeredAnimation'. */

#include "TriggeredAnimation.h"

namespace game_framework
{

#ifndef AIR_MOVE_RIGHT_ATTACK_TRIGGERED_ANIMATION_H
#define AIR_MOVE_RIGHT_ATTACK_TRIGGERED_ANIMATION_H
class AirMoveRightAttackTriggeredAnimation : TriggeredAnimation
{
    public:
        virtual ~AirMoveRightAttackTriggeredAnimation();
        //Destructor

        AirMoveRightAttackTriggeredAnimation();
        //Default constructor

        AirMoveRightAttackTriggeredAnimation(const AirMoveRightAttackTriggeredAnimation& objectValue);
        //Copy constructor

        AirMoveRightAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue);
        //Full constructor

        AirMoveRightAttackTriggeredAnimation& operator=(const AirMoveRightAttackTriggeredAnimation& rightObject);
        //Operator overloading function of the assignment operator

        void FinishTriggeredAnimationGameLogic() override;

        void SetTriggeredAnimationVariables() override;

        void InitiateTriggeredAction() override;

        void DoTriggeredAction() override;

    private:

};
#endif
}