#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "ui.h"

namespace game_framework
{
	string UI::ChosenButton(UINT nFlags, CPoint point)
	{
		for (auto i = _buttons.begin(); i != _buttons.end(); i++) {
			if ((*i)->state == (*i)->trigger)
				return (*i)->name;
		}
		return "empty";
	}
	int UI::GetButtonState(string name)
	{
		return (*Index(name))->state;
	}
	void UI::SetButtonState(UINT nFlags, CPoint point)
	{
		for (auto i = _buttons.begin(); i != _buttons.end(); i++) {
			(*i)->SetState(nFlags, point);
		}
	}
	void UI::AddButton(string name, int x, int y, int width, int height, int tri)
	{
		UI_Button* button = new UI_Button(x, y, width, height, tri);
		button->name = name;
		_buttons.push_back(button);
	}
	void UI::DelButton(string name)
	{
		auto erase = Index(name);
		if (erase != _buttons.end()) {
			delete(*erase);
			_buttons.erase(erase);
		}
	}
	void UI::OnShow()
	{
		for (auto i = _buttons.begin(); i != _buttons.end(); i++) {
			(*i)->OnShow();
		}
	}
	void UI::Reset()
	{
		for (auto i = _buttons.begin(); i != _buttons.end(); i++) {
			(*i)->Reset();
		}
	}
	vector<UI_Button*>::iterator UI::Index(string name)
	{
		for (auto i = _buttons.begin(); i != _buttons.end(); i++) {
			if ((*i)->name == name)
				return i;
		}
		return _buttons.begin();
	}
}