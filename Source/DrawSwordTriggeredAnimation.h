/* File description:
This is the header file 'DrawSwordTriggeredAnimation.h'. This is the API for the class 'DrawSwordTriggeredAnimation'. */

#include "TriggeredAnimation.h"

namespace game_framework
{
#ifndef DRAW_SWORD_TRIGGERED_ANIMATION_H
#define DRAW_SWORD_TRIGGERED_ANIMATION_H
class DrawSwordTriggeredAnimation : TriggeredAnimation
{
    public:
        virtual ~DrawSwordTriggeredAnimation();
        //Destructor

        DrawSwordTriggeredAnimation();
        //Default constructor

        DrawSwordTriggeredAnimation(const DrawSwordTriggeredAnimation& objectValue);
        //Copy constructor

        DrawSwordTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue);
        //Full constructor

        DrawSwordTriggeredAnimation& operator=(const DrawSwordTriggeredAnimation& rightObject);
        //Operator overloading function of the assignment operator

        void FinishTriggeredAnimationGameLogic() override;

        void SetTriggeredAnimationVariables() override;

        void InitiateTriggeredAction() override;

        void DoTriggeredAction() override;

    private:

};
#endif
}