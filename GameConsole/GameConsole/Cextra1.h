
#ifndef Cextra1
#define Cextra1


/////基本定义///////////////////////////////////////////

/////可用的宏 /////////////////////////////////////////////////



// 类型 //////////////////////////////////////////////////



// 函数原型 /////////////////////////////////////////////

// input
int DInput_Init(void);
void DInput_Shutdown(void);

int DInput_Init_Joystick(int min_x = -256, int max_x = 256,
	int min_y = -256, int max_y = 256, int dead_band = 10);

int DInput_Init_Mouse(void);
int DInput_Init_Keyboard(void);
int DInput_Read_Joystick(void);
int DInput_Read_Mouse(void);
int DInput_Read_Keyboard(void);
void DInput_Release_Joystick(void);
void DInput_Release_Mouse(void);
void DInput_Release_Keyboard(void);

// 全局变量 ////////////////////////////////////////////////


// EXTERNALS //////////////////////////////////////////////

extern HWND main_window_handle; 
extern HINSTANCE main_instance; 
// directinput globals
extern LPDIRECTINPUT8       lpdi;       // dinput object
extern LPDIRECTINPUTDEVICE8 lpdikey;    // dinput keyboard
extern LPDIRECTINPUTDEVICE8 lpdimouse;  // dinput mouse
extern LPDIRECTINPUTDEVICE8 lpdijoy;    // dinput joystick 
extern GUID                 joystickGUID; // guid for main joystick
extern char                 joyname[80];  // name of joystick
// 包含所有di输入数据包的目标记录
extern UCHAR keyboard_state[256]; // contains keyboard state table
extern DIMOUSESTATE mouse_state;  // contains state of mouse
extern DIJOYSTATE joy_state;      // contains state of joystick
extern int joystick_found;        // tracks if stick is plugged in

#endif