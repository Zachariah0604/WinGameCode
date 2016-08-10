#define WIN32_LEAN_AND_MEAN //指示编译器（实际上确定了头文件的包含逻辑）不要包含我们不需要的MFC内容
//#include "resource.H"
//#pragma comment(lib,"ws2_32.lib")
#include <tchar.h>
#include <windows.h> //包括所有的windows头文件
#include <windowsx.h>//含有许多重要的宏和常量的头文件，可以简化windows编程
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GameWindow.H"
#include <malloc.h>
#include <memory.h>
#include "iostream"
#include <conio.h>
#include <ddraw.h>
#include <io.h>
#include <fcntl.h>
//#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"ddraw.lib")

//#include "t3dlib1.h"



//#define ID_CURSOR_CROSSHAIR CURSOR crosshair.cur
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) //宏定义KEYDOWN函数
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); } //初始化一个direct draw结构
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 8
#define MAX_COLORS      256  // maximum colors

#define WINDOW_CLASS_NAME "WINCLASS1" //windows 宏定义

HWND      main_window_handle = NULL; // globally track main window
HINSTANCE hinstance_app      = NULL; // globally track hinstance
char buffer[80];
static int wm_paint_count=0;
int       window_closed      = 0;
int min_clip_x = ((SCREEN_WIDTH/2) - 100),      // clipping rectangle 
    max_clip_x = ((SCREEN_WIDTH/2) + 100),
    min_clip_y = ((SCREEN_HEIGHT/2) - 100),
    max_clip_y = ((SCREEN_HEIGHT/2) + 100);
UCHAR *double_buffer = NULL;
//LPDIRECTDRAW lpdd=NULL;
LPDIRECTDRAW7 lpdd=NULL;
LPDIRECTDRAWPALETTE lpddpal=NULL;
LPDIRECTDRAWSURFACE7 lpddsprimary=NULL;
DDSURFACEDESC2 ddsd;
PALETTEENTRY palette[256];

//基本无符类型
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

//bresenham算法绘制随机线条
int Draw_Line(int x0,int y0,int x1,int y1,UCHAR color,UCHAR *vb_start,int lpitch)
{
	int dx,dy,dx2,dy2,x_inc,y_inc,error,index;
	vb_start=vb_start+x0+y0*lpitch;//预先计算第一个像素内存地址

	dx=x1-x0;
	dy=y1-y0;
	//判断直线的左右走向
	if(dx>=0)
		x_inc=1;
	else
	{
		x_inc=-1;
		dx=-dx;
	}
	if(dy>=0)
		y_inc=lpitch;
	else
	{
		y_inc=-lpitch;
		dy=-dy;
	}

	dx2=dx<<1;
	dy2=dy<<1;

	//如果斜率<=1
	if(dx>dy)
	{
		//初始化错误的周期循环
		error=dy2-dx;

		for(index=0;index<=dx;index++)
		{
			*vb_start=color;
			if(error>=0)
			{
				error-=dx2;
				vb_start+=y_inc;//跳转到下一行
			}
			error+=dy2;
			vb_start+=x_inc;//跳转到下一个像素点
		}
	}
	else
	{
		error=dx2-dy;
		for(index=0;index<=dy;index++)
		{
			*vb_start=color;
			if(error>=0)
			{
				error-=dy2;
				vb_start+=x_inc;
			}
			error+=dx2;
			vb_start+=y_inc;
		}

	}
	return(1);
}



