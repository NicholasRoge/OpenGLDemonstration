#include <Windows.h>

#define WM_OPEN WM_USER & 0x1


namespace Window
{
	typedef LRESULT(CALLBACK *MessageHandler)(HWND window_handle,UINT message,WPARAM w_param,LPARAM l_param);
	
	namespace Class
	{
		extern ATOM atom;
		extern LPTSTR name;
		extern UINT extra_bytes;
		extern UINT style;
	}
	namespace Menu
	{
		extern HMENU handle;
		extern LPTSTR name;
	}

	extern HBRUSH background;
	extern HINSTANCE context;
	extern HCURSOR cursor;
	extern UINT extra_bytes;
	extern HWND handle;
	extern int height;
	extern HICON icon;
	extern HICON icon_small;
	extern MessageHandler message_handler;
	extern LPTSTR name;
	extern HWND parent;
	extern int position_x;
	extern int position_y;
	extern UINT style;
	extern UINT style_extended;
	extern int width;

	void Close();

	/* Lifecycle Stuff */
	void Create(HINSTANCE context);

	void Destroy();

	int Open();

	/* Message Stuff */
	void PostMessage(UINT message,WPARAM w_param,LPARAM l_param);

	void SendMessage(UINT message,WPARAM w_param,LPARAM l_param);

	/* Handlers */
	LRESULT CALLBACK HandleMessage(HWND window,UINT message,WPARAM w_param,LPARAM l_param);
}