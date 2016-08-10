#ifndef CONSOLE
#define CONSOLE
/////define/////////////////////////////////////
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 8
#define SCREEN_COLORS_PALETTE 256
#define MAX_COLORS_PALETTE  256

#define DEFAULT_PALETTE_FILE "PALDATA2.PAL"

//用于选择全屏/窗口模式
#define SCREEN_FULLSCREEN 0
#define SCREEN_WINDOWED 1

//位图定义
#define BITMAP_ID 0x4D42  //位图的通用id
#define BITMAP_STATE_DEAD 0
#define BITMAP_STATE_ALIVE 1
#define BITMAP_STATE_DYING 2
#define BITMAP_ATTR_LOADED 128
#define BITMAP_EXTRACT_MODE_CELL 0
#define BITMAP_EXTRACT_MODE_ABS 1

//directdraw像素格式定义,用于位图程序将数据加载成适当的格式
#define DD_PIXEL_FORMAT8        8
#define DD_PIXEL_FORMAT555      15
#define DD_PIXEL_FORMAT565      16
#define DD_PIXEL_FORMAT888      24
#define DD_PIXEL_FORMATALPHA888 32

//BOB引擎定义
#define BOB_STATE_DEAD         0    // 被销毁的BOB引擎
#define BOB_STATE_ALIVE        1    // 存在的BOB引擎
#define BOB_STATE_DYING        2    // 
#define BOB_STATE_ANIM_DONE    1    // 做动画状态
#define MAX_BOB_FRAMES         64   // 最大数量的BOB帧
#define MAX_BOB_ANIMATIONS     16   //最大数量的动画序列

#define BOB_ATTR_SINGLE_FRAME   1   // 使用单个帧创建BOB
#define BOB_ATTR_MULTI_FRAME    2   // 使用多个帧创建BOB，但是BOB将以现行次序0...n执行动画
#define BOB_ATTR_MULTI_ANIM     4   // 使用一个多帧的、支持动画序列的BOB
#define BOB_ATTR_ANIM_ONE_SHOT  8   // 动画纸播放一次，然后停止。
#define BOB_ATTR_VISIBLE        16  // BOB是可见的
#define BOB_ATTR_BOUNCE         32  // 通知BOB当碰到屏幕边缘时想一个球一样反弹，只有在使用Move_BOB()时，该标志才有效
#define BOB_ATTR_WRAPAROUND     64  // 通知BOB在移出屏幕时卷到屏幕的另一边，只有在使用Move_BOB()时，该标志才有效
#define BOB_ATTR_LOADED         128 // BOB已经被加载
#define BOB_ATTR_CLONE          256 // BOB克隆体

//屏幕转换命令
#define SCREEN_DARKNESS  0         // 消失在黑色中
#define SCREEN_WHITENESS 1         // 消失在白色中
#define SCREEN_SWIPE_X   2         // 水平滑动
#define SCREEN_SWIPE_Y   3         // 垂直滑动
#define SCREEN_DISOLVE   4         // 像素溶解
#define SCREEN_SCRUNCH   5         // 正方形的压缩
#define SCREEN_BLUENESS  6         // 消失在蓝色的
#define SCREEN_REDNESS   7         // 消失在红色中
#define SCREEN_GREENNESS 8         // 消失在绿色中

// 定义Blink_Colors
#define BLINKER_ADD           0    // 增加一个光点到信息库中  
#define BLINKER_DELETE        1    // 从信息库里删除一个光点
#define BLINKER_UPDATE        2    // 更新光点
#define BLINKER_RUN           3    // 正常运行

// 定义pi
#define PI         ((float)3.141592654f)
#define PI2        ((float)6.283185307f)
#define PI_DIV_2   ((float)1.570796327f)
#define PI_DIV_4   ((float)0.785398163f) 
#define PI_INV     ((float)0.318309886f) 

