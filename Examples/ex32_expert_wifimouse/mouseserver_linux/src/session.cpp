
#include "session.hpp"

#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <X11/extensions/XTest.h>
#include <iconv.h>

#include <pcrecpp.h>

#include "xwrapper.hpp"
#include "utils.hpp"

int iLogLevel = 4;
int iScrollSwitch = 0;

int screen_width = 1280;
int screen_height = 720;

XMouseInterface mousePointer;


int keycodes(char *buffer) {
	printf("keycodes :%s:\n", buffer);
	if (strcmp(buffer,"RTN") == 0) return XK_Return;
	if (strcmp(buffer,"BAS") == 0) return XK_BackSpace;
	if (strcmp(buffer,"TAB") == 0) return XK_Tab;
	if (strcmp(buffer,"SPC") == 0) return ' ';
	if (strcmp(buffer," ") == 0) return ' ';
	if (strcmp(buffer,"CAP") == 0) return XK_Caps_Lock;
	if (strcmp(buffer,"a") == 0) return 'a';
	if (strcmp(buffer,"0") == 0) return '0';
	if (strcmp(buffer,"1") == 0) return '1';
	if (strcmp(buffer,"2") == 0) return '2';
	if (strcmp(buffer,"3") == 0) return '3';
	if (strcmp(buffer,"4") == 0) return '4';
	if (strcmp(buffer,"5") == 0) return '5';
	if (strcmp(buffer,"6") == 0) return '6';
	if (strcmp(buffer,"7") == 0) return '7';
	if (strcmp(buffer,"8") == 0) return '8';
	if (strcmp(buffer,"9") == 0) return '9';
	if (strcmp(buffer,"-") == 0) return '-';
	if (strcmp(buffer,"=") == 0) return '=';
	if (strcmp(buffer,"`") == 0) return '`';
	if (strcmp(buffer,".") == 0) return '.';
	if (strcmp(buffer,",") == 0) return ',';
	if (strcmp(buffer,":") == 0) return ':';
	if (strcmp(buffer,"\"") == 0) return '"';
	if (strcmp(buffer,"/") == 0) return '/';
	if (strcmp(buffer,"b") == 0) return 'b';
	if (strcmp(buffer,"c") == 0) return 'c';
	if (strcmp(buffer,"d") == 0) return 'd';
	if (strcmp(buffer,"e") == 0) return 'e';
	if (strcmp(buffer,"f") == 0) return 'f';
	if (strcmp(buffer,"g") == 0) return 'g';
	if (strcmp(buffer,"h") == 0) return 'h';
	if (strcmp(buffer,"i") == 0) return 'i';
	if (strcmp(buffer,"j") == 0) return 'j';
	if (strcmp(buffer,"k") == 0) return 'k';
	if (strcmp(buffer,"l") == 0) return 'l';
	if (strcmp(buffer,"m") == 0) return 'm';
	if (strcmp(buffer,"n") == 0) return 'n';
	if (strcmp(buffer,"o") == 0) return 'o';
	if (strcmp(buffer,"p") == 0) return 'p';
	if (strcmp(buffer,"q") == 0) return 'q';
	if (strcmp(buffer,"r") == 0) return 'r';
	if (strcmp(buffer,"s") == 0) return 's';
	if (strcmp(buffer,"t") == 0) return 't';
	if (strcmp(buffer,"u") == 0) return 'u';
	if (strcmp(buffer,"v") == 0) return 'v';
	if (strcmp(buffer,"w") == 0) return 'w';
	if (strcmp(buffer,"x") == 0) return 'x';
	if (strcmp(buffer,"y") == 0) return 'y';
	if (strcmp(buffer,"z") == 0) return 'z';

	if (strcmp(buffer,"?") == 0) return 63;
	if (strcmp(buffer,"!") == 0) return '!';
	if (strcmp(buffer,"'") == 0) return '\'';
	if (strcmp(buffer,"@") == 0) return '@';
	if (strcmp(buffer,"+") == 0) return '+';
	if (strcmp(buffer,"-") == 0) return '-';
	if (strcmp(buffer,"*") == 0) return '*';
	if (strcmp(buffer,"/") == 0) return '/';
	if (strcmp(buffer,"\"") == 0) return '"';
	if (strcmp(buffer,";") == 0) return ';';
	if (strcmp(buffer,"&") == 0) return '&';
	if (strcmp(buffer,":") == 0) return ':';
	if (strcmp(buffer,"#") == 0) return '#';
	if (strcmp(buffer,"(") == 0) return '(';
	if (strcmp(buffer,")") == 0) return ')';
	if (strcmp(buffer,"%") == 0) return '%';
	if (strcmp(buffer,"$") == 0) return '$';
	if (strcmp(buffer,"~") == 0) return '~';
	if (strcmp(buffer,"[") == 0) return '[';
	if (strcmp(buffer,"]") == 0) return ']';
	if (strcmp(buffer,"\\") == 0) return '\\';
	if (strcmp(buffer,"|") == 0) return '|';
	if (strcmp(buffer,"{") == 0) return '{';
	if (strcmp(buffer,"}") == 0) return '}';
	if (strcmp(buffer,"<") == 0) return '<';
	if (strcmp(buffer,">") == 0) return '>';
	if (strcmp(buffer,"_") == 0) return '_';

	/* keypad */
	if (strcmp(buffer, "NUM_DIVIDE")==0) return XK_KP_Divide;
	if (strcmp(buffer, "NUM_MULTIPLY") == 0) return XK_KP_Multiply;
	if (strcmp(buffer,"NUM_SUBTRACT") == 0) return XK_KP_Subtract;
	if (strcmp(buffer,"NUM_ADD") == 0) return XK_KP_Add;
	//if (strcmp(buffer,"Enter") == 0) return XK_KP_Enter;
	if (strcmp(buffer,"NUM_DECIMAL") == 0) return XK_KP_Decimal;
	if (strcmp(buffer,"NUM0") == 0) return XK_KP_0;
	if (strcmp(buffer,"NUM1") == 0) return XK_KP_1;
	if (strcmp(buffer,"NUM2") == 0) return XK_KP_2;
	if (strcmp(buffer,"NUM3") == 0) return XK_KP_3;
	if (strcmp(buffer, "NUM4") == 0) return XK_KP_4;
	if (strcmp(buffer,"NUM5") == 0) return XK_KP_5;
	if (strcmp(buffer,"NUM6") == 0) return XK_KP_6;
	if (strcmp(buffer,"NUM7") == 0) return XK_KP_7;
	if (strcmp(buffer,"NUM8") == 0) return XK_KP_8;
	if (strcmp(buffer,"NUM9") == 0) return XK_KP_9;

	/* function page */
	if (strcmp(buffer,"ESC") == 0) return XK_Escape;
	if (strcmp(buffer,"DELETE") == 0) return XK_Delete;
	if (strcmp(buffer,"HOME") == 0) return XK_Home;
	if (strcmp(buffer,"END") == 0) return XK_End;
	if (strcmp(buffer,"PGUP") == 0) return XK_Page_Up;
	if (strcmp(buffer,"PGDN") == 0) return XK_Page_Down;
	if (strcmp(buffer,"UP") == 0) return XK_Up;
	if (strcmp(buffer,"DW") == 0) return XK_Down;
	if (strcmp(buffer,"RT") == 0) return XK_Right;
	if (strcmp(buffer,"LF") == 0) return XK_Left;
	if (strcmp(buffer,"F1") == 0) return XK_F1;
	if (strcmp(buffer,"F2") == 0) return XK_F2;
	if (strcmp(buffer,"F3") == 0) return XK_F3;
	if (strcmp(buffer,"F4") == 0) return XK_F4;
	if (strcmp(buffer,"F5") == 0) return XK_F5;
	if (strcmp(buffer,"F6") == 0) return XK_F6;
	if (strcmp(buffer,"F7") == 0) return XK_F7;
	if (strcmp(buffer,"F8") == 0) return XK_F8;
	if (strcmp(buffer,"F9") == 0) return XK_F9;
	if (strcmp(buffer,"F10") == 0) return XK_F10;
	if (strcmp(buffer,"F11") == 0) return XK_F11;
	if (strcmp(buffer,"F12") == 0) return XK_F12;
	if (strcmp(buffer,"CTRL") == 0) return XK_Control_L;
	if (strcmp(buffer,"ALT") == 0) return XK_Alt_L;
	if (strcmp(buffer,"SHIFT") == 0) return XK_Shift_L;

	if (strcmp(buffer,"VOLUMEDN") == 0) return XF86XK_AudioLowerVolume;
	if (strcmp(buffer,"VOLUMEUP") == 0) return XF86XK_AudioRaiseVolume;
	if (strcmp(buffer,"MUTE") == 0) return XF86XK_AudioMute;

	return -1;
}


