
#define WIN32_LEAN_AND_MEAN  

#include<tchar.h>
#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <sys/timeb.h>
#include <time.h>
#include <ddraw.h>  
#include "KeyBoardDevice.H"

extern HWND main_window_handle;
extern HINSTANCE mian_instance;

FILE *fp_error = NULL; 
char error_filename[80];              

LPDIRECTDRAW7 lpdd = NULL;
LPDIRECTDRAWSURFACE7 lpddsprimary = NULL;
LPDIRECTDRAWSURFACE7 lpddsback = NULL;
LPDIRECTDRAWPALETTE  lpddpal = NULL;  
LPDIRECTDRAWCLIPPER  lpddclipper = NULL;  
LPDIRECTDRAWCLIPPER  lpddclipperwin = NULL;

PALETTEENTRY palette[MAX_COLORS_PALETTE];
PALETTEENTRY save_palette[MAX_COLORS_PALETTE];
UCHAR                *primary_buffer = NULL;
UCHAR                *back_buffer = NULL;

DDSURFACEDESC2 ddsd;
DDSCAPS2             ddscaps;

int                  back_lpitch = 0;
BITMAP_FILE          bitmap8bit;           
BITMAP_FILE          bitmap16bit;
BITMAP_FILE          bitmap24bit;
int window_client_x0 = 0; 
int window_client_y0 = 0;

int screen_width = SCREEN_WIDTH, screen_height = SCREEN_HEIGHT, screen_bpp = SCREEN_BPP, screen_windowed = 0;
int min_clip_x = 0,max_clip_x = (SCREEN_WIDTH - 1),min_clip_y = 0,max_clip_y = (SCREEN_HEIGHT - 1);
int dd_pixel_format = DD_PIXEL_FORMAT565;
DWORD                start_clock_count = 0; 
int                  windowed_mode = FALSE;

USHORT(*RGB16Bit)(int r, int g, int b) = NULL;
USHORT RGB16Bit565(int r, int g, int b)
{
	r >>= 3; g >>= 2; b >>= 3;
	return(_RGB16BIT565((r), (g), (b)));

}

USHORT RGB16Bit555(int r, int g, int b)
{
	r >>= 3; g >>= 3; b >>= 3;
	return(_RGB16BIT555((r), (g), (b)));

}

int DDraw_Init(int width, int height, int bpp, int windowed)
{
	int index;
	if (FAILED(DirectDrawCreateEx(NULL, (void **)&lpdd, IID_IDirectDraw7, NULL)))
		return(0);
	if (windowed)
	{
		
		if (FAILED(lpdd->SetCooperativeLevel(main_window_handle, DDSCL_NORMAL)));
			return(0);

	}
	else
	{
		
		if (FAILED(lpdd->SetCooperativeLevel(main_window_handle,
			DDSCL_ALLOWMODEX | DDSCL_FULLSCREEN |
			DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT | DDSCL_MULTITHREADED)))
			return(0);

		if (FAILED(lpdd->SetDisplayMode(width, height, bpp, 0, 0)))
			return(0);

	}

	
	screen_height = height;
	screen_width = width;
	screen_bpp = bpp;
	screen_windowed = windowed;

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	if (!screen_windowed)
	{
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;

		ddsd.dwBackBufferCount = 1;
	} 
	else
	{
		
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		ddsd.dwBackBufferCount = 0;
	} 

	
	lpdd->CreateSurface(&ddsd, &lpddsprimary, NULL);

	DDPIXELFORMAT ddpf; 

	DDRAW_INIT_STRUCT(ddpf);

	lpddsprimary->GetPixelFormat(&ddpf);

	dd_pixel_format = ddpf.dwRGBBitCount;

	Write_Error("\npixel format = %d", dd_pixel_format);
	if (dd_pixel_format == DD_PIXEL_FORMAT555)
	{
		RGB16Bit = RGB16Bit555;
		Write_Error("\npixel format = 5.5.5");
	} 
	else
	{
		RGB16Bit = RGB16Bit565;
		Write_Error("\npixel format = 5.6.5");
	}

	if (!screen_windowed)
	{
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

		if (FAILED(lpddsprimary->GetAttachedSurface(&ddscaps, &lpddsback)))
			return(0);

	} 
	else
	{
		lpddsback = DDraw_Create_Surface(width, height); 

	} 

	if (screen_bpp == DD_PIXEL_FORMAT8)
	{
		memset(palette, 0, MAX_COLORS_PALETTE*sizeof(PALETTEENTRY));

		Load_Palette_From_File(DEFAULT_PALETTE_FILE, palette);

		if (screen_windowed)
		{
			for (index = 0; index < 10; index++)
				palette[index].peFlags = palette[index + 246].peFlags = PC_EXPLICIT;

			if (FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE,
				palette, &lpddpal, NULL)))
				return(0);
		}
		else
		{
			if (FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE | DDPCAPS_ALLOW256,
				palette, &lpddpal, NULL)))
				return(0);

		} 

		if (FAILED(lpddsprimary->SetPalette(lpddpal)))
			return(0);

	} 

	if (screen_windowed)
	{
		
		DDraw_Fill_Surface(lpddsback, 0);
	} 
	else
	{
		
		DDraw_Fill_Surface(lpddsprimary, 0);
		DDraw_Fill_Surface(lpddsback, 0);
	}

	
	min_clip_x = 0;
	max_clip_x = screen_width - 1;
	min_clip_y = 0;
	max_clip_y = screen_height - 1;
	RECT screen_rect = { 0, 0, screen_width, screen_height };
	lpddclipper = DDraw_Attach_Clipper(lpddsback, 1, &screen_rect);

	if (screen_windowed)
	{
		
		if (FAILED(lpdd->CreateClipper(0, &lpddclipperwin, NULL)))
			return(0);

		if (FAILED(lpddclipperwin->SetHWnd(0, main_window_handle)))
			return(0);

		if (FAILED(lpddsprimary->SetClipper(lpddclipperwin)))
			return(0);
	} 

	
	return(1);

}

