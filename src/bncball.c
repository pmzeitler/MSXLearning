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
  char BallColors[8]= {15,8,8,8,8,8,8,15};
  char BoardColors[8]={15,15,15,15,15,15,15,15};

  // Building Sprites
  SpriteReset();
  SpriteSmall();
  SetSpritePattern(0, board_pattern,8);
  SetSpritePattern(1, ball_pattern,8);

  SC8SpriteColors(0,BallColors);
  SC8SpriteColors(1,BoardColors);
  SC8SpriteColors(2,BoardColors);
  SC8SpriteColors(3,BoardColors);


}

void InitGameData(void) {
	x = 30;
	y = 20;
	dx = 1;
	dy = 1;
}

void UpdateGameData(void) {
	x += dx;
	y += dy;
	
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
	
}

void DrawGameData(void) {
	PutSprite(0,1,x,y,10);
}

void main(void) {
  SetColors(15,0,1);
  InitPSG();
  SpriteOn();
  Screen(8);
  SetColors(255,0,border);
  Cls();
  KeySound(0);
  VDP60Hz();
  KillKeyBuffer();
  
  InitGameData();
  FT_SpriteDef();
  
  while(1) {
	  UpdateGameData();
	  DrawGameData();
	  WaitCycles(DELAY);
  }
}