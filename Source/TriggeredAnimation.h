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

        virtual void SetTriggeredAnimationSelector() = 0;

        virtual void FinishTriggeredAnimationGameLogic() = 0;

        virtual void SetTriggeredAnimationVariables();

		virtual void InitiateTriggeredAction() = 0;

        virtual void DoTriggeredAction() = 0;

    protected:
        Player* _playerPtr;
        int _keyID;
        //Animations ID of '_ani'
        const int ANI_ID_RUN_LEFT = 0;
        const int ANI_ID_RUN_RIGHT = 1;
        const int ANI_ID_JUMP_LEFT = 2;
        const int ANI_ID_JUMP_RIGHT = 3;
        const int ANI_ID_STAND_LEFT = 4;
        const int ANI_ID_STAND_RIGHT = 5;
        const int ANI_ID_LEAN_LEFT = 6;
        const int ANI_ID_LEAN_RIGHT = 7;
        const int ANI_ID_LAND_FALL_LEFT = 8;
        const int ANI_ID_LAND_FALL_RIGHT = 9;
        const int ANI_ID_UNCONSCIOUS_FLYING_LEFT = 10;
        const int ANI_ID_UNCONSCIOUS_FLYING_RIGHT = 11;
        const int ANI_ID_DODGE_LEFT = 12;
        const int ANI_ID_DODGE_RIGHT = 13;
        //Animations ID of '_aniByWpn'
        const int ANI_WPN_ID_STAND_LEFT = 0;
        const int ANI_WPN_ID_STAND_RIGHT = 1;
        const int ANI_WPN_ID_ATTACK_LEFT = 2;
        const int ANI_WPN_ID_ATTACK_RIGHT = 3;
        const int ANI_WPN_ID_GND_MOVE_ATTACK_LEFT = 4;
        const int ANI_WPN_ID_GND_MOVE_ATTACK_RIGHT = 5;
        const int ANI_WPN_ID_SLIDE_ATTACK_LEFT = 6;
        const int ANI_WPN_ID_SLIDE_ATTACK_RIGHT = 7;
        const int ANI_WPN_ID_AIR_ATTACK_LEFT = 8;
        const int ANI_WPN_ID_AIR_ATTACK_RIGHT = 9;
        const int ANI_WPN_ID_AIR_MOVE_ATTACK_LEFT = 10;
        const int ANI_WPN_ID_AIR_MOVE_ATTACK_RIGHT = 11;
        const int ANI_WPN_ID_AIR_DOWN_ATTACK_LEFT = 12;
        const int ANI_WPN_ID_AIR_DOWN_ATTACK_RIGHT = 13;
        const int ANI_WPN_ID_DRAW_SWORD_LEFT = 14;
        const int ANI_WPN_ID_DRAW_SWORD_RIGHT = 15;
		//Others
		const int OFFSET_INITIAL_VELOCITY = 15;

};
#endif
}