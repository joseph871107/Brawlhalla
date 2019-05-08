#ifndef UI_H
#define UI_H
#include "object.h"
#include "fImage.h"
#define BUTTON_OUTSIDE 0
#define BUTTON_HOVER 1
#define BUTTON_CLICK 2
#define BUTTON_RELEASE 3


namespace game_framework
{
class UI_Button : public Object
{
public:
	UI_Button(int tx, int ty, int twidth, int theight, int tri, int tpos_x, int tpos_y);
	UI_Button(int tx, int ty, COLORREF color, int out, int hov, int clk, int tri, int tpos_x, int tpos_y);
	bool InRange(CPoint point);
	int GetTrigger();
	int GetState();
	void SetState(UINT nFlags, UINT nChar, CPoint point, vector<vector<int>> *_pos);
	string GetName();
	void AddCamera(Camera *cam);
	void SetOffset(CPoint point);
	void SetName(string _name);
	void SetStr(string _str);
	void OnShow();
	void Reset();
	void LoadBitmap(int out, int hov, int clk, COLORREF color);
private:
	string name, str;
	int state, trigger, pos_x, pos_y, offsetX, offsetY;
	UINT _bounce, _bounce_key, isBitmapLoaded;
	Object _outside, _hover, _click;
	CPoint _point;
};

class UI
{
public:
	UI(int = 1, int = 1);
	~UI();
	string ChosenButton();
	int GetButtonState(string name);
	void SetButtonState(UINT nFlags, UINT nChar, CPoint point);
	void AddCamera(Camera *cam);
	void AddButton(string name, int x = 0, int y = 0, int width = 50, int height = 50, int tpx = 0, int tpy = 0, string str = "", int tri = BUTTON_RELEASE);
	void AddButton(string name, int x, int y, COLORREF color, int out, int hov, int clk, int tpx = 0, int tpy = 0, string str = "", int tri = BUTTON_RELEASE);
	void SetXY(int x, int y);
	void DelButton(string name);
	void OnShow(int offsetX = 0, int offsetY = 0);
	void Reset();
	void OnKeyDown(UINT, UINT, UINT);
	vector<UI_Button*>::iterator Index(string name);
private:
	int offsetX, offsetY;
	vector<UI_Button*> _buttons;
	vector< vector<int> > _pos;
	void ChangeButton(int, int);
	bool first = true;
	Camera *camera;
};
}

#endif //define UI_H