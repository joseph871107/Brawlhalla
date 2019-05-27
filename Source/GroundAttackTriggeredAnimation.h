/* File description:
This is the header file 'GroundAttackTriggeredAnimation.h'. This is the API for the class 'GroundAttackTriggeredAnimation'. */

#include "TriggeredAnimation.h"

namespace game_framework
{
#ifndef GROUND_ATTACK_TRIGGERED_ANIMATION_H
#define GROUND_ATTACK_TRIGGERED_ANIMATION_H
class GroundAttackTriggeredAnimation : TriggeredAnimation
{
    public:
        virtual ~GroundAttackTriggeredAnimation();
        //Destructor

        GroundAttackTriggeredAnimation();
        //Default constructor

        GroundAttackTriggeredAnimation(const GroundAttackTriggeredAnimation& objectValue);
        //Copy constructor

        GroundAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue);
        //Full constructor

        GroundAttackTriggeredAnimation& operator=(const GroundAttackTriggeredAnimation& rightObject);
        //Operator overloading function of the assignment operator

		void FinishTriggeredAnimationGameLogic() override;

		void SetTriggeredAnimationVariables() override;

		void InitiateTriggeredAction() override;

		void DoTriggeredAction() override;

    private:

};
#endif
}