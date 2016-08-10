#ifndef CONSOLE
#define CONSOLE
/////define/////////////////////////////////////
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 8
#define SCREEN_COLORS_PALETTE 256
#define MAX_COLORS_PALETTE  256

#define DEFAULT_PALETTE_FILE "PALDATA2.PAL"

//����ѡ��ȫ��/����ģʽ
#define SCREEN_FULLSCREEN 0
#define SCREEN_WINDOWED 1

//λͼ����
#define BITMAP_ID 0x4D42  //λͼ��ͨ��id
#define BITMAP_STATE_DEAD 0
#define BITMAP_STATE_ALIVE 1
#define BITMAP_STATE_DYING 2
#define BITMAP_ATTR_LOADED 128
#define BITMAP_EXTRACT_MODE_CELL 0
#define BITMAP_EXTRACT_MODE_ABS 1

//directdraw���ظ�ʽ����,����λͼ�������ݼ��س��ʵ��ĸ�ʽ
#define DD_PIXEL_FORMAT8        8
#define DD_PIXEL_FORMAT555      15
#define DD_PIXEL_FORMAT565      16
#define DD_PIXEL_FORMAT888      24
#define DD_PIXEL_FORMATALPHA888 32

//BOB���涨��
#define BOB_STATE_DEAD         0    // �����ٵ�BOB����
#define BOB_STATE_ALIVE        1    // ���ڵ�BOB����
#define BOB_STATE_DYING        2    // 
#define BOB_STATE_ANIM_DONE    1    // ������״̬
#define MAX_BOB_FRAMES         64   // ���������BOB֡
#define MAX_BOB_ANIMATIONS     16   //��������Ķ�������

#define BOB_ATTR_SINGLE_FRAME   1   // ʹ�õ���֡����BOB
#define BOB_ATTR_MULTI_FRAME    2   // ʹ�ö��֡����BOB������BOB�������д���0...nִ�ж���
#define BOB_ATTR_MULTI_ANIM     4   // ʹ��һ����֡�ġ�֧�ֶ������е�BOB
#define BOB_ATTR_ANIM_ONE_SHOT  8   // ����ֽ����һ�Σ�Ȼ��ֹͣ��
#define BOB_ATTR_VISIBLE        16  // BOB�ǿɼ���
#define BOB_ATTR_BOUNCE         32  // ֪ͨBOB��������Ļ��Եʱ��һ����һ��������ֻ����ʹ��Move_BOB()ʱ���ñ�־����Ч
#define BOB_ATTR_WRAPAROUND     64  // ֪ͨBOB���Ƴ���Ļʱ����Ļ����һ�ߣ�ֻ����ʹ��Move_BOB()ʱ���ñ�־����Ч
#define BOB_ATTR_LOADED         128 // BOB�Ѿ�������
#define BOB_ATTR_CLONE          256 // BOB��¡��

//��Ļת������
#define SCREEN_DARKNESS  0         // ��ʧ�ں�ɫ��
#define SCREEN_WHITENESS 1         // ��ʧ�ڰ�ɫ��
#define SCREEN_SWIPE_X   2         // ˮƽ����
#define SCREEN_SWIPE_Y   3         // ��ֱ����
#define SCREEN_DISOLVE   4         // �����ܽ�
#define SCREEN_SCRUNCH   5         // �����ε�ѹ��
#define SCREEN_BLUENESS  6         // ��ʧ����ɫ��
#define SCREEN_REDNESS   7         // ��ʧ�ں�ɫ��
#define SCREEN_GREENNESS 8         // ��ʧ����ɫ��

// ����Blink_Colors
#define BLINKER_ADD           0    // ����һ����㵽��Ϣ����  
#define BLINKER_DELETE        1    // ����Ϣ����ɾ��һ�����
#define BLINKER_UPDATE        2    // ���¹��
#define BLINKER_RUN           3    // ��������

// ����pi
#define PI         ((float)3.141592654f)
#define PI2        ((float)6.283185307f)
#define PI_DIV_2   ((float)1.570796327f)
#define PI_DIV_4   ((float)0.785398163f) 
#define PI_INV     ((float)0.318309886f) 

