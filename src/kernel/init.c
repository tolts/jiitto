/*
    Copyright (C) 2023  schochtlts

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "algo/char.h"
#include "algo/conversion.h"

#include "drivers/drivers.h"
#include "drivers/vga.h"

#include "int/int.h"
#include "int/idt.h"
#include "int/pic.h"
#include "int/isr.h"

#include "drivers/keyboard.h"

// File name : fpMinecraft.c
// Fixed point version (10 bit precision for frac part)

// Typedefs
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
//typedef unsigned long uint64_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
//typedef long int64_t;

// Constants
#define PREC  1024
#define PI    3217 // CONSTANTS
#define HF_PI 1608 // CONSTANTS

#define MOVE_SPEED 204	// CONSTANTS 0.2
#define ROT_SPEED  204	// CONSTANTS 0.2

#define SCALE_COEF 32	// CONSTANTS 0.0312
#define DIST_COEF  1024	// CONSTANTS 1.0

#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 25
#define HF_SCREEN_WIDTH  32768  // CONSTANTS 32.0
#define HF_SCREEN_HEIGHT 12800  // CONSTANTS 12.5
#define OFFSET_X 0
#define OFFSET_Y 0

#define MAX_DIST  1024	// CONSTANTS 1.0
#define MAX_COUNT 10

#define WORLD_X 10
#define WORLD_Y 10
#define WORLD_Z 10
#define WORLD_YxWORLD_X 100
#define WORLD_SIZE 1000

// Variables
int32_t CamPosX = 1536 /*1.5*/, CamPosY = 512 /*0.5*/, CamPosZ = 0; // CONSTANTS
int32_t CamRotX = 0, CamRotY = 0;

int32_t CamFoX = 0, CamFoY = 0, CamFoZ = PREC;
int32_t CamRiX = PREC, CamRiY = 0, CamRiZ = 0;
int32_t CamUpX = 0, CamUpY = PREC, CamUpZ = 0;

int32_t s, c;

int32_t PntPosX, PntPosY;

int32_t RayPosX, RayPosY, RayPosZ;
int32_t RayDirX, RayDirY, RayDirZ;
int32_t RayLen;

int32_t LengthX, LengthY, LengthZ;
int32_t StepSizeX, StepSizeY, StepSizeZ;

int32_t MapPosX, MapPosY, MapPosZ;
int8_t StepX, StepY, StepZ;

int32_t Dist, SafeCounter;

int32_t Index;
uint8_t World[WORLD_SIZE];

uint8_t Face;
int32_t SampleX, SampleY;

uint32_t SideTexture[2] =   { 0xffff7e3c, 0x00089aff };
uint32_t TopTexture[2] =    { 0x04261000, 0x00226010 };
uint32_t BottomTexture[2] = { 0x7eff7e24, 0x247eff7e };
uint32_t *CurrentTexture;
int32_t TextureX, TextureY;

uint8_t KeyDown;
int8_t *vram;

int32_t a, i;
int32_t n3, n5, n7, tmp;

// Functions
static int32_t f_mul(int32_t, int32_t);
static int32_t f_div(int32_t, int32_t);
static int32_t f_sqrt(int32_t);
static int32_t f_sin(int32_t);
static int32_t f_cos(int32_t);

static void Ray(void);
static void CamControls(void);
static void CamOrientation(void);
static void Render(void);
static void Minecraft(void);

static int32_t f_mul(int32_t a, int32_t b)
{
	return a * b / PREC;
}

static int32_t f_div(int32_t a, int32_t b)
{
	return a * PREC / b;
}

static int32_t f_sqrt(int32_t n)
{
	if(n <= 0) return 0;
	
	if(n > PREC)
	{
		a = PREC * 4;
	}
	else
	{
		a = PREC / 2;
	}
	
	for(i=0; i<10; i++)
	{
		a = (a + f_div(n, a)) / 2;
	}
	
	return a;
}

static int32_t f_sin(int32_t n)
{	
	if(n < -HF_PI){ n = -PI - n; }
	else if(n > HF_PI){ n = PI - n; }
	
	tmp = n * n / PREC;
	n3 = n * tmp / PREC;
	n5 = n3 * tmp / PREC;
	n7 = n5 * tmp / PREC;
	
	n3 /= -6;
	n5 /= 120;
	n7 /= -5040;
	
	return n + n3 + n5 + n7;
}

static int32_t f_cos(int32_t n)
{	
	n += HF_PI;
	if(n > PI){ n = -2 * PI + n; }
	
	if(n < -HF_PI){ n = -PI - n; }
	else if(n > HF_PI){ n = PI - n; }
	
	tmp = n * n / PREC;
	n3 = n * tmp / PREC;
	n5 = n3 * tmp / PREC;
	n7 = n5 * tmp / PREC;
	
	n3 /= -6;
	n5 /= 120;
	n7 /= -5040;
	
	return n + n3 + n5 + n7;
}