int Write_Error(char *string, ...)
{
	
	char buffer[80]; 
	va_list arglist; 

	if (!string || !fp_error)
		return(0);

	va_start(arglist, string);
	vsprintf_s(buffer, string, arglist);
	va_end(arglist);

	fprintf(fp_error, buffer);

	fflush(fp_error);

	return(1);
} 
LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int width,int height,int mem_flags,USHORT color_key_value)
{
	
	DDSURFACEDESC2 ddsd;       
	LPDIRECTDRAWSURFACE7 lpdds; 

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	ddsd.dwWidth = width;
	ddsd.dwHeight = height;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;

	if (FAILED(lpdd->CreateSurface(&ddsd, &lpdds, NULL)))
		return(NULL);

	DDCOLORKEY color_key; 
	color_key.dwColorSpaceLowValue = color_key_value;
	color_key.dwColorSpaceHighValue = color_key_value;

	lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key);

	return(lpdds);
}
int Load_Palette_From_File(char *filename, LPPALETTEENTRY palette)
{

	FILE *fp_file;

	if ((fp_file = fopen(filename, "r")) == NULL)
		return(0);

	for (int index = 0; index<MAX_COLORS_PALETTE; index++)
	{
		fscanf(fp_file, "%d %d %d %d", &palette[index].peRed,
			&palette[index].peGreen,
			&palette[index].peBlue,
			&palette[index].peFlags);
	} 

	fclose(fp_file);
	return(1);
}

int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds, USHORT color, RECT *client)
{
	DDBLTFX ddbltfx; 

	DDRAW_INIT_STRUCT(ddbltfx);

	ddbltfx.dwFillColor = color;

	lpdds->Blt(client,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);  

	return(1);
}
LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds,int num_rects,LPRECT clip_list)

