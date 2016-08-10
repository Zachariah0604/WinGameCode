/////include//////////////////////////////////////////////

#define INITGUID  
#define WIN32_LEAN_ANDMEAN
#include <tchar.h>   //  _T()
#include<Windows.h>  //������Ҫ���ڵĶ���
#include<windowsx.h>
#include<mmsystem.h>
#include<iostream>  //������Ҫ��C / c++�Ķ���
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
#include<ddraw.h>  //����DirectX
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
#include "CONSOLE.h"  //��Ϸ��1
#include "Cextra1.h"  //��Ϸ��2
#include "CResource.h" //��Ϸ��3(��Ҫ����Դ���͵�ͷ�ļ�)
//#include <atltypes.h>
/////Define////////////////////////////////////////////////////


//Ϊwindows�������궨��
#define WINDOW_CLASS_NAME "WIN3CLASS"  //����
#define WINDOW_TITLE "GameConsole" 
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_BPP 8
#define WINDOWED_APP 0  //0ȫ������,1��������

/////����ԭ��//////////////////////////////////////////////////

//��Ϸ����̨
int Game_Init(void *parms = NULL);
int Game_Main(void *parms = NULL);
int Game_Shutdown(void *parms = NULL);
//ȫ�ֱ���
HWND main_window_handle = NULL;  //���洰�ھ��
HINSTANCE main_instance = NULL;  //����ʵ��
char buffer[256];                //���ڴ�ӡ���ı�

/////����/////////////////////////////////////////////////////

//WindowProc��Ϣ����
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps; //��ͼ�ṹ,��WM_PAINT���õ�
	HDC hdc;  //�豸�����ĵľ��
	//��ȡ��Ϣ
	switch (msg)
	{
	case WM_CREATE:
	{
					  //����������ʼ���Ķ���
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
	return(DefWindowProc(hwnd, msg, wparam, lparam));  //�����κ�����û�й�ȫ������Ϣ
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

	//���洰�ھ����ʵ����ȫ�ֱ�����
	main_window_handle = hwnd;
	main_instance = hinstance;

	//�ı䴰�ڵĴ�С,�Ա�ͻ�������ǿ��x�߶�
	if (WINDOWED_APP)
	{
		//���ڸı䴰�ڵĴ�С,���Կͻ�����ʵ�ʳߴ�Ҫ��,��Ϊ������Ҫ�߽�Ϳ���.����⽫��һ�����ڵ�Ӧ�ó������û�д��ڵ�Ӧ�ó���,��ô���Ͳ���Ҫ
		RECT window_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		//����window_rect
		AdjustWindowRectEx(&window_rect, GetWindowStyle(main_window_handle), GetMenu(main_window_handle) != NULL, GetWindowExStyle(main_window_handle));
		//���ͻ���ƫ�������浽ȫ�ֱ����У������DDraw_Flip()��Ҫ���õ�.
		window_client_x0 = -window_rect.left;
		window_client_y0 = -window_rect.top;
		//���������º����ı䴰�ڵĴ�С
		MoveWindow(main_window_handle, CW_USEDEFAULT, CW_USEDEFAULT, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, TRUE);
		//��ʾ����
		ShowWindow(main_window_handle, SW_SHOW);
	}

	Game_Init();

	//���¼�ѭ��
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

//��Ϸ��̿���̨����
int Game_Init(void *parms)
{
	//��Ϸ��ʼ��

	//����DirectDraw
	DDraw_Init(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_BPP,WINDOWED_APP);

	//��ʼ��DirectInput
	DInput_Init();

	//��ȡ����
	DInput_Init_Keyboard();

	//ͬʱҲ�����������ȡ������ʽ���豸

	//��ʼ������������
	DSound_Init();
	DMusic_Init();

	//�������
	if (!WINDOWED_APP)
		ShowCursor(FALSE);

	//ʱ�����������
	srand(Start_Clock());

	//���еĳ�ʼ������......

	return(1);
}
int Game_Shutdown(void *parms)
{
	//�ر���Ϸ���ͷ���Դ
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
	//��Ϸ�����壬���������Ϸ�����������Ϸʱ�е�������C���Ե�main()
	int index;//ѭ������

	//��ʱ����ʼ����
	Start_Clock();

	//����滭����
	DDraw_Fill_Surface(lpddsback,0);

	//��ȡ���̺������豸
	DInput_Read_Keyboard();

	//��Ϸ�߼�����.....

	//��ת����
	DDraw_Flip();

	//ͬ����30���ҵ�fps
	Wait_Clock(30);

	//����û��Ƿ��˳�
	if (KEY_DOWN(VK_ESCAPE) || keyboard_state[DIK_ESCAPE])
	{
		PostMessage(main_window_handle, WM_DESTROY, 0, 0);
	}

	return(1);
}