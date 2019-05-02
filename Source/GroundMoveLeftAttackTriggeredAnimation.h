/* File description:
This is the header file 'GroundMoveLeftAttackTriggeredAnimation.h'. This is the API for the class 'GroundMoveLeftAttackTriggeredAnimation'. */

#include "TriggeredAnimation.h"

namespace game_framework
{
#ifndef GROUND_MOVE_LEFT_ATTACK_TRIGGERED_ANIMATION_H
#define GROUND_MOVE_LEFT_ATTACK_TRIGGERED_ANIMATION_H
class GroundMoveLeftAttackTriggeredAnimation : TriggeredAnimation
{
    public:
        virtual ~GroundMoveLeftAttackTriggeredAnimation();
        //Destructor

        GroundMoveLeftAttackTriggeredAnimation();
        //Default constructor

        GroundMoveLeftAttackTriggeredAnimation(const GroundMoveLeftAttackTriggeredAnimation& objectValue);
        //Copy constructor

        GroundMoveLeftAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue);
        //Full constructor

        GroundMoveLeftAttackTriggeredAnimation& operator=(const GroundMoveLeftAttackTriggeredAnimation& rightObject);
        //Operator overloading function of the assignment operator

        void SetTriggeredAnimationSelector() override;

        void FinishTriggeredAnimationGameLogic() override;

        void SetTriggeredAnimationVariables() override;

        void InitiateTriggeredAction() override;

        void DoTriggeredAction() override;

    private:

};
#endif
}