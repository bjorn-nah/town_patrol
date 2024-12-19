#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

extern unsigned char title01[];
extern unsigned char title02[];
extern unsigned char sky03[];

unsigned char title_gross[6];

extern void std_functions();

unsigned int button_title, tic_screen, title_step, message;
unsigned int randomizator = 0;
unsigned int zoom_step1 = 0x0010;
unsigned int zoom_step2 = 0x0001;


SCB_REHVST_PAL title_tule_01 = {
	BPP_4 | TYPE_NORMAL, 
	REHVST,
	0x01,
	0x0000,
	title01,
	0, 0,
	0x0100, 0x0100,
	0x0000, 0x0000,
	{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF}
};
SCB_REHVST_PAL title_tule_02, title_back;

void screen_init(){
	title_back = title_tule_02= title_tule_01;

	title_tule_02.next = 0x0000;
	title_tule_01.next = (char *)&title_tule_02;
	title_back.next = (char *)&title_tule_01;
	
	
	title_tule_01.data = title01;
	title_tule_02.data = title02;
	title_back.data = sky03;

	title_tule_01.hpos = -96;
	title_tule_01.vpos = 12;
	title_tule_02.hpos = 168;
	title_tule_02.vpos = 36;
	title_back.hpos = 0;
	title_back.vpos = 0;
	
	title_tule_01.hsize = 0x0100;
	title_tule_02.hsize = 0x0100;
	title_back.hsize = 0x1400;
	
	title_tule_01.tilt = 0xFF60;
	title_tule_02.tilt = 0x0000;
	title_back.tilt = 0x0000;
	

}

void screen_update(){
	tgi_clear();
	tgi_sprite(&title_back);
	
	if(title_step==0){
		title_tule_01.hpos+=2;
		title_tule_02.hpos-=2;
		if(title_tule_01.hpos==36){title_step=1;}
	}
	if(title_step==1){
		title_tule_01.tilt+=0x0010;
		title_tule_02.tilt-=0x0010;
		if(title_tule_01.tilt==0xFFB0){title_step=2;}
	}
	
	if(title_step==2){
		switch(message){
			case 1:
				tgi_outtextxy( 32, 78, "Press A or B");
				break;
			case 2:
				tgi_outtextxy( 16, 78, "Graphics by 0x72");
				break;
			case 3:
				tgi_outtextxy( 4, 78, "Code&Music by Bjorn");
				break;
		}
		tic_screen--;
		if(tic_screen==0){
			message++;
			if(message>3){message=1;}
			tic_screen=120;
		}
	}
	tgi_outtextxy( 32, 88, "#LynxJam2024");
	

	
	tgi_updatedisplay();
}

void screen_logic(){
	unsigned char joy;


	joy = joy_read(JOY_1);
	if (JOY_BTN_1(joy) || JOY_BTN_2(joy) ) {
		button_title = 2;
	}
	else{
		if (button_title == 2) button_title = 0;
	}
	std_functions();
	
}


void title_screen(){
	
	button_title = 1;
	message = 1;
	tic_screen = 120;
	title_step = 0;
	tgi_clear();
	
	tgi_setcolor(COLOR_RED);
	
	//tgi_outtextxy( 8, 8, "TOWN");
	//tgi_outtextxy( 8, 16, "PATROL");
	
	screen_init();
	
	tgi_updatedisplay();
	
	while(button_title){
		if (!tgi_busy()){
			screen_update();
			tgi_sprite(&title_tule_01);
			screen_logic();
			randomizator++;
		}
	}	
	srand(randomizator);
	while(tgi_busy()){
	}
	tgi_clear();
}