LRESULT CALLBACK WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam) //完整的WinProc开始
{
	PAINTSTRUCT ps;  //在WM_PAINT中用到
	HDC hdc;
	switch(msg)
	{
	case WM_CREATE:
		{
			//当窗口第一次创建时传递该消息，以便你进行启动，初始化或资源配置工作
			//在这里写初始化事件
			return(0);
		}break;

		//PlaySound菜单，播放声音事件
	case WM_COMMAND:
		{
			switch(LOWORD(wparam))
			{
			case MENU_FILE_ID_EXIT:
				{
					PostQuitMessage(0);
				}break;
			case MENU_HELP_ABOUT:
				{
					MessageBox(hwnd,_T("Menu Sound Demo"),_T("About Sound Menu"),MB_OK | MB_ICONEXCLAMATION);
				}break;
			case MENU_PLAY_ID_ENERGIZE:
				{
					PlaySound(MAKEINTRESOURCE(SOUND_ID_ENERGIZE),hinstance_app,SND_RESOURCE | SND_ASYNC);
				}break;
				case MENU_PLAY_ID_BEAM:
				{
					PlaySound(MAKEINTRESOURCE(SOUND_ID_BEAM),hinstance_app,SND_RESOURCE | SND_ASYNC);
				}break;
				case MENU_PLAY_ID_TELEPORT:
				{
					PlaySound(MAKEINTRESOURCE(SOUND_ID_TELEPORT),hinstance_app,SND_RESOURCE | SND_ASYNC);
				}break;
				case MENU_PLAY_ID_WARP:
				{
					PlaySound(MAKEINTRESOURCE(SOUND_ID_WARP),hinstance_app,SND_RESOURCE | SND_ASYNC);
				}break;
				default:break;
			}
		}
	case WM_PAINT:
		{
			//<简单的确认窗口>当一个窗口内容需要重画时传递该消息。这可能有许多原因；用户移动窗口或改变其尺寸，弹出其他应用程序而遮挡了你的窗口等，
			hdc=BeginPaint(hwnd,&ps);  //do all your painting here

			////跟踪窗口变化
			//SetTextColor(hdc,RGB(0,0,255));
			//SetBkColor(hdc,RGB(0,0,0));
			//SetBkMode(hdc,TRANSPARENT);
			//sprintf(buffer,"WM_PAINT called %d times  ",++wm_paint_count);
			//TextOut(hdc,0,0,buffer,strlen(buffer));


			EndPaint(hwnd,&ps);  
			return(0);
		};break;

	//	//跟踪窗口尺寸
	//case WM_SIZE:
	//	{
	//		int width=LOWORD(lparam);
	//		int height=HIWORD(lparam);
	//		hdc=GetDC(hwnd);
	//		SetTextColor(hdc,RGB(0,255,0));
	//		SetBkColor(hdc,RGB(0,0,0));
	//		SetBkMode(hdc,TRANSPARENT);
	//		sprintf(buffer,"New Size=(%d,%d) ",width,height);
	//		TextOut(hdc,200,0,buffer,strlen(buffer));
	//		ReleaseDC(hwnd,hdc);
	//	};break;

		////跟踪窗口移动
		//case WM_MOVE:
		//{
		//	int xpos=LOWORD(lparam);
		//	int ypos=HIWORD(lparam);
		//	hdc=GetDC(hwnd);
		//	SetTextColor(hdc,RGB(0,255,0));
		//	SetBkColor(hdc,RGB(0,0,0));
		//	SetBkMode(hdc,TRANSPARENT);
		//	sprintf(buffer,"New Position=(%d,%d) ",xpos,ypos);
		//	TextOut(hdc,400,0,buffer,strlen(buffer));
		//	ReleaseDC(hwnd,hdc);
		//};break;

		////处理键盘事件
		//case WM_CHAR:
		//{
		//	char ascii_code=wparam;
		//	unsigned int key_state=lparam;
		//	hdc=GetDC(hwnd);
		//	SetTextColor(hdc,RGB(0,255,0));
		//	SetBkColor(hdc,RGB(0,0,0));
		//	SetBkMode(hdc,TRANSPARENT);
		//	sprintf(buffer,"输入字符：%c",ascii_code);
		//	TextOut(hdc,0,0,buffer,strlen(buffer));
		//	sprintf(buffer,"对应计算机地址：0X%X",key_state);
		//	TextOut(hdc,0,16,buffer,strlen(buffer));
		//	ReleaseDC(hwnd,hdc);
		//};break;

		////虚拟键盘消息事件
		//case WM_KEYDOWN:
		//	{
		//		int virtual_code=(int)wparam;
		//		int key_state=(int)lparam;
		//		switch(virtual_code)
		//		{
		//		case VK_TAB:
		//			{
		//				int a=5;
		//				hdc=GetDC(hwnd);
		//	SetTextColor(hdc,RGB(0,255,0));
		//	SetBkColor(hdc,RGB(0,0,0));
		//	SetBkMode(hdc,TRANSPARENT);
		//	sprintf(buffer,"按TAB输入字符：%d",a);
		//	TextOut(hdc,0,600,buffer,strlen(buffer));
		//			} break;
		//			default:break;
		//		}
		//		return(0);
		//	}break;
	//case WM_CLOSE:
	//	{
	//		int result=MessageBox(hwnd,"你确定退出？","退出提示",MB_YESNO | MB_ICONQUESTION);
	//		if(result==IDYES)
	//		{
	//			//return(DefWindowProc(hwnd,msg,wparam,lparam));
	//			PostQuitMessage(0);
	//		}
	//		else
	//			return(0);
	//	}break;
	case WM_DESTROY:
		{
			//当你的窗口将要被销毁时该消息会被传递到窗口，发送一个WM_QUIT消息完全终止应用程序
			PostQuitMessage(0);
			return(0);
		};break;
	default:break;
	}
	return(DefWindowProc(hwnd,msg,wparam,lparam));
}  //结束Winproc


