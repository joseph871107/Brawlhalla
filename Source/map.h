#ifndef MAP_H
#define MAP_H
#include "gameComFunc.h"
#include "camera.h"

#define GENERATE_COLLISION_ARRAY false

namespace game_framework
{

	struct MapPARM {
		MapPARM(BkPARM bk, int groundNum, GroundPARM n_grounds, ...) : _bkP(bk)
		{
			va_list ap;
			va_start(ap, groundNum);
			for (int i = 0; i < groundNum; i++)
				_groundsP.push_back(va_arg(ap, GroundPARM));
			va_end(ap);
		}
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
		Map(vector<Player*> *players);
		~Map();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT);							// ��L���U
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
		void AddCamera(Camera *cam);
		void AddBackground(BkPARM *i);
		void AddGround(GroundPARM *i);
		void AddPlayers(vector<Player*> *players);
		int GetTime();
		vector<Ground*> *GetGrounds();
		vector<Weapon*> *GetWeapons();
		Background background;
	private:
		void ProcessWeaponOutOfMap();
		//-----------------VARIABLES DECLARATIONS-----------------//
		clock_t start, lastTime;
		int nextTimeGenerateWeapon, maxWeapons;
		vector<Ground*> _grounds;
		vector<Player*> *_players;
		vector<Weapon*> _weapons;
		Camera *camera;
	};

}

#endif															//define OBJECT_H