// ������ѧ����
#define FIXP16_SHIFT     16
#define FIXP16_MAG       65536
#define FIXP16_DP_MASK   0x0000ffff
#define FIXP16_WP_MASK   0xffff0000
#define FIXP16_ROUND_UP  0x00008000

////���õĺ�////////////////////////////////////////////////////////

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
#define KEY_UP(vk_code) ((GetAsynState(vk_code)&0x8000)?1:0)

#define _RGB16BIT555(r,g,b) ((b&31)+((g&31)<<5)+((r&31)<<10))
#define _RGB16BIT565(r,g,b) ((b&31)+((g&63)<<5)+((r&31)<<11))
#define _RGB24BIT(a,r,g,b) ((b)+((g)<<8)+((r)<<16))
#define _RGB32BIT(a,r,g,b) ((b)+((g)<<8)+((r)<<16)+((a)<<24))

//λ�����ĺ�
#define SET_BIT(word,bit_flag) ((word)=(word)|(bit_flag))
#define RESET_BIT(word,bit_flag) ((word)=(word)&(~bit_flag))

//��ʼ��DirectDraw�ṹ,����ṹ�����㣬������dwSize�ֶδ�С
#define DDRAW_INIT_STRUCT(ddstruct) {(memset(&ddstruct,0,sizeof(ddstruct))); ddstruct.dwSize=sizeof(ddstruct);}

//���ڼ����������ʽ����С�����
#define MIN(a, b)  (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b)  (((a) > (b)) ? (b) : (a)) 

#define SWAP(a,b,t) {t=a; a=b; b=t;}

#define DEG_TO_RAD(ang) ((ang)*PI/180.0)
#define RAD_TO_DEG(rads) ((rads)*180.0/PI)

#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))

/////����/////////////////////////////////////////////////////////

//�����޷�������
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;
typedef unsigned int   QUAD;
typedef unsigned int   UINT;

//λͼ�ṹ��
typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER bitmapfileheader;
	BITMAPINFOHEADER bitmapinfoheader;
	PALETTEENTRY palette[256];
	UCHAR *buffer;
}BITMAP_FILE,*BITMAP_FILE_PTR;

//BOB����ṹ�����
typedef struct BOB_TYP
{
	int state;          // �����״̬
	int anim_state;     // ����״̬����
	int attr;           // �������λ�˵���Ķ��������
	float x, y;            // λͼ����ʾ��λ��
	float xv, yv;          // ������ٶ�
	int width, height;  // bob�Ŀ�Ⱥ͸߶�
	int width_fill;     // �ڲ�,������ʹ8 * x�����
	int bpp;            // 
	int counter_1;      // ͨ�ü�����
	int counter_2;
	int max_count_1;    // ͨ����ֵ;
	int max_count_2;
	int varsI[16];      // ��ջ��16λ����
	float varsF[16];    // 
	int curr_frame;     // ��ǰ����֡
	int num_frames;     // ����֡����
	int curr_animation; // ָ��ǰ����
	int anim_counter;   // ʹ�ö�������
	int anim_index;     // ����Ԫ������
	int anim_count_max; // �ڶ�����������
	int *animations[MAX_BOB_ANIMATIONS]; // ��������

	LPDIRECTDRAWSURFACE7 images[MAX_BOB_FRAMES]; // λͼͼ��DD����

} BOB, *BOB_PTR;

//�򵥵�λͼͼ��
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

 //��˸��ṹ
typedef struct BLINKER_TYP
{
	// ����ʵ���û�����
	int color_index;        
	PALETTEENTRY on_color;   
	PALETTEENTRY off_color;
	int on_time;             
	int off_time;          

	// �ڲ���Ա
	int counter;            
	int state;            
} BLINKER, *BLINKER_PTR;

//2D����
typedef struct VERTEX2DI_TYP
{
	int x, y;
} VERTEX2DI, *VERTEX2DI_PTR;

//2D����
typedef struct VERTEX2DF_TYP
{
	float x, y; // the vertex
} VERTEX2DF, *VERTEX2DF_PTR;