// 定点数学常量
#define FIXP16_SHIFT     16
#define FIXP16_MAG       65536
#define FIXP16_DP_MASK   0x0000ffff
#define FIXP16_WP_MASK   0xffff0000
#define FIXP16_ROUND_UP  0x00008000

////可用的宏////////////////////////////////////////////////////////

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
#define KEY_UP(vk_code) ((GetAsynState(vk_code)&0x8000)?1:0)

#define _RGB16BIT555(r,g,b) ((b&31)+((g&31)<<5)+((r&31)<<10))
#define _RGB16BIT565(r,g,b) ((b&31)+((g&63)<<5)+((r&31)<<11))
#define _RGB24BIT(a,r,g,b) ((b)+((g)<<8)+((r)<<16))
#define _RGB32BIT(a,r,g,b) ((b)+((g)<<8)+((r)<<16)+((a)<<24))

//位操作的宏
#define SET_BIT(word,bit_flag) ((word)=(word)|(bit_flag))
#define RESET_BIT(word,bit_flag) ((word)=(word)&(~bit_flag))

//初始化DirectDraw结构,将其结构体清零，并设置dwSize字段大小
#define DDRAW_INIT_STRUCT(ddstruct) {(memset(&ddstruct,0,sizeof(ddstruct))); ddstruct.dwSize=sizeof(ddstruct);}

//用于计算两个表达式的最小和最大
#define MIN(a, b)  (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b)  (((a) > (b)) ? (b) : (a)) 

#define SWAP(a,b,t) {t=a; a=b; b=t;}

#define DEG_TO_RAD(ang) ((ang)*PI/180.0)
#define RAD_TO_DEG(rads) ((rads)*180.0/PI)

#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))

/////类型/////////////////////////////////////////////////////////

//基本无符号类型
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;
typedef unsigned int   QUAD;
typedef unsigned int   UINT;

//位图结构体
typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER bitmapfileheader;
	BITMAPINFOHEADER bitmapinfoheader;
	PALETTEENTRY palette[256];
	UCHAR *buffer;
}BITMAP_FILE,*BITMAP_FILE_PTR;

//BOB引擎结构体对象
typedef struct BOB_TYP
{
	int state;          // 对象的状态
	int anim_state;     // 动画状态变量
	int attr;           // 用于修饰或说明的对象的属性
	float x, y;            // 位图将显示的位置
	float xv, yv;          // 对象的速度
	int width, height;  // bob的宽度和高度
	int width_fill;     // 内部,用于迫使8 * x宽表面
	int bpp;            // 
	int counter_1;      // 通用计数器
	int counter_2;
	int max_count_1;    // 通用阈值;
	int max_count_2;
	int varsI[16];      // 堆栈的16位整数
	float varsF[16];    // 
	int curr_frame;     // 当前动画帧
	int num_frames;     // 动画帧总数
	int curr_animation; // 指向当前动画
	int anim_counter;   // 使用动画过渡
	int anim_index;     // 动画元素索引
	int anim_count_max; // 在动画的周期数
	int *animations[MAX_BOB_ANIMATIONS]; // 动画序列

	LPDIRECTDRAWSURFACE7 images[MAX_BOB_FRAMES]; // 位图图像DD表面

} BOB, *BOB_PTR;

//简单的位图图像
typedef struct BITMAP_IMAGE_TYP
{
	int state;        
	int attr;           
	int x, y;           
	int width, height; 
	int num_bytes;      
	int bpp;           
	UCHAR *buffer;   

} BITMAP_IMAGE, *BITMAP_IMAGE_PTR;

 //闪烁光结构
typedef struct BLINKER_TYP
{
	// 这里实现用户定义
	int color_index;        
	PALETTEENTRY on_color;   
	PALETTEENTRY off_color;
	int on_time;             
	int off_time;          

	// 内部成员
	int counter;            
	int state;            
} BLINKER, *BLINKER_PTR;

//2D顶点
typedef struct VERTEX2DI_TYP
{
	int x, y;
} VERTEX2DI, *VERTEX2DI_PTR;

