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
        void ChaseTarget();
        void OnMove();
    private:
        Player* target;
        int _difficulty;
};
#endif
}