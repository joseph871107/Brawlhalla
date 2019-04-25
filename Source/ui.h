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
struct UI_Button
{
    UI_Button(int tx, int ty, int twidth, int theight, int tri, int tpos_x, int tpos_y)
    {
		x = tx, y = ty, width = twidth, height = theight; pos_x = tpos_x; pos_y = tpos_y;
        state = isBitmapLoaded = _bounce = _bounce_key = 0;
        trigger = tri;
    }
    string name, str;
    int x, y, width, height, state, trigger, pos_x, pos_y;
    UINT _bounce, _bounce_key, isBitmapLoaded;
    FImage* _outside, *_hover, *_click;
    bool InRange(CPoint point)
    {
        int tx = point.x + 3, ty = point.y + 3;
        return (tx >= x && tx <= x + width && ty >= y && ty <= y + height);
    }
    void SetState(UINT nFlags, UINT nChar, CPoint point, vector<vector<int>> *_pos)
    {
        if (nFlags)
        {
            if (InRange(point))
                state = BUTTON_CLICK;
        }
        else
        {
            if (InRange(point) || (*_pos)[pos_x][pos_y] == 1)
            {
                if (_bounce && InRange(point))
                    state = BUTTON_RELEASE;
				else {
					state = BUTTON_HOVER;
					for (auto &i : (*_pos)) {
						for (auto &j : i) {
							if (j == 1)
								j = 0;
						}
					}
					(*_pos)[pos_x][pos_y] = 1;
				}
            }
            else
                state = BUTTON_OUTSIDE;
        }
		if ((*_pos)[pos_x][pos_y] == 1) {
			if (nChar) {
				state = BUTTON_CLICK;
			}
			else {
				if (_bounce_key)
					state = BUTTON_RELEASE;
			}
		}

		_bounce = nFlags;
		_bounce_key = nChar;
    }
    void OnShow(int _x, int _y)
    {
        if (isBitmapLoaded)
        {
            switch (state)
            {
                case BUTTON_OUTSIDE:
                    _outside->ShowBitmap();
                    break;

                case BUTTON_HOVER:
                    _hover->ShowBitmap();
                    break;

                case BUTTON_CLICK:
                case BUTTON_RELEASE:
                    _click->ShowBitmap();
                    break;

                default:
                    break;
            }
        }
        else
        {
            switch (state)
            {
                case BUTTON_OUTSIDE:
                    DrawRectangle(_x, _y, width, height, RGB(255, 0, 0));
                    break;

                case BUTTON_HOVER:
                    DrawRectangle(_x, _y, width, height, RGB(0, 0, 255));
                    break;

                case BUTTON_CLICK:
                case BUTTON_RELEASE:
                    DrawRectangle(_x, _y, width, height, RGB(0, 255, 0));
                    break;

                default:
                    break;
            }

			OnShowText(name, _x, _y, 10, RGB(255, 255, 255));
			OnShowText(str, _x + (width - str.length() * 12) / 2, _y + (height - 12) / 2, 10, RGB(255, 255, 255));
        }
    }
    void Reset()
    {
        state = _bounce = 0;
    }
};

class UI
{
public:
	UI(int = 1, int = 1);
	~UI();
	string ChosenButton();
	int GetButtonState(string name);
	void SetButtonState(UINT nFlags, UINT nChar, CPoint point);
	void AddButton(string name, int x = 0, int y = 0, int width = 50, int height = 50, int tpx = 0, int tpy = 0, string str = "", int tri = BUTTON_RELEASE);
	void SetXY(int x, int y);
	void DelButton(string name);
	void OnShow();
	void Reset();
	void OnKeyDown(UINT, UINT, UINT);
	vector<UI_Button*>::iterator Index(string name);
private:
	int offsetX, offsetY;
	vector<UI_Button*> _buttons;
	vector< vector<int> > _pos;
	void ChangeButton(int, int);
	bool first = true;
};
}

#endif //define UI_H