//2D顶点
typedef struct VERTEX2DF_TYP
{
	float x, y; // the vertex
} VERTEX2DF, *VERTEX2DF_PTR;

//2D多边形
typedef struct POLYGON2D_TYP
{
	int state;        // 多边形的状态
	int num_verts;    // 顶点数量
	int x0, y0;        // 多边形的中心位置 
	int xv, yv;        // 初速度
	DWORD color;      
	VERTEX2DF *vlist; // 顶点指针列表

} POLYGON2D, *POLYGON2D_PTR;

/////定义矩阵////////////////////////////////////////////////////////////////////////////

//3*3矩阵
typedef struct MATRIX3X3_TYP
{
	union
	{
		float M[3][3];
		struct
		{
			float M00, M01, M02;
			float M10, M11, M12;
			float M20, M21, M22;
		};
	};
}MATRIX3X3, *MATRIX3X3_PTR;
//1*3矩阵
typedef struct MATRIX1X3_TYP
{
	union
	{
		float M[3];
		struct
		{
			float M00, M01, M02;
			
		};
	};
}MATRIX1X3, *MATRIX1X3_PTR;
//3*2矩阵
typedef struct MATRIX3X2_TYP
{
	union
	{
		float M[3][2];
		struct
		{
			float M00, M01;
			float M10, M11;
			float M20, M21;
		};
	};
}MATRIX3X2, *MATRIX3X2_PTR;
//1*2矩阵
typedef struct MATRIX1X2_TYP
{
	union
	{
		float M[2];
		struct
		{
			float M00, M01;
			
		};
	};
}MATRIX1X2, *MATRIX1X2_PTR;

/////全局变量/////////////////////////////////////////////
extern FILE *fp_error;                           // 通用的错误文件
extern char error_filename[80];                  // 错误文件名

extern LPDIRECTDRAW7        lpdd;                 // dd对象
extern LPDIRECTDRAWSURFACE7 lpddsprimary;         // dd主缓冲画面
extern LPDIRECTDRAWSURFACE7 lpddsback;            // dd后备缓冲画面
extern LPDIRECTDRAWPALETTE  lpddpal;              // 创建dd调色板指针
extern LPDIRECTDRAWCLIPPER  lpddclipper;          // dd后备缓冲画面裁剪器
extern LPDIRECTDRAWCLIPPER  lpddclipperwin;       // dd窗口裁剪器
extern PALETTEENTRY         palette[256];         // 调色板
extern PALETTEENTRY         save_palette[256];    // 存储调色板
extern DDSURFACEDESC2       ddsd;                 // dd表面结构描述
extern DDBLTFX              ddbltfx;              // 用来填充
extern DDSCAPS2             ddscaps;              // dd功能结构描述
extern HRESULT              ddrval;               // dd呼叫的结果
extern UCHAR                *primary_buffer;      // 主要的视频缓冲区
extern UCHAR                *back_buffer;         // 二次缓冲区

//lpitch的单位是字节，dwWidth的单位是像素。所以，一个640×480×8（640×480个像素，每像素用8位，即一个字节表示）页面，dwWidth=640 像素（字节）。但是，此时Lpitch不一定就等于640（字节）！同样一个640×480×16的页面，它的Lpitch值也不一定等于640×2（字节）. 这是因为，Lpitch中的字节数有时还包含了高速缓冲区的内存字节数。所以说，假如一个640×480×8的页面，它的dwWidth值是640，但是，它的Lpitch值应该是640+x，x就是高速缓冲区Cache占用的内存。所以，如果要访问这个页面缓冲区的第y行的第x个像素, 应该这样：ddsd.lpSurface[x + y*ddsd.lpitch]

extern int                  primary_lpitch;       // 
extern int                  back_lpitch;          //
extern BITMAP_FILE          bitmap8bit;           //一个8位位图文件
extern BITMAP_FILE          bitmap16bit;          // 一个16位位图文件
extern BITMAP_FILE          bitmap24bit;          // 一个24位位图文件