{
	int index;                         
	LPDIRECTDRAWCLIPPER lpddclipper;  
	LPRGNDATA region_data;            
	
	if (FAILED(lpdd->CreateClipper(0, &lpddclipper, NULL)))
		return(NULL);

	region_data = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)+num_rects*sizeof(RECT));

	memcpy(region_data->Buffer, clip_list, sizeof(RECT)*num_rects);

	region_data->rdh.dwSize = sizeof(RGNDATAHEADER);
	region_data->rdh.iType = RDH_RECTANGLES;
	region_data->rdh.nCount = num_rects;
	region_data->rdh.nRgnSize = num_rects*sizeof(RECT);

	region_data->rdh.rcBound.left = 64000;
	region_data->rdh.rcBound.top = 64000;
	region_data->rdh.rcBound.right = -64000;
	region_data->rdh.rcBound.bottom = -64000;

	for (index = 0; index<num_rects; index++)
	{
		
		if (clip_list[index].left < region_data->rdh.rcBound.left)
			region_data->rdh.rcBound.left = clip_list[index].left;

		if (clip_list[index].right > region_data->rdh.rcBound.right)
			region_data->rdh.rcBound.right = clip_list[index].right;

		if (clip_list[index].top < region_data->rdh.rcBound.top)
			region_data->rdh.rcBound.top = clip_list[index].top;

		if (clip_list[index].bottom > region_data->rdh.rcBound.bottom)
			region_data->rdh.rcBound.bottom = clip_list[index].bottom;

	} 
	if (FAILED(lpddclipper->SetClipList(region_data, 0)))
	{
		
		free(region_data);
		return(NULL);
	} 

	if (FAILED(lpdds->SetClipper(lpddclipper)))
	{
		
		free(region_data);
		return(NULL);
	} 

	free(region_data);
	return(lpddclipper);

}
int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename)
{
	
	int file_handle,index; 

	UCHAR   *temp_buffer = NULL; 
	OFSTRUCT file_data;         

	if ((file_handle = OpenFile(filename, &file_data, OF_READ)) == -1)
		return(0);

	_lread(file_handle, &bitmap->bitmapfileheader, sizeof(BITMAPFILEHEADER));

	if (bitmap->bitmapfileheader.bfType != BITMAP_ID)
	{
		
		_lclose(file_handle);

		return(0);
	}

	_lread(file_handle, &bitmap->bitmapinfoheader, sizeof(BITMAPINFOHEADER));

	if (bitmap->bitmapinfoheader.biBitCount == 8)
	{
		_lread(file_handle, &bitmap->palette, MAX_COLORS_PALETTE*sizeof(PALETTEENTRY));

		for (index = 0; index < MAX_COLORS_PALETTE; index++)
		{
			int temp_color = bitmap->palette[index].peRed;
			bitmap->palette[index].peRed = bitmap->palette[index].peBlue;
			bitmap->palette[index].peBlue = temp_color;
			bitmap->palette[index].peFlags = PC_NOCOLLAPSE;
		} 

	} 

	SetFilePointer((HANDLE)file_handle, -(int)(bitmap->bitmapinfoheader.biSizeImage), NULL, FILE_BEGIN);

	if (bitmap->bitmapinfoheader.biBitCount == 8 || bitmap->bitmapinfoheader.biBitCount == 16)
	{
		if (bitmap->buffer)
			free(bitmap->buffer);

		if (!(bitmap->buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage)))
		{
			
			_lclose(file_handle);
			return(0);
		}

		_lread(file_handle, bitmap->buffer, bitmap->bitmapinfoheader.biSizeImage);

	} 
	else
	if (bitmap->bitmapinfoheader.biBitCount == 24)
	{
		if (!(temp_buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage)))
		{
			
			_lclose(file_handle);
			return(0);
		}
		if (!(bitmap->buffer = (UCHAR *)malloc(2 * bitmap->bitmapinfoheader.biWidth*bitmap->bitmapinfoheader.biHeight)))
		{
			_lclose(file_handle);
			free(temp_buffer);
			return(0);
		} 
		_lread(file_handle, temp_buffer, bitmap->bitmapinfoheader.biSizeImage);
		for (index = 0; index < bitmap->bitmapinfoheader.biWidth*bitmap->bitmapinfoheader.biHeight; index++)
		{
			
			USHORT color;
			if (dd_pixel_format == DD_PIXEL_FORMAT555)
			{
				UCHAR blue = (temp_buffer[index * 3 + 0] >> 3),
					green = (temp_buffer[index * 3 + 1] >> 3),
					red = (temp_buffer[index * 3 + 2] >> 3);
					color = _RGB16BIT555(red, green, blue);
			} 
			else
			if (dd_pixel_format == DD_PIXEL_FORMAT565)
			{
				
				UCHAR blue = (temp_buffer[index * 3 + 0] >> 3),
					green = (temp_buffer[index * 3 + 1] >> 2),
					red = (temp_buffer[index * 3 + 2] >> 3);
					color = _RGB16BIT565(red, green, blue);

			} 

			((USHORT *)bitmap->buffer)[index] = color;

		} 
		bitmap->bitmapinfoheader.biBitCount = 16;
		free(temp_buffer);

	} 
	else
	{
		return(0);

	}

