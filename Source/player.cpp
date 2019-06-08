#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
//
#include "player.h"
// Triggered Animation
#include "TriggeredAnimation.h"
#include "AirAttackTriggeredAnimation.h"
#include "AirLandDownAttackTriggeredAnimation.h"
#include "AirMoveLeftAttackTriggeredAnimation.h"
#include "AirMoveRightAttackTriggeredAnimation.h"
#include "DodgeTriggeredAnimation.h"
#include "DrawSwordTriggeredAnimation.h"
#include "GroundAttackTriggeredAnimation.h"
#include "GroundLandDownAttackTriggeredAnimation.h"
#include "GroundMoveLeftAttackTriggeredAnimation.h"
#include "GroundMoveRightAttackTriggeredAnimation.h"
// Others
#include "BattleSystem.h"
#include "ExplosionEffect.h"

namespace game_framework
{
//-----------------STATIC DEFINITIONS-----------------//
double Player::INITIAL_ACCELERATION = 1.2;
const double Player::EDGE_SLIDING_ACCELERATION = 0.1;
const double Player::MOVE_ACCELERATION = 0.5;
const double Player::MAX_MOVE_VELOCITY = 10;
const double Player::LANDING_ACCELERATION = 5;
const double Player::INITIAL_VELOCITY = 20;
const double Player::STOP_ACCELERATION = 1;
const double Player::MAX_VERTICAL_VELOCITY = 30;

//-----------------CONSTANTS DEFINITIONS-----------------//
const int MAX_JUMP_COUNT = 2;
const int GND_ATTACK_MOVEMENT_UNIT = 12;
const double COLLISION_ERRORS = 1.0;
const int MAX_ATTACK_AFFECTION_FRAMES = 150; // 5 secs
const int RESPAWN_DISTANCE_ABOVE_GROUND = 100;
const int RESPAWN_MOVEMENT_OFFSET_MAGNITUDE = 10;
const int RESPAWN_LEFT_START_POS_X = 0;
const int RESPAWN_LEFT_START_POS_Y = 0;
const int RESPAWN_RIGHT_START_POS_X = SIZE_X;
const int RESPAWN_RIGHT_START_POS_Y = 0;
const int IMMUNE_FLASHING_FRAME = 11 ;

//-----------------FUNCTIONS DEFINITIONS-----------------//
Player::Player() : _identifier(PLAYER_MODE_PLAYER), _allowRespawn(true) // 我覺得之後應該先不用更改這個constructor，好多喔。。。
{
    /* Body intentionally empty */
}

Player::~Player()
{
    for (auto elementPtr : _triggeredAnis)
    {
        delete elementPtr;
    }
}

void Player::InitializeTriggeredAnimations()
{
    /* ATTACK ON GROUND */
    _triggeredAnis.push_back((TriggeredAnimation*) new GroundAttackTriggeredAnimation(this, KEY_GND_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new GroundMoveRightAttackTriggeredAnimation(this, KEY_GND_MOVE_RIGHT_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new GroundMoveLeftAttackTriggeredAnimation(this, KEY_GND_MOVE_LEFT_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new GroundLandDownAttackTriggeredAnimation(this, KEY_GND_LAND_DOWN_ATTACK));
    /* ATTACK ON AIR */
    _triggeredAnis.push_back((TriggeredAnimation*) new AirAttackTriggeredAnimation(this, KEY_AIR_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new AirMoveRightAttackTriggeredAnimation(this, KEY_AIR_MOVE_RIGHT_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new AirMoveLeftAttackTriggeredAnimation(this, KEY_AIR_MOVE_LEFT_ATTACK));
    _triggeredAnis.push_back((TriggeredAnimation*) new AirLandDownAttackTriggeredAnimation(this, KEY_AIR_LAND_DOWN_ATTACK));
    /* OTHERS */
    _triggeredAnis.push_back((TriggeredAnimation*) new DrawSwordTriggeredAnimation(this, KEY_DRAW_SWORD));
    _triggeredAnis.push_back((TriggeredAnimation*) new DodgeTriggeredAnimation(this, KEY_DODGE));
}

void Player::SetAttacker(Player* const& newAttacker, const int& newAttackerAffectionFrameCount)
{
    _attacker = newAttacker;
    _attackerAffectionFrameCount = newAttackerAffectionFrameCount;
}

void Player::Initialize(BattleSystem* battleSystemPtrValue, vector<Ground*> groundsValue, vector<Player*>* playersPtrValue, string nameValue, vector<long> keysValue, ExplosionEffect* const explosionEffectPtrValue)
{
    /* Remarks: all Animation and Bitmaps variables are initialized in 'LoadBitmap()' */
    Ground* g = GetRandomGround(&groundsValue);		// Randomly select Ground
    _x = random(g->GetCor(0), g->GetCor(2) - GetWidth());		// Randomly set x coordinate within Ground's width
    _y = g->GetCor(1) - GetHeight();
    //
    _width = (int)(_collision_box.Width() * _size);
    _height = (int)(_collision_box.Height() * _size);

    //
    if (!keysValue.size())
        _keys = { KEY_W, KEY_D, KEY_S, KEY_A, KEY_C, KEY_V, KEY_X };
    else
        _keys = keysValue;

    //
    _playersPtr = playersPtrValue;
    //
    _grounds = groundsValue;
    //
    _life = MAX_LIFE;
    //
    _name = nameValue;
    //
    _roundPrevPickedUpWpnID = 2;
    //
    InitializeTriggeredAnimations();
    //
    _hitTargetPlayers = vector<Player*>();
    //
    _battleSystemPtr = battleSystemPtrValue;
    //
    _explosionEffectPtr = explosionEffectPtrValue;
    //
    _respawnCourier.Initialize(camera);
    //
    _consciousState = PlayerConsciousState(this);
    _unconsciousState = PlayerUnconsciousState(this);
    _respawnState = PlayerRespawnState(this);
    _immuneState = PlayerImmuneState(this, 0);
    //
    InitializeOnRespawn();
    //
    DoRespawn();
}

void Player::LoadBitmap()
{
    _collision_box.LoadBitmap(IDB_P_COLLISION_BOX, RGB(0, 0, 0));
    _respawnCourier.LoadBitmap();
    SetAnimation();
}



void Player::SetCurrentTriggeredAnimationByWeapon()
{
    /*	~ Remark:
    	~ The player is performing a trigger animation
    	~ The animation is dependent on the weapon (decided by the actual sprite of the player)
    */
    SetAnimationStateByWeapon(_triggeredAniAnimationID);
}

void Player::SetCurrentAniByWeapon()
{
    if (_isTriggeredAni)
        SetCurrentTriggeredAnimationByWeapon();
    else
        SetCurrentNonTriggeredAnimationByWeapon();
}

void Player::SetCurrentAnimation()
{
    SetCurrentAniByWeapon();
}

void Player::FinishTriggeredAnimationAnimationLogic()
{
    // Reset the animation of the finished triggered animation
    _aniByWpn[_wpnID][_triggeredAniAnimationID].Reset();
    // Save the finished Triggered Animation's key ID '_triggeredAniKeyID'
    _finishedTriggeredAniKeyID = _triggeredAniKeyID;
    // Mark that the trigger animation has finished by resetting all animation variables
    ResetTriggeredAnimationVariables();
}

void Player::FinishTriggeredAnimationGameLogic()
{
    // Finish other logic regarding the triggered animation
    // Since '_triggeredAniKeyID' has already been reset in 'OnMoveAnimationLogic()'
    // once the triggered animatin is finished, we use '_finishedTriggeredAniKeyID' for our estimation
    for (auto elementPtr : _triggeredAnis)
        if (elementPtr->GetKeyID() == _finishedTriggeredAniKeyID)
            elementPtr->FinishTriggeredAnimationGameLogic();
}

void Player::MoveCurrentAnimation()
{
    _aniByWpn[_wpnID][_currentAniByWpn].OnMove();
}

void Player::OnMoveAnimationLogic()
{
    /*	~ Remark:
    	~ This function is responsible for the animation logic of the player.
    	~ Its primary task is to set and "move" the player's current animation.
    */

    //-----------------STATE SECTION-----------------//
    switch (_state)
    {
        case CONSCIOUS_STATE:
            _consciousState.OnMoveAnimationLogic();
            break;

        case UNCONSCIOUS_STATE:
            _unconsciousState.OnMoveAnimationLogic();
            break;

        case RESPAWN_STATE:
            _respawnState.OnMoveAnimationLogic();
            break;

        case IMMUNE_STATE:
            _immuneState.OnMoveAnimationLogic();
            break;

        default:
            // Never happen
            return;
    }

    //-----------------COMMON SECTION-----------------//

    /*	~ RESET JUMP ANIMATION
    	~ Reset the jump animation for the player
    */
    if (IsOnGround() || IsOnEdge() || (_isTriggerJump && _jumpCount > 0))
        ResetAnimations(ANI_WPN_ID_JUMP_LEFT); // Reset the jump animation so that it keeps displaying while the player is jumping regardless of reaching its final bitmap

    /*	~ SET CURRENT ANIMATION
    	~ Set the current animation based on '_triggeredAniAnimationID'
    */
    SetCurrentAnimation();
    /*	~ MOVE CURRENT ANIMATION
    	~ Proceed to the next CMovingBitmap of the current animation,
    */
    MoveCurrentAnimation();
}

void Player::DoReturnHomeRespawnCourier()
{
    int currentX = _respawnCourier.GetX();
    int currentY = _respawnCourier.GetY();
    // Move top left
    _respawnCourier.SetDir(false);
    _respawnCourier.SetXY(currentX - 10, currentY - 10);
}

void Player::EvaluateDeadAndRespawn()
{
    if (!_isDead && IsOutMapBorder())
        DoDead();

    if (_isDead)
    {
        if (_explosionEffectPtr->GetIsTrigger()) // If the dead explosion effect has not been finished, then halt all the movement of the player
            return;
        else // If the dead explosion effect is finished, then respawn the player
            DoRespawn();
    }
}

void Player::OnMoveGameLogic()
{
    /*	~ Remark:
    	~ This function is responsible for the game logic of the player,
    	~ including his positioning and the way he interacts with other
    	~ objects (such as grounds, other players, etc.)
    */
    //-----------------PRIOR COMMON SECTION-----------------//
    /*	~	DEAD & RESPAWN
    	~	Remark: When the player is out of life, the player's 'OnMove()' and 'OnShow()' will
    	~	no longer being called by the 'BattleSystem'
    */
    this->EvaluateDeadAndRespawn();

    //-----------------STATE SECTION-----------------//
    switch (_state)
    {
        case CONSCIOUS_STATE:
            _consciousState.OnMoveGameLogic();
            break;

        case UNCONSCIOUS_STATE:
            _unconsciousState.OnMoveGameLogic();
            break;

        case RESPAWN_STATE:
            _respawnState.OnMoveGameLogic();
            break;

        case IMMUNE_STATE:
            _immuneState.OnMoveGameLogic();
            break;

        default:
            // Never happen
            return;
    }

    /* MOVE RESPAWN COURIER OUT OF THE MAP */
    if (_state != RESPAWN_STATE)
        DoReturnHomeRespawnCourier();

    //-----------------POSTERIOR COMMON SECTION-----------------//

    /*	~ ESTIMATE FINISH OF TRIGGERED ANIMATION
    	~ Do note that both 'UnconsciouslyOnMoveAnimationLogic()' and 'ConsciouslyOnMoveAnimationLogic()'
    	~ can finish the Triggered Animation. Thus, this estimation is in the common section of
    	~ 'OnMoveGameLogic()'
    */
    if (!_isTriggeredAni) // If the "Triggered Animation" is finished (determined by 'FinishTriggeredAnimationAnimationLogic()')
        FinishTriggeredAnimationGameLogic();

    /*	~ RESET JUMP COUNT
    	~ Reset the jump count of the player, so that he can jump when eligible
    */
    if (IsOnGround() || IsOnEdge())
        ResetJumpCount();

    /* EVALUATE KILLER AFFECTION */
    if (_attackerAffectionFrameCount >= MAX_ATTACK_AFFECTION_FRAMES)
        SetAttacker(nullptr, 0); // Discard the affection of the attacker
    else
        _attackerAffectionFrameCount++;
}

void Player::DoParseKeyPressed()
{
    /*	~ Remark: When both keys are held, the right key prevails the left key
    	~ Thus, when both keys are held and the right key is released, then
    	~ the left key will come to play
    */
    if (_isTriggerPressingLeft && !_isPressingRight)
    {
        _dir = false;
        _isPressingLeft = true;
        _isTriggerPressingLeft = false;
    }
}

void Player::OnMove()
{
    DoParseKeyPressed();
    _currentKeyID = GetKeyCombination(); // Get the current key combination to re-use in 'OnMoveAnimationLogic()' and 'OnMoveGameLogic()'
    OnMoveAnimationLogic();
    OnMoveGameLogic();
}

void Player::OnShow()
{
    // Show respawn courier
    _respawnCourier.OnShow();
    // Show current animation
    ShowCurrentAnimation();
    // Play current audio
    PlayAudioByState();
    // Set the camera for showing name tag
    CPoint cam = camera->GetXY(DoubleToInteger(_x - 4 * _size), DoubleToInteger(_y + _collision_box.Height() * _size));
    // Show the name tag
    CString playerName;
    playerName.SetString(_name);
    playerName.SetSize(camera->GetSize() / 2);
    playerName.SetTopLeft(cam.x - 60, cam.y);
    playerName.ShowBitmap();
}

void Player::OnKeyDown(const UINT& nChar)
{
    if (nChar == _keys[0]) // Up
    {
        _isTriggerJump = true;
    }
    else if (nChar == _keys[1]) // Right
    {
        _dir = true;
        _isPressingRight = true;
    }
    else if (nChar == _keys[2]) // Down
    {
        _isPressingDown = true;
    }
    else if (nChar == _keys[3]) // Left
    {
        _isTriggerPressingLeft = true;
    }
    else if (nChar == _keys[4]) // Attack
    {
        _isTriggerAttack = true;
    }
    else if (nChar == _keys[5]) //Dodge
    {
        _isTriggerDodge = true;
    }
    else if (nChar == _keys[6])	//Throw
    {
        if (GetHoldWeapon())
            DoThrowWeapon();
    }
    else
    {
        // Do nothing
    }
}

void Player::DoThrowWeapon()
{
    _battleSystemPtr->GetReferenceMap()->PlayerThrowWeapon(this);
    SetHoldWeapon(false);
    ResetWeaponID(); // TODO: Josept wrote this line, but Bill does not understand why resetting the weapon ID is necessary
}


void Player::OnKeyUp(const UINT& nChar)
{
    if (nChar == _keys[1]) // Right
    {
        _isPressingRight = false;
    }
    else if (nChar == _keys[2]) // Down
    {
        _isPressingDown = false;
    }
    else if (nChar == _keys[3]) // Left
    {
        _isPressingLeft = false;
    }
    else
    {
        // Do nothing
    }
}

void Player::SetName(string name)
{
    _name = name;
}

void Player::SetHoldWeapon(bool isHolding)
{
    _isHoldingWeapon = isHolding;
    _isTriggerDrawWeapon = isHolding;
    _isTriggerAttack = false; // We are picking weapon, not performing an attack
}

void Player::InitializeUnconsciousState(bool beingAttackedDirection)
{
    SetState(UNCONSCIOUS_STATE);
    _unconsciousFramesCount = 0;
    _unconsciousAniDir = beingAttackedDirection;
}

void Player::BeenAttacked(Vector2 displaymentVector, bool beingAttackedDirection)
{
    int displaceX = displaymentVector.GetX();
    int displaceY = displaymentVector.GetY();

    // y-coordinate
    if (displaceY < 0)
    {
        InitiateOffsetUp(abs(displaceY));
    }
    else if (displaceY > 0)
    {
        InitiateOffsetDown(abs(displaceY));
    }
    else // displayY == 0
    {
        InitiateOffsetUp(10); // Give the player a little lift when being hit
    }

    // x-coordinate
    if (displaceX < 0)
    {
        InitiateOffsetLeft(abs(displaceX));
    }
    else if (displaceX > 0)
    {
        InitiateOffsetRight(abs(displaceX));
    }
    else // displayX == 0
    {
        /* Do nothing */
    }

    InitializeUnconsciousState(beingAttackedDirection);
}

bool Player::GetHoldWeapon()
{
    return _isHoldingWeapon;
}

bool Player::GetDirection()
{
    return _dir;
}

int Player::GetCor(int index)
{
    switch (index)
    {
        case 0:
            return _x;

        case 1:
            return _y;

        case 2:
            return _x + (int)(_collision_box.Width() * _size);

        case 3:
            return _y + (int)(_collision_box.Height() * _size);

        default:
            return NULL;
    }
}

int Player::GetWidth()
{
    return GetCor(2) - GetCor(0);
}

int Player::GetHeight()
{
    return GetCor(3) - GetCor(1);
}

const string& Player::GetName() const
{
    return (_name);
}

const int& Player::GetLife() const
{
    return (_life);
}

const bool Player::IsOutOfLife() const
{
    return (_life == 0);
}

const long& Player::GetAttackKey() const
{
    return (_keys[4]);
}

void Player::GenerateAndSetWeaponID() // This function is intended to be called by 'Weapon::HitPlayer()'
{
    // The player will pick his weapon alternatively through out the round
    if (_roundPrevPickedUpWpnID == 1)
    {
        _roundPrevPickedUpWpnID = 2;
        _wpnID = 2;
    }
    else if (_roundPrevPickedUpWpnID == 2)
    {
        _roundPrevPickedUpWpnID = 1;
        _wpnID = 1;
    }
}

void Player::ResetWeaponID()
{
    _wpnID = 0; // reset to default weapon: punch
}

void Player::AddCAnimationWithSprite(vector<CAnimation>* tempAniByWpn, vector< vector<CMovingBitmap>>* sprite, vector<CPoint>* list, double size, int delay, bool repeat, int times)
{
    CAnimation temp(repeat, times);

    for (auto i : *list)
        temp.AddBitmap((*sprite)[i.x][i.y]);

    temp.SetSize(size);
    temp.SetDelayCount(delay);
    tempAniByWpn->push_back(temp);
}

void Player::ResetAnimations(int leftAnimationID)
{
    _aniByWpn[_wpnID][leftAnimationID].Reset(); // Reset left animation
    _aniByWpn[_wpnID][leftAnimationID + 1].Reset(); // Reset right animation
}

void Player::SetAnimation()
{
    //-----------------ANIMATION BY WEAPONS-----------------//
    _aniByWpn = vector<vector<CAnimation>>();
    /*	~ WEAPON 0
    	~ Fist (Default)
    */
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
    //
    s = vector<CPoint> { CPoint(0, 0), CPoint(0, 1), CPoint(0, 2), CPoint(0, 3) };
    a = vector<CPoint> { CPoint(1, 0), CPoint(1, 1), CPoint(1, 2), CPoint(1, 3) };
    gma = vector<CPoint> { CPoint(3, 0), CPoint(3, 1), CPoint(3, 2), CPoint(3, 3) };
    sa = vector<CPoint> { CPoint(0, 0), CPoint(0, 1), CPoint(0, 2), CPoint(0, 3), CPoint(0, 4), CPoint(0, 5), CPoint(0, 6), CPoint(0, 7), CPoint(0, 8), CPoint(0, 9) };
    aa = vector<CPoint> { CPoint(1, 4), CPoint(1, 5), CPoint(1, 6), CPoint(1, 7), CPoint(1, 8) };
    ama = vector<CPoint> { CPoint(1, 4), CPoint(1, 5), CPoint(1, 6), CPoint(1, 7), CPoint(1, 8) };
    ada = vector<CPoint> { CPoint(6, 2), CPoint(6, 3), CPoint(6, 4), CPoint(6, 5), CPoint(6, 6) };
    sd = vector<CPoint> { CPoint(3, 0), CPoint(3, 1), CPoint(3, 2), CPoint(3, 3), CPoint(3, 4), CPoint(3, 5), CPoint(3, 6), CPoint(3, 7), CPoint(3, 8), CPoint(3, 9), CPoint(4, 0), CPoint(4, 1), CPoint(4, 2), CPoint(4, 3), CPoint(4, 4), CPoint(4, 5), CPoint(4, 6), CPoint(4, 7), CPoint(4, 8) };
    r = vector<CPoint> { CPoint(2, 0), CPoint(2, 1), CPoint(2, 2), CPoint(2, 1) };
    j = vector<CPoint> { CPoint(6, 2) };
    l = vector<CPoint> { CPoint(5, 2) };
    lf = vector<CPoint> { CPoint(6, 4) };
    uf = vector<CPoint> { CPoint(3, 0), CPoint(3, 1), CPoint(3, 2), CPoint(3, 3), CPoint(3, 4) };
    dg = vector<CPoint> { CPoint(5, 6), CPoint(5, 7) };
    //
    vector<CAnimation> tempAniByWpn = vector<CAnimation>();
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &s, _size); //ani[0] Stand (Idle) Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &s, _size); //ani[1] Stand (Idle) Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &a, _size, 5, false); //ani[2] Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &a, _size, 5, false); //ani[3] Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l1, &gma, _size, 3, false); //ani[4] On-Ground-Moving Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r1, &gma, _size, 3, false); //ani[5] On-Ground-Moving Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l2, &sa, _size, 3, false); //ani[6] Slide Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r2, &sa, _size, 3, false); //ani[7] Slide Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &aa, _size, 3, false); //ani[8] Air Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &aa, _size, 3, false); //ani[9] Air Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &ama, _size, 3, false); //ani[10] On-Air-Moving Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &ama, _size, 3, false); //ani[11] On-Air-Moving Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l1, &ada, _size, 3, false); //ani[12] On-Air-Down Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r1, &ada, _size, 3, false); //ani[13] On-Air-Down Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l2, &sd, _size, 3, false); //ani[14] Draw sword Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r2, &sd, _size, 3, false); //ani[15] Draw sword Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &r, _size); //ani[16] Run Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &r, _size); //ani[17] Run Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &j, _size, 5, false); //ani[18] Jump Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &j, _size, 5, false); //ani[19] Jump Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &l, _size); //ani[20] Lean Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &l, _size); //ani[21] Lean Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &lf, _size); //ani[22] Landing Falling Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &lf, _size); //ani[23] Landing Falling Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &uf, _size); //ani[24] Unconsciously Flying Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &uf, _size); //ani[25] Unconsciously Flying Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l0, &dg, _size, 15); //ani[26] Dodging Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r0, &dg, _size, 15); //ani[27] Dodging Right
    _aniByWpn.push_back(tempAniByWpn);
    /*	~ WEAPON 1
    	~ Sword
    */
    gma = a;
    a = vector<CPoint> { CPoint(0, 0), CPoint(0, 1), CPoint(0, 2), CPoint(0, 3), CPoint(0, 4), CPoint(0, 5) };
    sa = vector<CPoint> { CPoint(0, 0), CPoint(0, 1), CPoint(0, 2), CPoint(0, 3), CPoint(0, 4), CPoint(0, 5), CPoint(0, 6), CPoint(0, 7), CPoint(0, 8), CPoint(0, 9), CPoint(1, 0), CPoint(1, 1), CPoint(1, 2), CPoint(1, 3) };
    aa = sa;
    ama = vector<CPoint> { CPoint(4, 2), CPoint(4, 3), CPoint(4, 4) };
    //
    tempAniByWpn = vector<CAnimation>();
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &s, _size); //ani[0] Stand (Idle) Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &s, _size); //ani[1] Stand (Idle) Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l3, &a, _size, 5, false); //ani[2] Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r3, &a, _size, 5, false); //ani[3] Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &gma, _size, 3, false); //ani[4] On-Ground-Moving Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &gma, _size, 3, false); //ani[5] On-Ground-Moving Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l2, &sa, _size, 3, false); //ani[6] Slide Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r2, &sa, _size, 3, false); //ani[7] Slide Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l2, &aa, _size, 2, false); //ani[8] Air Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r2, &aa, _size, 2, false); //ani[9] Air Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l1, &ama, _size, 3, false); //ani[10] On-Air-Moving Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r1, &ama, _size, 3, false); //ani[11] On-Air-Moving Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l1, &ada, _size, 3, false); //ani[12] On-Air-Down Attack Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r1, &ada, _size, 3, false); //ani[13] On-Air-Down Attack Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_l2, &sd, _size, 3, false); //ani[14] Draw sword Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_r2, &sd, _size, 3, false); //ani[15] Draw sword Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &r, _size); //ani[16] Run Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &r, _size); //ani[17] Run Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &j, _size, 5, false); //ani[18] Jump Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &j, _size, 5, false); //ani[19] Jump Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &l, _size); //ani[20] Lean Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &l, _size); //ani[21] Lean Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &lf, _size); //ani[22] Landing Falling Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &lf, _size); //ani[23] Landing Falling Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &uf, _size); //ani[24] Unconsciously Flying Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &uf, _size); //ani[25] Unconsciously Flying Right
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_l0, &dg, _size, 15); //ani[26] Dodging Left
    AddCAnimationWithSprite(&tempAniByWpn, &louis_ex_r0, &dg, _size, 15); //ani[27] Dodging Right
    _aniByWpn.push_back(tempAniByWpn);
    /*	~ WEAPON 2
    	~ N/A
    */
    _aniByWpn.push_back(tempAniByWpn);
}

void Player::ShowCurrentAnimation()
{
    if (_PLAYER_DEBUG)
    {
        CPoint cam = camera->GetXY(_x, _y);
        _collision_box.SetTopLeft(cam.x, cam.y);		//actual player blocks
        _collision_box.ShowBitmap(_size * camera->GetSize());
    }

    // Calculate and set the position of the player current animation in respect to the collision box's
    if (
        _state != IMMUNE_STATE
        ||
        (_state == IMMUNE_STATE && _immuneState.GetFrameCounter() % IMMUNE_FLASHING_FRAME == 1)
    )
    {
        CPoint cam = camera->GetXY(_x - (int)(_OFFSET_X * _size), _y - (int)(_OFFSET_Y * _size));
        _aniByWpn[_wpnID][_currentAniByWpn].SetSize(_size * camera->GetSize());
        _aniByWpn[_wpnID][_currentAniByWpn].SetTopLeft(cam.x, cam.y);
        _aniByWpn[_wpnID][_currentAniByWpn].OnShow();
    }
}

bool Player::IsOnLeftEdge()
{
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (vector<Ground*>::iterator i = _grounds.begin(); i != _grounds.end(); i++)
    {
        int groundX1 = (*i)->GetCor(0);
        int groundY1 = (*i)->GetCor(1);
        int groundX2 = (*i)->GetCor(2);
        int groundY2 = (*i)->GetCor(3);

        if (((playerY2 - groundY1 >= COLLISION_ERRORS) && (playerY1 - groundY2 <= COLLISION_ERRORS)) && (_x == groundX1 - _width))
            return true;
    }

    return false;
}

bool Player::IsOnRightEdge()
{
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (vector<Ground*>::iterator i = _grounds.begin(); i != _grounds.end(); i++)
    {
        int groundX1 = (*i)->GetCor(0);
        int groundY1 = (*i)->GetCor(1);
        int groundX2 = (*i)->GetCor(2);
        int groundY2 = (*i)->GetCor(3);

        if ((playerY2 - groundY1 >= COLLISION_ERRORS) && (playerY1 - groundY2 <= COLLISION_ERRORS) && (_x == groundX2))
            return true;
    }

    return false;
}

bool Player::IsOutMapBorder()
{
    return (!(
                ((MAP_BORDER_X1 <= GetCor(0)/*x1*/) && (GetCor(2)/*x2*/ <= MAP_BORDER_X2))
                &&
                ((MAP_BORDER_Y1 <= GetCor(1)/*y1*/) && (GetCor(3)/*y2*/ <= MAP_BORDER_Y2))
            ));
}

void Player::InitiateOffsetUp(double initialOffsetVelocityMagnitude)
{
    _verticalVelocity = -initialOffsetVelocityMagnitude;
    _y -= DoubleToInteger(initialOffsetVelocityMagnitude); //Trick explaination: By intuition, '_y' of the player should not be
    // modified here, because it is expected to be modified whenever 'Player::OnMove()' is called. However,
    // since the player is currently on the ground, 'Player::OnMove()' will fix its '_y' onto the surface
    // instead of modifying it as expectation. Thus, '_y' must be altered here to set the player jump his ass up!!
}

void Player::InitiateOffsetDown(double initialOffsetVelocityMagnitude)
{
    _verticalVelocity = initialOffsetVelocityMagnitude;
}

void Player::InitiateOffsetLeft(double initialOffsetVelocityMagnitude)
{
    _horizontalVelocity = initialOffsetVelocityMagnitude;
    _isOffsetLeft = true;
    _isOffsetRight = false;
}

void Player::InitiateOffsetRight(double initialOffsetVelocityMagnitude)
{
    _horizontalVelocity = initialOffsetVelocityMagnitude;
    _isOffsetRight = true;
    _isOffsetLeft = false;
}

bool Player::IsBeingOffsetHorizontally()
{
    return (_isOffsetLeft || _isOffsetRight);
}

void Player::ResetJumpCount()
{
    _jumpCount = MAX_JUMP_COUNT;
}


bool Player::IsAttackable(Player* potentialTargetPlayer)
{
    for (auto elementPtr : _hitTargetPlayers)
        if (elementPtr == potentialTargetPlayer) // If the potential target player has already been attacked by "this" player in his attack duration
            return (false);

    return ((potentialTargetPlayer != this) // Not the "this" player himself
            &&
            (!potentialTargetPlayer->_isDodging) // The potential target player is not dodging
            &&
            (HitPlayer(potentialTargetPlayer, _triggeredAniDir)) // The potential player is in hit range
           );
}

void Player::DoAttack()
{
    if (_attackList.size() == 0) // Not boss mode
    {
        for (auto eachPlayerPtr : (*_playersPtr))
        {
            if (IsAttackable(eachPlayerPtr))
            {
                if (_isHoldingWeapon)
                    CAudio::Instance()->Play(AUDIO_SWOOSH);
                else
                    CAudio::Instance()->Play(AUDIO_PUNCH);

                PerformAttack(eachPlayerPtr, _triggeredAniDir);
            }
        }
    }
    else // Boss mode
    {
        for (auto eachPlayerPtr : _attackList)
        {
            if (IsAttackable(eachPlayerPtr))
            {
                if (_isHoldingWeapon)
                    CAudio::Instance()->Play(AUDIO_SWOOSH);
                else
                    CAudio::Instance()->Play(AUDIO_PUNCH);

                PerformAttack(eachPlayerPtr, _triggeredAniDir);
            }
        }
    }
}

void Player::PerformAttack(Player* targetPlayer, bool attackDirection)
{
    // Set the attacker for the target player
    targetPlayer->SetAttacker(this, 0);
    // Put the target player in the list, so as not to advertently hit him in a repeated manner during a single attack animation
    _hitTargetPlayers.push_back(targetPlayer);
    // Determine the attack "direction", which is a 2D Vector
    Vector2 vectorAttackerToTargetPlayer;
    vectorAttackerToTargetPlayer.SetXY(GetCor(0), GetCor(1), targetPlayer->GetCor(0), targetPlayer->GetCor(1));
    // Increment the taken damage of the target player
    targetPlayer->_takenDmg += INCREMENT_AMOUNT_OF_TAKEN_DAMAGE;
    // Set the offset magnitude of the attack 2D Vector
    int attackOffsetMagnitude = targetPlayer->GetSpecializedTakenDamage();
    // Determine the attack 2D vector
    double multiplier = (vectorAttackerToTargetPlayer.GetLength() == 0 ? attackOffsetMagnitude : attackOffsetMagnitude / vectorAttackerToTargetPlayer.GetLength()); // Avoid division by 0
    Vector2 targetPlayerDisplacementVector(DoubleToInteger(vectorAttackerToTargetPlayer.GetX() * multiplier),
                                           DoubleToInteger(vectorAttackerToTargetPlayer.GetY() * multiplier));
    //
    targetPlayer->BeenAttacked(targetPlayerDisplacementVector, attackDirection);
}

bool Player::HitPlayer(Player* targetPlayer, bool attackDirection)
{
    int attackerX1 = GetCor(0);
    int attackerY1 = GetCor(1);
    int attackerX2 = GetCor(2);
    int attackerY2 = GetCor(3);
    int attackRangeX1, attackRangeY1, attackRangeX2, attackRangeY2;
    attackRangeY1 = attackerY1;
    attackRangeY2 = attackerY2;

    if (attackDirection)   // Player in triggered animation is facing right
    {
        attackRangeX1 = (attackerX1 + attackerX2) / 2;
        attackRangeX2 = attackerX2 + 50;
    }
    else   // Player in triggered animation is facing left
    {
        attackRangeX1 = attackerX1 - 50;
        attackRangeX2 = (attackerX1 + attackerX2) / 2;
    }

    return (targetPlayer->GetCor(2) >= attackRangeX1 && targetPlayer->GetCor(0) <= attackRangeX2
            &&
            targetPlayer->GetCor(3) >= attackRangeY1 && targetPlayer->GetCor(1) <= attackRangeY2);
}

void Player::SetAttackList(vector<Player*> list)
{
    _attackList = list;
}

void Player::SetTakenDmg(int dmg)
{
    _takenDmg = dmg;
}

vector<long> Player::GetKeys()
{
    return _keys;
}

const vector<Player*>& Player::GetAttackListByGameMode() const
{
    return (_attackList.size() == 0 ? (*_playersPtr) : _attackList);
}

void Player::PlayAudioByState()
{
    int aboutToPlay = -1;

    if (WpnStateChanged() && _isTriggeredAni)
    {
        switch (_triggeredAniAnimationID)
        {
            case ANI_WPN_ID_DRAW_SWORD_LEFT:
            case ANI_WPN_ID_DRAW_SWORD_RIGHT:
                aboutToPlay = AUDIO_DRAW_WEAPON;
                break;

            case ANI_WPN_ID_STAND_LEFT:
            case ANI_WPN_ID_STAND_RIGHT:
            case ANI_WPN_ID_ATTACK_LEFT:
            case ANI_WPN_ID_ATTACK_RIGHT:
            case ANI_WPN_ID_GND_MOVE_ATTACK_LEFT:
            case ANI_WPN_ID_GND_MOVE_ATTACK_RIGHT:
            case ANI_WPN_ID_SLIDE_ATTACK_LEFT:
            case ANI_WPN_ID_SLIDE_ATTACK_RIGHT:
            case ANI_WPN_ID_AIR_ATTACK_LEFT:
            case ANI_WPN_ID_AIR_ATTACK_RIGHT:
            case ANI_WPN_ID_AIR_MOVE_ATTACK_LEFT:
            case ANI_WPN_ID_AIR_MOVE_ATTACK_RIGHT:
            case ANI_WPN_ID_AIR_DOWN_ATTACK_LEFT:
            case ANI_WPN_ID_AIR_DOWN_ATTACK_RIGHT:
                aboutToPlay = AUDIO_SWING_ATTACK;
                break;
        }
    }

    if (StateChanged() && !_isTriggeredAni)
    {
        switch (_currentKeyID)
        {
            case KEY_GND_IDLE:
                break;
        }
    }

    if (aboutToPlay != -1)
        CAudio::Instance()->Play(aboutToPlay);
}

void Player::DoDead()
{
    // Set the player to be dead
    _isDead = true;
    // Reset weapon
    SetHoldWeapon(false);
    ResetWeaponID(); // reset to the default weapon - punch
    // Decrement the player's life
    _life--;
    // Activate dead effect
    _battleSystemPtr->TriggerExplosionEffect(this);

    // Display the attacker
    if (_attacker == nullptr)
        _battleSystemPtr->TriggerDisplayMessage(_name + " suicided!", 350, 200, 80); // 2.5 secs
    else
        _battleSystemPtr->TriggerDisplayMessage(_attacker->_name + " killed " + _name + "!", 300, 200, 80); // 2.5 secs
}

void Player::SetRespawnMovementVector(const int& startPosX, const int& startPosY, const int& destinationPosX, const int& destinationPosY)
{
    Vector2 vectorStartToDestination;
    vectorStartToDestination.SetXY(startPosX, startPosY, destinationPosX, destinationPosY);
    double multiplier = (vectorStartToDestination.GetLength() == 0 ? RESPAWN_MOVEMENT_OFFSET_MAGNITUDE : RESPAWN_MOVEMENT_OFFSET_MAGNITUDE / vectorStartToDestination.GetLength()); // Avoid division by 0
    _vectorRespawnMovement = Vector2(DoubleToInteger(vectorStartToDestination.GetX() * multiplier),
                                     DoubleToInteger(vectorStartToDestination.GetY() * multiplier));
}

void Player::DoRespawn()
{
    if (_allowRespawn)
    {
        // Set the player to be alive
        _isDead = false;
        // Set the state of the player to be 'RESPAWN_STATE'
        SetState(RESPAWN_STATE);
        // Set prev length to max integer value
        _preDistance = 10E5;
        // Set the player to be able to dodge while respawning
        SetIsDodging(true);

        // Get the dead position of the player, which is now the current position
        if (_x < SIZE_X / 2) // Respawn from left side
        {
            _x = RESPAWN_LEFT_START_POS_X;
            _y = RESPAWN_LEFT_START_POS_Y;
            _respawnCourier.SetDir(true); // Set the respawn courier direction to face left
        }
        else // Respawn from right side
        {
            _x = RESPAWN_RIGHT_START_POS_X;
            _y = RESPAWN_RIGHT_START_POS_Y;
            _respawnCourier.SetDir(false); // Set the respawn courier direction to face left
        }

        // Set the respawn position
        Ground* g = GetRandomGround(&_grounds);		// Randomly select Ground
        _resDestPosX = random(g->GetCor(0), g->GetCor(2) - GetWidth());		// Randomly set x coordinate within Ground's width
        _resDestPosY = g->GetCor(1) - GetHeight() - RESPAWN_DISTANCE_ABOVE_GROUND;
        //
        SetRespawnMovementVector(_x, _y, _resDestPosX, _resDestPosY);
    }
}

void Player::InitializeOnRespawn()
{
    //
    ResetTriggeredAnimationVariables();
    _finishedTriggeredAniKeyID = 0;
    //
    _isPressingLeft = _isPressingRight = _dir = false;
    _isTriggerPressingLeft = false;
    //
    _isPressingDown = false;
    //
    _isTriggerJump = false;
    ResetJumpCount();
    //
    _horizontalVelocity = OFFSET_INITIAL_VELOCITY;
    _isOffsetLeft = _isOffsetRight = false;
    //
    _verticalVelocity = INITIAL_VELOCITY;
    _verticalAcceleration = INITIAL_ACCELERATION;
    //
    _isTriggerDrawWeapon = false;
    //
    _isHoldingWeapon = _isTriggerAttack = false;
    _takenDmg = INITIAL_TAKEN_DAMAGE;
    //
    _isTriggerDodge = false;
    _isDodging = false;
    //
    ResetWeaponID();
    //
    _currentAniByWpn = 0;
    //
    SetConscious();
    //
    _isFirstTimeOnEdge = true;
    //
    ResetMovementVelocity();
    //
    SetAttacker(nullptr, 0);
    //
    _isDead = false;
    //
    SetState(CONSCIOUS_STATE);
    //
    _vectorRespawnMovement = Vector2();
    _resDestPosX = _resDestPosY = 0;
    //_prevLength is initialized whenever the player 'DoRespawn()'
}

void Player::SetAnimationStateByWeapon(int num)
{
    _currentAniByWpn = num;
}

void Player::AddCAnimationByWeapon(vector<CAnimation>& tempAniByWpn, vector<int>* list, double size, int delay, bool repeat, int times)
//void AddCAnimation(vector<int>*, double = 1.0, int = 10, bool = true, int = 1);
{
    CAnimation temp(repeat, times);

    for (auto i = list->begin(); i != list->end(); i++)
        temp.AddBitmap(*i, RGB(0, 0, 0));

    temp.SetSize(size);
    temp.SetDelayCount(delay);
    tempAniByWpn.push_back(temp);
}

int Player::GetKeyCombination()
{
    /* Key combination structure:
    [ 1 ][ 2 ][ 3 ]

    [ 1 ] determines the player is on air or on ground
    [ 2 ] determine the right button is pressed, the left button is pressed, the down button is pressed, or none of the movement keys are pressed
    [ 3 ] determine whether it is the case that the attack button is pressed and the player is holding a weapon
    */
    string keyCombString = "";

    if (IsOnGround())
        keyCombString = keyCombString + "1";
    else
        keyCombString = keyCombString + "2";

    if (_isPressingRight)
        keyCombString = keyCombString + "2";
    else if (_isPressingLeft)
        keyCombString = keyCombString + "3";
    else if (_isPressingDown)
        keyCombString = keyCombString + "4";
    else // All movement keys are up
        keyCombString = keyCombString + "1";

    if (_isTriggerAttack)
        keyCombString = keyCombString + "2";
    else
        keyCombString = keyCombString + "1";

    /* SPECIAL CASES */
    if (_isTriggerDrawWeapon) // The player draws his weapon
        keyCombString = "113";
    else if (_isTriggerDodge) // The player dodges
        keyCombString = "114";

    return (stoi(keyCombString));
}

void Player::ResetMovementVelocity()
{
    _moveVelocity = 0;
}

void Player::ResetTriggeredAnimationVariables()
{
    SetTriggeredAnimation(false);
    SetTriggeredAnimationKeyID(0);
    SetTriggeredAnimationDir(false);
    SetTriggeredAnimationAnimationID(-1);
    _isInitiatedTriggeredAni = false;
}

void Player::SetTriggeredAnimation(bool newIsTriggeredAni)
{
    _isTriggeredAni = newIsTriggeredAni;
}

void Player::SetTriggeredAnimationKeyID(int newTriggeredAniKeyID)
{
    _triggeredAniKeyID = newTriggeredAniKeyID;
}

void Player::SetTriggeredAnimationDir(bool newTriggeredAniDir)
{
    _triggeredAniDir = newTriggeredAniDir;
}

void Player::SetTriggeredAnimationVariables(int keyCombInt)
{
    for (auto elementPtr : _triggeredAnis)
        if (elementPtr->GetKeyID() == keyCombInt)
            elementPtr->SetTriggeredAnimationVariables();
}

bool Player::IsFinishedTriggeredAnimation()
{
    return (_aniByWpn[_wpnID][_triggeredAniAnimationID].IsFinalBitmap());
}

int Player::GetCurrentAniByWeaponNum()
{
    return _aniByWpn[_wpnID][_currentAniByWpn].GetCurrentBitmapNumber();
}

void Player::SetCurrentNonTriggeredAnimationByWeapon()
{
    /*	~ Remark:
    	~ The player is NOT performing a trigger animation
    	~ The animation is dependent on the weapon (decided by the actual sprite of the player)
    */
    switch (_state)
    {
        case CONSCIOUS_STATE:
            _consciousState.SetCurrentNonTriggeredAnimationByWeapon();
            break;

        case UNCONSCIOUS_STATE:
            if (_unconsciousAniDir)
                SetAnimationStateByWeapon(ANI_WPN_ID_UNCONSCIOUS_FLYING_RIGHT);
            else
                SetAnimationStateByWeapon(ANI_WPN_ID_UNCONSCIOUS_FLYING_LEFT);

            break;

        case RESPAWN_STATE:
            if (_dir)
                SetAnimationStateByWeapon(ANI_WPN_ID_STAND_RIGHT);
            else
                SetAnimationStateByWeapon(ANI_WPN_ID_STAND_LEFT);

            break;

        case IMMUNE_STATE:
            _immuneState.SetCurrentNonTriggeredAnimationByWeapon();
            break;

        default:
            // Never happen
            return;
    }
}

void Player::AddCamera(Camera* cam)
{
    camera = cam;
}

void Player::SetPlayer(int id)
{
    _identifier = id;
}

int Player::GetPlayerMode()
{
    return _identifier;
}

void Player::SetSize(double size)
{
    _size = size;
}

double Player::GetSize()
{
    return _size;
}

void Player::SetRespawn(bool tri)
{
    _allowRespawn = tri;
}

bool Player::StateChanged()
{
    bool ret = false;

    if (_lastTriggeredAniKeyID != _currentKeyID)
        ret = true;

    _lastTriggeredAniKeyID = _currentKeyID;
    return ret;
}

bool Player::WpnStateChanged()
{
    bool ret = false;

    if (_lastTriggeredAniByWpnID != _triggeredAniAnimationID)
        ret = true;

    _lastTriggeredAniByWpnID = _triggeredAniAnimationID;
    return ret;
}

void Player::SetConscious()
{
    SetState(CONSCIOUS_STATE);
    _unconsciousFramesCount = 0;
    _unconsciousAniDir = false;
}

const bool& Player::GetTriggeredAnimationDirection() const
{
    return (_triggeredAniDir);
}

void Player::SetTriggeredAnimationAnimationID(const int& newTriggeredAniAnimationID)
{
    _triggeredAniAnimationID = newTriggeredAniAnimationID;
}

void Player::SetIsTriggerAttack(const bool& newIsTriggerAttack)
{
    _isTriggerAttack = newIsTriggerAttack;
}

void Player::SetIsDodging(const bool& newIsDodging)
{
    _isDodging = newIsDodging;
}

void Player::SetIsTriggerDodge(const bool& newIsTriggerDodge)
{
    _isTriggerDodge = newIsTriggerDodge;
}

void Player::SetIsTriggerDrawWeapon(const bool& newIsTriggerDrawWeapon)
{
    _isTriggerDrawWeapon = newIsTriggerDrawWeapon;
}

void Player::EmptyHitTargetPlayers()
{
    _hitTargetPlayers.clear();
}

void Player::SetX(const int& newX)
{
    _x = newX;
}

void Player::SetY(const int& newY)
{
    _y = newY;
}

bool Player::IsOnGround()
{
    Ground* ground = OnGround();
    return (ground != nullptr ? true : false);
}

Ground* Player::OnGround()
{
    int playerX1 = GetCor(0);
    int playerY1 = GetCor(1);
    int playerX2 = GetCor(2);
    int playerY2 = GetCor(3);

    for (auto groundPtr : _grounds)
    {
        int groundX1 = groundPtr->GetCor(0);
        int groundY1 = groundPtr->GetCor(1);
        int groundX2 = groundPtr->GetCor(2);
        int groundY2 = groundPtr->GetCor(3);

        if (groundPtr->IsOnGround(playerX1, playerY1, playerX2, playerY2))
            return groundPtr;
    }

    return nullptr;
}

ExplosionEffect* Player::GetExplosionEffect()
{
    return (_explosionEffectPtr);
}

void Player::SetState(const int& newState)
{
    _state = newState;
}

bool Player::IsOnEdge()
{
    return (IsOnLeftEdge() || IsOnRightEdge());
}

void Player::DoHorizontalOffset()
{
    if (_isOffsetLeft)
    {
        if (_horizontalVelocity > 0)
        {
            _horizontalVelocity--;
            _x -= DoubleToInteger(_horizontalVelocity);
        }
        else
        {
            _isOffsetLeft = false;
        }
    }
    else if (_isOffsetRight)
    {
        if (_horizontalVelocity > 0)
        {
            _horizontalVelocity--;
            _x += DoubleToInteger(_horizontalVelocity);
        }
        else
        {
            _isOffsetRight = false;
        }
    }
}

void Player::DoLand()
{
    _verticalAcceleration = LANDING_ACCELERATION;
}

const double& Player::GetVerticalVelocity() const
{
    return (_verticalVelocity);
}

const int& Player::GetTakenDamage() const
{
    return (_takenDmg);
}

const int& Player::GetState() const
{
    return (_state);
}

int Player::GetSpecializedTakenDamage() const
{
    return (_takenDmg);
}

}