extern DWORD                start_clock_count;    // 用于计时
extern int                  windowed_mode;        // 用于跟踪dd是否是窗口

// 剪辑矩形
extern int min_clip_x, max_clip_x, min_clip_y, max_clip_y;

//DD_Init()用于覆盖全局变量
extern int screen_width, screen_height, screen_bpp, screen_windowed;

extern int dd_pixel_format;                         // 默认的像素格式

extern int window_client_x0;   // 用来跟踪起始(x,y)客户区
extern int window_client_y0;   // 窗口模式的dd操作

// 存储查找表
extern float cos_look[361]; // 存储0-360
extern float sin_look[361]; // 存储0-360

extern USHORT(*RGB16Bit)(int r, int g, int b);

extern USHORT RGB16Bit565(int r, int g, int b);
extern USHORT RGB16Bit555(int r, int g, int b);

/////函数原型//////////////////////////////////////////////////////////////////////////////////////

// DirectDraw功能
int DDraw_Init(int width, int height, int bpp, int windowed=0);
int DDraw_Shutdown(void);
LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds, int num_rects, LPRECT clip_list);
LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int width, int height, int mem_flags = 0, USHORT color_key_value = 0);

int DDraw_Flip();
int DDraw_Wait_For_Vsync(void);
int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds, USHORT color, RECT *client = NULL);
UCHAR *DDraw_Lock_Surface(LPDIRECTDRAWSURFACE7 lpdds, int *lpitch);
int DDraw_Unlock_Surface(LPDIRECTDRAWSURFACE7 lpdds);
UCHAR *DDraw_Lock_Primary_Surface(void);
int DDraw_Unlock_Primary_Surface(void);
UCHAR *DDraw_Lock_Back_Surface(void);
int DDraw_Unlock_Back_Surface(void);

// BOB引擎
int Create_BOB(BOB_PTR bob, int x, int y, int width, int height, int num_frames, int attr,
	int mem_flags = 0, USHORT color_key_value = 0, int bpp = 8);
int Clone_BOB(BOB_PTR source, BOB_PTR dest);
int Destroy_BOB(BOB_PTR bob);
int Draw_BOB(BOB_PTR bob, LPDIRECTDRAWSURFACE7 dest);
int Draw_Scaled_BOB(BOB_PTR bob, int swidth, int sheight, LPDIRECTDRAWSURFACE7 dest);
int Draw_BOB16(BOB_PTR bob, LPDIRECTDRAWSURFACE7 dest);
int Draw_Scaled_BOB16(BOB_PTR bob, int swidth, int sheight, LPDIRECTDRAWSURFACE7 dest);

int Load_Frame_BOB(BOB_PTR bob, BITMAP_FILE_PTR bitmap, int frame, int cx, int cy, int mode);
int Load_Frame_BOB16(BOB_PTR bob, BITMAP_FILE_PTR bitmap, int frame, int cx, int cy, int mode);
int Animate_BOB(BOB_PTR bob);
int Move_BOB(BOB_PTR bob);
int Load_Animation_BOB(BOB_PTR bob, int anim_index, int num_frames, int *sequence);
int Set_Pos_BOB(BOB_PTR bob, int x, int y);
int Set_Vel_BOB(BOB_PTR bob, int xv, int yv);
int Set_Anim_Speed_BOB(BOB_PTR bob, int speed);
int Set_Animation_BOB(BOB_PTR bob, int anim_index);
int Hide_BOB(BOB_PTR bob);
int Show_BOB(BOB_PTR bob);
int Collision_BOBS(BOB_PTR bob1, BOB_PTR bob2);


//通用工具
DWORD Get_Clock(void);
DWORD Start_Clock(void);
DWORD Wait_Clock(DWORD count);

int Collision_Test(int x1, int y1, int w1, int h1,int x2, int y2, int w2, int h2);