//2D�����
typedef struct POLYGON2D_TYP
{
	int state;        // ����ε�״̬
	int num_verts;    // ��������
	int x0, y0;        // ����ε�����λ�� 
	int xv, yv;        // ���ٶ�
	DWORD color;      
	VERTEX2DF *vlist; // ����ָ���б�

} POLYGON2D, *POLYGON2D_PTR;

/////�������////////////////////////////////////////////////////////////////////////////

//3*3����
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
//1*3����
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
//3*2����
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
//1*2����
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

/////ȫ�ֱ���/////////////////////////////////////////////
extern FILE *fp_error;                           // ͨ�õĴ����ļ�
extern char error_filename[80];                  // �����ļ���

extern LPDIRECTDRAW7        lpdd;                 // dd����
extern LPDIRECTDRAWSURFACE7 lpddsprimary;         // dd�����廭��
extern LPDIRECTDRAWSURFACE7 lpddsback;            // dd�󱸻��廭��
extern LPDIRECTDRAWPALETTE  lpddpal;              // ����dd��ɫ��ָ��
extern LPDIRECTDRAWCLIPPER  lpddclipper;          // dd�󱸻��廭��ü���
extern LPDIRECTDRAWCLIPPER  lpddclipperwin;       // dd���ڲü���
extern PALETTEENTRY         palette[256];         // ��ɫ��
extern PALETTEENTRY         save_palette[256];    // �洢��ɫ��
extern DDSURFACEDESC2       ddsd;                 // dd����ṹ����
extern DDBLTFX              ddbltfx;              // �������
extern DDSCAPS2             ddscaps;              // dd���ܽṹ����
extern HRESULT              ddrval;               // dd���еĽ��
extern UCHAR                *primary_buffer;      // ��Ҫ����Ƶ������
extern UCHAR                *back_buffer;         // ���λ�����

//lpitch�ĵ�λ���ֽڣ�dwWidth�ĵ�λ�����ء����ԣ�һ��640��480��8��640��480�����أ�ÿ������8λ����һ���ֽڱ�ʾ��ҳ�棬dwWidth=640 ���أ��ֽڣ������ǣ���ʱLpitch��һ���͵���640���ֽڣ���ͬ��һ��640��480��16��ҳ�棬����LpitchֵҲ��һ������640��2���ֽڣ�. ������Ϊ��Lpitch�е��ֽ�����ʱ�������˸��ٻ��������ڴ��ֽ���������˵������һ��640��480��8��ҳ�棬����dwWidthֵ��640�����ǣ�����LpitchֵӦ����640+x��x���Ǹ��ٻ�����Cacheռ�õ��ڴ档���ԣ����Ҫ�������ҳ�滺�����ĵ�y�еĵ�x������, Ӧ��������ddsd.lpSurface[x + y*ddsd.lpitch]

extern int                  primary_lpitch;       // 
extern int                  back_lpitch;          //
extern BITMAP_FILE          bitmap8bit;           //һ��8λλͼ�ļ�
extern BITMAP_FILE          bitmap16bit;          // һ��16λλͼ�ļ�
extern BITMAP_FILE          bitmap24bit;          // һ��24λλͼ�ļ�


extern DWORD                start_clock_count;    // ���ڼ�ʱ
extern int                  windowed_mode;        // ���ڸ���dd�Ƿ��Ǵ���

// ��������
extern int min_clip_x, max_clip_x, min_clip_y, max_clip_y;

//DD_Init()���ڸ���ȫ�ֱ���
extern int screen_width, screen_height, screen_bpp, screen_windowed;

extern int dd_pixel_format;                         // Ĭ�ϵ����ظ�ʽ

extern int window_client_x0;   // ����������ʼ(x,y)�ͻ���
extern int window_client_y0;   // ����ģʽ��dd����

// �洢���ұ�
extern float cos_look[361]; // �洢0-360
extern float sin_look[361]; // �洢0-360

extern USHORT(*RGB16Bit)(int r, int g, int b);

extern USHORT RGB16Bit565(int r, int g, int b);
extern USHORT RGB16Bit555(int r, int g, int b);

/////����ԭ��//////////////////////////////////////////////////////////////////////////////////////