//inline void Plot_Pixel_Faster16(int x,int y,int red,int green,int blue,USHORT *video_buffer,int lpitch16)
//{
//	USHORT pixel=_RGB16BIT565(red,green,blue);
//	video_buffer[x+y*lpitch16]=pixel;
//}
//游戏主循环，游戏进程
int Game_Main(void *params=NULL,int num_parms = 0)
{
	UCHAR *primary_buffer=NULL;
	if(window_closed)
		return(0);
	//用户单击ESC，调用WM_CLOSE，即退出窗口
	if(KEYDOWN(VK_ESCAPE))
	{
		PostMessage(main_window_handle,WM_CLOSE,0,0);
		window_closed=1;
	}

	//memset((void *)double_buffer,0,SCREEN_WIDTH*SCREEN_HEIGHT);
	//for(int index=0;index<5000;index++)
	//{
	//	int x=rand()%SCREEN_WIDTH;
	//	int y=rand()%SCREEN_HEIGHT;
	//	UCHAR col=rand()%256;
	//	double_buffer[x+y*SCREEN_WIDTH]=col;
	//}
	////在主显示面上制作100个随机像素点并返回
	DDRAW_INIT_STRUCT(ddsd); 
	if(FAILED(lpddsprimary->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,NULL)))
		return(0);
	////现在，ddsd.lPitch和ddsd.lpSurface是有效值
	////int mempitch=(int)ddsd.lPitch;
	//primary_buffer=(UCHAR *)ddsd.lpSurface;

	for (int index=0; index < 1000; index++)
    {
    Draw_Line(rand()%SCREEN_WIDTH, rand()%SCREEN_HEIGHT,
              rand()%SCREEN_WIDTH, rand()%SCREEN_HEIGHT,
              rand()%256,
              (UCHAR *)ddsd.lpSurface, ddsd.lPitch);

    }
	//if(ddsd.lPitch==SCREEN_WIDTH)
	//{
	//	memcpy((void *)primary_buffer,(void *)double_buffer,SCREEN_WIDTH*SCREEN_HEIGHT);
	//}
	//else
	//{
	//	UCHAR *dest_ptr=primary_buffer;
	//	UCHAR *src_ptr=double_buffer;
	//	for(int y=0;y<SCREEN_HEIGHT;y++)
	//	{
	//		memcpy((void *)dest_ptr,(void *)src_ptr,SCREEN_WIDTH);
	//		dest_ptr+=ddsd.lPitch;
	//		src_ptr+=SCREEN_WIDTH;
	//	}
	//}

	////int lpitch16=(int)(ddsd.lPitch>>1);
	////USHORT *video_buffer=(USHORT *)ddsd.lpSurface;
	//////在主显示面上用随机颜色制作100个随机像素点
	////for(int i=0;i<1000;i++)
	////{
	////	//在640*480*8上随机选择像素颜色和位置
	////	//UCHAR color=rand()%256;
	////	int red=rand()%256;
	////	int green=rand()%256;
	////	int blue=rand()%256;
	////	int x=rand()%640;
	////	int y=rand()%480;
	////	//绘制像素点
	////	Plot_Pixel_Faster16(x,y,red,green,blue,video_buffer,lpitch16);
	////	//video_buffer[x+y*mempitch]=color;


	////}
	//解锁主显示面
	if(FAILED(lpddsprimary->Unlock(NULL)))
		return(0);
	Sleep(50);
	return(1);
}

