/* File description:
This is the header file 'DodgeTriggeredAnimation.h'. This is the API for the class 'DodgeTriggeredAnimation'. */

#include "TriggeredAnimation.h"

namespace game_framework
{
#ifndef DODGE_TRIGGERED_ANIMATION_H
#define DODGE_TRIGGERED_ANIMATION_H
class DodgeTriggeredAnimation : TriggeredAnimation
{
    public:
        virtual ~DodgeTriggeredAnimation();
        //Destructor

        DodgeTriggeredAnimation();
        //Default constructor

        DodgeTriggeredAnimation(const DodgeTriggeredAnimation& objectValue);
        //Copy constructor

        DodgeTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue);
        //Full constructor

        DodgeTriggeredAnimation& operator=(const DodgeTriggeredAnimation& rightObject);
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