#include <stdlib.h>
#include <stdio.h>
#include "vdp_graph2.h"
#include "msx_fusion.h"
#include "vdp_sprites.h"
#include "vars_msxSystem.h"

#define HALT __asm halt __endasm

char border;
int x;
int y;
int dx;
int dy;
int x2;
int y2;
int dx2;
int dy2;

char palette[60];

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

void FT_SpriteDef(void)
{
	
	char mypalette[] = {
		0, 0,0,0,
		1, 2,1,1,
		2, 6,5,4,
		3, 5,4,3,
		4, 5,5,3,
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
		15, 6,6,6
	};
	
	int i;
	for(i = 0; i<64; i++) {
		palette[i] = mypalette[i];
	}
	
  // Sprites Definition Patterns
//
 unsigned char board_pattern[]={
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b00000000,
  0b00000000,
  0b00000000
};

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
  char BallColors[8]= {15,8,8,8,8,8,8,15};
  char Ball2Colors[8]= {1,1,4,4,4,4,4,1};
  char BoardColors[8]={15,15,15,15,15,15,15,15};

  // Building Sprites
  SpriteReset();
  SpriteSmall();
  SetSpritePattern(0, board_pattern,8);
  SetSpritePattern(1, ball_pattern,8);
  SetSpritePattern(2, ball2_pattern,8);

  SC5SpriteColors(0,BallColors);
  //SC5SpriteColors(1,BoardColors);
  //SC5SpriteColors(2,BoardColors);
  //SC5SpriteColors(3,BoardColors);
  SC5SpriteColors(1,Ball2Colors);


}

void InitGameData(void) {
	x = 30;
	y = 20;
	dx = 1;
	dy = 1;
	x = 230;
	y = 180;
	dx = -1;
	dy = -1;
	border=8;
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

void DrawGameData(void) {
	PutSprite(0,1,x,y,10);
	PutSprite(1,2,x,y,3);
}

void main(void) {
  printf("Loading, please wait...\n");
  SetColors(0,0,1);
  InitPSG();
  SpriteOn();
  SetSC5Palette((Palette *)palette);
  Screen(5);
  SetColors(1,1,border);

  KeySound(0);
  VDP60Hz();
  KillKeyBuffer();
  
  InitGameData();
  FT_SpriteDef();
  Cls();
  
  while(1) {
	  UpdateGameData();
	  DrawGameData();
	  WaitCycles(DELAY);
  }
}