//在窗口初始化之后而且在进入主事件循环之前将被调用此函数，在这里实现游戏的初始化
int Game_Init(void *params=NULL,int num_parms=0)
{
	//创建IDirectDraw 7.0接口对象并且测试错误	
	if(FAILED(DirectDrawCreateEx(NULL,(void **)&lpdd,IID_IDirectDraw7,NULL)))
		return(0);
	//设置与windows协作级别
	if(FAILED(lpdd->SetCooperativeLevel(main_window_handle,DDSCL_FULLSCREEN|DDSCL_ALLOWMODEX|DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT)))
		return(0);
	//设置显示模式
	if(FAILED(lpdd->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,0,0)))
		return(0);

	//清楚ddsd并设置大小（size）
	DDRAW_INIT_STRUCT(ddsd);
	ddsd.dwFlags=DDSD_CAPS;
	ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;
	//创建一个主显示表面
	if(FAILED(lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL)))
		return(0);
	for(int color=1;color<255;color++)
	{
		palette[color].peRed=rand()%256;
		palette[color].peBlue=rand()%256;
		palette[color].peGreen=rand()%256;
		palette[color].peFlags=PC_NOCOLLAPSE;

	}
	palette[0].peRed=0;
	palette[0].peGreen=0;
	palette[0].peBlue=0;
	palette[0].peFlags=PC_NOCOLLAPSE;

	palette[255].peRed=255;
	palette[255].peGreen=255;
	palette[255].peBlue=255;
	palette[255].peFlags=PC_NOCOLLAPSE;
	if(FAILED(lpdd->CreatePalette(DDPCAPS_8BIT|DDPCAPS_ALLOW256|DDPCAPS_INITIALIZE,palette,&lpddpal,NULL)))
		return(0);
	//关联调色板
	if(FAILED(lpddsprimary->SetPalette(lpddpal)))
		return(0);
	//分配double buffer
	if ((double_buffer=new UCHAR[SCREEN_WIDTH * SCREEN_HEIGHT])==NULL)
		return(0);
	return(1);
}
int Game_Shutdown(void *params=NULL,int num_parms = 0)
{
	
	if(lpddpal)
	{
		lpddpal->Release();
		lpddpal=NULL;
	}
	if(lpddsprimary)
	{
		lpddsprimary->Release();
		lpddsprimary=NULL;
	}
	if(lpdd)
	{
		lpdd->Release();
		lpdd=NULL;
	}
	return(1);
}

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hprevinstance,LPSTR lpcmdline,int ncmdshow) //主入口函数
{
	//创建Windows类
	WNDCLASSEX winclass;
	HWND hwnd;
	MSG msg;
	winclass.cbSize=sizeof(WNDCLASSEX);  //WNDCLASSEX结构本身的大小
	winclass.style=CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;  //窗口一般属性的样式信息标志   具体P45
	winclass.lpfnWndProc=WindowProc; //指向时间处理的程序的函数指针
	winclass.cbClsExtra=0;
	winclass.cbWndExtra=0;  //这上下两个字段原是为指示Windows将附加的运行时间信息保存到Windows类某些单元中而设计的，一般设置为0
	winclass.hInstance=hinstance; //它就是在启动时传递给WinMain()函数的hInstance，因此只需要简单的从WinMain（）复制即可
	winclass.hIcon=LoadIcon(hinstance, MAKEINTRESOURCE(ICON_T3DX));  //装载一个标准的应用程序图标  具体图标选项参见P47. HINSTANCE设置为NULL类装载一个标准的图标
	winclass.hCursor=LoadCursor(hinstance, MAKEINTRESOURCE(CURSOR_CROSSHAIR));  //直到鼠标指正进入窗口的用户区才显示的光标句柄  具体参见P48
	winclass.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);  //用于窗口刷新的画笔句柄 具体参见P48
	winclass.lpszMenuName=NULL;  //它是菜单资源名称的空值终止ASCII字符串，用于加载和选用窗口。目前将值设置为NULL
	winclass.lpszClassName=_T(WINDOW_CLASS_NAME); //该字段用于Windows跟踪和识别
    winclass.hIconSm=LoadIcon(hinstance, MAKEINTRESOURCE(ICON_T3DX)); //应用程序图标

	hinstance_app = hinstance;
	Game_Init();  //初始化游戏

	if(!RegisterClassEx(&winclass))  //注册Windows类，指向上面创建的Windows类
		return(0);
	//以下开始创建窗口,使用CreatWindowEx()函数

	//第一个窗口
	if(!(hwnd=CreateWindowEx(NULL,  //扩展窗口样式 
		_T(WINDOW_CLASS_NAME), //指向所创建窗口的基础类名
		_T("Window1 Base On WINCLASS1"),  //指向窗口名
		WS_POPUP | WS_VISIBLE,   //说明窗口外光和行为的通用窗口标志 具体参见P51DwStyle的通用样式值
		0,0,  //窗口坐上角的像素坐标。如果无所谓，可使用CW_USEDEFAULT,这将由Windows来决定
		SCREEN_WIDTH,SCREEN_HEIGHT,  //窗口宽度和高度
		NULL,  //加入存在父窗口，这是指向付窗口的句柄，如果没有父窗口，取NULL，桌面就父窗口.
		NULL,  //指向附属于该窗口菜单的句柄。
		hinstance,  //应用程序的实例，这里使用 WinMain()中的hinstance。
		NULL  //高级特征，设置为NULL
		)))
   return(0);

	main_window_handle = hwnd;
	Game_Init();
