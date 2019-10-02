#include <stdlib.h>
#include <stdio.h>
#include "vdp_graph2.h"
#include "msx_fusion.h"
#include "vdp_sprites.h"
#include "vars_msxSystem.h"

#define HALT __asm halt __endasm

int border;
int fg;
int bg;
int x;
int y;
int dx;
int dy;
int x2;
int y2;
int dx2;
int dy2;
int staticDataChanged;

char palette[64];

const int XMIN=0;
const int XMAX=256;
const int YMIN=0;
const int YMAX=212;

const int DELAY=1;

void WaitCycles(int cycles) {
	int i;
	for (i = 0; i < cycles; i++) {
		HALT;
	}
	return;
} 

void InitPalette(void) {
	
	fg=15;
	bg=0;
	border=4;
	
		char mypalette[] = {
		0, 0,0,0,
		1, 7,0,0,
		2, 0,7,0,
		3, 0,0,7,
		4, 0,0,0,
		5, 6,5,3,
		6, 7,6,4,
		7, 3,2,1,
		8, 7,5,2,
		9, 6,4,2,
		10, 4,3,2,
		11, 6,0,1,
		12, 5,3,2,
		13, 3,3,2,
		14, 3,1,0,
		15, 7,7,7
	};
	
	int i;
	for(i = 0; i<64; i++) {
		palette[i] = mypalette[i];
	}
	
}


void FT_SpriteDef(void)
{
	

  // Sprites Definition Patterns
//
 unsigned char ball_pattern[]={
    0b00111100,
    0b01111110,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100
};

 unsigned char ball2_pattern[]={
    0b00000000,
    0b00000000,
    0b00100100,
    0b00100100,
    0b00000000,
    0b01000010,
    0b00111100,
    0b00000000
};
  char BallColors[8]= {15,1,1,1,1,1,1,15};
  char Ball2Colors[8]= {0,0,3,3,3,3,3,0};
  

  // Building Sprites
  SpriteReset();
  SpriteSmall();
  
  //Sprites are not *just* patterns. 
  //Patterns are the pixel configurations. 
  //You can have a lot of patterns.
  SetSpritePattern(1, ball_pattern,8);
  SetSpritePattern(2, ball2_pattern,8);

  //Patterns are not sprites.
  //Putting a sprite on-screen means fitting a pattern into a sprite slot.
  //On screens 5 and 8 this means you need to define the colors for the sprites.
  SC5SpriteColors(0,BallColors);
  SC5SpriteColors(1,Ball2Colors);


}

void InitGameData(void) {
	x = 30;
	y = 20;
	dx = 1;
	dy = 1;
	x2 = 34;
	y2 = 24;
	dx2 = -3;
	dy2 = -1;
}

void UpdateGameData(void) {
	x += dx;
	y += dy;
	x2 += dx2;
	y2 += dy2;
	
	if (x <= XMIN) {
		x = 0;
		dx *= -1;
	} else if (x >= (XMAX-8)) {
		x = XMAX-8;
		dx *= -1;
	}
	if (y <= YMIN) {
		y = 0;
		dy *= -1;
	} else if (y >= (YMAX-8)) {
		y = YMAX-8;
		dy *= -1;
	}
	
	if (x2 <= XMIN) {
		x2 = 0;
		dx2 *= -1;
	} else if (x2 >= (XMAX-8)) {
		x2 = XMAX-8;
		dx2 *= -1;
	}
	if (y2 <= YMIN) {
		y2 = 0;
		dy2 *= -1;
	} else if (y2 >= (YMAX-8)) {
		y2 = YMAX-8;
		dy2 *= -1;
	}
	
}

void DrawStaticData(void) {
	if (staticDataChanged > 0) {
		Cls();
		
		PutText(10,10,"More Stuff",LOGICAL_TIMP);
		PutText(15,15,"Stuff",LOGICAL_TIMP);
		
		staticDataChanged = 0;
	}
}

void DrawGameData(void) {
	//Locate(10,10);
	
	//put pattern 1 into sprite slot 0 with color 1 at x,y
	PutSprite(0,1,x,y,1);
	//put pattern 2 into sprite slot 1 with color 2 at x2,y2
	PutSprite(1,2,x2,y2,2);
}

void main(void) {
  printf("Loading, please wait...\n");
  //SetColors(0,0,1);
  InitPSG();
  SpriteOn();
  Screen(5);
  
  InitPalette();
  SetSC5Palette((Palette *)palette);
  SetColors(fg,bg,border);
  
  staticDataChanged = 1;

  KeySound(0);
  VDP60Hz();
  KillKeyBuffer();
  
  InitGameData();
  FT_SpriteDef();
  
  
  Cls();
  
  while(1) {
	  UpdateGameData();
	  DrawStaticData();
	  DrawGameData();
	  WaitCycles(DELAY);
  }
}