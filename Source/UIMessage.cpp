/* File description:
This is the source code file 'TriggeredAnimation.cpp'. This is the implementation for the class 'TriggeredAnimation'. */

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
//
#include "UIMessage.h"

namespace game_framework
{
UIMessage::~UIMessage()
{
    /* Body intentionally empty */
}

UIMessage::UIMessage() : UIMessage(string(), int(), int(), int(), int())
{
    /* Body intentionally empty */
}

UIMessage::UIMessage(const UIMessage& objectValue) : UIMessage(objectValue._message, objectValue._x, objectValue._y, objectValue._currentFrame, objectValue._maxFrame)
{
    /* Body intentionally empty */
}

UIMessage& UIMessage::operator=(const UIMessage& rightObject)
{
    if (this != &rightObject)
    {
        _message = rightObject._message;
        _x = rightObject._x;
        _y = rightObject._y;
        _currentFrame = rightObject._currentFrame;
        _maxFrame = rightObject._maxFrame;
    }

    return (*this);
}

UIMessage::UIMessage(const string& messageValue, const int& xValue, const int& yValue, const int& currentFrameValue, const int& maxFrameValue) : _message(messageValue), _x(xValue), _y(yValue), _currentFrame(currentFrameValue), _maxFrame(maxFrameValue)
{
    /* Body intentionally empty */
}

const string& UIMessage::GetMessage() const
{
    return (_message);
}

const int& UIMessage::GetX() const
{
    return (_x);
}

const int& UIMessage::GetY() const
{
    return (_y);
}

const int& UIMessage::GetCurrentFrame() const
{
    return (_currentFrame);
}

const int& UIMessage::GetMaxFrame() const
{
    return (_maxFrame);
}

void UIMessage::Initialize(const string& messageValue, const int& xValue, const int& yValue, const int& maxFrameValue)
{
    _message = messageValue;
    _x = xValue;
    _y = yValue;
    _currentFrame = 0;
    _maxFrame = maxFrameValue;
}

void UIMessage::IncrementCurrentFrame()
{
    _currentFrame++;
}

}