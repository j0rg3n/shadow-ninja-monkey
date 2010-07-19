#include "input/windows/WindowsMessageInput.h"
#include "render/windows/WindowsRenderWindow.h"

#include "boost/bind.hpp"

#include <Windows.h>
#include <WindowsX.h> //< GET_X_LPARAM, GET_Y_LPARAM

#include <map>
#include <string>


// -----------------------------------------------------------------------------


using std::map;
using std::string;
using namespace boost;
using namespace boost::signals2;


// -----------------------------------------------------------------------------


static const char* MOUSE_WHEEL_AXIS_NAME = "MOUSEWHEEL";
static const char* MOUSE_X_AXIS_NAME = "MOUSEX";
static const char* MOUSE_Y_AXIS_NAME = "MOUSEY";


// -----------------------------------------------------------------------------


class WindowsMessageInputImpl : public WindowsMessageInput
{
public:
	WindowsMessageInputImpl(RenderWindow& renderWindow) :
	  m_nPrevMouseX(0),
	  m_nPrevMouseY(0)
	{
		PopulateVKNameMap(m_vkNameMap);
		m_windowsMessageConnection = ((WindowsRenderWindow*) &renderWindow)->ConnectWindowsMessageSlot(bind(&WindowsMessageInputImpl::OnWindowsMessage, this, _1, _2, _3, _4));
	}


	virtual ~WindowsMessageInputImpl()
	{
		m_windowsMessageConnection.disconnect();
	}


	virtual bool IsAxisRelative(const std::string& sAxisName) const
	{
		// TODO: Emulate relative X, Y axes with mouse?
		return sAxisName != MOUSE_WHEEL_AXIS_NAME;
	}


	virtual boost::signals2::connection ConnectButtonUpdateSlot(const ButtonUpdateSignal::slot_type& slot)
	{
		return m_buttonUpdate.connect(slot);
	}


	virtual boost::signals2::connection ConnectAxisUpdateSlot(const AxisUpdateSignal::slot_type& slot)
	{
		return m_axisUpdate.connect(slot);
	}


private:
	typedef int VKCode;
	typedef std::map<VKCode, const char*> VKNameMap;


