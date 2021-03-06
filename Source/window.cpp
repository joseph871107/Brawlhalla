#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "object.h"
#include "window.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Window class
/////////////////////////////////////////////////////////////////////////////

Window::Window(int _width, int _height)
{
    width = _width;
    height = _height;
}

Window::~Window()
{
    for (auto element : items)
    {
        delete element;
    }

    camera = nullptr;
    delete ani;
}

void Window::Initialize(int _buttonCol, int _buttonRow, bool buttonEnable, bool visible)
{
    ui = UI(_buttonCol, _buttonRow);
    ui.AddCamera(camera);
    _lButton = false;
    _key = false;
    _buttonEnable = buttonEnable;
    _visible = visible;
    _point.x = 0;
    _point.y = 0;
}

void Window::AddItem(Object* item)
{
    item->AddCamera(camera);
    items.push_back(item);
}

void Window::AddAni(CAnimation* _ani)
{
    ani = _ani;
}

void Window::SetButtonEnable(bool enable)
{
    _buttonEnable = enable;
}

void Window::SetVisible(bool visible)
{
    _visible = visible;
}

void Window::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (_buttonEnable)
        ui.OnKeyDown(nChar, nRepCnt, nFlags);

    if (nChar == KEY_ENTER)
        _key = true;
}

void Window::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == KEY_ENTER)
        _key = false;
}

void Window::OnLButtonDown(UINT nFlags, CPoint point)
{
    _lButton = true;
}

void Window::OnLButtonUp(UINT nFlags, CPoint point)
{
    _lButton = false;
}

void Window::OnMouseMove(UINT nFlags, CPoint point)
{
    _point = point;
}

void Window::OnShow()
{
    if (_visible)
    {
        if (ani != nullptr)
            ani->OnShow();

        for (auto item : items)
            item->OnShow(x, y);

        ui.OnShow(x, y);
    }
}

void Window::OnMove()
{
    if (_visible && ani != nullptr)
        ani->OnMove();

    if (_buttonEnable)
        ui.SetButtonState(_lButton, _key, _point);

    if (camera != nullptr)
    {
        CPoint cam = camera->GetXY(x, y);
        ui.SetXY(cam.x, cam.y);
    }
    else
        ui.SetXY(x, y);
}
UI* Window::GetUI()
{
    return &ui;
}
}