static void Ray(void)
{	
	RayPosX = CamPosX;
	RayPosY = CamPosY;
	RayPosZ = CamPosZ;
	
	RayDirX = CamFoX;
	RayDirY = CamFoY;
	RayDirZ = CamFoZ;
	
	LengthX = f_div(RayDirY, RayDirX);
	LengthY = f_div(RayDirZ, RayDirX);
	StepSizeX = f_sqrt(f_mul(LengthX, LengthX) + f_mul(LengthY, LengthY) + PREC);
	
	LengthX = f_div(RayDirX, RayDirY);
	LengthY = f_div(RayDirZ, RayDirY);
	StepSizeY = f_sqrt(f_mul(LengthX, LengthX) + f_mul(LengthY, LengthY) + PREC);
	
	LengthX = f_div(RayDirX, RayDirZ);
	LengthY = f_div(RayDirY, RayDirZ);
	StepSizeZ = f_sqrt(f_mul(LengthX, LengthX) + f_mul(LengthY, LengthY) + PREC);
	
	MapPosX = RayPosX / PREC;
	MapPosY = RayPosY / PREC;
	MapPosZ = RayPosZ / PREC;
	
	if(RayDirX > 0)
	{
		StepX = 1;
		LengthX = f_mul(MapPosX * PREC + PREC - RayPosX, StepSizeX);
	}
	else
	{
		StepX = -1;
		LengthX = f_mul(RayPosX - MapPosX * PREC, StepSizeX);
	}
	
	if(RayDirY > 0)
	{
		StepY = 1;
		LengthY = f_mul(MapPosY * PREC + PREC - RayPosY, StepSizeY);
	}
	else
	{
		StepY = -1;
		LengthY = f_mul(RayPosY - MapPosY * PREC, StepSizeY);
	}
	
	if(RayDirZ > 0)
	{
		StepZ = 1;
		LengthZ = f_mul(MapPosZ * PREC + PREC - RayPosZ, StepSizeZ);
	}
	else
	{
		StepZ = -1;
		LengthZ = f_mul(RayPosZ - MapPosZ * PREC, StepSizeZ);
	}
	
	Dist = 0;
	SafeCounter = 0;
	
	while(Dist < MAX_DIST && SafeCounter < MAX_COUNT)
	{
		switch((LengthY < LengthX && LengthY < LengthZ) + ((LengthZ < LengthX && LengthZ < LengthY) * 2))
		{
			case 0:
				MapPosX += StepX;
				Dist = LengthX;
				LengthX += StepSizeX;
				Face = 0;
				break;
				
			case 1:
				MapPosY += StepY;
				Dist = LengthY;
				LengthY += StepSizeY;
				Face = 1;
				break;
				
			case 2:
				MapPosZ += StepZ;
				Dist = LengthZ;
				LengthZ += StepSizeZ;
				Face = 2;
				break;
		}
		
		Index = MapPosY * WORLD_YxWORLD_X + MapPosX * WORLD_Z + MapPosZ;
		
		if(Index >= 0 && Index < WORLD_SIZE && World[Index]) break;
		
		SafeCounter++;
	}
}

