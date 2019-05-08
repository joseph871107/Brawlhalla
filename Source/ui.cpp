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
	camera = nullptr;
}

string UI::ChosenButton()
{
    for (auto i = _buttons.begin(); i != _buttons.end(); i++)
    {
        if ((*i)->GetState() == (*i)->GetTrigger())
            return (*i)->GetName();
    }

    return "empty";
}
int UI::GetButtonState(string name)
{
    return (*Index(name))->GetState();
}
void UI::SetButtonState(UINT nFlags, UINT nChar, CPoint point)
{
    for (auto i = _buttons.begin(); i != _buttons.end(); i++)
    {
        (*i)->SetState(nFlags, nChar, CPoint(point.x, point.y), &_pos);
    }
}
void UI::AddCamera(Camera * cam)
{
	camera = cam;
}
void UI::AddButton(string name, int x, int y, int width, int height, int tpx, int tpy, string str, int tri)
{
    UI_Button* button = new UI_Button(x, y, width, height, tri, tpx, tpy);
    button->SetName(name);
	button->SetStr(str);
	button->AddCamera(camera);
    _buttons.push_back(button);
	static bool first = true;
	if (!first) {
		_pos[tpx][tpy] = 0;
	}
}
void UI::AddButton(string name, int x, int y, COLORREF color, int out, int hov, int clk, int tpx, int tpy, string str, int tri)
{
	UI_Button* button = new UI_Button(x, y, color, out, hov, clk, tri, tpx, tpy);
	button->SetName(name);
	button->SetStr(str);
	button->AddCamera(camera);
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
void UI::OnShow(int offsetX, int offsetY)
{
    for (auto i = _buttons.begin(); i != _buttons.end(); i++)
    {
		(*i)->SetOffset(CPoint(offsetX, offsetY));
        (*i)->OnShow();
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
			_pos[i][j] = 0;
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
		ChangeButton(-1, 0);
		break;
	case KEY_DOWN:
		ChangeButton(1, 0);
		break;
	case KEY_LEFT:
		ChangeButton(0, -1);
		break;
	case KEY_RIGHT:
		ChangeButton(0, 1);
		break;
	}
}
vector<UI_Button*>::iterator UI::Index(string name)
{
    for (auto i = _buttons.begin(); i != _buttons.end(); i++)
    {
        if ((*i)->GetName() == name)
            return i;
    }

    return _buttons.begin();
}
UI_Button::UI_Button(int tx, int ty, int twidth, int theight, int tri, int tpos_x, int tpos_y)
{
	x = tx, y = ty, width = twidth, height = theight; pos_x = tpos_x; pos_y = tpos_y;
	state = isBitmapLoaded = _bounce = _bounce_key = offsetX = offsetY = 0;
	trigger = tri;
}
UI_Button::UI_Button(int tx, int ty, COLORREF color, int out, int hov, int clk, int tri, int tpos_x, int tpos_y)
{
	LoadBitmap(out, hov, clk, color);
		x = tx, y = ty, width = _outside.GetWidth(), height = _outside.GetHeight(); pos_x = tpos_x; pos_y = tpos_y;
	state = _bounce = _bounce_key = offsetX = offsetY = 0;
	isBitmapLoaded = true;
	trigger = tri;
}
bool UI_Button::InRange(CPoint point)
{
	int _x = point.x + 3, _y = point.y + 3, x1 = x + offsetX, y1 = y + offsetY, x2 = x1 + width, y2 = y1 + height;
	if (camera != nullptr) {
		CPoint cam = camera->GetXY(x1, y1), cam2 = camera->GetCameraXY();
		x1 = cam.x;
		y1 = cam.y;
		x2 = cam.x + (int)(GetWidth() * camera->GetSize());
		y2 = cam.y + (int)(GetHeight() * camera->GetSize());
	}
	return (_x >= x1 && _x <= x2 && _y >= y1 && _y <= y2);
}
int UI_Button::GetTrigger()
{
	return trigger;
}
int UI_Button::GetState()
{
	return state;
}
void UI_Button::SetState(UINT nFlags, UINT nChar, CPoint point, vector<vector<int>>* _pos)
{
	_point = point;
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
				for (auto &i : (*_pos))
					for (auto &j : i)
						j = 0;
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
string UI_Button::GetName()
{
	return name;
}
void UI_Button::AddCamera(Camera * cam)
{
	_outside.AddCamera(cam);
	_hover.AddCamera(cam);
	_click.AddCamera(cam);
	camera = cam;
}
void UI_Button::SetOffset(CPoint point)
{
	offsetX = point.x;
	offsetY = point.y;
}
void UI_Button::SetName(string _name)
{
	name = _name;
}
void UI_Button::SetStr(string _str)
{
	str = _str;
}
void UI_Button::OnShow()
{
	int textSize = 50;
	SIZE strSize= GetStringSize(str, textSize);
	int _x = x + offsetX, _y = y + offsetY, _x2 = x + (GetWidth() - strSize.cx) / 2 + offsetX, _y2 = y + (GetHeight() - strSize.cy) / 2 + offsetY;
	double size = _size;
	if (camera != nullptr) {
		CPoint cam = camera->GetXY(_x, _y), cam2 = camera->GetXY(_x2, _y2);
		_x = cam.x; _y = cam.y;
		_x2 = cam2.x; _y2 = cam2.y;
		size = _size * camera->GetSize();
	}
	if (isBitmapLoaded)
	{
		switch (state)
		{
		case BUTTON_OUTSIDE:
			_outside.SetXY(_x, _y);
			_outside.OnShow();
			break;

		case BUTTON_HOVER:
			_hover.SetXY(_x, _y);
			_hover.OnShow();
			break;

		case BUTTON_CLICK:
		case BUTTON_RELEASE:
			_click.SetXY(_x, _y);
			_click.OnShow();
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
			DrawRectangle(_x, _y, (int)(GetWidth() * (camera != nullptr ? camera->GetSize() : 1)), (int)(GetHeight() * (camera != nullptr ? camera->GetSize() : 1)), RGB(255, 0, 0));
			break;

		case BUTTON_HOVER:
			DrawRectangle(_x, _y, (int)(GetWidth() * (camera != nullptr ? camera->GetSize() : 1)), (int)(GetHeight() * (camera != nullptr ? camera->GetSize() : 1)), RGB(0, 0, 255));
			break;

		case BUTTON_CLICK:
		case BUTTON_RELEASE:
			DrawRectangle(_x, _y, (int)(GetWidth() * (camera != nullptr ? camera->GetSize() : 1)), (int)(GetHeight() * (camera != nullptr ? camera->GetSize() : 1)), RGB(0, 255, 0));
			break;

		default:
			break;
		}
		OnShowText(name, _x, _y, (int)(20 * (camera != nullptr ? camera->GetSize() : 1)));
		OnShowText(str, _x2, _y2, (int)(textSize * (camera != nullptr ? camera->GetSize() : 1)));
	}
}
void UI_Button::Reset()
{
	state = _bounce = 0;
}
void UI_Button::LoadBitmap(int out, int hov, int clk, COLORREF color)
{
	_outside.LoadBitmap(out, color);
	_hover.LoadBitmap(hov, color);
	_click.LoadBitmap(clk, color);
	isBitmapLoaded = true;
}
}