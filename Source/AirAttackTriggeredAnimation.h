/* File description:
This is the header file 'AirAttackTriggeredAnimation.h'. This is the API for the class 'AirAttackTriggeredAnimation'. */

#include "TriggeredAnimation.h"

namespace game_framework
{
#ifndef AIR_ATTACK_TRIGGERED_ANIMATION_H
#define AIR_ATTACK_TRIGGERED_ANIMATION_H
class AirAttackTriggeredAnimation : TriggeredAnimation
{
    public:
        virtual ~AirAttackTriggeredAnimation();
        //Destructor

        AirAttackTriggeredAnimation();
        //Default constructor

        AirAttackTriggeredAnimation(const AirAttackTriggeredAnimation& objectValue);
        //Copy constructor

        AirAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue);
        //Full constructor

        AirAttackTriggeredAnimation& operator=(const AirAttackTriggeredAnimation& rightObject);
        //Operator overloading function of the assignment operator

        void FinishTriggeredAnimationGameLogic() override;

        void SetTriggeredAnimationVariables() override;

        void InitiateTriggeredAction() override;

        void DoTriggeredAction() override;

    private:

};
#endif
}