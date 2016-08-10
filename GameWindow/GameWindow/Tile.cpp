#define WIN32_LEAN_AND_MEAN
#define INITGUID

#include <tchar.h>
#include<windows.h>
#include<windowsx.h>
#include<mmsystem.h>
#include<objbase.h>
#include<iostream>
#include<conio.h>
#include<stdlib.h>
#include<malloc.h>
#include<memory.h>
#include<string.h>
#include<stdarg.h>
#include<stdio.h>
#include<math.h>
#include<io.h>
#include<fcntl.h>

#include<ddraw.h>
#include "T3DLIB1.H"

#define WINDOW_CLASS_NAME "WINCLASS1"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 8
#define NUM_TEXTURES 10

int world_x = 0, world_y = 0;
int Game_Init(void *parms = NULL, int num_parms = 0);
int Game_Shutdown(void *parms = NULL, int num_parms = 0);
int Game_Main(void *parms = NULL, int num_parms = 0);
char buffer[80];
char *world[21] =
{
	"111111111111111111111111111111",
	"100000000000000000000000000001",
	"100002222220000000000000077701",
	"100002222223333333333000077701",
	"100002222227777777773000070001",
	"100002222227777777773000070001",
	"100000000377777777773000070001",
	"107777700377777777773000070001",
	"177777770377777777773000770001",
	"107777700377777777773007700001",
	"100777770377777777773777000001",
	"100000707377777777773000000001",
	"100007777377777777773000000001",
	"100000000302222777773000000001",
	"100000000332222777773000000001",
	"100000000002222333333000000001",
	"100000666666666666666666600001",
	"100000800000000000000000800001",
	"100000800000000000000000800001",
	"100000000000000000000000000001",
	"111111111111111111111111111111",
};

HWND main_window_handle = NULL;
int window_closed = 0;
HINSTANCE main_instance = NULL;
BOB          textures;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	char buffer[80];
	switch (msg)
	{
	case WM_CREATE:
	{
					  return(0);
	}break;
	case WM_PAINT:
	{
					 hdc = BeginPaint(hwnd,&ps);
					 EndPaint(hwnd,&ps);
					 return(0);
	}break;
	case WM_DESTROY:
	{
					   PostQuitMessage(0);
					   return(0);
	}break;
	default:break;
	}
	return(DefWindowProc(hwnd,msg,wparam,lparam));
}

int WINAPI WinMain(HINSTANCE hinstace, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	WNDCLASSEX winclass;
	HWND hwnd;
	MSG msg;
	HDC hdc;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstace;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = _T(WINDOW_CLASS_NAME);
	winclass.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	main_instance = hinstace;

	if (!RegisterClassEx(&winclass))
		return(0);

	if (!(hwnd = CreateWindowEx(NULL,
		_T(WINDOW_CLASS_NAME),
		_T("æ˘‘»∆Ω∆Ã≤‚ ‘"),
		WS_POPUP | WS_VISIBLE,
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL,
		NULL,
		hinstace,
		NULL)))
	return(0);
	main_window_handle = hwnd;

	Game_Init();

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) 
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Game_Main();
	}

	Game_Shutdown();

	return(msg.wParam);
}

int Game_Init(void *parms, int num_parms)
{
	int index;
	char filename[80];

	DDraw_Init(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP);
	Load_Bitmap_File(&bitmap8bit,"scrolltextures.bmp");
	Set_Palette(bitmap8bit.palette);

	if (!Create_BOB(&textures, 0, 0, 64, 64, 10, BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME, DDSCAPS_SYSTEMMEMORY))
		return(0);

	for (index = 0; index <= NUM_TEXTURES; index++)
		Load_Frame_BOB(&textures, &bitmap8bit, index, index % 4, index / 4, BITMAP_EXTRACT_MODE_CELL);

	Unload_Bitmap_File(&bitmap8bit);

	RECT screen_rect = { 0, 0, screen_width, screen_height - 32 };

	lpddclipper = DDraw_Attach_Clipper(lpddsback,1,&screen_rect);

	ShowCursor(FALSE);
	return(1);
}
int Game_Shutdown(void *parms, int num_parms)
{
	Destroy_BOB(&textures);
	DDraw_Shutdown();
	return(1);
}
int Game_Main(void *parms, int num_parms)
{
	int index, index_x, index_y;
	int start_map_x, start_map_y, end_map_x, end_map_y;
	int offset_x, offset_y;

	if (KEY_DOWN(VK_ESCAPE) || KEY_DOWN(VK_SPACE))
		PostMessage(main_window_handle,WM_DESTROY,0,0);

	Start_Clock();

	DDraw_Fill_Surface(lpddsback,0);

	if (KEY_DOWN(VK_RIGHT))
	{
		if ((world_x += 4) >= 1280)
			world_x = 1279;
	}
	else
	if (KEY_DOWN(VK_LEFT))
	{
		if ((world_x -= 4) <= 0)
			world_x = 0;
	}
	if (KEY_DOWN(VK_DOWN))
	{
		if ((world_y += 4) >= 896)
			world_y = 895;
	}
	else
	if (KEY_DOWN(VK_UP))
	{
		if ((world_y -= 4) <= 0)
			world_y = 0;
	}

	start_map_x = world_x / 64;
	start_map_y = world_y / 64;
	end_map_x = start_map_x + 10 - 1;
	end_map_y = start_map_y + 7 - 1;
	offset_x = -(world_x % 64);
	offset_y = -(world_y % 64);
	
	int texture_x = offset_x;
	int texture_y = offset_y;

	if (offset_x)
		end_map_x++;
	if (offset_y)
		end_map_y++;
	for (index_y = start_map_y; index_y <= end_map_y; index_y++)
	{
		for (index_x = start_map_x; index_x <= end_map_x; index_x++)
		{
			textures.x = texture_x;
			textures.y = texture_y;

			textures.curr_frame = world[index_y][index_x] - '0';

			Draw_BOB(&textures,lpddsback);
			texture_x += 64;
		}
		texture_x = offset_x;
		texture_y += 64;
	}
	Draw_Text_GDI("USE ARROW KEYS TO MOVE, <ESC> to Exit.", 8, 8, RGB(255, 255, 255), lpddsback);

	sprintf_s(buffer, "World Position = [%d, %d]     ", world_x, world_y);
	Draw_Text_GDI(buffer, 8, screen_height - 32 - 24, RGB(0, 255, 0), lpddsback);

	
	DDraw_Flip();

	
	Wait_Clock(30);

	return(1);
}