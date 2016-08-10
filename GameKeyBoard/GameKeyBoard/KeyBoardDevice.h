#ifndef KeyBoardDevice
#define KeyBoardDevice

#define SCREEN_WIDTH        640  
#define SCREEN_HEIGHT       480
#define SCREEN_BPP          8   
#define MAX_COLORS_PALETTE  256
#define DEFAULT_PALETTE_FILE "PALDATA2.PAL"

#define DD_PIXEL_FORMAT8        8
#define DD_PIXEL_FORMAT555      15
#define DD_PIXEL_FORMAT565      16
#define DD_PIXEL_FORMAT888      24
#define DD_PIXEL_FORMATALPHA888 32 

// bitmap defines
#define BITMAP_ID            0x4D42 
#define BITMAP_STATE_DEAD    0
#define BITMAP_STATE_ALIVE   1
#define BITMAP_STATE_DYING   2 
#define BITMAP_ATTR_LOADED   128

#define BITMAP_EXTRACT_MODE_CELL  0
#define BITMAP_EXTRACT_MODE_ABS   1

#define MAX_BOB_ANIMATIONS 16
#define MAX_BOB_FRAMES 64
// defines for BOBs
#define BOB_STATE_DEAD         0    // this is a dead bob
#define BOB_STATE_ALIVE        1    // this is a live bob
#define BOB_STATE_DYING        2    // this bob is dying
#define BOB_STATE_ANIM_DONE    1    // done animation state
#define MAX_BOB_FRAMES         64   // maximum number of bob frames
#define MAX_BOB_ANIMATIONS     16   // maximum number of animation sequeces

#define BOB_ATTR_SINGLE_FRAME   1   // bob has single frame
#define BOB_ATTR_MULTI_FRAME    2   // bob has multiple frames
#define BOB_ATTR_MULTI_ANIM     4   // bob has multiple animations
#define BOB_ATTR_ANIM_ONE_SHOT  8   // bob will perform the animation once
#define BOB_ATTR_VISIBLE        16  // bob is visible
#define BOB_ATTR_BOUNCE         32  // bob bounces off edges
#define BOB_ATTR_WRAPAROUND     64  // bob wraps around edges
#define BOB_ATTR_LOADED         128 // the bob has been loaded
#define BOB_ATTR_CLONE          256 // the bob is a clone
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;
typedef unsigned int   QUAD;
typedef unsigned int   UINT;
////////////////////////////////////////////////////////////////////////////////////////////////////
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define _RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))
#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))
#define _RGB24BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) )
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER bitmapfileheader; 
	BITMAPINFOHEADER bitmapinfoheader; 
	PALETTEENTRY     palette[256];      
	UCHAR            *buffer;         

} BITMAP_FILE, *BITMAP_FILE_PTR;
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


typedef struct BLINKER_TYP
{
	// user sets these
	int color_index;         
	PALETTEENTRY on_color;   
	PALETTEENTRY off_color;  
	int on_time;             
	int off_time;           

	// internal member
	int counter;            
	int state;               
} BLINKER, *BLINKER_PTR;
typedef struct BOB_TYP
{
	int state;         
	int anim_state;    
	int attr;           
	float x, y;           
	float xv, yv;          
	int width, height;  
	int width_fill;     
	int bpp;           
	int counter_1;     
	int counter_2;
	int max_count_1;   
	int max_count_2;
	int varsI[16];      
	float varsF[16];  
	int curr_frame;    
	int num_frames;    
	int curr_animation; 
	int anim_counter;  
	int anim_index;   
	int anim_count_max;
	int *animations[MAX_BOB_ANIMATIONS]; 

	LPDIRECTDRAWSURFACE7 images[MAX_BOB_FRAMES]; 

} BOB, *BOB_PTR;







////////////////////////////////////////////////////////////////////////////////////////////////////
// DirectDraw functions
int DDraw_Init(int width, int height, int bpp, int windowed = 0);
int DDraw_Shutdown(void);
LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds, int num_rects, LPRECT clip_list);
LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int width, int height, int mem_flags = 0, USHORT color_key_value = 0);