static void CamControls(void)
{
	KeyDown = 0;
	
	if(1/*IsKeyDown(KEY_CTRL_UP)*/) // /!\ Mettre la vraie fonction de detection des touches
	{
		CamRotY -= ROT_SPEED;
		if(CamRotY < -PI){ CamRotY = 2 * PI + CamRotY; }
		CamOrientation();
		KeyDown = 1;
	}
	else if(1/*IsKeyDown(KEY_CTRL_DOWN)*/)
	{
		CamRotY += ROT_SPEED;
		if(CamRotY > PI){ CamRotY = -2 * PI + CamRotY; }
		CamOrientation();
		KeyDown = 1;
	}
	
	if(1/*IsKeyDown(KEY_CTRL_RIGHT)*/)
	{
		CamRotX += ROT_SPEED;
		if(CamRotX > PI){ CamRotX = -2 * PI + CamRotX; }
		CamOrientation();
		KeyDown = 1;
	}
	else if(1/*IsKeyDown(KEY_CTRL_LEFT)*/)
	{
		CamRotX -= ROT_SPEED;
		if(CamRotX < -PI){ CamRotX = 2 * PI + CamRotX; }
		CamOrientation();
		KeyDown = 1;
	}
	
	if(1/*IsKeyDown(KEY_CHAR_8)*/)
	{
		CamPosX += f_mul(CamFoX, MOVE_SPEED);
		CamPosY += f_mul(CamFoY, MOVE_SPEED);
		CamPosZ += f_mul(CamFoZ, MOVE_SPEED);
		KeyDown = 1;
	}
	else if(1/*IsKeyDown(KEY_CHAR_5)*/)
	{
		CamPosX -= f_mul(CamFoX, MOVE_SPEED);
		CamPosY -= f_mul(CamFoY, MOVE_SPEED);
		CamPosZ -= f_mul(CamFoZ, MOVE_SPEED);
		KeyDown = 1;
	}
	
	if(1/*IsKeyDown(KEY_CHAR_6)*/)
	{
		CamPosX += f_mul(CamRiX, MOVE_SPEED);
		CamPosY += f_mul(CamRiY, MOVE_SPEED);
		CamPosZ += f_mul(CamRiZ, MOVE_SPEED);
		KeyDown = 1;
	}
	else if(1/*IsKeyDown(KEY_CHAR_4)*/)
	{
		CamPosX -= f_mul(CamRiX, MOVE_SPEED);
		CamPosY -= f_mul(CamRiY, MOVE_SPEED);
		CamPosZ -= f_mul(CamRiZ, MOVE_SPEED);
		KeyDown = 1;
	}
	
	if(1/*IsKeyDown(KEY_CHAR_9)*/)
	{
		CamPosY += MOVE_SPEED;
		KeyDown = 1;
	}
	else if(1/*IsKeyDown(KEY_CHAR_7)*/)
	{
		CamPosY -= MOVE_SPEED;
		KeyDown = 1;
	}
	
	if(1/*IsKeyDown(KEY_CTRL_SHIFT)*/)
	{
		Ray();
		switch(Face)
		{
			case 0:
				MapPosX -= StepX;
				break;
				
			case 1:
				MapPosY -= StepY;
				break;
				
			case 2:
				MapPosZ -= StepZ;
				break;
		}
		World[MapPosY * WORLD_YxWORLD_X + MapPosX * WORLD_Z + MapPosZ] = 1;
		KeyDown = 1;
	}
	else if(1/*IsKeyDown(KEY_CTRL_ALPHA)*/)
	{
		Ray();
		World[Index] = 0;
		KeyDown = 1;
	}
	
	if(KeyDown) Render();
}

static void CamOrientation(void)
{
	s = f_sin(CamRotY);
	c = f_cos(CamRotY);
	
	CamFoX = c;
	CamFoY = -s;
	CamFoZ = c;
	
	CamUpX = s;
	CamUpY = c;
	CamUpZ = s;
	
	s = f_sin(CamRotX);
	c = f_cos(CamRotX);
	
	CamRiX = c;
	CamRiY = 0;
	CamRiZ = -s;
	
	CamFoX = f_mul(CamFoX, s);
	CamFoZ = f_mul(CamFoZ, c);
	
	CamUpX = f_mul(CamUpX, s);
	CamUpZ = f_mul(CamUpZ, c);
}

