#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "gameComFunc.h"
#include "ui.h"

namespace game_framework
{
	UI::UI(int x, int y)
	{
		x = (x < 1 ? 1 : x);
		y = (y < 1 ? 1 : y);
		for (int i = 0; i < x; i++) {
			vector<int> temp;
			for (int j = 0;j < y;j++) {
				temp.push_back(-1);
			}
			_pos.push_back(temp);
		}
	}
	UI::~UI()
{
    for (auto element : _buttons)
    {
        delete element;
    }
}

string UI::ChosenButton()
{
    for (auto i = _buttons.begin(); i != _buttons.end(); i++)
    {
        if ((*i)->state == (*i)->trigger)
            return (*i)->name;
    }

    return "empty";
}
int UI::GetButtonState(string name)
{
    return (*Index(name))->state;
}
void UI::SetButtonState(UINT nFlags, UINT nChar, CPoint point)
{
    for (auto i = _buttons.begin(); i != _buttons.end(); i++)
    {
        (*i)->SetState(nFlags, nChar, CPoint(point.x - offsetX, point.y - offsetY), &_pos);
    }
}
void UI::AddButton(string name, int x, int y, int width, int height, int tpx, int tpy, int tri)
{
    UI_Button* button = new UI_Button(x, y, width, height, tri, tpx, tpy);
    button->name = name;
    _buttons.push_back(button);
	static bool first = true;
	if (!first) {
		_pos[tpx][tpy] = 0;
	}
}
void UI::SetXY(int x, int y)
{
	offsetX = x;
	offsetY = y;
}
void UI::DelButton(string name)
{
    auto erase = Index(name);

    if (erase != _buttons.end())
    {
        delete (*erase);
        _buttons.erase(erase);
    }
}
void UI::OnShow()
{
    for (auto i = _buttons.begin(); i != _buttons.end(); i++)
    {
        (*i)->OnShow((*i)->x + offsetX, (*i)->y + offsetY);
    }
}
void UI::Reset()
{
    for (auto i = _buttons.begin(); i != _buttons.end(); i++)
    {
		(*i)->Reset();
	}
	for (int i = 0; i < (signed int)_pos.size(); i++) {
		for (int j = 0;j < (signed int)_pos[0].size();j++) {
			if (_pos[i][j] == 1) {
				_pos[i][j] = 0;
			}
		}
	}
	first = true;
}

void UI::ChangeButton(int i, int j){
	if (first) {
		_pos[0][0] = 1;
		first = false;
	}
	else {
		int x, y;
		for (int i = 0; i < (signed int)_pos.size(); i++) {
			for (int j = 0;j < (signed int)_pos[0].size();j++) {
				if (_pos[i][j] == 1) {
					x = i;
					y = j;
					_pos[i][j] = 0;
				}
			}
		}
		x = (x + i >= 0 && x + i < (signed int)_pos.size() ? x + i : x);
		y = (y + j >= 0 && y + j < (signed int)_pos[0].size() ? y + j : y);
		_pos[x][y] = 1;
	}
}

void UI::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case KEY_UP:
		ChangeButton(0, -1);
		break;
	case KEY_DOWN:
		ChangeButton(0, 1);
		break;
	case KEY_LEFT:
		ChangeButton(-1, 0);
		break;
	case KEY_RIGHT:
		ChangeButton(1, 0);
		break;
	}
}
vector<UI_Button*>::iterator UI::Index(string name)
{
    for (auto i = _buttons.begin(); i != _buttons.end(); i++)
    {
        if ((*i)->name == name)
            return i;
    }

    return _buttons.begin();
}
}