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
		7, 2,5,7,
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

unsigned char jagged_pattern[]={
	0b00000000,
    0b01010010,
    0b00101000,
    0b00010100,
    0b01001010,
    0b00100100,
    0b01010010,
    0b00000000
};


  char BallColors[8]= {15,1,1,7,7,1,1,15};
  char Ball2Colors[8]= {0,0,3,3,3,3,3,0};
  

  // Building Sprites
  SpriteReset();
  SpriteSmall();
  
  //Sprites are not *just* patterns. 
  //Patterns are the pixel configurations. 
  //You can have a lot of patterns.
  SetSpritePattern(1, ball_pattern,8);
  SetSpritePattern(2, ball2_pattern,8);

  //position 100 in the pattern table
  VpokeFirst(0x0800);
  int i;
  for(i = 0; i<8; i++) {
	  VpokeNext(jagged_pattern[i]);
  }


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
		
		//PutText(10,10,"More Stuff",LOGICAL_TIMP);
		//PutText(15,15,"Stuff",LOGICAL_TIMP);
		
		//767 tiles per page
		VpokeFirst(0x1800);
			int i;
			for(i = 0; i<767; i++) {
				//palette[i] = mypalette[i];
				VpokeNext(100);
			}
		staticDataChanged = 0;
	}
}

void DrawGameData(void) {
	//Locate(10,10);
	
	//put pattern 1 into sprite slot 0 with color 1 at x,y
	PutSprite(0,1,x,y,1);
	//put pattern 2 into sprite slot 1 with color 3 at x2,y2
	PutSprite(1,2,x2,y2,3);
}

const unsigned int PATTERN_BASE = 0x0000;
const unsigned int PALETTE_BASE = 0x2000;
const unsigned int SECTION_SIZE = 0x0800;
const unsigned int PATTERN_SIZE = 0x0008;

const unsigned char TOP_SECTION = 0x01;
const unsigned char MID_SECTION = 0x02;
const unsigned char BOT_SECTION = 0x04;

void assignBGTile(unsigned char section, unsigned char index, unsigned char patternData[], unsigned char paletteData[]) {
	if (section < 0 || section > 7) {
		//Invalid section code
		return;
	}
	unsigned int patternAddressBase = PATTERN_BASE + (index * PATTERN_SIZE);
	unsigned int paletteAddressBase = PALETTE_BASE + (index * PATTERN_SIZE);
	
	if ((TOP_SECTION & section) == TOP_SECTION) {
		setPatternAtVAddress(patternAddressBase, patternData);
		setPatternAtVAddress(paletteAddressBase, paletteData);
	}
	
	if ((MID_SECTION & section) == MID_SECTION) {
		setPatternAtVAddress((patternAddressBase + SECTION_SIZE), patternData);
		setPatternAtVAddress((paletteAddressBase + SECTION_SIZE), paletteData);
	}
	
	if ((BOT_SECTION & section) == BOT_SECTION) {
		setPatternAtVAddress((patternAddressBase + (2 * SECTION_SIZE)), patternData);
		setPatternAtVAddress((paletteAddressBase + (2 * SECTION_SIZE)), paletteData);
	}
}

void setPatternAtVAddress(unsigned int address0, unsigned char data[]) {
	VPokeFirst(address0);
	VPokeNext(data[0]);
	VPokeNext(data[1]);
	VPokeNext(data[2]);
	VPokeNext(data[3]);
	VPokeNext(data[4]);
	VPokeNext(data[5]);
	VPokeNext(data[6]);
	VPokeNext(data[7]);
}

const unsigned int NAME_TABLE_BASE = 0x1800;

void setBGTileAt(int x, int y, unsigned char index) {
	//x min 0 max 31
	//y min 0 max 23
	
	unsigned int nameTableOffset = 0;
	if ( y > 23 ) {
		//invalid y value
		return;
	} else if ( y < 0 ) {
		//invalid y value
		return;
	}
	if ( x > 31 ) {
		//invalid x value
		return;
	} else if ( x < 0 ) {
		//invalid x value
		return;
	}
	//calculate line offset
	nameTableOffset = y * 32;
	nameTableOffset += x;
	
	VPoke(NAME_TABLE_BASE + nameTableOffset, index);
}

void main(void) {
  printf("Loading, please wait...\n");
  //SetColors(0,0,1);
  InitPSG();
  SpriteOn();
  Screen(4);
  
  //InitPalette();
  //SetSC5Palette((Palette *)palette);
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