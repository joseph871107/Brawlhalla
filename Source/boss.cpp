#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "enemy.h"
#include "battleSystem.h"
#include "boss.h"
namespace game_framework
{
//-----------------CONSTANTS DEFINITIONS-----------------//
const vector<long> keys{ KEY_W, KEY_D, KEY_S, KEY_A, KEY_C, KEY_V, KEY_X };
const int KILL_BOSS_HIT_COUNT = 20;
const int MAX_HEALTH = Player::INITIAL_TAKEN_DAMAGE + Player::INCREMENT_AMOUNT_OF_TAKEN_DAMAGE * KILL_BOSS_HIT_COUNT;

//-----------------FUNCTIONS DEFINITIONS-----------------//
Boss::Boss()
{
    _identifier = PLAYER_MODE_BOSS;
    _difficulty = 0;
    SetSize(2.5);
    INITIAL_ACCELERATION = 0.5;
    _health = MAX_HEALTH;
}

Boss::Boss(int diff)
{
    _identifier = PLAYER_MODE_BOSS;
    _difficulty = diff;
    SetSize(2.5);
    INITIAL_ACCELERATION = 0.5;
    _health = MAX_HEALTH;
}
bool Boss::ChaseTarget(CPoint point, int width, int height)
{
    Ground* min = *_grounds.begin(), *max = min;

    for (auto i : _grounds)
    {
        if (min->GetCor(0) > i->GetCor(0))
            min = i;

        if (max->GetCor(0) < i->GetCor(0))
            max = i;
    }

    //
    bool tri = true;
    int offset = (int)(_size * _difficulty * 10);

    if (GetCor(0) > point.x + width && min->GetCor(0) < GetCor(0))  	// Go left
    {
        OnKeyDown(keys[3]);
        tri = false;
    }
    else
        OnKeyUp(keys[3]);

    //
    if (GetCor(2) < point.x && max->GetCor(2) > GetCor(2))  	// Go right
    {
        OnKeyDown(keys[1]);
        tri = false;
    }
    else
        OnKeyUp(keys[1]);

    //
    if (IsOnEdge() || 	// Do jump
            (IsOnGround() && (OnGround()->GetCor(2) - 10 <= GetCor(2) || OnGround()->GetCor(0) + 10 >= GetCor(0))) ||
            _isTriggerJump)
    {
        OnKeyDown(keys[0]);
        tri = false;
    }
    else
        OnKeyUp(keys[0]);

    //
    return tri;
}
void Boss::OnMove()
{
    /* CHASE & ATTACK */
    static int counter = 0, ptr = 0;
    CPoint targetPos;
    vector<Player*>::iterator targ;

    if (_attackList.size() == 0)
    {
        targ = PlayerNearby(_playersPtr);

        if (targ != _playersPtr->end())
            ChaseTarget(CPoint((*targ)->GetCor(0), (*targ)->GetCor(3)), (*targ)->GetWidth(), (*targ)->GetHeight());

        DoAttack(*_playersPtr, targ);
    }
    else
    {
        targ = PlayerNearby(&_attackList);

        if (targ != _attackList.end())
            ChaseTarget(CPoint((*targ)->GetCor(0), (*targ)->GetCor(3)), (*targ)->GetWidth(), (*targ)->GetHeight());

        DoAttack(_attackList, targ);
    }

    /* UPDATE BOSS HEALTH */
    _health = MAX_HEALTH - _takenDmg;
    /* BASE CLASS */
    Player::OnMove();
}
void Boss::SetAnimation()
{
    _OFFSET_Y = 30;
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
    s = vector<CPoint> { CPoint(0, 0), CPoint(0, 1), CPoint(0, 2), CPoint(0, 3) };	// bmps of standing
    a = vector<CPoint> { CPoint(1, 0), CPoint(1, 1), CPoint(1, 2), CPoint(1, 3) };
    gma = vector<CPoint> { CPoint(0, 1), CPoint(0, 2), CPoint(0, 3), CPoint(0, 3) };
    sa = vector<CPoint> { CPoint(1, 2), CPoint(1, 2), CPoint(1, 3), CPoint(1, 4), CPoint(1, 5), CPoint(1, 6), CPoint(1, 7), CPoint(1, 8) };
    aa = vector<CPoint> { CPoint(4, 3), CPoint(4, 4), CPoint(4, 5), CPoint(4, 6), CPoint(4, 7), CPoint(4, 8) };
    ama = vector<CPoint> { CPoint(0, 0) };
    ada = vector<CPoint> { CPoint(1, 5), CPoint(1, 6), CPoint(1, 7), CPoint(1, 8), CPoint(1, 9) };
    sd = vector<CPoint> { CPoint(3, 3), CPoint(3, 4), CPoint(3, 5), CPoint(3, 6), CPoint(3, 7), CPoint(3, 8), CPoint(3, 9) };
    r = vector<CPoint> { CPoint(2, 6), CPoint(2, 7), CPoint(2, 8) };	// bmps of running
    j = vector<CPoint> { CPoint(2, 5) };	// bmps of jumping
    l = vector<CPoint> { CPoint(2, 0) }; // bmps of leaning
    lf = vector<CPoint> { CPoint(1, 5), CPoint(1, 4) }; // bmps of landing fallin
    uf = vector<CPoint> { CPoint(0, 4), CPoint(0, 5), CPoint(0, 6) }; // bmps of unconsciously flying
    dg = vector<CPoint> { CPoint(4, 0), CPoint(4, 1), CPoint(4, 2), CPoint(4, 3) }; // bmps of dodging
    vector<CAnimation> tempAniByWpn = vector<CAnimation>();
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &s, _size); //ani[0] Stand (Idle) Left with sword
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &s, _size); //ani[1] Stand (Idle) Right with sword
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &a, _size, 5, false); //ani[2] Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &a, _size, 5, false); //ani[3] Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l1, &gma, _size, 5, false); //ani[4] On-Ground-Moving Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r1, &gma, _size, 5, false); //ani[5] On-Ground-Moving Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l2, &sa, _size, 3, false); //ani[6] Slide Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r2, &sa, _size, 3, false); //ani[7] Slide Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l2, &aa, _size, 5, false); //ani[8] Air Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r2, &aa, _size, 5, false); //ani[9] Air Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l1, &ama, _size, 7, false); //ani[10] On-Air-Moving Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r1, &ama, _size, 7, false); //ani[11] On-Air-Moving Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &ada, _size, 3, false); //ani[12] On-Air-Down Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &ada, _size, 3, false); //ani[13] On-Air-Down Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l2, &sd, _size, 5, false); //ani[14] Draw sword Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r2, &sd, _size, 5, false); //ani[15] Draw sword Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &r, _size); //ani[16] Run Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &r, _size); //ani[17] Run Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &j, _size, 5, false); //ani[18] Jump Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &j, _size, 5, false); //ani[19] Jump Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &l, _size); //ani[20] Lean Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &l, _size); //ani[21] Lean Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &lf, _size); //ani[22] Landing Falling Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &lf, _size); //ani[23] Landing Falling Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r1, &uf, _size); //ani[24] Unconsciously Flying Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l1, &uf, _size); //ani[25] Unconsciously Flying Right
    AddCAnimationWithSprite(&tempAniByWpn, &julian_l0, &dg, _size, 15); //ani[26] Dodging Left
    AddCAnimationWithSprite(&tempAniByWpn, &julian_r0, &dg, _size, 15); //ani[27] Dodging Right
    _aniByWpn.push_back(tempAniByWpn);
    // ~
    // ~ Weapon 1
    _aniByWpn.push_back(tempAniByWpn);
    // ~
    // ~ Weapon 2
    _aniByWpn.push_back(tempAniByWpn);
}