#if 0
	// write the file info out 
	printf("\nfilename:%s \nsize=%d \nwidth=%d \nheight=%d \nbitsperpixel=%d \ncolors=%d \nimpcolors=%d",
		filename,
		bitmap->bitmapinfoheader.biSizeImage,
		bitmap->bitmapinfoheader.biWidth,
		bitmap->bitmapinfoheader.biHeight,
		bitmap->bitmapinfoheader.biBitCount,
		bitmap->bitmapinfoheader.biClrUsed,
		bitmap->bitmapinfoheader.biClrImportant);
#endif
	_lclose(file_handle);
	Flip_Bitmap(bitmap->buffer,
		bitmap->bitmapinfoheader.biWidth*(bitmap->bitmapinfoheader.biBitCount / 8),
		bitmap->bitmapinfoheader.biHeight);
	return(1);

}
int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height)
{
	UCHAR *buffer;
	int index;   
	if (!(buffer = (UCHAR *)malloc(bytes_per_line*height)))
		return(0);
	memcpy(buffer, image, bytes_per_line*height);

	for (index = 0; index < height; index++)
		memcpy(&image[((height - 1) - index)*bytes_per_line],
		&buffer[index*bytes_per_line], bytes_per_line);
	free(buffer);
	return(1);

}
int Set_Palette(LPPALETTEENTRY set_palette)
{
	memcpy(palette, set_palette, MAX_COLORS_PALETTE*sizeof(PALETTEENTRY));
	lpddpal->SetEntries(0, 0, MAX_COLORS_PALETTE, palette);
	return(1);
}
int Create_Bitmap(BITMAP_IMAGE_PTR image, int x, int y, int width, int height, int bpp)
{
	if (!(image->buffer = (UCHAR *)malloc(width*height*(bpp >> 3))))
		return(0);
	image->state = BITMAP_STATE_ALIVE;
	image->attr = 0;
	image->width = width;
	image->height = height;
	image->bpp = bpp;
	image->x = x;
	image->y = y;
	image->num_bytes = width*height*(bpp >> 3);
	memset(image->buffer, 0, width*height*(bpp >> 3));
	return(1);

}
int Load_Image_Bitmap(BITMAP_IMAGE_PTR image,BITMAP_FILE_PTR bitmap,int cx, int cy,int mode) 
	
{
	if (!image)
		return(0);

	UCHAR *source_ptr,  
		*dest_ptr;
	if (mode == BITMAP_EXTRACT_MODE_CELL)
	{
		cx = cx*(image->width + 1) + 1;
		cy = cy*(image->height + 1) + 1;
	} 
	source_ptr = bitmap->buffer +
		cy*bitmap->bitmapinfoheader.biWidth + cx;
	dest_ptr = (UCHAR *)image->buffer;

	for (int index_y = 0; index_y<image->height; index_y++)
	{
		
		memcpy(dest_ptr, source_ptr, image->width);
		dest_ptr += image->width;
		source_ptr += bitmap->bitmapinfoheader.biWidth;
	} 

	image->attr |= BITMAP_ATTR_LOADED;
	return(1);

}
int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap)
{
	if (bitmap->buffer)
	{
		free(bitmap->buffer);
		bitmap->buffer = NULL;

	} 
	return(1);

}
int Create_BOB(BOB_PTR bob,int x, int y,int width, int height,int num_frames,int attr,int mem_flags,USHORT color_key_value,int bpp)