void process(char *buffer) {

	XKeyboardInterface keyBoard;

	if (buffer[0] == '\0')
		return;
	printf("process buffer :%s:", buffer);
	if(strncmp(buffer, "mos", 3) == 0) {
		char *p = buffer + 3;
		char sl[3];
		strncpy(sl, p, 3);
		int il = atoi(sl);
		if (iLogLevel > 2)
			printf("data length = %d\n", il);

		if (buffer[6] == 'm') {
			p = p+5;
			char *p1 = strstr(p, " ");
            char datax[6];
            bzero(datax, 6);
            strncpy(datax, p, p1 - p);
			int ix = atoi(datax);
			if (iLogLevel > 2)
				printf("x=%d\n", ix);

			p1++;
            char datay[6];
			bzero(datay, 6);
            strncpy(datay, p1, il - strlen(datax) - 3);
            int iy = atoi(datay);
			if (iLogLevel > 2)
				printf("y=%d\n", iy);

			mousePointer.MouseMove(ix, iy);
		}
        else if (buffer[6] == 'p')
        {
			p = p+5;
			char *p1 = strstr(p, " ");
            char datax[6];
            bzero(datax, 6);
            strncpy(datax, p, p1 - p);
			int ix = atoi(datax);
			if (iLogLevel > 2)
				printf("x=%d\n", ix);

			p1++;
            char datay[6];
			bzero(datay, 6);
            strncpy(datay, p1, il - strlen(datax) - 3);
            int iy = atoi(datay);
			if (iLogLevel > 2)
				printf("y=%d\n", iy);

			mousePointer.MousePos(ix, iy);

        }
        else if (buffer[6] == 'r')
        {
            mousePointer.MousePos(screen_width/2, screen_height/2);
        }
        else if (buffer[6] == 'R')
        {
			p = p+5;
            if (p[0] == 'l') {
				if (p[2]=='d') {
					mousePointer.MouseLeft(XMouseInterface::BTN_DOWN);
				}else if (p[2] =='u') {
					mousePointer.MouseLeft(XMouseInterface::BTN_UP);
				}
			}else if (p[0] == 'r') {
				if (p[2]=='d') {
					mousePointer.MouseRight(XMouseInterface::BTN_DOWN);
				}else if (p[2] =='u') {
					mousePointer.MouseRight(XMouseInterface::BTN_UP);
				}
			}else if (p[0] == 'm') {
				if (p[2]=='d') {
					mousePointer.MouseMid(XMouseInterface::BTN_DOWN);
				}else if (p[2] =='u') {
					mousePointer.MouseMid(XMouseInterface::BTN_UP);
				}
			}
		}else if (buffer[6] == 'c') {
			mousePointer.MouseLeft(XMouseInterface::BTN_DOWN);
			mousePointer.MouseLeft(XMouseInterface::BTN_UP);
		}else if (buffer[6] == 'w') {
			printf("is w : scroll\n");
			if (iScrollSwitch == 0) {
				iScrollSwitch = 1;
				if (buffer[8] == '1')
					mousePointer.MouseWheelY(-1);
				else
					mousePointer.MouseWheelY(1);
			}else {
				iScrollSwitch = 0;
			}
		}

		process(buffer+6+il);
	}else if(strncmp(buffer, "key", 3) == 0) {
		char *p = buffer + 3;
		char sl[3];
		strncpy(sl, p, 3);
		int il = atoi(sl);
		if (iLogLevel > 2)
		printf("key data length = %d\n", il);

		p += 3;
		if (il > 3 && p[0] == '[' && p[1] == 'R' && p[2] == ']') {
			//key pressed up and down
			p += 4;
			char *p1 = strstr(p, " ");

			char skey[32];
			bzero(skey, 32);
			strncpy(skey, p, p1 - p);
			if (iLogLevel > 2)
			printf("key press :%s:\n", skey);
			int keycode = keycodes(skey);
			if (iLogLevel > 2)
			printf("key press state :%c:\n", p1[1]);
			if (keycode != -1) {
				if (p1[1] == 'd')
					keyBoard.PressKey(keycode);
				else if (p1[1] == 'u')
					keyBoard.ReleaseKey(keycode);
			}

		}else {
	 		char keydata[1024];
			bzero(keydata, 0);
			strncpy(keydata, p, il);

			if (keycodes(keydata) != -1)
				keyBoard.SendKey(keycodes(keydata));

		}

		process(buffer+6+il);
	}else if(strncmp(buffer, "drag", 4) == 0) {
		char *p = buffer + 5;
		if (p[0] == 's'){
			mousePointer.MouseLeft(XMouseInterface::BTN_DOWN);
		}else if (p[0] == 'e'){
			mousePointer.MouseLeft(XMouseInterface::BTN_UP);
		}else {
			char *p1 = strstr(p, " ");
			char dragx[6], dragy[6];
			bzero(dragx, 6);
			bzero(dragy, 6);
			strncpy(dragx, p, p1 - p);

			p = p1+1;
			p1 = strstr(p, "\n");
			strncpy(dragy, p, p1 - p);
			mousePointer.MouseMove(atoi(dragx), atoi(dragy));
		}
		process(strstr(buffer, "\n") + 1);
	}else if(strncmp(buffer, "utf8", 4) == 0) {
		char *p = buffer + 5;
		char *p1 = strstr(p, "\n");
		for (char *tmp=p; tmp < p1; tmp++) {
			char chr[2];
			chr[0] = *tmp;
			chr[1] = 0;
			int dcode = keycodes(chr);

			if (dcode != -1) {

				if (dcode== '~'){
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('`');
					keyBoard.SendKey(keys);
				}else if (dcode=='!') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('1');
					keyBoard.SendKey(keys);
				}else if (dcode=='@') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('2');
					keyBoard.SendKey(keys);
				}else if (dcode=='#') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('3');
					keyBoard.SendKey(keys);
				}else if (dcode=='$') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('4');
					keyBoard.SendKey(keys);
				}else if (dcode=='%') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('5');
					keyBoard.SendKey(keys);
				}else if (dcode=='^') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('6');
					keyBoard.SendKey(keys);
				}else if (dcode=='&') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('7');
					keyBoard.SendKey(keys);
				}else if (dcode=='*') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('8');
					keyBoard.SendKey(keys);
				}else if (dcode=='(') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('9');
					keyBoard.SendKey(keys);
				}else if (dcode==')') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('0');
					keyBoard.SendKey(keys);
				}else if (dcode=='_') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('-');
					keyBoard.SendKey(keys);
				}else if (dcode=='+') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('=');
					keyBoard.SendKey(keys);
				}else if (dcode=='{') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('[');
					keyBoard.SendKey(keys);
				}else if (dcode=='}') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back(']');
					keyBoard.SendKey(keys);
				}else if (dcode==':') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back(';');
					keyBoard.SendKey(keys);
				}else if (dcode=='"') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('\'');
					keyBoard.SendKey(keys);
				}else if (dcode=='|') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('\\');
					keyBoard.SendKey(keys);
				}else if (dcode=='<') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back(',');
					keyBoard.SendKey(keys);
				}else if (dcode=='>') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('.');
					keyBoard.SendKey(keys);
				}else if (dcode=='?') {
					std::list<int> keys;
					keys.push_back(XK_Shift_L);
					keys.push_back('/');
					keyBoard.SendKey(keys);
				}else
					keyBoard.SendKey(keycodes(chr));
			}
		}

		process(strstr(buffer, "\n") + 1);
	}else if(strncmp(buffer, "slide", 5) == 0) {
		char *p = buffer + 5;
		if (p[0] == ' ' && p[1] == 'b') {
			keyBoard.PressKey(XK_Alt_L);
		}else if (p[0] == ' ' && p[1] == 'e') {
			keyBoard.ReleaseKey(XK_Shift_L);
			keyBoard.ReleaseKey(XK_Alt_L);

		}else {
			if (p[0] == 'l') {
				keyBoard.ReleaseKey(XK_Shift_L);
				keyBoard.SendKey(XK_Tab);
			}else {
				keyBoard.PressKey(XK_Shift_L);
				keyBoard.SendKey(XK_Tab);
			}
		}
		process(strstr(buffer, "\n") + 1);
	}else if(strncmp(buffer, "media", 5) == 0) {
		char *p = buffer + 6;
		char *p1 = strstr(p, "\n");
		char mcommand[32];

		bzero(mcommand, 32);
		strncpy(mcommand, p, p1 - p);

		if (strcmp(mcommand, "play") == 0) {
			keyBoard.SendKey(XF86XK_AudioPlay);
		}else if (strcmp(mcommand, "next") == 0) {
			keyBoard.SendKey(XF86XK_AudioNext);
		}else if (strcmp(mcommand, "prev") == 0) {
			keyBoard.SendKey(XF86XK_AudioPrev);
		}else if (strcmp(mcommand, "volumeup") == 0) {
			keyBoard.SendKey(XF86XK_AudioRaiseVolume);
		}else if (strcmp(mcommand, "volumedown") == 0) {
			keyBoard.SendKey(XF86XK_AudioLowerVolume);
		}
		process(strstr(buffer, "\n") + 1);
	}else if(strncmp(buffer, "zoom", 4) == 0) {
		char *p = buffer + 4;
		if (p[0] == 'i'){
			std::list<int> keys;
			keys.push_back(XK_Control_L);
			keys.push_back('+');
			keyBoard.SendKey(keys);
		}else {
			std::list<int> keys;
			keys.push_back(XK_Control_L);
			keys.push_back('-');
			keyBoard.SendKey(keys);
		}
		process(strstr(buffer, "\n") + 1);
	}else if(strncmp(buffer, "browser", 7) == 0) {
		char *p = buffer + 8;
		char *p1 = strstr(p, "\n");
		char bcommand[32];

		bzero(bcommand, 32);
		strncpy(bcommand, p, p1 - p);
		if (strcmp(bcommand, "forward") == 0) {
			std::list<int> keys;
			keys.push_back(XK_Alt_L);
			keys.push_back(XK_Right);
			keyBoard.SendKey(keys);
		}else if (strcmp(bcommand, "back") == 0) {
			std::list<int> keys;
			keys.push_back(XK_Alt_L);
			keys.push_back(XK_Left);
			keyBoard.SendKey(keys);
		}else if (strcmp(bcommand, "home") == 0) {
			std::list<int> keys;
			keys.push_back(XK_Alt_L);
			keys.push_back(XK_Home);
			keyBoard.SendKey(keys);
		}else if (strcmp(bcommand, "search") == 0) {
			std::list<int> keys;
			keys.push_back(XK_Control_L);
			keys.push_back('k');
			keyBoard.SendKey(keys);
		}else if (strcmp(bcommand, "refresh") == 0) {
			std::list<int> keys;
			keys.push_back(XK_Control_L);
			keys.push_back('r');
			keyBoard.SendKey(keys);
		}else if (strcmp(bcommand, "stop") == 0) {
			std::list<int> keys;
			keys.push_back(XK_Escape);
			keyBoard.SendKey(keys);
		}else if (strcmp(bcommand, "favorite") == 0) {
			std::list<int> keys;
			keys.push_back(XK_Control_L);
			keys.push_back('b');
			keyBoard.SendKey(keys);
		}else if (strcmp(bcommand, "newtab") == 0) {
			std::list<int> keys;
			keys.push_back(XK_Control_L);
			keys.push_back('t');
			keyBoard.SendKey(keys);
		}
		process(strstr(buffer, "\n") + 1);
	}else if(strncmp(buffer, "shutdown", 8) == 0) {
		std::list<int> keys;
		keys.push_back(XK_Alt_L);
		keys.push_back(XK_Control_L);
		keys.push_back(XK_Delete);
		keyBoard.SendKey(keys);
		process(buffer + 9);
	}

}

