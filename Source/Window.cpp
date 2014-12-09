#include "Window.h"

#include <thread>


/* [Window] Definition */
namespace Window
{
	namespace Class
	{
		ATOM atom = 0;
		LPTSTR name = L"WindowClass";
		UINT extra_bytes = 0;
		UINT style = CS_HREDRAW | CS_VREDRAW;
	}
	namespace Menu
	{
		HMENU handle = nullptr;
		LPTSTR name = nullptr;
	}

	HBRUSH background = (HBRUSH)(COLOR_BACKGROUND);
	HINSTANCE context = nullptr;
	HCURSOR cursor = LoadCursor(nullptr,IDC_ARROW);
	UINT extra_bytes = 0;
	HWND handle = nullptr;
	int height = 480;
	HICON icon = LoadIcon(nullptr,IDI_APPLICATION);
	HICON icon_small = LoadIcon(nullptr,IDI_APPLICATION);
	MessageHandler message_handler = Window::HandleMessage;
	LPTSTR name = L"Window";
	HWND parent = nullptr;
	int position_x = 0;
	int position_y = 0;
	UINT style = WS_POPUP;
	UINT style_extended = WS_EX_APPWINDOW;
	int width = 640;

	void Create(HINSTANCE context)
	{
		Window::context = context;

		if(!Window::Class::atom)
		{
			//Grab the preexisting one with that name
			WNDCLASSEX window_class = {
				sizeof(WNDCLASSEX),
				Window::Class::style,
				Window::message_handler,
				Window::Class::extra_bytes,
				Window::extra_bytes,
				Window::context,
				Window::icon,
				Window::cursor,
				Window::background,
				Window::Menu::name,
				Window::Class::name,
				Window::icon_small
			};


			Window::Class::atom = RegisterClassEx(&window_class);
			if(!Window::Class::atom)
			{
				LPTSTR error_message;


				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
				MessageBox(nullptr,error_message,L"An Error Has Occurred",MB_OK | MB_ICONERROR);
				LocalFree((LPTSTR)&error_message);

				throw GetLastError();
			}
		}

		Window::handle = CreateWindowEx(
			Window::style_extended,
			Window::Class::name,
			Window::name,
			Window::style,
			Window::position_x,
			Window::position_y,
			Window::width,
			Window::height,
			Window::parent,
			Window::Menu::handle,
			Window::context,
			nullptr
		);
		if(Window::handle == nullptr)
		{
			LPTSTR error_message;


			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
			MessageBox(nullptr,error_message,L"An Error Has Occurred",MB_OK | MB_ICONERROR);
			LocalFree((LPTSTR)&error_message);

			throw GetLastError();
		}
	}

	void Destroy()
	{
		DestroyWindow(Window::handle);
	}

	LRESULT CALLBACK HandleMessage(HWND window,UINT message,WPARAM w_param,LPARAM l_param)
	{
		switch(message)
		{
			case WM_CLOSE:
				PostQuitMessage(0);
				break;

			case WM_OPEN:
				ShowWindow(Window::handle,SW_SHOW);
				SetForegroundWindow(Window::handle);
				SetFocus(Window::handle);
				break;
		}

		return DefWindowProc(window,message,w_param,l_param);
	}

	int Open()
	{
		MSG message;


		Window::SendMessage(WM_OPEN,0,0);
		
		while(true)
		{
			if(PeekMessage(&message,nullptr,0,0,PM_REMOVE))
			{
				if(message.message == WM_QUIT)
				{
					break;
				}
				else
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
			else
			{
				std::this_thread::yield();
			}
		}

		return message.wParam;
	}

	void PostMessage(UINT message,WPARAM w_param,LPARAM l_param)
	{
		::PostMessage(Window::handle,message,w_param,l_param);
	}

	void SendMessage(UINT message,WPARAM w_param,LPARAM l_param)
	{
		::SendMessage(Window::handle,message,w_param,l_param);
	}
}