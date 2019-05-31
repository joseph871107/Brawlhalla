#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "player.h"
#include "battleSystem.h"
#include "enemy.h"
namespace game_framework
{
const vector<long> keys{ KEY_W, KEY_D, KEY_S, KEY_A, KEY_C, KEY_F, KEY_X };

Enemy::Enemy()
{
	_identifier = PLAYER_MODE_ENEMY;
    _difficulty = 0;
}

Enemy::Enemy(int diff)
{
	_identifier = PLAYER_MODE_ENEMY;
	_difficulty = diff;
}

void Enemy::DoAttack(vector<Player*> attackList, vector<Player*>::iterator target)
{
	static int counter = 0;
	if (attackList.size() == 0)
		return;
	if (IsCollide(GetCor(0), GetCor(1), GetCor(2), GetCor(3), (*target)->GetCor(0), (*target)->GetCor(1), (*target)->GetCor(2), (*target)->GetCor(3)))
		counter++;
	if (counter > (5 - _difficulty) * 5) {
		OnKeyDown(keys[4]);
		counter = 0;
	}
	else
		OnKeyUp(keys[4]);
}
bool Enemy::ChaseTarget(CPoint point, int width, int height)
{
	Ground *min = *_grounds.begin(), *max = min;
	for (auto i : _grounds) {
		if (min->GetCor(0) > i->GetCor(0))
			min = i;
		if (max->GetCor(0) < i->GetCor(0))
			max = i;
	}
	//
	bool tri = true;
	int offset = (int)(BITMAP_SIZE * _difficulty * 10);
	if (GetCor(0) > point.x + width && min->GetCor(0) + 60 < GetCor(0)) {	// Go left
		OnKeyDown(keys[3]);
		tri = false;
	}
    else
        OnKeyUp(keys[3]);
	//
    if (GetCor(2) < point.x && max->GetCor(2) - 60 > GetCor(2)) {	// Go right
        OnKeyDown(keys[1]);
		tri = false;
	}
    else
        OnKeyUp(keys[1]);
	//
    if (GetCor(1) - point.y > 10 && abs(GetCor(0) - point.x) < GetWidth() * 1.5 || 	// Do jump
		(GetCor(1) - point.y > 10 && abs(GetCor(0) - point.x) < GetWidth() * 1.5) ||
		IsOnEdge() || 
		(IsOnGround() && (OnGround()->GetCor(2) - 10 <= GetCor(2) || OnGround()->GetCor(0) +10 >= GetCor(0))) || 
		_isTriggerJump )
	{
        OnKeyDown(keys[0]);
		tri = false;
	}
    else
        OnKeyUp(keys[0]);
	//
	return tri;
}

void Enemy::OnMove()
{
    static int counter = 0, ptr = 0;
	CPoint targetPos;
	if (!_isHoldingWeapon && weapons->size()>0) {
		vector<Weapon*>::iterator tar = WeaponNearby();
		if (ChaseTarget(CPoint((*tar)->GetCor(0), (*tar)->GetCor(3)), (*tar)->GetWidth(), (*tar)->GetHeight())) {
			(*tar)->OnKeyDown(KEY_C);
			delete *tar;
			weapons->erase(tar);
		}
	}
	else {
		vector<Player*>::iterator targ;
		if (_attackList.size() == 0) {
			targ = PlayerNearby(_playersPtr);
			if (targ != _playersPtr->end())
				ChaseTarget(CPoint((*targ)->GetCor(0), (*targ)->GetCor(3)), (*targ)->GetWidth(), (*targ)->GetHeight());
			DoAttack(*_playersPtr, targ);
		}
		else{
			targ = PlayerNearby(&_attackList);
			if (targ != _attackList.end())
				ChaseTarget(CPoint((*targ)->GetCor(0), (*targ)->GetCor(3)), (*targ)->GetWidth(), (*targ)->GetHeight());
			DoAttack(_attackList, targ);
		}
	}
	Player::OnMove();
}
void Enemy::SetAnimation()
{
	_OFFSET_Y = 30;
	/*
	vector<CPoint> r = vector<CPoint>{ CPoint(2, 6), CPoint(2, 7), CPoint(2, 8) };	// bmps of running
	vector<CPoint> j = vector<CPoint>{ CPoint(2, 5) };	// bmps of jumping
	vector<CPoint> s = vector<CPoint>{ CPoint(0, 0), CPoint(0, 1), CPoint(0, 2), CPoint(0, 3) };	// bmps of standing
	vector<CPoint> l = vector<CPoint>{ CPoint(2, 0) }; // bmps of leaning
	vector<CPoint> lf = vector<CPoint>{ CPoint(1, 5), CPoint(1, 4) }; // bmps of landing fallin
	vector<CPoint> uf = vector<CPoint>{ CPoint(0, 4), CPoint(0, 5), CPoint(0, 6) }; // bmps of unconsciously flying
	vector<CPoint> dg = vector<CPoint>{ CPoint(4, 0), CPoint(4, 1), CPoint(4, 2), CPoint(4, 3) }; // bmps of dodging
	/// Comment for future devs: I duplicate the bitmaps for longer animation duration, which is dirty, should be improved
	AddCAnimationWithSprite(&ani, &julian_l0, &r, BITMAP_SIZE); //ani[0] Run Left
	AddCAnimationWithSprite(&ani, &julian_r0, &r, BITMAP_SIZE); //ani[1] Run Right
	AddCAnimationWithSprite(&ani, &julian_l0, &j, BITMAP_SIZE, 5, false); //ani[2] Jump Left
	AddCAnimationWithSprite(&ani, &julian_r0, &j, BITMAP_SIZE, 5, false); //ani[3] Jump Right
	AddCAnimationWithSprite(&ani, &julian_l0, &s, BITMAP_SIZE); //ani[4] Stand (Idle) Left
	AddCAnimationWithSprite(&ani, &julian_r0, &s, BITMAP_SIZE); //ani[5] Stand (Idle) Right
	AddCAnimationWithSprite(&ani, &julian_r0, &l, BITMAP_SIZE); //ani[6] Lean Left
	AddCAnimationWithSprite(&ani, &julian_l0, &l, BITMAP_SIZE); //ani[7] Lean Right
	AddCAnimationWithSprite(&ani, &julian_l0, &lf, BITMAP_SIZE); //ani[8] Landing Falling Left
	AddCAnimationWithSprite(&ani, &julian_r0, &lf, BITMAP_SIZE); //ani[9] Landing Falling Right
	AddCAnimationWithSprite(&ani, &julian_r1, &uf, BITMAP_SIZE); //ani[10] Unconsciously Flying Left
	AddCAnimationWithSprite(&ani, &julian_l1, &uf, BITMAP_SIZE); //ani[11] Unconsciously Flying Right
	AddCAnimationWithSprite(&ani, &julian_l0, &dg, BITMAP_SIZE, 15); //ani[12] Dodging Left
	AddCAnimationWithSprite(&ani, &julian_r0, &dg, BITMAP_SIZE, 15); //ani[13] Dodging Right
	*/
	//-----------------ANIMATION BY WEAPONS-----------------//
	_aniByWpn = vector<vector<CAnimation>>();
	vector<CPoint> s;	// bmps of standing
	vector<CPoint> a;	// bmps of attacking
	vector<CPoint> gma;	// bmps of on-ground-moving attack
	vector<CPoint> sa;	// bmps of slide-attack
	vector<CPoint> aa;	// bmps of air-attack
	vector<CPoint> ama;	// bmps of on-air-moving attack
	vector<CPoint> ada;	// bmps of on-air-down attack
	vector<CPoint> sd;	// bmps of drawing sword
	vector<CPoint> r;	// bmps of running
	vector<CPoint> j;	// bmps of jumping
	vector<CPoint> l;	// bmps of leaning
	vector<CPoint> lf;	// bmps of landing falling
	vector<CPoint> uf;	// bmps of unconsciously flying
	vector<CPoint> dg;	// bmps of dodging
	// ~
	// ~ Weapon 0 - default
	s = vector<CPoint>{ CPoint(0, 0), CPoint(0, 1), CPoint(0, 2), CPoint(0, 3) };	// bmps of standing
	a = vector<CPoint>{ CPoint(1, 0), CPoint(1, 1), CPoint(1, 2), CPoint(1, 3) };
	gma = vector<CPoint>{ CPoint(0, 1), CPoint(0, 2), CPoint(0, 3), CPoint(0, 3) };
	sa = vector<CPoint>{ CPoint(1, 2), CPoint(1, 2), CPoint(1, 3), CPoint(1, 4), CPoint(1, 5), CPoint(1, 6), CPoint(1, 7), CPoint(1, 8) };
	aa = vector<CPoint>{ CPoint(4, 3), CPoint(4, 4), CPoint(4, 5), CPoint(4, 6), CPoint(4, 7), CPoint(4, 8) };
	ama = vector<CPoint>{ CPoint(0, 0) };
	ada = vector<CPoint>{ CPoint(1, 5), CPoint(1, 6), CPoint(1, 7), CPoint(1, 8), CPoint(1, 9) };
	sd = vector<CPoint>{ CPoint(3, 3), CPoint(3, 4), CPoint(3, 5), CPoint(3, 6), CPoint(3, 7), CPoint(3, 8), CPoint(3, 9) };
	r = vector<CPoint>{ CPoint(2, 6), CPoint(2, 7), CPoint(2, 8) };	// bmps of running
	j = vector<CPoint>{ CPoint(2, 5) };	// bmps of jumping
	l = vector<CPoint>{ CPoint(2, 0) }; // bmps of leaning
	lf = vector<CPoint>{ CPoint(1, 5), CPoint(1, 4) }; // bmps of landing fallin
	uf = vector<CPoint>{ CPoint(0, 4), CPoint(0, 5), CPoint(0, 6) }; // bmps of unconsciously flying
	dg = vector<CPoint>{ CPoint(4, 0), CPoint(4, 1), CPoint(4, 2), CPoint(4, 3) }; // bmps of dodging
	vector<CAnimation> tempAniByWpn = vector<CAnimation>();
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &s, BITMAP_SIZE); //ani[0] Stand (Idle) Left with sword
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &s, BITMAP_SIZE); //ani[1] Stand (Idle) Right with sword
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &a, BITMAP_SIZE, 5, false); //ani[2] Attack Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &a, BITMAP_SIZE, 5, false); //ani[3] Attack Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l1, &gma, BITMAP_SIZE, 5, false); //ani[4] On-Ground-Moving Attack Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r1, &gma, BITMAP_SIZE, 5, false); //ani[5] On-Ground-Moving Attack Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l2, &sa, BITMAP_SIZE, 3, false); //ani[6] Slide Attack Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r2, &sa, BITMAP_SIZE, 3, false); //ani[7] Slide Attack Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l2, &aa, BITMAP_SIZE, 5, false); //ani[8] Air Attack Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r2, &aa, BITMAP_SIZE, 5, false); //ani[9] Air Attack Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l1, &ama, BITMAP_SIZE, 7, false); //ani[10] On-Air-Moving Attack Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r1, &ama, BITMAP_SIZE, 7, false); //ani[11] On-Air-Moving Attack Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &ada, BITMAP_SIZE, 3, false); //ani[12] On-Air-Down Attack Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &ada, BITMAP_SIZE, 3, false); //ani[13] On-Air-Down Attack Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l2, &sd, BITMAP_SIZE, 5, false); //ani[14] Draw sword Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r2, &sd, BITMAP_SIZE, 5, false); //ani[15] Draw sword Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &r, BITMAP_SIZE); //ani[16] Run Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &r, BITMAP_SIZE); //ani[17] Run Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &j, BITMAP_SIZE, 5, false); //ani[18] Jump Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &j, BITMAP_SIZE, 5, false); //ani[19] Jump Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &l, BITMAP_SIZE); //ani[20] Lean Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &l, BITMAP_SIZE); //ani[21] Lean Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &lf, BITMAP_SIZE); //ani[22] Landing Falling Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &lf, BITMAP_SIZE); //ani[23] Landing Falling Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r1, &uf, BITMAP_SIZE); //ani[24] Unconsciously Flying Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l1, &uf, BITMAP_SIZE); //ani[25] Unconsciously Flying Right
	AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &dg, BITMAP_SIZE, 15); //ani[26] Dodging Left
	AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &dg, BITMAP_SIZE, 15); //ani[27] Dodging Right
	_aniByWpn.push_back(tempAniByWpn);
	// ~
	// ~ Weapon 1
	_aniByWpn.push_back(tempAniByWpn);
	// ~
	// ~ Weapon 2
	_aniByWpn.push_back(tempAniByWpn);
}
vector<Weapon*>::iterator Enemy::WeaponNearby()
{
	vector<Weapon*>::iterator tar = weapons->begin();
	if (weapons->size() > 0)
		tar = weapons->begin();
	for (auto weapon = weapons->begin(); weapon != weapons->end(); weapon++) {
		Vector2 diffO((*tar)->GetCor(0) - GetCor(0), (*tar)->GetCor(1) - GetCor(1));
		Vector2 diffT((*weapon)->GetCor(0) - GetCor(0), (*weapon)->GetCor(1) - GetCor(1));
		if (diffT.GetLength() < diffO.GetLength())
			tar = weapon;
	}
	return tar;
}
vector<Player*>::iterator Enemy::PlayerNearby(vector<Player*> *playerList)
{
	vector<Player*>::iterator tar = playerList->end();
	if (playerList->size() > 0)
		tar = playerList->begin();
	for (auto player = playerList->begin(); player != playerList->end(); player++) {
		Vector2 diffO((*tar)->GetCor(0) - GetCor(0), (*tar)->GetCor(1) - GetCor(1));
		Vector2 diffT((*player)->GetCor(0) - GetCor(0), (*player)->GetCor(1) - GetCor(1));
		if (diffT.GetLength() < diffO.GetLength() && (*player)->GetName() != this->GetName())
			tar = player;
	}
	return tar;
}
}
