#define WIN32_LEAN_AND_MEAN

#define INITGUID
#include<tchar.h>
#include<Windows.h>
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
#include<dinput.h>
#include"KeyBoardDevice.h"

#define WINDOW_CLASS_NAME "WINCLASS1"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 8
#define BITMAP_ID 0x4D42
#define MAX_COLORS_PALETTE 256

#define SKELATON_EAST         0
#define SKELATON_NEAST        1  
#define SKELATON_NORTH        2
#define SKELATON_NWEST        3
#define SKELATON_WEST         4
#define SKELATON_SWEST        5
#define SKELATON_SOUTH        6
#define SKELATON_SEAST        7

#define WALL_ANIMATION_COLOR 29

int Game_Init(void *parms = NULL, int num_parms = 0);
int Game_Shutdown(void *parms = NULL, int num_parms = 0);
int Game_Main(void *parms = NULL, int num_parms = 0);

HWND main_window_handle = NULL;
HINSTANCE main_instance = NULL;
LPDIRECTINPUT8 lpdi = NULL;
LPDIRECTINPUTDEVICE8 lpdikey = NULL;
GUID joystickGUID;
UCHAR keyboard_state[256];
BOB skelaton;
BOB plasma;
BITMAP_IMAGE reactor;

int window_closed = 0;
char buffer[80];
int skelaton_anims[8][4] = 
{ 
{ 0, 1, 0, 2 },
{ 0 + 4, 1 + 4, 0 + 4, 2 + 4 },
{ 0 + 8, 1 + 8, 0 + 8, 2 + 8 },
{ 0 + 12, 1 + 12, 0 + 12, 2 + 12 },
{ 0 + 16, 1 + 16, 0 + 16, 2 + 16 },
{ 0 + 20, 1 + 20, 0 + 20, 2 + 20 },
{ 0 + 24, 1 + 24, 0 + 24, 2 + 24 },
{ 0 + 28, 1 + 28, 0 + 28, 2 + 28 },
};

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
					 hdc = BeginPaint(hwnd, &ps);
					 EndPaint(hwnd, &ps);
					 return(0);
	}break;
	case WM_DESTROY:
	{
					   PostQuitMessage(0);
					   return(0);
	}break;
	default:break;
	}
	return(DefWindowProc(hwnd, msg, wparam, lparam));
}
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	WNDCLASSEX winclass;
	HWND hwnd;
	MSG msg;
	HDC hdc;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC |
		CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = _T(WINDOW_CLASS_NAME);
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	main_instance = hinstance;
	if (!RegisterClassEx(&winclass))
		return(0);

	if (!(hwnd = CreateWindowEx(NULL,
		_T(WINDOW_CLASS_NAME),
		_T("DirectInput Keyboard Demo"),
		WS_POPUP | WS_VISIBLE,
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL,
		NULL,
		hinstance,
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

	DDraw_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

#if 0
	if(DirectDrawCreateEx(main_instance,DIRECTINPUT_VERSION,IID_IDirectDraw7,(void **)&lpdi,NULL)!=DI_OK)
		return(0);
	if (lpdi->CreateDeviceEx(GUID_SysKeyboard, IID_IDirectDraw7, (void **)&lpdikey, NULL) != DI_OK)
		return(0);

#endif
	if (DirectInput8Create(main_instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&lpdi, NULL) != DI_OK)
		return(0);
	if (lpdi->CreateDevice(GUID_SysKeyboard, &lpdikey, NULL) != DI_OK)
		return(0);
	if (lpdikey->SetCooperativeLevel(main_window_handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK)
		return(0);
	if (lpdikey->SetDataFormat(&c_dfDIKeyboard) != DI_OK)
		return(0);
	if (lpdikey->Acquire() != DI_OK)
		return(0);
	Load_Bitmap_File(&bitmap8bit, "REACTOR.BMP");
	Set_Palette(bitmap8bit.palette);
	Create_Bitmap(&reactor, 0, 0, 640, 480);
	Load_Image_Bitmap(&reactor, &bitmap8bit, 0, 0, BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bitmap8bit);
	if (!Create_BOB(&skelaton, 0, 0, 56, 72, 32,
		BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_ANIM, DDSCAPS_SYSTEMMEMORY))
		return(0);
	int direction;
	for (direction = 0; direction < 8; direction++)
	{
		sprintf_s(filename, "SKELSP%d.BMP", direction);
		Load_Bitmap_File(&bitmap8bit, filename);
		Load_Frame_BOB(&skelaton, &bitmap8bit, 0 + direction * 4, 0, 0, BITMAP_EXTRACT_MODE_CELL);
		Load_Frame_BOB(&skelaton, &bitmap8bit, 1 + direction * 4, 1, 0, BITMAP_EXTRACT_MODE_CELL);
		Load_Frame_BOB(&skelaton, &bitmap8bit, 2 + direction * 4, 2, 0, BITMAP_EXTRACT_MODE_CELL);
		Load_Frame_BOB(&skelaton, &bitmap8bit, 3 + direction * 4, 0, 1, BITMAP_EXTRACT_MODE_CELL);
		Unload_Bitmap_File(&bitmap8bit);
		Load_Animation_BOB(&skelaton, direction, 4, skelaton_anims[direction]);
	}
	Set_Animation_BOB(&skelaton, 0);
	Set_Anim_Speed_BOB(&skelaton, 4);
	Set_Vel_BOB(&skelaton, 0, 0);
	Set_Pos_BOB(&skelaton, 0, 128);

	RECT screen_rect = { 0, 0, screen_width, screen_height };
	lpddclipper = DDraw_Attach_Clipper(lpddsback, 1, &screen_rect);

	ShowCursor(FALSE);
	return(1);

}
int Game_Shutdown(void *parms, int num_parms)
{
	Destroy_Bitmap(&reactor);
	Destroy_BOB(&skelaton);
	lpdikey->Unacquire();
	lpdikey->Release();
	lpdi->Release();
	DDraw_Shutdown();
	return(1);
}
int Game_Main(void *parms, int num_parms)
{
	int index;
	int dx, dy;
	static int player_moving = 0;
	static PALETTEENTRY glow = { 0, 0, 0, PC_NOCOLLAPSE };
	static int glow_count = 0, glow_dx = 5;
	
	if (KEY_DOWN(VK_ESCAPE) || KEY_DOWN(VK_SPACE))
		PostMessage(main_window_handle, WM_DESTROY, 0, 0);

	Start_Clock();
	DDraw_Fill_Surface(lpddsback, 0);
	DDraw_Lock_Back_Surface();
	Draw_Bitmap(&reactor, back_buffer, back_lpitch, 0);
	DDraw_Unlock_Back_Surface();
	lpdikey->GetDeviceState(256, (LPVOID)keyboard_state);
	player_moving = 0;

	if (keyboard_state[DIK_RIGHT] && keyboard_state[DIK_UP])
	{
		skelaton.x += 2;
		skelaton.y -= 2;
		dx = 2; dy = -2;
		player_moving = 1;
		if (skelaton.curr_animation != SKELATON_NEAST)
			Set_Animation_BOB(&skelaton, SKELATON_NEAST);
	}
	else
	if (keyboard_state[DIK_LEFT] && keyboard_state[DIK_UP])
	{
		
		skelaton.x -= 2;
		skelaton.y -= 2;
		dx = -2; dy = -2;
		player_moving = 1;

		
		if (skelaton.curr_animation != SKELATON_NWEST)
			Set_Animation_BOB(&skelaton, SKELATON_NWEST);

	} 
	else
	if (keyboard_state[DIK_LEFT] && keyboard_state[DIK_DOWN])
	{
		
		skelaton.x -= 2;
		skelaton.y += 2;
		dx = -2; dy = 2;

		
		player_moving = 1;

		
		if (skelaton.curr_animation != SKELATON_SWEST)
			Set_Animation_BOB(&skelaton, SKELATON_SWEST);

	} 
	else
	if (keyboard_state[DIK_RIGHT] && keyboard_state[DIK_DOWN])
	{
		
		skelaton.x += 2;
		skelaton.y += 2;
		dx = 2; dy = 2;

		
		player_moving = 1;

		
		if (skelaton.curr_animation != SKELATON_SEAST)
			Set_Animation_BOB(&skelaton, SKELATON_SEAST);

	} 
	else
	if (keyboard_state[DIK_RIGHT])
	{
		
		skelaton.x += 2;
		dx = 2; dy = 0;

		
		player_moving = 1;

		
		if (skelaton.curr_animation != SKELATON_EAST)
			Set_Animation_BOB(&skelaton, SKELATON_EAST);

	} 
	else
	if (keyboard_state[DIK_LEFT])
	{
		
		skelaton.x -= 2;
		dx = -2; dy = 0;

		player_moving = 1;

		if (skelaton.curr_animation != SKELATON_WEST)
			Set_Animation_BOB(&skelaton, SKELATON_WEST);

	} 
	else
	if (keyboard_state[DIK_UP])
	{
		
		skelaton.y -= 2;
		dx = 0; dy = -2;

		player_moving = 1;

		
		if (skelaton.curr_animation != SKELATON_NORTH)
			Set_Animation_BOB(&skelaton, SKELATON_NORTH);

	} 
	else
	if (keyboard_state[DIK_DOWN])
	{
	
		skelaton.y += 2;
		dx = 0; dy = +2;

		player_moving = 1;

		if (skelaton.curr_animation != SKELATON_SOUTH)
			Set_Animation_BOB(&skelaton, SKELATON_SOUTH);

	}
	if (player_moving)
	{
		Animate_BOB(&skelaton);
		DDraw_Lock_Back_Surface();
		if (Color_Scan(skelaton.x + 16, skelaton.y + 16, skelaton.x + skelaton.width - 16, skelaton.y + skelaton.height - 16, WALL_ANIMATION_COLOR, WALL_ANIMATION_COLOR, back_buffer, back_lpitch))
		{
			skelaton.x -= dx;
			skelaton.y -= dy;

		}
		DDraw_Unlock_Back_Surface();
		if (skelaton.x<0 || skelaton.x>(screen_width - skelaton.width))
			skelaton.x -= dx;
		if (skelaton.y < 0 || skelaton.y >(screen_height - skelaton.height))
			skelaton.y -= dy;
	}
	Draw_BOB(&skelaton, lpddsback);

	glow.peGreen += glow_dx;

	if (glow.peGreen == 0 || glow.peGreen == 255)
		glow_dx = -glow_dx;
	Set_Palette_Entry(WALL_ANIMATION_COLOR, &glow);

	Draw_Text_GDI("I STILL HAVE A BONE TO PICK!", 0, screen_height - 32, WALL_ANIMATION_COLOR, lpddsback);

	Draw_Text_GDI("USE ARROW KEYS TO MOVE, <ESC> TO EXIT.", 0, 0, RGB(32, 32, 32), lpddsback);

	DDraw_Flip();

	Wait_Clock(30);

	// return success
	return(1);
}