////按钮创建开始
//#define BUTTON_BASE_ID 100
//#define NUM_BUTTONS 8
//
//	char *button_names[NUM_BUTTONS]=
//	{
//		"PUSHBUTTON",
//		"RADIOBUTTON",
//		"CHECKBOX",
//		"3STATE",
//		"AUTO3STATE",
//		"AUTOCHECKBOX",
//		"AUTORADIOBUTTON",
//		"OWNERDRAW"
//	};
//	long button_types[NUM_BUTTONS]=
//	{
//		BS_PUSHBUTTON,
//		BS_RADIOBUTTON,
//		BS_CHECKBOX,
//		BS_3STATE,
//		BS_AUTO3STATE,
//		BS_AUTOCHECKBOX,
//		BS_AUTORADIOBUTTON,
//		BS_OWNERDRAW
//	};
//
//	for(int button=0;button<NUM_BUTTONS;button++)
//	{
//		CreateWindowEx
//			(
//			NULL,
//			"button",
//			button_names[button],
//			WS_CHILD|WS_VISIBLE|button_types[button],
//			10,10+button*36,
//			strlen(button_names[button])*16,24,
//			main_window_handle,
//			(HMENU)(BUTTON_BASE_ID+button),
//			hinstance,
//			NULL
//			);
//	}
	//按钮创建结束
	HMENU hmenuhandle=LoadMenu(hinstance,_T("SoundMenu"));//创建菜单，关系rc资源文件
	SetMenu(hwnd,hmenuhandle);

	
	HDC hdc=GetDC(hwnd);
	
	
