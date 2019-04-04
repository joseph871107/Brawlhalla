#pragma   once
#ifndef UI_H
#define UI_H
#include "object.h"
#define BUTTON_OUTSIDE 0
#define BUTTON_HOVER 1
#define BUTTON_CLICK 2
#define BUTTON_RELEASE 3


namespace game_framework
{
	struct UI_Button {
		UI_Button(int tx, int ty, int twidth, int theight, int tri) {
			x = tx, y = ty, width = twidth, height = theight; state = isBitmapLoaded = _bounce = 0;
			trigger = tri;
		}
		string name;
		int x, y, width, height, state, trigger;
		UINT _bounce, isBitmapLoaded;
		CMovingBitmap* _outside, *_hover, *_click;
		bool InRange(CPoint point) {
			int tx = point.x + 3, ty = point.y + 3;
			return (tx >= x && tx <= x + width && ty >= y && ty <= y + height);
		}
		void SetState(UINT nFlags, CPoint point) {
			if (nFlags) {
				if (InRange(point))
					state = BUTTON_CLICK;
			}
			else
			{
				if (InRange(point)) {
					if (_bounce)
						state = BUTTON_RELEASE;
					else
						state = BUTTON_HOVER;
				}
				else
					state = BUTTON_OUTSIDE;
			}
			_bounce = nFlags;
		}
		void OnShow() {
			if (isBitmapLoaded) {
				switch (state) {
				case BUTTON_OUTSIDE:
					_outside->ShowBitmap();
					break;
				case BUTTON_HOVER:
					_hover->ShowBitmap();
					break;
				case BUTTON_CLICK: case BUTTON_RELEASE:
					_click->ShowBitmap();
					break;
				default:
					break;
				}
			}
			else {
				switch (state) {
				case BUTTON_OUTSIDE:
					DrawRectangle(x, y, width, height, RGB(255, 0, 0));
					break;
				case BUTTON_HOVER:
					DrawRectangle(x, y, width, height, RGB(0, 0, 255));
					break;
				case BUTTON_CLICK: case BUTTON_RELEASE:
					DrawRectangle(x, y, width, height, RGB(0, 255, 0));
					break;
				default:
					break;
				}
				OnShowText(name, x, y, 10, RGB(255, 255, 255));
			}
		}
		void Reset() {
			state = _bounce = 0;
		}
	};

	class UI {
	public:
		string ChosenButton(UINT nFlags, CPoint point);
		int GetButtonState(string name);
		void SetButtonState(UINT nFlags, CPoint point);
		void AddButton(string name, int x = 0, int y = 0, int width = 50, int height = 50, int tri = BUTTON_RELEASE);
		void DelButton(string name);
		void OnShow();
		void Reset();
		vector<UI_Button*>::iterator Index(string name);
	private:
		vector<UI_Button*> _buttons;
		
	};
}

#endif //define UI_H