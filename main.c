#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 
#include <peekpoke.h>

extern int title_screen();
extern int game();
extern int result_screen();

extern unsigned int pause;

unsigned char palette[] = {
	// green
	0x0ef0 >> 8, 
	0x0000 >> 8, 
	0x0cdc >> 8, 
	0x0888 >> 8, 
	0x0121 >> 8, 
	0x0268 >> 8, 
	0x0252 >> 8, 
	0x0331 >> 8, 
	0x035a >> 8, 
	0x0556 >> 8, 
	0x036e >> 8, 
	0x03be >> 8, 
	0x0763 >> 8, 
	0x0a96 >> 8, 
	0x0fef >> 8, 
	0x0eb8 >> 8, 

	// blue + red
	0x0ef0 & 0xff, 
	0x0000 & 0xff, 
	0x0cdc & 0xff, 
	0x0888 & 0xff, 
	0x0121 & 0xff, 
	0x0268 & 0xff, 
	0x0252 & 0xff, 
	0x0331 & 0xff, 
	0x035a & 0xff, 
	0x0556 & 0xff, 
	0x036e & 0xff, 
	0x03be & 0xff, 
	0x0763 & 0xff, 
	0x0a96 & 0xff, 
	0x0fef & 0xff, 
	0x0eb8 & 0xff, 
};


unsigned int level, score, hi_score=0;


void setpalette(const int* palette)
{
  char index;
  for (index = 0; index < 0x10; index++)
  {
    POKE(0xFDA0 + index, palette[index] >> 8);
    POKE(0xFDB0 + index, palette[index] & 0xFF);
  }
}

//chiper functions

void init_music(void)
{
	asm("jsr SndInit");
}


void start_music(void)
{
	asm("jsr SndPauseOn");
	asm("lda musicptr+0");
	asm("ldy musicptr+1");
	asm("ldx #0");
	asm("jsr SndStartSound");
	asm("lda musicptr+2");
	asm("ldy musicptr+3");
	asm("ldx #1");
	asm("jsr SndStartSound");
	// keep channel 3 for sfx
	//asm("lda musicptr+4");
	//asm("ldy musicptr+5");
	//asm("ldx #2");
	//asm("jsr SndStartSound");
	asm("lda musicptr+6");
	asm("ldy musicptr+7");
	asm("ldx #3");
	asm("jsr SndStartSound");
	asm("jsr SndPauseOff");
}
void start_crash(void)
{
	asm("lda musicptr+4");
	asm("ldy musicptr+5");
	asm("ldx #2");
	asm("jsr SndStartSound");
}

void initialize()
{
	tgi_install(&tgi_static_stddrv);
	joy_install(&joy_static_stddrv);
	tgi_init();

	CLI();

	while (tgi_busy()) ;
	
	tgi_setpalette(palette);
	tgi_setcolor(COLOR_WHITE);
	tgi_setbgcolor(COLOR_BLACK);
	
	pause = 0;

	tgi_clear();
}

void reboot()
{
	asm("sei");
	asm("stz $FFF9");
	asm("jmp ($FFFC)");
}

void std_functions()
{
	if (kbhit()){
		switch (cgetc())
			{
			case 'F':
				tgi_flip();
				break;
			case 'R':
				reboot();
				break;
			case 'P':
				pause = !pause;
				break;

			default:
				break;
			}
	}
}



void main(void)
{
	initialize();

	while (1)
	{
		title_screen();
		game();
		result_screen();
	}
	return;
}