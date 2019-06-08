#ifndef MAP_H
#define MAP_H
#include "gameComFunc.h"
#include "camera.h"

#define GENERATE_COLLISION_ARRAY false

namespace game_framework
{

struct MapPARM
{
    MapPARM(string name, BkPARM bk, int groundNum, GroundPARM n_grounds, ...) : _bkP(bk)
    {
        _name = name;
        va_list ap;
        va_start(ap, groundNum);

        for (int i = 0; i < groundNum; i++)
            _groundsP.push_back(va_arg(ap, GroundPARM));

        va_end(ap);
    }
    string _name;
    BkPARM _bkP;
    vector<GroundPARM> _groundsP;
};

/////////////////////////////////////////////////////////////////////////////
// Map class
/////////////////////////////////////////////////////////////////////////////

class Map
{
    public:
        Map();
        Map(vector<Player*>* players);
        ~Map();
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT);							// ��L���U
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
        void AddCamera(Camera* cam);
        void AddBackground(BkPARM* i);
        void AddGround(GroundPARM* i);
        void AddPlayers(vector<Player*>* players);
        int GetTime();
        vector<Ground*>* GetGrounds();
        vector<Weapon*>* GetDroppingWeapons();
        void PlayerThrowWeapon(Player* thrower);
        Background background; // TODO: This is a variable, and should not be declared as public (Code from Joseph)


    private:
        //-----------------FUNCTIONS DECLARATIONS-----------------//
        void ClearFlyingWeapons();
        void ClearDroppingWeapons();
        void ProcessWeaponsOutOfMap(vector<Weapon*>& weapons);
        void ProcessFlyingWeaponsExpired();
        void EvaluatePickingWeaponOnKeyDown(vector<Weapon*>& weapons, const UINT& nChar);
        //-----------------VARIABLES DECLARATIONS-----------------//
        clock_t start, lastTime;
        int nextTimeGenerateWeapon, maxWeapons;
        vector<Ground*> _grounds;
        vector<Player*>* _players;
        vector<Weapon*> _droppingWeapons;
        vector<Weapon*> _flyingWeapons;
        Camera* camera;
};

}

#endif															//define OBJECT_H