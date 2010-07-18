#include "input/win32/WindowsMessageInput.h"


// -----------------------------------------------------------------------------


class WindowsMessageInputImpl : public WindowsMessageInput
{
public:
	WindowsMessageInputImpl(RenderWindow* pRenderWindow)
	{
		// TODO: Refactor render window into win32 and platform-independent parts.
		// TODO: Use Win32-specific render window interface and connect to message 
		// processing.
	}


	virtual boost::signals2::connection WindowsMessageInput::ConnectKeySlot(const KeySignal::slot_type& slot)
	{
		return m_key.connect(slot);
	}


private:
	KeySignal m_key;
};


// -----------------------------------------------------------------------------


WindowsMessageInput* CreateWindowsMessageInput(RenderWindow* pRenderWindow)
{
	return new WindowsMessageInputImpl(pRenderWindow);
}