void Boss::OnShow()
{
    // Set the camera for showing boss health
    CPoint cam = camera->GetXY(DoubleToInteger(_x - 4 * _size), DoubleToInteger(_y + _collision_box.Height() * _size));
    // Show boss health
    CString bossHealth;
    bossHealth.SetString(to_string(_health));
    bossHealth.SetSize(camera->GetSize() / 2);
    bossHealth.SetTopLeft(cam.x + 40, cam.y);
    bossHealth.ShowBitmap();
    /* BASE CLASS */
    // TODO: If the call to the base class moves up, the program is fucked up and Bill does not know why
    Player::OnShow();
}

int Boss::GetSpecializedTakenDamage() const
{
    return (Player::INITIAL_TAKEN_DAMAGE);
}

void Boss::EvaluateDeadAndRespawn()
{
    if (IsOutMapBorder()) // Respawn the boss immediately
    {
        Ground* g = GetRandomGround(&_grounds);		// Randomly select Ground
        _x = random(g->GetCor(0), g->GetCor(2) - GetWidth());		// Randomly set x coordinate within Ground's width
        _y = g->GetCor(1) - GetHeight();
        //
        int currentTakenDmg = _takenDmg; // Since 'InitializeOnRespawn()' reset '_takenDmg', we need to preserve it
        InitializeOnRespawn();
        _takenDmg = currentTakenDmg;
    }
}

int Boss::GetHealth()
{
    return _health;
}

}