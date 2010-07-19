#pragma once
#ifndef WINDOWSMESSAGEINPUT_H_INCLUDED
#define WINDOWSMESSAGEINPUT_H_INCLUDED


#include "../Input.h"


// -----------------------------------------------------------------------------


struct RenderWindow;


// -----------------------------------------------------------------------------


struct WindowsMessageInput : Input
{
	static WindowsMessageInput* CreateInstance(RenderWindow& renderWindow);
};


#endif // WINDOWSMESSAGEINPUT_H_INCLUDED