int Color_Scan(int x1, int y1, int x2, int y2,UCHAR scan_start, UCHAR scan_end,UCHAR *scan_buffer, int scan_lpitch);

int Color_Scan16(int x1, int y1, int x2, int y2,USHORT scan_start, USHORT scan_end,UCHAR *scan_buffer, int scan_lpitch);

// 图形
int Draw_Clip_Line(int x0, int y0, int x1, int y1, int color, UCHAR *dest_buffer, int lpitch);
int Draw_Clip_Line16(int x0, int y0, int x1, int y1, int color, UCHAR *dest_buffer, int lpitch);
int Clip_Line(int &x1, int &y1, int &x2, int &y2);
int Draw_Line(int xo, int yo, int x1, int y1, int color, UCHAR *vb_start, int lpitch);
int Draw_Line16(int xo, int yo, int x1, int y1, int color, UCHAR *vb_start, int lpitch);
int Draw_Pixel(int x, int y, int color, UCHAR *video_buffer, int lpitch);
int Draw_Rectangle(int x1, int y1, int x2, int y2, int color, LPDIRECTDRAWSURFACE7 lpdds);

void HLine(int x1, int x2, int y, int color, UCHAR *vbuffer, int lpitch);
void VLine(int y1, int y2, int x, int color, UCHAR *vbuffer, int lpitch);
void HLine16(int x1, int x2, int y, int color, UCHAR *vbuffer, int lpitch);
void VLine16(int y1, int y2, int x, int color, UCHAR *vbuffer, int lpitch);
void Screen_Transitions(int effect, UCHAR *vbuffer, int lpitch);
int Draw_Pixel(int x, int y, int color, UCHAR *video_buffer, int lpitch);
int Draw_Pixel16(int x, int y, int color, UCHAR *video_buffer, int lpitch);

// 调色板
int Set_Palette_Entry(int color_index, LPPALETTEENTRY color);
int Get_Palette_Entry(int color_index, LPPALETTEENTRY color);
int Load_Palette_From_File(char *filename, LPPALETTEENTRY palette);
int Save_Palette_To_File(char *filename, LPPALETTEENTRY palette);
int Save_Palette(LPPALETTEENTRY sav_palette);
int Set_Palette(LPPALETTEENTRY set_palette);
int Rotate_Colors(int start_index, int end_index);
int Blink_Colors(int command, BLINKER_PTR new_light, int id);

// 简单位图
int Create_Bitmap(BITMAP_IMAGE_PTR image, int x, int y, int width, int height, int bpp = 8);
int Destroy_Bitmap(BITMAP_IMAGE_PTR image);
int Draw_Bitmap(BITMAP_IMAGE_PTR source_bitmap, UCHAR *dest_buffer, int lpitch, int transparent);
int Draw_Bitmap16(BITMAP_IMAGE_PTR source_bitmap, UCHAR *dest_buffer, int lpitch, int transparent);
int Load_Image_Bitmap(BITMAP_IMAGE_PTR image, BITMAP_FILE_PTR bitmap, int cx, int cy, int mode);
int Load_Image_Bitmap16(BITMAP_IMAGE_PTR image, BITMAP_FILE_PTR bitmap, int cx, int cy, int mode);
int Scroll_Bitmap(BITMAP_IMAGE_PTR image, int dx, int dy = 0);

int Copy_Bitmap(BITMAP_IMAGE_PTR dest_bitmap, int dest_x, int dest_y,BITMAP_IMAGE_PTR source_bitmap, int source_x, int source_y,int width, int height);

int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height);

// 位图文件
int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename);
int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap);

// 图形设备接口GDI
int Draw_Text_GDI(char *text, int x, int y, COLORREF color, LPDIRECTDRAWSURFACE7 lpdds);
int Draw_Text_GDI(char *text, int x, int y, int color, LPDIRECTDRAWSURFACE7 lpdds);

