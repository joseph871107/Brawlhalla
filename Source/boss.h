#include "ground.h"
#include "weapon.h"
#include "camera.h"
#include "enemy.h"

namespace game_framework
{
#ifndef BOSS_H
#define BOSS_H
class Boss : public Enemy
{
    public:
        Boss();
        Boss(int diff);
        virtual bool ChaseTarget(CPoint point, int width = 0, int height = 0);
        virtual void OnMove();
        virtual void OnShow() override;
        virtual void SetAnimation();

        virtual int GetSpecializedTakenDamage() const override;
        // Return the value used for calculating offset magnitude of the boss, as well as
        // the unconscious time of the boss

        virtual void EvaluateDeadAndRespawn() override;
		int GetHealth();
    private:
        int _health;
};
#endif
}