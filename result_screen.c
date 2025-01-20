#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

extern void draw_score(unsigned char x, unsigned char y, unsigned char wx, unsigned char wy, const unsigned int num);

extern unsigned char text_gameover[];
extern unsigned char text_score[];
extern unsigned char text_hiscore[];
extern unsigned char text_press[];
extern unsigned char text_version[];
extern unsigned char text_thx[];

extern unsigned int level, score, hi_score;
extern void std_functions();

unsigned int button_result;

SCB_REHV_PAL end_text_01 = {
	BPP_4 | TYPE_NORMAL, 
	REHV,
	0x01,
	0x0000,
	text_gameover,
	53, 8,
	0x0100, 0x0100,
	{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF}
};
SCB_REHV_PAL end_text_02, end_text_03, end_text_04, end_text_05, end_text_06;

void end_init (){
	button_result = 1;
	if(score>hi_score){
		hi_score=score;
	}
	tgi_clear();
	end_text_02 = end_text_03 = end_text_04 = end_text_05 = end_text_06 = end_text_01;
	
	end_text_01.next=(char *)&end_text_02;
	
	end_text_02.data=text_score;
	end_text_02.hpos=40;
	end_text_02.vpos=24;
	end_text_02.next=(char *)&end_text_03;
	end_text_03.data=text_hiscore;
	end_text_03.hpos=55;
	end_text_03.vpos=40;
	end_text_03.next=(char *)&end_text_04;
	end_text_04.data=text_press;
	end_text_04.hpos=44;
	end_text_04.vpos=64;
	end_text_04.next=(char *)&end_text_05;
	end_text_05.data=text_version;
	end_text_05.hpos=50;
	end_text_05.vpos=80;
	end_text_05.next=(char *)&end_text_06;
	end_text_06.data=text_thx;
	end_text_06.hpos=24;
	end_text_06.vpos=88;
	end_text_06.next=0x0000;
	draw_score(64,32,6,0,score);
	draw_score(64,48,6,0,hi_score);
	
}

void end_logic(){
	unsigned char joy;

	joy = joy_read(JOY_1);
	if (JOY_BTN_1(joy) || JOY_BTN_2(joy) ) {
		button_result = 2;
	}
	else{
		if (button_result == 2) button_result = 0;
	}
	std_functions();
}


void result_screen(){
	
	button_result = 1;
	tgi_clear();
	end_init();
	tgi_sprite(&end_text_01);
	
	tgi_updatedisplay();
	
	while(button_result){
		if (!tgi_busy()){
			end_logic();
		}
	}
}