{
	DDSURFACEDESC2 ddsd; 
	int index; 

	bob->state = BOB_STATE_ALIVE;
	bob->attr = attr;
	bob->anim_state = 0;
	bob->counter_1 = 0;
	bob->counter_2 = 0;
	bob->max_count_1 = 0;
	bob->max_count_2 = 0;

	bob->curr_frame = 0;
	bob->num_frames = num_frames;
	bob->bpp = bpp;
	bob->curr_animation = 0;
	bob->anim_counter = 0;
	bob->anim_index = 0;
	bob->anim_count_max = 0;
	bob->x = x;
	bob->y = y;
	bob->xv = 0;
	bob->yv = 0;

	bob->width = width;
	bob->height = height;

	for (index = 0; index<MAX_BOB_FRAMES; index++)
		bob->images[index] = NULL;

	for (index = 0; index<MAX_BOB_ANIMATIONS; index++)
		bob->animations[index] = NULL;

#if 0
	// make sure surface width is a multiple of 8, some old version of dd like that
	// now, it's unneeded...
	bob->width_fill = ((width % 8 != 0) ? (8 - width % 8) : 0);
	Write_Error("\nCreate BOB: width_fill=%d", bob->width_fill);
#endif

	for (index = 0; index<bob->num_frames; index++)
	{
		memset(&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

		ddsd.dwWidth = bob->width + bob->width_fill;
		ddsd.dwHeight = bob->height;

		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;

		if (FAILED(lpdd->CreateSurface(&ddsd, &(bob->images[index]), NULL)))
			return(0);

		DDCOLORKEY color_key; 
		color_key.dwColorSpaceLowValue = color_key_value;
		color_key.dwColorSpaceHighValue = color_key_value;

		(bob->images[index])->SetColorKey(DDCKEY_SRCBLT, &color_key);

	} 

	return(1);

}
int Load_Frame_BOB(BOB_PTR bob,BITMAP_FILE_PTR bitmap,int frame,int cx, int cy,int mode)  
	
{
	DDSURFACEDESC2 ddsd; 

	if (!bob)
		return(0);

	UCHAR *source_ptr,  
		*dest_ptr;

	if (mode == BITMAP_EXTRACT_MODE_CELL)
	{
		
		cx = cx*(bob->width + 1) + 1;
		cy = cy*(bob->height + 1) + 1;
	}

	source_ptr = bitmap->buffer + cy*bitmap->bitmapinfoheader.biWidth + cx;
	ddsd.dwSize = sizeof(ddsd);
	(bob->images[frame])->Lock(NULL,
		&ddsd,
		DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
		NULL);
	dest_ptr = (UCHAR *)ddsd.lpSurface;
	for (int index_y = 0; index_y<bob->height; index_y++)
	{
	
		memcpy(dest_ptr, source_ptr, bob->width);
		dest_ptr += (ddsd.lPitch); 
		source_ptr += bitmap->bitmapinfoheader.biWidth;
	} 

	
	(bob->images[frame])->Unlock(NULL);

	bob->attr |= BOB_ATTR_LOADED;

	return(1);

}
int Load_Animation_BOB(BOB_PTR bob,int anim_index,int num_frames,int *sequence)
{
	if (!bob)
		return(0);
	if (!(bob->animations[anim_index] = (int *)malloc((num_frames + 1)*sizeof(int))))
		return(0);
	int index;
	for (index = 0; index<num_frames; index++)
		bob->animations[anim_index][index] = sequence[index];
	bob->animations[anim_index][index] = -1;
	return(1);

}
int Set_Animation_BOB(BOB_PTR bob, int anim_index)
{
	if (!bob)
		return(0);
	bob->curr_animation = anim_index;
	bob->anim_index = 0;
	return(1);

}
int Set_Anim_Speed_BOB(BOB_PTR bob, int speed)
{
	if (!bob)
		return(0);
	bob->anim_count_max = speed;
	return(1);

}
int Set_Vel_BOB(BOB_PTR bob, int xv, int yv)
{
	if (!bob)
		return(0);
	bob->xv = xv;
	bob->yv = yv;
	return(1);
}
int Set_Pos_BOB(BOB_PTR bob, int x, int y)
{
	if (!bob)
		return(0);
	bob->x = x;
	bob->y = y;
	return(1);
}
int Destroy_Bitmap(BITMAP_IMAGE_PTR image)
{
	if (image && image->buffer)
	{
		
		free(image->buffer);
		memset(image, 0, sizeof(BITMAP_IMAGE));
		return(1);

	} 
	else  
		return(0);

}
int Destroy_BOB(BOB_PTR bob)
{
	int index;
	if (!bob)
		return(0);

	if (bob->attr && BOB_ATTR_CLONE)
	{
		
		for (index = 0; index<MAX_BOB_FRAMES; index++)
		if (bob->images[index])
			bob->images[index] = NULL;

		for (index = 0; index<MAX_BOB_ANIMATIONS; index++)
		if (bob->animations[index])
			bob->animations[index] = NULL;

	} 
	else
	{
		
		for (index = 0; index<MAX_BOB_FRAMES; index++)
		if (bob->images[index])
			(bob->images[index])->Release();

		for (index = 0; index<MAX_BOB_ANIMATIONS; index++)
		if (bob->animations[index])
			free(bob->animations[index]);

	} 
	return(1);

}
int DDraw_Shutdown(void)
{
	if (lpddclipper)
		lpddclipper->Release();

	if (lpddclipperwin)
		lpddclipperwin->Release();
	if (lpddpal)
		lpddpal->Release();
	if (lpddsback)
		lpddsback->Release();
	if (lpddsprimary)
		lpddsprimary->Release();
	if (lpdd)
		lpdd->Release();
	return(1);
}
DWORD Start_Clock(void)
{
	return(start_clock_count = Get_Clock());

}
DWORD Get_Clock(void)
{
	
	return(GetTickCount());

}
UCHAR *DDraw_Lock_Back_Surface(void)
{
	if (back_buffer)
	{
		
		return(back_buffer);
	} 
	DDRAW_INIT_STRUCT(ddsd);
	lpddsback->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

	
	back_buffer = (UCHAR *)ddsd.lpSurface;
	back_lpitch = ddsd.lPitch;

	return(back_buffer);

} 


int DDraw_Unlock_Back_Surface(void)
{
	
	if (!back_buffer)
		return(0);

	lpddsback->Unlock(NULL);
	back_buffer = NULL;
	back_lpitch = 0;

	
	return(1);
} 
int Draw_Bitmap(BITMAP_IMAGE_PTR source_bitmap, UCHAR *dest_buffer, int lpitch, int transparent)
{
	if (!(source_bitmap->attr & BITMAP_ATTR_LOADED))
		return(0);

	UCHAR *dest_addr,  
		*source_addr; 

	UCHAR pixel;     

	int index,        
		pixel_x;


	dest_addr = dest_buffer + source_bitmap->y*lpitch + source_bitmap->x;
	source_addr = source_bitmap->buffer;

	if (transparent)
	{
		
		for (index = 0; index<source_bitmap->height; index++)
		{
			for (pixel_x = 0; pixel_x<source_bitmap->width; pixel_x++)
			{
				if ((pixel = source_addr[pixel_x]) != 0)
					dest_addr[pixel_x] = pixel;

			} 

			dest_addr += lpitch;
			source_addr += source_bitmap->width;

		} 
	}
	else
	{
		for (index = 0; index < source_bitmap->height; index++)
		{
			
			memcpy(dest_addr, source_addr, source_bitmap->width);
			dest_addr += lpitch;
			source_addr += source_bitmap->width;

		} 

	} 
	return(1);

}
int Color_Scan(int x1, int y1, int x2, int y2,UCHAR scan_start, UCHAR scan_end,UCHAR *scan_buffer, int scan_lpitch)
{
	
	if (x1 >= screen_width)
		x1 = screen_width - 1;
	else
	if (x1 < 0)
		x1 = 0;

	if (x2 >= screen_width)
		x2 = screen_width - 1;
	else
	if (x2 < 0)
		x2 = 0;

	if (y1 >= screen_height)
		y1 = screen_height - 1;
	else
	if (y1 < 0)
		y1 = 0;

	if (y2 >= screen_height)
		y2 = screen_height - 1;
	else
	if (y2 < 0)
		y2 = 0;

	scan_buffer += y1*scan_lpitch;

	for (int scan_y = y1; scan_y <= y2; scan_y++)
	{
		for (int scan_x = x1; scan_x <= x2; scan_x++)
		{
			if (scan_buffer[scan_x] >= scan_start && scan_buffer[scan_x] <= scan_end)
				return(1);
		} 
		scan_buffer += scan_lpitch;

	} 
	return(0);

}
int Draw_BOB(BOB_PTR bob,LPDIRECTDRAWSURFACE7 dest) // surface to draw the bob on
{
	
	RECT dest_rect,source_rect;                    


	if (!bob)
		return(0);

	
	if (!(bob->attr & BOB_ATTR_VISIBLE))
		return(1);

	
	dest_rect.left = bob->x;
	dest_rect.top = bob->y;
	dest_rect.right = bob->x + bob->width;
	dest_rect.bottom = bob->y + bob->height;

	source_rect.left = 0;
	source_rect.top = 0;
	source_rect.right = bob->width;
	source_rect.bottom = bob->height;

	if (FAILED(dest->Blt(&dest_rect, bob->images[bob->curr_frame],
		&source_rect, (DDBLT_WAIT | DDBLT_KEYSRC),
		NULL)))
		return(0);

	
	return(1);
}
int Set_Palette_Entry(int color_index, LPPALETTEENTRY color)
{
	lpddpal->SetEntries(0, color_index, 1, color);

	memcpy(&palette[color_index], color, sizeof(PALETTEENTRY));

	return(1);
}
int Draw_Text_GDI(char *text, int x, int y, int color, LPDIRECTDRAWSURFACE7 lpdds)
{
	
	HDC xdc; 
	LPCWSTR Ltext = NULL;
	
	if (FAILED(lpdds->GetDC(&xdc)))
		return(0);

	SetTextColor(xdc, RGB(palette[color].peRed, palette[color].peGreen, palette[color].peBlue));

	SetBkMode(xdc, TRANSPARENT);

	TextOut(xdc, x, y, _T(text), strlen(text));

	lpdds->ReleaseDC(xdc);

	return(1);
}
int Draw_Text_GDI(char *text, int x, int y, COLORREF color, LPDIRECTDRAWSURFACE7 lpdds)
{
	
	HDC xdc; 
	LPCWSTR Ltext = NULL;
	
	if (FAILED(lpdds->GetDC(&xdc)))
		return(0);

	
	SetTextColor(xdc, color);

	SetBkMode(xdc, TRANSPARENT);

	TextOut(xdc, x, y, _T(text), strlen(text));

	lpdds->ReleaseDC(xdc);

	return(1);
}
int DDraw_Flip(void)
{

	if (primary_buffer || back_buffer)
		return(0);

	if (!screen_windowed)
	while (FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)));
	else
	{
		RECT    dest_rect;  

		GetWindowRect(main_window_handle, &dest_rect);

		dest_rect.left += window_client_x0;
		dest_rect.top += window_client_y0;

		dest_rect.right = dest_rect.left + screen_width;
		dest_rect.bottom = dest_rect.top + screen_height;

		if (FAILED(lpddsprimary->Blt(&dest_rect, lpddsback, NULL, DDBLT_WAIT, NULL)))
			return(0);

	} 

	return(1);

}
DWORD Wait_Clock(DWORD count)
{
	
	while ((Get_Clock() - start_clock_count) < count);
	return(Get_Clock());

}
int Animate_BOB(BOB_PTR bob)
{
	if (!bob)
		return(0);

	if (bob->attr & BOB_ATTR_SINGLE_FRAME)
	{
		bob->curr_frame = 0;
		return(1);
	} 
	else
	if (bob->attr & BOB_ATTR_MULTI_FRAME)
	{
		if (++bob->anim_counter >= bob->anim_count_max)
		{
			
			bob->anim_counter = 0;

			if (++bob->curr_frame >= bob->num_frames)
				bob->curr_frame = 0;

		} 

	} 
	else
	if (bob->attr & BOB_ATTR_MULTI_ANIM)
	{
		if (++bob->anim_counter >= bob->anim_count_max)
		{
			
			bob->anim_counter = 0;

			
			bob->anim_index++;

			bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];

			if (bob->curr_frame == -1)
			{
				if (bob->attr & BOB_ATTR_ANIM_ONE_SHOT)
				{
					
					bob->anim_state = BOB_STATE_ANIM_DONE;

					bob->anim_index--;

					bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];

				}
				else
				{
					bob->anim_index = 0;

					bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];
				} 

			} 

		} 

	} 

	return(1);

}