int DDraw_Flip(void);
int DDraw_Wait_For_Vsync(void);
int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds, USHORT color, RECT *client = NULL);
UCHAR *DDraw_Lock_Surface(LPDIRECTDRAWSURFACE7 lpdds, int *lpitch);
int DDraw_Unlock_Surface(LPDIRECTDRAWSURFACE7 lpdds);
UCHAR *DDraw_Lock_Primary_Surface(void);
int DDraw_Unlock_Primary_Surface(void);
UCHAR *DDraw_Lock_Back_Surface(void);
int DDraw_Unlock_Back_Surface(void);
// error functions
int Open_Error_File(char *filename, FILE *fp_override = NULL);
int Close_Error_File(void);
int Write_Error(char *string, ...);
// palette functions
int Load_Palette_From_File(char *filename, LPPALETTEENTRY palette);
int Set_Palette_Entry(int color_index, LPPALETTEENTRY color);
int Get_Palette_Entry(int color_index, LPPALETTEENTRY color);
int Load_Palette_From_File(char *filename, LPPALETTEENTRY palette);
int Save_Palette_To_File(char *filename, LPPALETTEENTRY palette);
int Save_Palette(LPPALETTEENTRY sav_palette);
int Set_Palette(LPPALETTEENTRY set_palette);
int Rotate_Colors(int start_index, int end_index);
int Blink_Colors(int command, BLINKER_PTR new_light, int id);
// simple bitmap image functions
int Create_Bitmap(BITMAP_IMAGE_PTR image, int x, int y, int width, int height, int bpp = 8);
int Destroy_Bitmap(BITMAP_IMAGE_PTR image);
int Draw_Bitmap(BITMAP_IMAGE_PTR source_bitmap, UCHAR *dest_buffer, int lpitch, int transparent);
int Draw_Bitmap16(BITMAP_IMAGE_PTR source_bitmap, UCHAR *dest_buffer, int lpitch, int transparent);
int Load_Image_Bitmap(BITMAP_IMAGE_PTR image, BITMAP_FILE_PTR bitmap, int cx, int cy, int mode);
int Load_Image_Bitmap16(BITMAP_IMAGE_PTR image, BITMAP_FILE_PTR bitmap, int cx, int cy, int mode);
int Scroll_Bitmap(BITMAP_IMAGE_PTR image, int dx, int dy = 0);

int Copy_Bitmap(BITMAP_IMAGE_PTR dest_bitmap, int dest_x, int dest_y,
	BITMAP_IMAGE_PTR source_bitmap, int source_x, int source_y,
	int width, int height);

int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height);

// bitmap file functions
int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename);
int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap);
// BOB functions
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
// general utility functions
DWORD Get_Clock(void);
DWORD Start_Clock(void);
DWORD Wait_Clock(DWORD count);
int Collision_Test(int x1, int y1, int w1, int h1,int x2, int y2, int w2, int h2);

int Color_Scan(int x1, int y1, int x2, int y2,UCHAR scan_start, UCHAR scan_end,UCHAR *scan_buffer, int scan_lpitch);

int Color_Scan16(int x1, int y1, int x2, int y2,USHORT scan_start, USHORT scan_end,UCHAR *scan_buffer, int scan_lpitch);
// gdi functions
int Draw_Text_GDI(char *text, int x, int y, COLORREF color, LPDIRECTDRAWSURFACE7 lpdds);
int Draw_Text_GDI(char *text, int x, int y, int color, LPDIRECTDRAWSURFACE7 lpdds);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern BITMAP_FILE          bitmap8bit;
extern BITMAP_FILE          bitmap16bit;
extern BITMAP_FILE          bitmap24bit;
extern int screen_width, screen_height, screen_bpp, screen_windowed;
extern LPDIRECTDRAW7        lpdd;
extern LPDIRECTDRAWSURFACE7 lpddsprimary;
extern LPDIRECTDRAWSURFACE7 lpddsback;
extern LPDIRECTDRAWPALETTE  lpddpal;
extern LPDIRECTDRAWCLIPPER  lpddclipper;
extern LPDIRECTDRAWCLIPPER  lpddclipperwin;
extern PALETTEENTRY         palette[256];
extern PALETTEENTRY         save_palette[256];
extern int                  back_lpitch;
extern BITMAP_FILE          bitmap8bit;
extern BITMAP_FILE          bitmap16bit;
extern BITMAP_FILE          bitmap24bit;
extern DWORD                start_clock_count;
extern int                  windowed_mode;
extern UCHAR                *primary_buffer;
extern UCHAR                *back_buffer;
extern int window_client_x0;
extern int window_client_y0;

#endif