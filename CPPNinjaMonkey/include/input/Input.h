#pragma once
#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED


#include "boost/signals2/signal.hpp"
#include <string>


// -----------------------------------------------------------------------------


struct RenderWindow;


// -----------------------------------------------------------------------------


struct Input
{
	typedef boost::signals2::signal<void (const std::string&, bool)> ButtonUpdateSignal;
	typedef boost::signals2::signal<void (const std::string&, float)> AxisUpdateSignal;

	virtual ~Input() {}

	virtual bool IsAxisRelative(const std::string& sAxisName) const = 0;

	virtual boost::signals2::connection ConnectButtonUpdateSlot(const ButtonUpdateSignal::slot_type& slot) = 0;
	virtual boost::signals2::connection ConnectAxisUpdateSlot(const AxisUpdateSignal::slot_type& slot) = 0;

	// TODO: Consider methods for enumerating axes and keys.

	// Discuss: This is cross-platform-contamination; input is not necessarily tied 
	// to the window.
	// TODO: Move this to the windows-specific implementation.
	static Input* CreateInstance(RenderWindow& renderWindow);
};


#endif // INPUT_H_INCLUDED