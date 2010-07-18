#pragma once
#ifndef WINDOWSMESSAGEINPUT_H_INCLUDED
#define WINDOWSMESSAGEINPUT_H_INCLUDED


#include "../Input.h"


// -----------------------------------------------------------------------------


class RenderWindow;


// -----------------------------------------------------------------------------


struct WindowsMessageInput : public Input
{
};


// -----------------------------------------------------------------------------


extern WindowsMessageInput* CreateWindowsMessageInput(RenderWindow* pRenderWindow);


#endif // WINDOWSMESSAGEINPUT_H_INCLUDED