while (TRUE)  //主事件循环，GetMessage(消息结构地址，窗口句柄，起始消息，结束消息)
	{
		//当消息被GetMessage（）获得以后，由函数TransLateMessage（）稍加处理和转换，通过函数DispatchMessage（）调用WinProc进行进一步处理
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
				break;
			TranslateMessage(&msg);  //虚拟加速翻译器（输入工具）  只需调用，不必管其功能
			DispatchMessage(&msg);  //指出所有操作发生的位置
		}

		Game_Main();//主要游戏进程

		////基本文本显示
		//SetTextColor(hdc,RGB(255,255,255));
		//SetBkColor(hdc,RGB(0,0,0));
		//SetBkMode(hdc,TRANSPARENT);
		//TextOut(hdc,rand()%400,rand()%400,"测试文字",strlen("测试文字"));
		//Sleep(10);

		////绘制随机点
		//for(int i=0;i<1000;i++)
		//{
		//	int x=rand()%800;
		//	int y=rand()%600;
		//	COLORREF color=RGB(rand()%256,rand()%256,rand()%256);
		//	SetPixel(hdc,x,y,color);
		//}

		////绘制线段
		//HPEN blue_pen=CreatePen(PS_SOLID,1,RGB(0,0,255));
		//HPEN old_pen=NULL;
		//old_pen=(HPEN)SelectObject(hdc,blue_pen);
		//MoveToEx(hdc,10,10,NULL);
		//LineTo(hdc,50,60);
		//SelectObject(hdc,old_pen);
		//DeleteObject(blue_pen);

		////绘制三角形
		//HPEN blue_pen=CreatePen(PS_SOLID,1,RGB(0,0,255));
		//HPEN old_pen=NULL;
		//old_pen=(HPEN)SelectObject(hdc,blue_pen);
		//MoveToEx(hdc,20,10,NULL);
		//LineTo(hdc,30,20);
		//LineTo(hdc,10,20);
		//LineTo(hdc,20,10);
		//SelectObject(hdc,old_pen);
		//DeleteObject(blue_pen);

		////绘制矩形
		//RECT rect1={10,10,100,100};
		//RECT rect2={100,100,200,200};
		//HBRUSH hbrush=CreateSolidBrush(RGB(255,0,0));
		//FillRect(hdc,&rect1,hbrush);//实心矩形
		//FrameRect(hdc,&rect2,hbrush);//空心（透明）矩形
		//DeleteObject(hbrush);

		////绘制圆心为（200,200），半径为100的圆
		//HBRUSH hbrush=CreateSolidBrush(RGB(255,0,0));
		//SelectObject(hdc,hbrush);
		//Ellipse(hdc,100,100,300,300);
		//DeleteObject(hbrush);

		////绘制随机多边形
		//HPEN   pen_color   = CreatePen(PS_SOLID, 1, RGB(rand()%256,rand()%256,rand()%256));
		//HBRUSH brush_color = CreateSolidBrush(RGB(rand()%256,rand()%256,rand()%256));
		//SelectObject(hdc,pen_color);
		//SelectObject(hdc,brush_color);
		//int num_points = 3+rand()%8;
		//POINT point_list[10];
		//for (int index = 0; index < num_points; index++)
  //      {
  //     
  //      point_list[index].x = rand()%800;
  //      point_list[index].y = rand()%600;
  //      } 
		//Polygon(hdc, point_list, num_points);
		//Sleep(500);
	}
Game_Shutdown();//关闭游戏
ReleaseDC(hwnd,hdc);
return(msg.wParam);
	//return(msg.wParam);
}