static void Render(void)
{
	register int32_t PixPosX, PixPosY;
	clear();
	//PtrDist = RayDist;
	
	for(PixPosY=0; PixPosY<SCREEN_HEIGHT; PixPosY++)
	{
		for(PixPosX=0; PixPosX<SCREEN_WIDTH; PixPosX++)
		{
			PntPosX = PixPosX * PREC - HF_SCREEN_WIDTH;
			PntPosY = HF_SCREEN_HEIGHT - PixPosY * PREC;
			
			RayPosX = CamPosX + f_mul(f_mul(PntPosX, CamRiX) + f_mul(PntPosY, CamUpX), SCALE_COEF);
			RayPosY = CamPosY + f_mul(f_mul(PntPosX, CamRiY) + f_mul(PntPosY, CamUpY), SCALE_COEF);
			RayPosZ = CamPosZ + f_mul(f_mul(PntPosX, CamRiZ) + f_mul(PntPosY, CamUpZ), SCALE_COEF);
			
			RayDirX = RayPosX - CamPosX + f_mul(CamFoX, DIST_COEF); //*PtrDist;
			RayDirY = RayPosY - CamPosY + f_mul(CamFoY, DIST_COEF); //*PtrDist;
			RayDirZ = RayPosZ - CamPosZ + f_mul(CamFoZ, DIST_COEF); //*PtrDist;
			
			RayLen = f_sqrt(f_mul(RayDirX, RayDirX) + f_mul(RayDirY, RayDirY) + f_mul(RayDirZ, RayDirZ));
			RayDirX = f_div(RayDirX, RayLen);
			RayDirY = f_div(RayDirY, RayLen);
			RayDirZ = f_div(RayDirZ, RayLen);
			
			LengthX = f_div(RayDirY, RayDirX);
			LengthY = f_div(RayDirZ, RayDirX);
			StepSizeX = f_sqrt(f_mul(LengthX, LengthX) + f_mul(LengthY, LengthY) + PREC);
			
			LengthX = f_div(RayDirX, RayDirY);
			LengthY = f_div(RayDirZ, RayDirY);
			StepSizeY = f_sqrt(f_mul(LengthX, LengthX) + f_mul(LengthY, LengthY) + PREC);
			
			LengthX = f_div(RayDirX, RayDirZ);
			LengthY = f_div(RayDirY, RayDirZ);
			StepSizeZ = f_sqrt(f_mul(LengthX, LengthX) + f_mul(LengthY, LengthY) + PREC);
			
			MapPosX = RayPosX / PREC;
			MapPosY = RayPosY / PREC;
			MapPosZ = RayPosZ / PREC;
			
			if(RayDirX > 0)
			{
				StepX = 1;
				LengthX = f_mul(MapPosX * PREC + PREC - RayPosX, StepSizeX);
			}
			else
			{
				StepX = -1;
				LengthX = f_mul(RayPosX - MapPosX * PREC, StepSizeX);
			}
			
			if(RayDirY > 0)
			{
				StepY = 1;
				LengthY = f_mul(MapPosY * PREC + PREC - RayPosY, StepSizeY);
			}
			else
			{
				StepY = -1;
				LengthY = f_mul(RayPosY - MapPosY * PREC, StepSizeY);
			}
			
			if(RayDirZ > 0)
			{
				StepZ = 1;
				LengthZ = f_mul(MapPosZ * PREC + PREC - RayPosZ, StepSizeZ);
			}
			else
			{
				StepZ = -1;
				LengthZ = f_mul(RayPosZ - MapPosZ * PREC, StepSizeZ);
			}
			
			Dist = 0;
			SafeCounter = 0;
			
			while(Dist < MAX_DIST && SafeCounter < MAX_COUNT)
			{
				switch((LengthY < LengthX && LengthY < LengthZ) + ((LengthZ < LengthX && LengthZ < LengthY) * 2))
				{
					case 0:
						MapPosX += StepX;
						Dist = LengthX;
						LengthX += StepSizeX;
						Face = 0;
						break;
						
					case 1:
						MapPosY += StepY;
						Dist = LengthY;
						LengthY += StepSizeY;
						Face = 1;
						break;
						
					case 2:
						MapPosZ += StepZ;
						Dist = LengthZ;
						LengthZ += StepSizeZ;
						Face = 2;
						break;
				}
				
				Index = MapPosY * WORLD_YxWORLD_X + MapPosX * WORLD_Z + MapPosZ;
				
				if(Index >= 0 && Index < WORLD_SIZE && World[Index])
				{
					switch(Face)
					{
						case 0:
							SampleX = RayPosZ + f_mul(RayDirZ, Dist);
							SampleY = RayPosY + f_mul(RayDirY, Dist);
							CurrentTexture = SideTexture;
							break;
							
						case 1:
							SampleX = RayPosX + f_mul(RayDirX, Dist);
							SampleY = RayPosZ + f_mul(RayDirZ, Dist);
							CurrentTexture = StepY < 0 ? TopTexture : BottomTexture;
							break;
							
						case 2:
							SampleX = RayPosX + f_mul(RayDirX, Dist);
							SampleY = RayPosY + f_mul(RayDirY, Dist);
							CurrentTexture = SideTexture;
							break;
					}
					
					TextureX = f_mul(SampleX & 0x3ff, 8191 /*7.999*/) / PREC; // CONSTANTS
					TextureY = f_mul(SampleY & 0x3ff, 8191 /*7.999*/) / PREC; // CONSTANTS
					
					if( !(CurrentTexture[TextureY >> 2] & (1 << ((TextureY % 4 << 3) + TextureX))) ) // CONSTANTS
					{
						kprintc('#', OFFSET_X + PixPosX, OFFSET_Y + PixPosY, 0x0E);
					}
					
					break;
				}
				
				SafeCounter++;
			}
			//PtrDist++;
		}
	}
	
	kprintc('+', OFFSET_X + HF_SCREEN_WIDTH / PREC, OFFSET_Y + HF_SCREEN_HEIGHT / PREC, 0x04);
	
	/*
	vram[488] ^= 128;
	vram[504] ^= 128;
	vram[536] ^= 128;
	vram[552] ^= 128;

	vram[519] ^= 3;
	vram[520] ^= 224;
	*/
}

static void Minecraft(void)
{
	vram = 0xB8000;
	
	uint8_t *ptr = World;
	int32_t size = sizeof(World) / sizeof(uint8_t);
	
	for(int32_t i=0; i<size; i++)
	{
		*ptr = 0; ptr++;
	}
	
	World[11] = 1;
	
	Render();
	
	/*
	while(1) // /!\ Détecter une touche
	{
		CamControls();
	}
	*/
}

void init(void)
{
	isrInstall();
	setIdtr();
	setIdt((unsigned int) &IDTR);

	cursorEnable(0, 15);
	keyboardInit();

	Minecraft();

	while(1);
}
