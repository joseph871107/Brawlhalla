/* File description:
This is the header file 'AirLandDownAttackTriggeredAnimation.h'. This is the API for the class 'AirLandDownAttackTriggeredAnimation'. */

#include "TriggeredAnimation.h"

namespace game_framework
{
#ifndef AIR_LAND_DOWN_ATTACK_TRIGGERED_ANIMATION_H
#define AIR_LAND_DOWN_ATTACK_TRIGGERED_ANIMATION_H
class AirLandDownAttackTriggeredAnimation : TriggeredAnimation
{
    public:
        virtual ~AirLandDownAttackTriggeredAnimation();
        //Destructor

        AirLandDownAttackTriggeredAnimation();
        //Default constructor

        AirLandDownAttackTriggeredAnimation(const AirLandDownAttackTriggeredAnimation& objectValue);
        //Copy constructor

        AirLandDownAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue);
        //Full constructor

        AirLandDownAttackTriggeredAnimation& operator=(const AirLandDownAttackTriggeredAnimation& rightObject);
        //Operator overloading function of the assignment operator

        void FinishTriggeredAnimationGameLogic() override;

        void SetTriggeredAnimationVariables() override;

        void InitiateTriggeredAction() override;

        void DoTriggeredAction() override;

    private:

};
#endif
}