// DirectDraw����
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

// BOB����
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


//ͨ�ù���
DWORD Get_Clock(void);
DWORD Start_Clock(void);
DWORD Wait_Clock(DWORD count);

int Collision_Test(int x1, int y1, int w1, int h1,int x2, int y2, int w2, int h2);

int Color_Scan(int x1, int y1, int x2, int y2,UCHAR scan_start, UCHAR scan_end,UCHAR *scan_buffer, int scan_lpitch);

int Color_Scan16(int x1, int y1, int x2, int y2,USHORT scan_start, USHORT scan_end,UCHAR *scan_buffer, int scan_lpitch);

// ͼ��
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

// ��ɫ��
int Set_Palette_Entry(int color_index, LPPALETTEENTRY color);
int Get_Palette_Entry(int color_index, LPPALETTEENTRY color);
int Load_Palette_From_File(char *filename, LPPALETTEENTRY palette);
int Save_Palette_To_File(char *filename, LPPALETTEENTRY palette);
int Save_Palette(LPPALETTEENTRY sav_palette);
int Set_Palette(LPPALETTEENTRY set_palette);
int Rotate_Colors(int start_index, int end_index);
int Blink_Colors(int command, BLINKER_PTR new_light, int id);

// ��λͼ
int Create_Bitmap(BITMAP_IMAGE_PTR image, int x, int y, int width, int height, int bpp = 8);
int Destroy_Bitmap(BITMAP_IMAGE_PTR image);
int Draw_Bitmap(BITMAP_IMAGE_PTR source_bitmap, UCHAR *dest_buffer, int lpitch, int transparent);
int Draw_Bitmap16(BITMAP_IMAGE_PTR source_bitmap, UCHAR *dest_buffer, int lpitch, int transparent);
int Load_Image_Bitmap(BITMAP_IMAGE_PTR image, BITMAP_FILE_PTR bitmap, int cx, int cy, int mode);
int Load_Image_Bitmap16(BITMAP_IMAGE_PTR image, BITMAP_FILE_PTR bitmap, int cx, int cy, int mode);
int Scroll_Bitmap(BITMAP_IMAGE_PTR image, int dx, int dy = 0);

int Copy_Bitmap(BITMAP_IMAGE_PTR dest_bitmap, int dest_x, int dest_y,BITMAP_IMAGE_PTR source_bitmap, int source_x, int source_y,int width, int height);

int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height);

// λͼ�ļ�
int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename);
int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap);

// ͼ���豸�ӿ�GDI
int Draw_Text_GDI(char *text, int x, int y, COLORREF color, LPDIRECTDRAWSURFACE7 lpdds);
int Draw_Text_GDI(char *text, int x, int y, int color, LPDIRECTDRAWSURFACE7 lpdds);

// ��������
int Open_Error_File(char *filename, FILE *fp_override = NULL);
int Close_Error_File(void);
int Write_Error(char *string, ...);

// 2Dƽ����8λ��16λ�����γ���
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


// 2Dƽ����8λ��16λ���εĳ�����任
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

// ��ѧ����
int Fast_Distance_2D(int x, int y);
float Fast_Distance_3D(float x, float y, float z);

// ��ײ���
int Find_Bounding_Box_Poly2D(POLYGON2D_PTR poly,float &min_x, float &max_x,float &min_y, float &max_y);

int Mat_Mul_1X2_3X2(MATRIX1X2_PTR ma,MATRIX3X2_PTR mb,MATRIX1X2_PTR mprod);

int Mat_Mul_1X3_3X3(MATRIX1X3_PTR ma,MATRIX3X3_PTR mb,MATRIX1X3_PTR mprod);

int Mat_Mul_3X3(MATRIX3X3_PTR ma,MATRIX3X3_PTR mb,MATRIX3X3_PTR mprod);

inline int Mat_Init_3X2(MATRIX3X2_PTR ma,float m00, float m01,float m10, float m11,float m20, float m21);


// �ڴ����
inline void Mem_Set_WORD(void *dest, USHORT data, int count);
inline void Mem_Set_QUAD(void *dest, UINT   data, int count);

#endif