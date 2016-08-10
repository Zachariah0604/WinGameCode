/////include//////////////////////////////////////////////

#define INITGUID  
#define WIN32_LEAN_ANDMEAN
#include <tchar.h>   //  _T()
#include<Windows.h>  //包括重要窗口的东西
#include<windowsx.h>
#include<mmsystem.h>
#include<iostream>  //包括重要的C / c++的东西
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
#include<ddraw.h>  //包括DirectX
//#include <initguid.h>
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#include <dinput.h>
#include "CONSOLE.h"  //游戏库1
#include "Cextra1.h"  //游戏库2
#include "CResource.h" //游戏库3(主要是资源类型的头文件)
//#include <atltypes.h>
/////Define////////////////////////////////////////////////////


//为windows界面做宏定义
#define WINDOW_CLASS_NAME "WIN3CLASS"  //类名
#define WINDOW_TITLE "GameConsole" 
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_BPP 8
#define WINDOWED_APP 0  //0全屏运行,1窗口运行

/////函数原型//////////////////////////////////////////////////

//游戏控制台
int Game_Init(void *parms = NULL);
int Game_Main(void *parms = NULL);
int Game_Shutdown(void *parms = NULL);
//全局变量
HWND main_window_handle = NULL;  //保存窗口句柄
HINSTANCE main_instance = NULL;  //保存实例
char buffer[256];                //用于打印的文本

/////函数/////////////////////////////////////////////////////

//WindowProc消息处理
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps; //绘图结构,在WM_PAINT中用到
	HDC hdc;  //设备上下文的句柄
	//获取消息
	switch (msg)
	{
	case WM_CREATE:
	{
					  //在这里做初始化的东西
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
	return(DefWindowProc(hwnd, msg, wparam, lparam));  //处理任何我们没有顾全到的消息
}

//WinMain
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstace, LPSTR lpcmdline, int ncmdshow)
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
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = _T(WINDOW_CLASS_NAME);
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&winclass))
		return(0);

	if (!(hwnd = CreateWindowEx(NULL,
		_T(WINDOW_CLASS_NAME),
		_T(WINDOW_TITLE),
		(WINDOWED_APP ? (WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION) : (WS_POPUP | WS_VISIBLE)),
		0, 0,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL,
		NULL,
		hinstance,
		NULL
		)))
		return(0);

	//保存窗口句柄和实例在全局变量中
	main_window_handle = hwnd;
	main_instance = hinstance;

	//改变窗口的大小,以便客户端真的是宽度x高度
	if (WINDOWED_APP)
	{
		//现在改变窗口的大小,所以客户区是实际尺寸要求,因为可能需要边界和控制.如果这将是一个窗口的应用程序如果没有窗口的应用程序,那么它就不重要
		RECT window_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		//调整window_rect
		AdjustWindowRectEx(&window_rect, GetWindowStyle(main_window_handle), GetMenu(main_window_handle) != NULL, GetWindowExStyle(main_window_handle));
		//将客户端偏移量储存到全局变量中，这个在DDraw_Flip()需要被用到.
		window_client_x0 = -window_rect.left;
		window_client_y0 = -window_rect.top;
		//现在用以下函数改变窗口的大小
		MoveWindow(main_window_handle, CW_USEDEFAULT, CW_USEDEFAULT, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, TRUE);
		//显示窗口
		ShowWindow(main_window_handle, SW_SHOW);
	}

	Game_Init();

	//主事件循环
	while (1)
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

//游戏编程控制台功能
int Game_Init(void *parms)
{
	//游戏初始化

	//启动DirectDraw
	DDraw_Init(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_BPP,WINDOWED_APP);

	//初始化DirectInput
	DInput_Init();

	//获取键盘
	DInput_Init_Keyboard();

	//同时也可以再这里获取其他形式的设备

	//初始化声音和音乐
	DSound_Init();
	DMusic_Init();

	//隐藏鼠标
	if (!WINDOWED_APP)
		ShowCursor(FALSE);

	//时钟随机数生成
	srand(Start_Clock());

	//所有的初始化代码......

	return(1);
}
int Game_Shutdown(void *parms)
{
	//关闭游戏，释放资源
	DSound_Stop_All_Sounds();
	DSound_Delete_All_Sounds();
	DSound_Shutdown();

	DMusic_Delete_All_MIDI();
	DMusic_Shutdown();

	DInput_Release_Keyboard();

	DInput_Shutdown();

	DDraw_Shutdown();

	return(1);
}
int Game_Main(void *parms)
{
	//游戏的主体，这里产生游戏的所有命令，游戏时有点类似有C语言的main()
	int index;//循环变量

	//计时器开始工作
	Start_Clock();

	//清理绘画表面
	DDraw_Fill_Surface(lpddsback,0);

	//读取键盘和其他设备
	DInput_Read_Keyboard();

	//游戏逻辑运算.....

	//翻转表面
	DDraw_Flip();

	//同步到30左右的fps
	Wait_Clock(30);

	//检测用户是否退出
	if (KEY_DOWN(VK_ESCAPE) || keyboard_state[DIK_ESCAPE])
	{
		PostMessage(main_window_handle, WM_DESTROY, 0, 0);
	}

	return(1);
}