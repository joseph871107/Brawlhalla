/* File description:
This is the header file 'GroundLandDownAttackTriggeredAnimation.h'. This is the API for the class 'GroundLandDownAttackTriggeredAnimation'. */

#include "TriggeredAnimation.h"

namespace game_framework
{
#ifndef GROUND_LAND_DOWN_ATTACK_TRIGGERED_ANIMATION_H
#define GROUND_LAND_DOWN_ATTACK_TRIGGERED_ANIMATION_H
class GroundLandDownAttackTriggeredAnimation : TriggeredAnimation
{
    public:
        virtual ~GroundLandDownAttackTriggeredAnimation();
        //Destructor

        GroundLandDownAttackTriggeredAnimation();
        //Default constructor

        GroundLandDownAttackTriggeredAnimation(const GroundLandDownAttackTriggeredAnimation& objectValue);
        //Copy constructor

        GroundLandDownAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue);
        //Full constructor

        GroundLandDownAttackTriggeredAnimation& operator=(const GroundLandDownAttackTriggeredAnimation& rightObject);
        //Operator overloading function of the assignment operator

        void FinishTriggeredAnimationGameLogic() override;

        void SetTriggeredAnimationVariables() override;

        void InitiateTriggeredAction() override;

        void DoTriggeredAction() override;

    private:

};
#endif
}