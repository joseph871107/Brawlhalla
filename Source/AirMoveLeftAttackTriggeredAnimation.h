/* File description:
This is the header file 'AirMoveLeftAttackTriggeredAnimation.h'. This is the API for the class 'AirMoveLeftAttackTriggeredAnimation'. */

#include "TriggeredAnimation.h"

namespace game_framework
{
#ifndef AIR_MOVE_LEFT_ATTACK_TRIGGERED_ANIMATION_H
#define AIR_MOVE_LEFT_ATTACK_TRIGGERED_ANIMATION_H
class AirMoveLeftAttackTriggeredAnimation : TriggeredAnimation
{
    public:
        virtual ~AirMoveLeftAttackTriggeredAnimation();
        //Destructor

        AirMoveLeftAttackTriggeredAnimation();
        //Default constructor

        AirMoveLeftAttackTriggeredAnimation(const AirMoveLeftAttackTriggeredAnimation& objectValue);
        //Copy constructor

        AirMoveLeftAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue);
        //Full constructor

        AirMoveLeftAttackTriggeredAnimation& operator=(const AirMoveLeftAttackTriggeredAnimation& rightObject);
        //Operator overloading function of the assignment operator

        void FinishTriggeredAnimationGameLogic() override;

        void SetTriggeredAnimationVariables() override;

        void InitiateTriggeredAction() override;

        void DoTriggeredAction() override;

    private:

};
#endif
}