	void OnWindowsMessage(HWND hWnd, UINT nMsg, WPARAM nWParam, LPARAM nLParam)
	{
		switch (nMsg)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			{
				bool wasDown = (nLParam >> 30) != 0;
				if (!wasDown)
				{
					DispatchButtonUpdate(nWParam, true);
				}
			}
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			DispatchButtonUpdate(nWParam, false);
			break;

		case WM_MOUSEWHEEL:
			DispatchAxisUpdate(MOUSE_WHEEL_AXIS_NAME, (float) GET_WHEEL_DELTA_WPARAM(nWParam) / WHEEL_DELTA);
			break;

		case WM_MOUSEMOVE:
			{
				// We need to check for changes, as we keep receiving a steady stream
				// of mouse move events even when the mouse is standing still.
				int16_t nMouseX = GET_X_LPARAM(nLParam);
				if(m_nPrevMouseX != nMouseX)
				{
					DispatchAxisUpdate(MOUSE_X_AXIS_NAME, (float) nMouseX);
					m_nPrevMouseX = nMouseX;
				}

				int16_t nMouseY = GET_Y_LPARAM(nLParam);
				if(m_nPrevMouseY != nMouseY)
				{
					DispatchAxisUpdate(MOUSE_Y_AXIS_NAME, (float) nMouseY);
					m_nPrevMouseY = nMouseY;
				}
			}
			break;

		case WM_LBUTTONDOWN:
			DispatchButtonUpdate(VK_LBUTTON, true);
			break;
		case WM_MBUTTONDOWN:
			DispatchButtonUpdate(VK_MBUTTON, true);
			break;
		case WM_RBUTTONDOWN:
			DispatchButtonUpdate(VK_RBUTTON, true);
			break;
		case WM_XBUTTONDOWN:
			DispatchButtonUpdate(GET_KEYSTATE_WPARAM(nWParam), true);
			break;

		case WM_LBUTTONUP:
			DispatchButtonUpdate(VK_LBUTTON, false);
			break;
		case WM_MBUTTONUP:
			DispatchButtonUpdate(VK_MBUTTON, false);
			break;
		case WM_RBUTTONUP:
			DispatchButtonUpdate(VK_RBUTTON, false);
			break;
		case WM_XBUTTONUP:
			DispatchButtonUpdate(GET_KEYSTATE_WPARAM(nWParam), false);
			break;
		}
	}


	void DispatchButtonUpdate(VKCode nKey, bool bPressed)
	{
		VKNameMap::const_iterator i = m_vkNameMap.find(nKey);
		if (i == m_vkNameMap.end())
		{
			return;
		}

		m_buttonUpdate(i->second, bPressed);
	}


	void DispatchAxisUpdate(const char* pszAxisName, float nValue)
	{
		string sAxisName(pszAxisName);
		m_axisUpdate(sAxisName, nValue);
	}


	// From WinUser.h, with comments and defines preserved.
	// TODO: Add friendly names, too.
	static void PopulateVKNameMap(VKNameMap& map)
	{
		/*
		 * Virtual Keys, Standard Set
		 */
		map[VK_LBUTTON] = "LBUTTON";
		map[VK_RBUTTON] = "RBUTTON";
		map[VK_CANCEL] = "CANCEL";
		map[VK_MBUTTON] = "MBUTTON"; //< NOT contiguous with L & RBUTTON

		#if(_WIN32_WINNT >= 0x0500)
		map[VK_XBUTTON1] = "XBUTTON1"; //< NOT contiguous with L & RBUTTON
		map[VK_XBUTTON2] = "XBUTTON2"; //< NOT contiguous with L & RBUTTON
		#endif /* _WIN32_WINNT >= 0x0500 */

		/*
		 * 0x07 : unassigned
		 */

		map[VK_BACK] = "BACK";
		map[VK_TAB] = "TAB";

		/*
		 * 0x0A - 0x0B : reserved
		 */

		map[VK_CLEAR] = "CLEAR";
		map[VK_RETURN] = "RETURN";

		map[VK_SHIFT] = "SHIFT";
		map[VK_CONTROL] = "CONTROL";
		map[VK_MENU] = "MENU";
		map[VK_PAUSE] = "PAUSE";
		map[VK_CAPITAL] = "CAPITAL";

		map[VK_KANA] = "KANA";
		map[VK_HANGEUL] = "HANGEUL"; //< old name - should be here for compatibility
		map[VK_HANGUL] = "HANGUL";
		map[VK_JUNJA] = "JUNJA";
		map[VK_FINAL] = "FINAL";
		map[VK_HANJA] = "HANJA";
		map[VK_KANJI] = "KANJI";

		map[VK_ESCAPE] = "ESCAPE";

		map[VK_CONVERT] = "CONVERT";
		map[VK_NONCONVERT] = "NONCONVERT";
		map[VK_ACCEPT] = "ACCEPT";
		map[VK_MODECHANGE] = "MODECHANGE";

		map[VK_SPACE] = "SPACE";
		map[VK_PRIOR] = "PRIOR";
		map[VK_NEXT] = "NEXT";
		map[VK_END] = "END";
		map[VK_HOME] = "HOME";
		map[VK_LEFT] = "LEFT";
		map[VK_UP] = "UP";
		map[VK_RIGHT] = "RIGHT";
		map[VK_DOWN] = "DOWN";
		map[VK_SELECT] = "SELECT";
		map[VK_PRINT] = "PRINT";
		map[VK_EXECUTE] = "EXECUTE";
		map[VK_SNAPSHOT] = "SNAPSHOT";
		map[VK_INSERT] = "INSERT";
		map[VK_DELETE] = "DELETE";
		map[VK_HELP] = "HELP";

		/*
		 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
		 * 0x40 : unassigned
		 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
		 */

		map[VK_LWIN] = "LWIN";
		map[VK_RWIN] = "RWIN";
		map[VK_APPS] = "APPS";

		/*
		 * 0x5E : reserved
		 */

		map[VK_SLEEP] = "SLEEP";

		map[VK_NUMPAD0] = "NUMPAD0";
		map[VK_NUMPAD1] = "NUMPAD1";
		map[VK_NUMPAD2] = "NUMPAD2";
		map[VK_NUMPAD3] = "NUMPAD3";
		map[VK_NUMPAD4] = "NUMPAD4";
		map[VK_NUMPAD5] = "NUMPAD5";
		map[VK_NUMPAD6] = "NUMPAD6";
		map[VK_NUMPAD7] = "NUMPAD7";
		map[VK_NUMPAD8] = "NUMPAD8";
		map[VK_NUMPAD9] = "NUMPAD9";
		map[VK_MULTIPLY] = "MULTIPLY";
		map[VK_ADD] = "ADD";
		map[VK_SEPARATOR] = "SEPARATOR";
		map[VK_SUBTRACT] = "SUBTRACT";
		map[VK_DECIMAL] = "DECIMAL";
		map[VK_DIVIDE] = "DIVIDE";
		map[VK_F1] = "F1";
		map[VK_F2] = "F2";
		map[VK_F3] = "F3";
		map[VK_F4] = "F4";
		map[VK_F5] = "F5";
		map[VK_F6] = "F6";
		map[VK_F7] = "F7";
		map[VK_F8] = "F8";
		map[VK_F9] = "F9";
		map[VK_F10] = "F10";
		map[VK_F11] = "F11";
		map[VK_F12] = "F12";
		map[VK_F13] = "F13";
		map[VK_F14] = "F14";
		map[VK_F15] = "F15";
		map[VK_F16] = "F16";
		map[VK_F17] = "F17";
		map[VK_F18] = "F18";
		map[VK_F19] = "F19";
		map[VK_F20] = "F20";
		map[VK_F21] = "F21";
		map[VK_F22] = "F22";
		map[VK_F23] = "F23";
		map[VK_F24] = "F24";

		/*
		 * 0x88 - 0x8F : unassigned
		 */

		map[VK_NUMLOCK] = "NUMLOCK";
		map[VK_SCROLL] = "SCROLL";

		/*
		 * NEC PC-9800 kbd definitions
		 */
		map[VK_OEM_NEC_EQUAL] = "OEM_NEC_EQUAL"; //< '=' key on numpad

		/*
		 * Fujitsu/OASYS kbd definitions
		 */
		map[VK_OEM_FJ_JISHO] = "OEM_FJ_JISHO"; //< 'Dictionary' key
		map[VK_OEM_FJ_MASSHOU] = "OEM_FJ_MASSHOU"; //< 'Unregister word' key
		map[VK_OEM_FJ_TOUROKU] = "OEM_FJ_TOUROKU"; //< 'Register word' key
		map[VK_OEM_FJ_LOYA] = "OEM_FJ_LOYA"; //< 'Left OYAYUBI' key
		map[VK_OEM_FJ_ROYA] = "OEM_FJ_ROYA"; //< 'Right OYAYUBI' key

		/*
		 * 0x97 - 0x9F : unassigned
		 */

		/*
		 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
		 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
		 * No other API or message will distinguish left and right keys in this way.
		 */
		map[VK_LSHIFT] = "LSHIFT";
		map[VK_RSHIFT] = "RSHIFT";
		map[VK_LCONTROL] = "LCONTROL";
		map[VK_RCONTROL] = "RCONTROL";
		map[VK_LMENU] = "LMENU";
		map[VK_RMENU] = "RMENU";

		#if(_WIN32_WINNT >= 0x0500)
		map[VK_BROWSER_BACK] = "BROWSER_BACK";
		map[VK_BROWSER_FORWARD] = "BROWSER_FORWARD";
		map[VK_BROWSER_REFRESH] = "BROWSER_REFRESH";
		map[VK_BROWSER_STOP] = "BROWSER_STOP";
		map[VK_BROWSER_SEARCH] = "BROWSER_SEARCH";
		map[VK_BROWSER_FAVORITES] = "BROWSER_FAVORITES";
		map[VK_BROWSER_HOME] = "BROWSER_HOME";

		map[VK_VOLUME_MUTE] = "VOLUME_MUTE";
		map[VK_VOLUME_DOWN] = "VOLUME_DOWN";
		map[VK_VOLUME_UP] = "VOLUME_UP";
		map[VK_MEDIA_NEXT_TRACK] = "MEDIA_NEXT_TRACK";
		map[VK_MEDIA_PREV_TRACK] = "MEDIA_PREV_TRACK";
		map[VK_MEDIA_STOP] = "MEDIA_STOP";
		map[VK_MEDIA_PLAY_PAUSE] = "MEDIA_PLAY_PAUSE";
		map[VK_LAUNCH_MAIL] = "LAUNCH_MAIL";
		map[VK_LAUNCH_MEDIA_SELECT] = "LAUNCH_MEDIA_SELECT";
		map[VK_LAUNCH_APP1] = "LAUNCH_APP1";
		map[VK_LAUNCH_APP2] = "LAUNCH_APP2";

		#endif /* _WIN32_WINNT >= 0x0500 */

		/*
		 * 0xB8 - 0xB9 : reserved
		 */

		map[VK_OEM_1] = "OEM_1"; //< ';:' for US
		map[VK_OEM_PLUS] = "OEM_PLUS"; //< '+' any country
		map[VK_OEM_COMMA] = "OEM_COMMA"; //< ',' any country
		map[VK_OEM_MINUS] = "OEM_MINUS"; //< '-' any country
		map[VK_OEM_PERIOD] = "OEM_PERIOD"; //< '.' any country
		map[VK_OEM_2] = "OEM_2"; //< '/?' for US
		map[VK_OEM_3] = "OEM_3"; //< '`~' for US

		/*
		 * 0xC1 - 0xD7 : reserved
		 */

		/*
		 * 0xD8 - 0xDA : unassigned
		 */

		map[VK_OEM_4] = "OEM_4"; //< '[{' for US
		map[VK_OEM_5] = "OEM_5"; //< '\|' for US
		map[VK_OEM_6] = "OEM_6"; //< ']}' for US
		map[VK_OEM_7] = "OEM_7"; //< ''"' for US
		map[VK_OEM_8] = "OEM_8";

		/*
		 * 0xE0 : reserved
		 */

		/*
		 * Various extended or enhanced keyboards
		 */
		map[VK_OEM_AX] = "OEM_AX"; //< 'AX' key on Japanese AX kbd
		map[VK_OEM_102] = "OEM_102"; //< "<>" or "\|" on RT 102-key kbd.
		map[VK_ICO_HELP] = "ICO_HELP"; //< Help key on ICO
		map[VK_ICO_00] = "ICO_00"; //< 00 key on ICO

		#if(WINVER >= 0x0400)
		map[VK_PROCESSKEY] = "PROCESSKEY";
		#endif /* WINVER >= 0x0400 */

		map[VK_ICO_CLEAR] = "ICO_CLEAR";


		#if(_WIN32_WINNT >= 0x0500)
		map[VK_PACKET] = "PACKET";
		#endif /* _WIN32_WINNT >= 0x0500 */

		/*
		 * 0xE8 : unassigned
		 */

		/*
		 * Nokia/Ericsson definitions
		 */
		map[VK_OEM_RESET] = "OEM_RESET";
		map[VK_OEM_JUMP] = "OEM_JUMP";
		map[VK_OEM_PA1] = "OEM_PA1";
		map[VK_OEM_PA2] = "OEM_PA2";
		map[VK_OEM_PA3] = "OEM_PA3";
		map[VK_OEM_WSCTRL] = "OEM_WSCTRL";
		map[VK_OEM_CUSEL] = "OEM_CUSEL";
		map[VK_OEM_ATTN] = "OEM_ATTN";
		map[VK_OEM_FINISH] = "OEM_FINISH";
		map[VK_OEM_COPY] = "OEM_COPY";
		map[VK_OEM_AUTO] = "OEM_AUTO";
		map[VK_OEM_ENLW] = "OEM_ENLW";
		map[VK_OEM_BACKTAB] = "OEM_BACKTAB";

		map[VK_ATTN] = "ATTN";
		map[VK_CRSEL] = "CRSEL";
		map[VK_EXSEL] = "EXSEL";
		map[VK_EREOF] = "EREOF";
		map[VK_PLAY] = "PLAY";
		map[VK_ZOOM] = "ZOOM";
		map[VK_NONAME] = "NONAME";
		map[VK_PA1] = "PA1";
		map[VK_OEM_CLEAR] = "OEM_CLEAR";

		/*
		 * 0xFF : reserved
		 */
	}


	int16_t m_nPrevMouseX;
	int16_t m_nPrevMouseY;
	VKNameMap m_vkNameMap;
	connection m_windowsMessageConnection;
	ButtonUpdateSignal m_buttonUpdate;
	AxisUpdateSignal m_axisUpdate;
};


// -----------------------------------------------------------------------------


Input* Input::CreateInstance(RenderWindow& renderWindow)
{
	return new WindowsMessageInputImpl(renderWindow);
}
