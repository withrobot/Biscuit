
#ifndef _XWRAPPER_HPP_
#define _XWRAPPER_HPP_

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <string>
#include <list>

class XMouseInterface
{
	public:
		enum MouseState {
			BTN_DOWN,
			BTN_UP,
		};

		XMouseInterface(const std::string display = "");
		~XMouseInterface();

		void MouseLeft(MouseState state);
		void MouseMid(MouseState state);
		void MouseRight(MouseState state);
		void MouseWheelY(int offset);
		void MouseMove(int x, int y);
		void MousePos(int x, int y);
	private:
		Display *m_display;
};

class XKeyboardInterface
{
	public:
		XKeyboardInterface(const std::string display = "");
		~XKeyboardInterface();

		void SendKey(const std::list<int>& keycode);
		void SendKey(int keycode);
		bool KeyState(int keycode);
		void PressKey(int keycode);
		void ReleaseKey(int keycode);

	private:

		Display *m_display;
};

#endif
