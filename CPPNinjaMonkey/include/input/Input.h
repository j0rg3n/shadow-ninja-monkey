#pragma once
#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED


#include "boost/signals2/signal.hpp"


// -----------------------------------------------------------------------------


struct RenderWindow;


// -----------------------------------------------------------------------------


struct Input
{
	typedef int KeyCode;
	typedef boost::signals2::signal<void (KeyCode, bool)> KeySignal;

	virtual ~Input() {}

	virtual boost::signals2::connection ConnectKeySlot(const KeySignal::slot_type& slot) = 0;

	// Discuss: This is cross-platform-contamination; input is not necessarily tied 
	// to the window.
	// TODO: Move this to the windows-specific implementation.
	static Input* CreateInstance(RenderWindow& renderWindow);
};


#endif // INPUT_H_INCLUDED