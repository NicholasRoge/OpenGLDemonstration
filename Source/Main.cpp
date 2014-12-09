#pragma comment(lib,"dwmapi.lib")

#include <dwmapi.h>
#include "OpenGL.h"
#include <string>
#include "UI.h"
#include "Window.h"

#define TIMER_RENDER 0x1


/* Function Declarations */
void EnableTransparency();

int GetTaskbarHeight();

LRESULT CALLBACK HandleMessage(HWND window_handle,UINT message,WPARAM w_param,LPARAM l_param);

LRESULT Paint();

/* Main */
int WINAPI WinMain(HINSTANCE m,HINSTANCE p,LPSTR command_line,int show_command)
{
	int exit_code;

	
	Window::Class::name = L"OpenGLWindow";
	Window::name = L"OpenGL Window";
	Window::Class::style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	Window::background = nullptr;
	Window::width = 1920;
	Window::height = 1080 - GetTaskbarHeight();
	Window::message_handler = HandleMessage;
	
	Window::Create(m);
	OpenGL::Load();
	UI::Load();
	exit_code = Window::Open();
	UI::Unload();
	OpenGL::Unload();
	Window::Destroy();

	return exit_code;
}

/* Function Definitions */
void EnableTransparency()
{
	DWM_BLURBEHIND blurbehind;


	memset(&blurbehind,0,sizeof(blurbehind));
	blurbehind.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
	blurbehind.fEnable = true;
	blurbehind.hRgnBlur = CreateRectRgn(0,0,-1,-1);
	DwmEnableBlurBehindWindow(Window::handle,&blurbehind);
}

int GetTaskbarHeight()
{
	RECT rect;
	HWND taskbar;


	taskbar = FindWindow(L"Shell_traywnd",NULL);
	if(taskbar && GetWindowRect(taskbar,&rect))
	{
		return rect.bottom - rect.top;
	}
	else
	{
		throw GetLastError();
	}
}

LRESULT CALLBACK HandleMessage(HWND window_handle,UINT message,WPARAM w_param,LPARAM l_param)
{
	switch(message)
	{
		case WM_ERASEBKGND:
			return TRUE;
		case WM_CLOSE:
			KillTimer(Window::handle,TIMER_RENDER);
			break;
		case WM_KEYDOWN:
			SendMessage(Window::handle,WM_CLOSE,0,0);

			return 0;
		case WM_OPEN:
			EnableTransparency();
			SetTimer(Window::handle,TIMER_RENDER,1000 / 60,nullptr);
			break;
		case WM_PAINT:
			return Paint();
		case WM_TIMER:
			switch(w_param)
			{
				case TIMER_RENDER:
					UI::Draw();
					OpenGL::Render();

					return 0;
			}
			break;
	}

	return Window::HandleMessage(window_handle,message,w_param,l_param);
}

LRESULT Paint()
{
	PAINTSTRUCT paint_struct;

	BeginPaint(Window::handle,&paint_struct);
	EndPaint(Window::handle,&paint_struct);

	return 0;
}