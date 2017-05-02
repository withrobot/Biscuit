
#include "xwrapper.hpp"

#include <X11/extensions/XTest.h>
#include <stdexcept>
#include <stdlib.h>

XMouseInterface::XMouseInterface(const std::string display)
{
	if ((m_display = XOpenDisplay(display.empty()?NULL:display.c_str())) == NULL)
	{
		// sorry for throwing in constructor...
		throw std::runtime_error("cannot open xdisplay");
	}
}

XMouseInterface::~XMouseInterface()
{
	if (m_display)
	{
		XCloseDisplay(m_display);
	}
}

void XMouseInterface::MouseLeft(MouseState state)
{
	XTestFakeButtonEvent(m_display, 1,
			state==BTN_DOWN?True:False,
			CurrentTime);
	XFlush(m_display);
}

void XMouseInterface::MouseMid(MouseState state)
{
	XTestFakeButtonEvent(m_display, 2,
			state==BTN_DOWN?True:False,
			CurrentTime);
	XFlush(m_display);
}

void XMouseInterface::MouseRight(MouseState state)
{
	XTestFakeButtonEvent(m_display, 3,
			state==BTN_DOWN?True:False,
			CurrentTime);
	XFlush(m_display);
}

void XMouseInterface::MouseWheelY(int offset)
{
	for(int i = abs(offset); i > 0; i--)
	{
		XTestFakeButtonEvent(m_display, offset>0?5:4,
				True,
				CurrentTime);
		XTestFakeButtonEvent(m_display, offset>0?5:4,
				False,
				CurrentTime);
	}
	XFlush(m_display);
}

void XMouseInterface::MouseMove(int x, int y)
{
	XTestFakeRelativeMotionEvent(m_display, x, y, CurrentTime);
	XFlush(m_display);
}

void XMouseInterface::MousePos(int x, int y)
{
	XTestFakeMotionEvent(m_display, -1, x, y, CurrentTime);
	XFlush(m_display);
}

XKeyboardInterface::XKeyboardInterface(const std::string display)
{
	if ((m_display = XOpenDisplay(display.empty()?NULL:display.c_str())) == NULL)
	{
		// sorry for throwing in constructor...
		throw std::runtime_error("cannot open xdisplay");
	}
}

XKeyboardInterface::~XKeyboardInterface()
{
	if (m_display)
	{
		XCloseDisplay(m_display);
	}
}

void XKeyboardInterface::SendKey(int keycode)
{
	std::list<int> keys;
	keys.push_back(keycode);
	SendKey(keys);
}

void XKeyboardInterface::PressKey(int keycode)
{
	KeyCode key = XKeysymToKeycode(m_display, keycode);
	if(key == NoSymbol) return;

	XTestFakeKeyEvent(m_display, key, True, CurrentTime);
}

void XKeyboardInterface::ReleaseKey(int keycode)
{
	KeyCode key = XKeysymToKeycode(m_display, keycode);
	if(key == NoSymbol) return;

	XTestFakeKeyEvent(m_display, key, False, CurrentTime);
}

void XKeyboardInterface::SendKey(const std::list<int>& keycode)
{
	for(std::list<int>::const_iterator i = keycode.begin();
			i != keycode.end(); i++)
	{
		KeyCode key = XKeysymToKeycode(m_display, *i);
		if(key == NoSymbol) continue;

		XTestFakeKeyEvent(m_display, key, True, CurrentTime);
	}
	for(std::list<int>::const_reverse_iterator i = keycode.rbegin();
			i != keycode.rend(); i++)
	{
		KeyCode key = XKeysymToKeycode(m_display, *i);
		if(key == NoSymbol) continue;

		XTestFakeKeyEvent(m_display, key, False, CurrentTime);
	}
	XFlush(m_display);
}