// 发生错误
int Open_Error_File(char *filename, FILE *fp_override = NULL);
int Close_Error_File(void);
int Write_Error(char *string, ...);

// 2D平面上8位，16位三角形呈现
void Draw_Top_Tri(int x1, int y1, int x2, int y2, int x3, int y3, int color, UCHAR *dest_buffer, int mempitch);

void Draw_Bottom_Tri(int x1, int y1, int x2, int y2, int x3, int y3, int color, UCHAR *dest_buffer, int mempitch);

void Draw_Top_Tri16(int x1, int y1, int x2, int y2, int x3, int y3, int color, UCHAR *dest_buffer, int mempitch);

void Draw_Bottom_Tri16(int x1, int y1, int x2, int y2, int x3, int y3, int color, UCHAR *dest_buffer, int mempitch);

void Draw_Top_TriFP(int x1, int y1, int x2, int y2, int x3, int y3, int color, UCHAR *dest_buffer, int mempitch);

void Draw_Bottom_TriFP(int x1, int y1, int x2, int y2, int x3, int y3, int color, UCHAR *dest_buffer, int mempitch);

void Draw_Triangle_2D(int x1, int y1, int x2, int y2, int x3, int y3,int color, UCHAR *dest_buffer, int mempitch);

void Draw_Triangle_2D16(int x1, int y1, int x2, int y2, int x3, int y3,int color, UCHAR *dest_buffer, int mempitch);

void Draw_TriangleFP_2D(int x1, int y1, int x2, int y2, int x3, int y3,int color, UCHAR *dest_buffer, int mempitch);

inline void Draw_QuadFP_2D(int x0, int y0, int x1, int y1,int x2, int y2, int x3, int y3,int color, UCHAR *dest_buffer, int mempitch);


// 2D平面上8位，16位矩形的呈现与变换
void Draw_Filled_Polygon2D(POLYGON2D_PTR poly, UCHAR *vbuffer, int mempitch);
void Draw_Filled_Polygon2D16(POLYGON2D_PTR poly, UCHAR *vbuffer, int mempitch);
int Translate_Polygon2D(POLYGON2D_PTR poly, int dx, int dy);
int Rotate_Polygon2D(POLYGON2D_PTR poly, int theta);
int Scale_Polygon2D(POLYGON2D_PTR poly, float sx, float sy);
void Build_Sin_Cos_Tables(void);
int Translate_Polygon2D_Mat(POLYGON2D_PTR poly, int dx, int dy);
int Rotate_Polygon2D_Mat(POLYGON2D_PTR poly, int theta);
int Scale_Polygon2D_Mat(POLYGON2D_PTR poly, float sx, float sy);
int Draw_Polygon2D(POLYGON2D_PTR poly, UCHAR *vbuffer, int lpitch);
int Draw_Polygon2D16(POLYGON2D_PTR poly, UCHAR *vbuffer, int lpitch);

// 数学功能
int Fast_Distance_2D(int x, int y);
float Fast_Distance_3D(float x, float y, float z);

// 碰撞检测
int Find_Bounding_Box_Poly2D(POLYGON2D_PTR poly,float &min_x, float &max_x,float &min_y, float &max_y);

int Mat_Mul_1X2_3X2(MATRIX1X2_PTR ma,MATRIX3X2_PTR mb,MATRIX1X2_PTR mprod);

int Mat_Mul_1X3_3X3(MATRIX1X3_PTR ma,MATRIX3X3_PTR mb,MATRIX1X3_PTR mprod);

int Mat_Mul_3X3(MATRIX3X3_PTR ma,MATRIX3X3_PTR mb,MATRIX3X3_PTR mprod);

inline int Mat_Init_3X2(MATRIX3X2_PTR ma,float m00, float m01,float m10, float m11,float m20, float m21);


// 内存操作
inline void Mem_Set_WORD(void *dest, USHORT data, int count);
inline void Mem_Set_QUAD(void *dest, UINT   data, int count);

#endif