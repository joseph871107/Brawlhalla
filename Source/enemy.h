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
        void DoAttack(vector<Player*>::iterator target);
		virtual bool ChaseTarget(CPoint point, int width = 0, int height = 0);
        virtual void OnMove();
		virtual void SetAnimation();
    protected:
		vector<Weapon*>::iterator WeaponNearby();
		vector<Player*>::iterator PlayerNearby(vector<Player*> *playerList);
		vector <Player*> attackList;
        int _difficulty;
};
#endif
}