/* File description:
This is the header file 'TriggeredAnimation.h'. This is the API for the class 'TriggeredAnimation'. */

namespace game_framework
{
#ifndef TRIGGERED_ANIMATION_H
#define TRIGGERED_ANIMATION_H
class Player;

class TriggeredAnimation
{
    public:
        virtual ~TriggeredAnimation();
        //Destructor

        TriggeredAnimation();
        //Default constructor

        TriggeredAnimation(const TriggeredAnimation& objectValue);
        //Copy constructor

        TriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue);
        //Full constructor

        TriggeredAnimation& operator=(const TriggeredAnimation& rightObject);
        //Operator overloading function of the assignment operator

		const int& GetKeyID() const;

        virtual void FinishTriggeredAnimationGameLogic() = 0;

        virtual void SetTriggeredAnimationVariables();

		virtual void InitiateTriggeredAction() = 0;

        virtual void DoTriggeredAction() = 0;

    protected:
        Player* _playerPtr;
        int _keyID;

};
#endif
}