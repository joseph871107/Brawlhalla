#include "ground.h"
#include "weapon.h"
#include "camera.h"
#include "player.h"

namespace game_framework
{
#ifndef ENEMY_H
#define ENEMY_H
class Enemy : public Player
{
    public:
        Enemy();
        Enemy(int diff);
        void DoAttack();
		bool ChaseTarget(CPoint point, int width = 0, int height = 0);
        void OnMove();
    private:
		vector<Weapon*>::iterator WeaponNearby();
        Player* target;
        int _difficulty;
};
#endif
}