void* MobileMouseSession(void* context)
{
	//Configuration& appConfig = static_cast<SessionContext*>(context)->m_appConfig;
	int client = static_cast<SessionContext*>(context)->m_sock;
	std::string address = static_cast<SessionContext*>(context)->m_address;
	delete static_cast<SessionContext*>(context);

	//XMouseInterface mousePointer;
	//XKeyboardInterface keyBoard;

	syslog(LOG_INFO, "[%s] connected", address.c_str());

	char buffer[1024];
	ssize_t n;

	char ssys[64];
	bzero(ssys, 64);
	strcpy(ssys, "system linux 10.04\n");
	write(client, ssys, strlen(ssys));


	struct timeval lastMouseEvent;
	timerclear(&lastMouseEvent);


	/* protocol loop */
	std::string packet_buffer;
	std::string packet;
	while(1)
	{
		n = read(client, buffer, sizeof(buffer));
		if (n < 1)
		{
			syslog(LOG_INFO, "[%s] disconnected ", address.c_str());
			close(client);
			return NULL;
		}
		buffer[n] = '\0';

		if (iLogLevel > 3)
		printf("received buffer:%s\n", buffer);


		process(buffer);
	}

	syslog(LOG_INFO, "[%s] connection ended", address.c_str());
	return NULL;
}
