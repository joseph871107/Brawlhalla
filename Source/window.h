#ifndef WINDOW_H
#define WINDOW_H
#include "ui.h"
#include <map>

#define GENERATE_COLLISION_ARRAY false

namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// Window class
	/////////////////////////////////////////////////////////////////////////////

	class Window : public Object, public CGameState
	{
	public:
		Window(CGame* g, int _width = SIZE_X, int _height = SIZE_Y);
		~Window();
		void Initialize(int _buttonCol, int _buttonRow, bool buttonEnable = true, bool visible = true);
		void AddButton(string name, int x, int y, int width, int height, int tpx, int tpy, int tri = BUTTON_RELEASE);
		void AddItem(Object*);
		void SetButtonEnable(bool enable);
		void SetVisible(bool visible);
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnLButtonUp(UINT nFlags, CPoint point);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnShow();
		void OnMove();
		void Reset();
		string ChosenButton();
	protected:
		CPoint _point;
		UI ui;
		bool _lButton;
		bool _key;
		bool _buttonEnable;
		bool _visible;
		vector<Object*> items;
	};
}

#endif															//define WINDOW_H