/* File description:
This is the header file 'GroundMoveRightAttackTriggeredAnimation.h'. This is the API for the class 'GroundMoveRightAttackTriggeredAnimation'. */

#include "TriggeredAnimation.h"

namespace game_framework
{
#ifndef GROUND_MOVE_RIGHT_ATTACK_TRIGGERED_ANIMATION_H
#define GROUND_MOVE_RIGHT_ATTACK_TRIGGERED_ANIMATION_H
	class GroundMoveRightAttackTriggeredAnimation : TriggeredAnimation
	{
	public:
		virtual ~GroundMoveRightAttackTriggeredAnimation();
		//Destructor

		GroundMoveRightAttackTriggeredAnimation();
		//Default constructor

		GroundMoveRightAttackTriggeredAnimation(const GroundMoveRightAttackTriggeredAnimation& objectValue);
		//Copy constructor

		GroundMoveRightAttackTriggeredAnimation(Player* const& playerPtrValue, const int& keyIDValue);
		//Full constructor

		GroundMoveRightAttackTriggeredAnimation& operator=(const GroundMoveRightAttackTriggeredAnimation& rightObject);
		//Operator overloading function of the assignment operator

		void FinishTriggeredAnimationGameLogic() override;

		void SetTriggeredAnimationVariables() override;

		void InitiateTriggeredAction() override;

		void DoTriggeredAction() override;

	private:

	};
#endif
}