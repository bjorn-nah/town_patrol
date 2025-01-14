#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h>
#include <time.h> 

#include "game.h"

extern void init_music();
extern void start_music();
extern void start_crash();
extern void std_functions();
extern void draw_score(unsigned char x, unsigned char y, unsigned char wx, unsigned char wy, const unsigned int num);

extern unsigned int level, score;

typedef struct enemy{
	unsigned char type;
	unsigned char explode_type;
	unsigned char status;
	unsigned int tic;
	unsigned int step;
	unsigned int wait_action;
	unsigned int speed;
	signed int hdir;
	signed int vdir;
	signed int hlhitbox;	//hitbox horizontal left
	signed int hrhitbox;	//hitbox horizontal right
	signed int vuhitbox;	//hitbox vertical up
	signed int vdhitbox;	//hitbox vertical down
} enemy;

typedef struct hero{
	unsigned char status;
	unsigned int tic;
	unsigned int step;
	unsigned int wait_shoot;
	signed int hdir;
	signed int vdir;
} hero;


unsigned int playing, pause;
unsigned int x_map, y_map;
unsigned char game_status;
unsigned int max_enemy_speed;
unsigned int randomizator2 = 0;

extern unsigned char palette[];

int map_h = 12;
int max_shoot = 6;
int max_enemy = 9;
unsigned char shoot_status[6];
enemy enemy_list[9];
hero hero_attr;



extern unsigned char hero_run01[];
extern unsigned char hero_run02[];
extern unsigned char hero_run03[];
extern unsigned char hero_run04[];
extern unsigned char hero_shr01[];
extern unsigned char hero_shr02[];
extern unsigned char hero_shr03[];
extern unsigned char hero_shr04[];
extern unsigned char hero_shu01[];
extern unsigned char hero_shu02[];
extern unsigned char hero_shu03[];
extern unsigned char hero_shu04[];
extern unsigned char shoot01[];

extern unsigned char enemy_fly01[];
extern unsigned char enemy_fly02[];
extern unsigned char enemy_fly03[];
extern unsigned char enemy_fly04[];
extern unsigned char explode00[];
extern unsigned char explode01[];
extern unsigned char explode02[];
extern unsigned char explode03[];
extern unsigned char explode04[];
extern unsigned char explode05[];
extern unsigned char explode06[];
extern unsigned char explode07[];
extern unsigned char explode08[];
extern unsigned char explode09[];
extern unsigned char explode10[];
extern unsigned char explode11[];
extern unsigned char explode12[];
extern unsigned char explode13[];
extern unsigned char explode14[];
extern unsigned char explode15[];
extern unsigned char explode16[];
extern unsigned char explode17[];
extern unsigned char explode18[];
extern unsigned char explode19[];
extern unsigned char explode20[];
extern unsigned char explode21[];
extern unsigned char explode22[];
extern unsigned char explode23[];
extern unsigned char explode24[];
extern unsigned char explode25[];
extern unsigned char explode26[];
extern unsigned char explode27[];
extern unsigned char explode28[];
extern unsigned char explode29[];
extern unsigned char missile00[];

extern unsigned char enemy_run01[];
extern unsigned char enemy_run02[];
extern unsigned char enemy_run03[];
extern unsigned char enemy_run04[];
extern unsigned char enemy_run11[];
extern unsigned char enemy_run12[];
extern unsigned char enemy_run13[];

extern unsigned char sky02[];
extern unsigned char city01[];
extern unsigned char city02[];
extern unsigned char city03[];
extern unsigned char city04[];
extern unsigned char skyline01[];
extern unsigned char skyline02[];
extern unsigned char skyline03[];
extern unsigned char skyline04[];
extern unsigned char skyline05[];
extern unsigned char skyline06[];
extern unsigned char skyline07[];
extern unsigned char skyline11[];
extern unsigned char skyline12[];
extern unsigned char skyline13[];

extern unsigned char pause_msg[];

unsigned int hero_tics, hero_step;
unsigned int difficulty, debug;
unsigned int debug2, debug3, debug4, debug5;
unsigned char hero_status, hero_tics_max;
unsigned int shoot_gap, enemy_gap;
int xdiffdebug, ydiffdebug;


unsigned int distance =0;

SCB_REHV_PAL hero_spr = {
	BPP_4 | TYPE_NORMAL, 
	REHV,
	0x01,
	0x0000,
	hero_run01,
	20, 82,
	0x0100, 0x0100,
	{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF}
};

SCB_REHVST_PAL background = {
	BPP_4 | TYPE_NORMAL, 
	REHVST ,
	0x01,
	0x0000,
	sky02,
	0, 0,
	0x0100, 0x0100,
	0x0000, 0x0000,
	{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF}
};

SCB_REHV_PAL pause_spr = {
	BPP_4 | TYPE_NORMAL, 
	REHV,
	0x01,
	0x0000,
	pause_msg,
	48, 46,
	0x0100, 0x0100,
	{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF}
};

SCB_REHVST_PAL skyline_bg01, skyline_bg02, skyline_bg03, skyline_bg04, skyline_bg05, skyline_bg06;
SCB_REHVST_PAL skyline_bg11, skyline_bg12, skyline_bg13, skyline_bg14, skyline_bg15, skyline_bg16;
SCB_REHVST_PAL city_11, city_12, city_13, city_14, city_15, city_16;
SCB_REHV_PAL shoot_01, shoot_02, shoot_03, shoot_04, shoot_05, shoot_06;
SCB_REHV_PAL enemy_01, enemy_02, enemy_03, enemy_04, enemy_05, enemy_06, enemy_07, enemy_08, enemy_09;
SCB_REHVST_PAL *work_spr, *work_spr1, *work_spr2, *work_spr3;
SCB_REHV_PAL *work_spr_spl, *work_spr_spl1;

//functions goes here:
unsigned char *get_skyline(int tule_index){
	switch(tule_index){
		case 1:
			return skyline02;
			break;
		case 2:
			return skyline03;
			break;
		case 3:
			return skyline04;
			break;
		case 4:
			return skyline05;
			break;
		case 5:
			return skyline06;
			break;
		case 6:
			return skyline07;
			break;
		case 7:
			return skyline11;
			break;
		case 8:
			return skyline12;
			break;
		case 9:
			return skyline13;
			break;
		default:
			return skyline01;
	}
}
unsigned char *get_city(int tule_index){
	switch(tule_index){
		case 1:
			return city02;
			break;
		case 2:
			return city03;
			break;
		case 3:
			return city04;
			break;
		default:
			return city01;
	}
}

unsigned char *get_explosion(tic,explosion_type){
	switch(explosion_type){
		case 0:
			switch(tic){
				case 0:
					return explode00;
					break;
				case 1:
					return explode01;
					break;
				case 2:
					return explode02;
					break;
				case 3:
					return explode03;
					break;
				case 4:
					return explode04;
					break;
				case 5:
					return explode05;
					break;
				case 6:
					return explode06;
					break;
				case 7:
					return explode07;
					break;
				case 8:
					return explode08;
					break;
				case 9:
					return explode09;
					break;
			}
		case 1:
			switch(tic){
				case 0:
					return explode10;
					break;
				case 1:
					return explode11;
					break;
				case 2:
					return explode12;
					break;
				case 3:
					return explode13;
					break;
				case 4:
					return explode14;
					break;
				case 5:
					return explode15;
					break;
				case 6:
					return explode16;
					break;
				case 7:
					return explode17;
					break;
				case 8:
					return explode18;
					break;
				case 9:
					return explode19;
					break;
			}
		case 2:
			switch(tic){
				case 0:
					return explode20;
					break;
				case 1:
					return explode21;
					break;
				case 2:
					return explode22;
					break;
				case 3:
					return explode23;
					break;
				case 4:
					return explode24;
					break;
				case 5:
					return explode25;
					break;
				case 6:
					return explode26;
					break;
				case 7:
					return explode27;
					break;
				case 8:
					return explode28;
					break;
				case 9:
					return explode29;
					break;
			}
	}
}

unsigned char *get_hero(){
	switch(hero_attr.status){
		case RUN:
			switch(hero_attr.tic){
				case 0:
					return hero_run01;
					break;
				case 1:
					return hero_run02;
					break;
				case 2:
					return hero_run03;
					break;
				case 3:
					return hero_run04;
					break;
			}
			break;
		case SHOOT:
			switch(hero_attr.tic){
				case 0:
					return hero_shr01;
					break;
				case 1:
					return hero_shr02;
					break;
				case 2:
					return hero_shr03;
					break;
				case 3:
					return hero_shr04;
					break;
			}
			break;
		case SHOOT_UP:
			switch(hero_attr.tic){
				case 0:
					return hero_shu01;
					break;
				case 1:
					return hero_shu02;
					break;
				case 2:
					return hero_shu03;
					break;
				case 3:
					return hero_shu04;
					break;
			}
			break;
		case EXPLODE:
			return get_explosion(hero_attr.tic,1);
			break;
		default:
			return hero_run01;
	}
}

unsigned char *get_enemy(enemy_id){
	switch(enemy_list[enemy_id].status){
		case ENEMY_FLY:
			switch(enemy_list[enemy_id].tic){
				case 0:
					return enemy_fly01;
					break;
				case 1:
					return enemy_fly02;
					break;
				case 2:
					return enemy_fly03;
					break;
				case 3:
					return enemy_fly04;
					break;
			}
			break;
		case ENEMY_EXPLO:
			return get_explosion(enemy_list[enemy_id].tic,enemy_list[enemy_id].explode_type);
			break;
		case ENEMY_RUN:
			switch(enemy_list[enemy_id].tic){
				case 0:
					return enemy_run01;
					break;
				case 1:
					return enemy_run02;
					break;
				case 2:
					return enemy_run03;
					break;
				case 3:
					return enemy_run04;
					break;
			}
		case ENEMY_RUN2:
			switch(enemy_list[enemy_id].tic){
				case 0:
					return enemy_run11;
					break;
				case 1:
					return enemy_run12;
					break;
				case 2:
					return enemy_run13;
					break;
			}
			break;
		case ENEMY_SHT:
			return missile00;
			break;
	}
}



void make_shoot(direction){
	unsigned int i;
	if(shoot_gap==0){
		work_spr_spl = &shoot_01;
		i=0;
		while(shoot_status[i]!=SHOOT_OFF && i<max_shoot){
			i++;
			work_spr_spl = (SCB_REHV_PAL *)work_spr_spl->next;
		}
		if(i<max_shoot){
			shoot_status[i]=direction;
			work_spr_spl->sprctl1=REHV;
			work_spr_spl->vpos=hero_spr.vpos;
			work_spr_spl->hpos=hero_spr.hpos+6;
		}
		shoot_gap = 8;
	}
}
void make_enemyfly(hight){
	unsigned int i, speed;
	speed=rand()%max_enemy_speed+1;
	work_spr_spl = &enemy_01;
	i=0;
	//while(enemy_status[i]!=SHOOT_OFF && i<max_enemy){
	while(enemy_list[i].status!=ENEMY_OFF && i<max_enemy){
		i++;
		work_spr_spl = (SCB_REHV_PAL *)work_spr_spl->next;
	}
	if(i<max_enemy){
		enemy_list[i].status=ENEMY_FLY;
		work_spr_spl->sprctl0=BPP_4 | TYPE_NORMAL;
		work_spr_spl->sprctl1=REHV;
		work_spr_spl->vpos=hight;
		work_spr_spl->hpos=160;
		work_spr_spl->data=get_enemy(i);
		enemy_list[i].vdir=-1;
		enemy_list[i].hdir=-speed;
		enemy_list[i].speed=speed;
		enemy_list[i].wait_action=32;
		enemy_list[i].hlhitbox=4;
		enemy_list[i].hrhitbox=4;
		enemy_list[i].vuhitbox=4;
		enemy_list[i].vdhitbox=4;
	}
}
void make_enemyshoot(id,h,v){
	unsigned int i;
	work_spr_spl1 = &enemy_01;
	i=0;
	while(enemy_list[i].status!=ENEMY_OFF && i<max_enemy){
		i++;
		work_spr_spl1 = (SCB_REHV_PAL *)work_spr_spl1->next;
	}
	if(i<max_enemy){
		enemy_list[i].status=ENEMY_SHT;
		work_spr_spl->sprctl0=BPP_4 | TYPE_NORMAL;
		work_spr_spl1->sprctl1=REHV;
		work_spr_spl1->vpos=v;
		work_spr_spl1->hpos=h;
		work_spr_spl1->data=get_enemy(i);
		enemy_list[i].vdir=1;
		enemy_list[i].hdir=0;
		enemy_list[i].hlhitbox=2;
		enemy_list[i].hrhitbox=2;
		enemy_list[i].vuhitbox=3;
		enemy_list[i].vdhitbox=3;
		enemy_list[id].wait_action=300;
	}
}
void make_enemyrun(run_status){
	unsigned int i, speed;
	speed=rand()%max_enemy_speed+1;
	work_spr_spl = &enemy_01;
	i=0;
	while(enemy_list[i].status!=ENEMY_OFF && i<max_enemy){
		i++;
		work_spr_spl = (SCB_REHV_PAL *)work_spr_spl->next;
	}
	if(i<max_enemy){
		enemy_list[i].status=run_status;
		work_spr_spl->sprctl1=REHV;
		if(speed==1){
			work_spr_spl->sprctl0=BPP_4 | TYPE_NORMAL;
		}else{
			work_spr_spl->sprctl0=BPP_4 | TYPE_NORMAL | HFLIP;
		}
		work_spr_spl->vpos=82;
		work_spr_spl->hpos=176;
		work_spr_spl->data=get_enemy(i);
		enemy_list[i].vdir=0;
		enemy_list[i].hdir=-speed;
		enemy_list[i].wait_action=32;
		enemy_list[i].hlhitbox=0;
		enemy_list[i].hrhitbox=8;
		enemy_list[i].vuhitbox=10;
		enemy_list[i].vdhitbox=10;
	}
}
void inc_score(enemy_type){
	switch(enemy_type){
		case ENEMY_FLY:
			score+=10;
			break;
		case ENEMY_RUN:
			score+=5;
			break;
		case ENEMY_RUN2:
			score+=5;
			break;
		case ENEMY_SHT:
			score+=1;
			break;
	}
}

void init_level(){
	unsigned int y;
	

	skyline_bg01 = skyline_bg02 = skyline_bg03 = skyline_bg04 = skyline_bg05 = skyline_bg06 = background;
	skyline_bg11 = skyline_bg12 = skyline_bg13 = skyline_bg14 = skyline_bg15 = skyline_bg16 = background;
	city_11= city_12= city_13= city_14= city_15= city_16 = background;
	shoot_01 = shoot_02 = shoot_03 = shoot_04 = shoot_05 = shoot_06 = hero_spr;
	enemy_01 = enemy_02 = enemy_03 = enemy_04 = enemy_05 = enemy_06 = hero_spr;
	enemy_07 = enemy_08 = enemy_09 = hero_spr;

	city_16.next = (char *)&enemy_01;
	
	//city layer 1 (front)
	city_15.next = (char *)&city_16;
	city_14.next = (char *)&city_15;
	city_13.next = (char *)&city_14;
	city_12.next = (char *)&city_13;
	city_11.next = (char *)&city_12;
	skyline_bg16.next = (char *)&city_11;

	
	//city layer 2

	skyline_bg15.next = (char *)&skyline_bg16;
	skyline_bg14.next = (char *)&skyline_bg15;
	skyline_bg13.next = (char *)&skyline_bg14;
	skyline_bg12.next = (char *)&skyline_bg13;
	skyline_bg11.next = (char *)&skyline_bg12;
	skyline_bg06.next = (char *)&skyline_bg11;
	
	//city layer 3
	skyline_bg05.next = (char *)&skyline_bg06;
	skyline_bg04.next = (char *)&skyline_bg05;
	skyline_bg03.next = (char *)&skyline_bg04;
	skyline_bg02.next = (char *)&skyline_bg03;
	skyline_bg01.next = (char *)&skyline_bg02;
	background.next = (char *)&skyline_bg01;
	
	
	//background 
	background.hsize = 0x1400;
	
	// hero sprites
	hero_spr.next = 0x0000;
	shoot_06.next = (char *)&hero_spr;
	shoot_05.next = (char *)&shoot_06;
	shoot_04.next = (char *)&shoot_05;
	shoot_03.next = (char *)&shoot_04;
	shoot_02.next = (char *)&shoot_03;
	shoot_01.next = (char *)&shoot_02;
	
	//enemy sprites
	enemy_09.next = (char *)&shoot_01;
	enemy_08.next = (char *)&enemy_09;
	enemy_07.next = (char *)&enemy_08;
	enemy_06.next = (char *)&enemy_07;
	enemy_05.next = (char *)&enemy_06;
	enemy_04.next = (char *)&enemy_05;
	enemy_03.next = (char *)&enemy_04;
	enemy_02.next = (char *)&enemy_03;
	enemy_01.next = (char *)&enemy_02;
	
	work_spr = &skyline_bg01;
	for(y=0;y<6;y++){
		work_spr->vpos=54;
		work_spr->hpos=y*32;
		work_spr->data=get_skyline(rand()%6);
		work_spr->vsize=0x00E0;
		work_spr->hsize = 0x0100;
		work_spr->sprctl1=REUSEPAL;
		work_spr = (SCB_REHVST_PAL *)work_spr->next;
	}
	skyline_bg01.sprctl1=REHVST;
	
	work_spr = &skyline_bg11;
	for(y=0;y<6;y++){
		work_spr->vpos=48;
		work_spr->hpos=y*32;
		work_spr->data=get_skyline(rand()%6);
		work_spr->hsize = 0x0100;
		work_spr->penpal[3]=0x76;
		work_spr->sprctl1=REUSEPAL;
		work_spr = (SCB_REHVST_PAL *)work_spr->next;
	}
	skyline_bg11.sprctl1=REHVST;
	
	work_spr = &city_11;
	for(y=0;y<6;y++){
		work_spr->vpos=80;
		work_spr->hpos=y*32;
		work_spr->data=get_city(y);
		work_spr->hsize = 0x0100;
		work_spr->penpal[2]=0x65;
		work_spr->penpal[3]=0x47;
		work_spr->sprctl1=REUSEPAL;
		work_spr = (SCB_REHVST_PAL *)work_spr->next;
	}
	city_11.sprctl1=REHVST;
	
	work_spr_spl = &shoot_01;
	for(y=0;y<max_shoot;y++){
		work_spr_spl->vpos=20;
		work_spr_spl->hpos=y*16;
		work_spr_spl->sprcoll=0x0;	//collision value
		work_spr_spl->data=shoot01;
		work_spr_spl->sprctl1=SKIP;
		work_spr_spl = (SCB_REHV_PAL *)work_spr_spl->next;
		// init shoot_status
		shoot_status[y]=SHOOT_OFF;
	}
	
	
	work_spr_spl = &enemy_01;
	for(y=0;y<max_enemy;y++){
		work_spr_spl->vpos=40;
		work_spr_spl->hpos=y*16+16;
		work_spr_spl->data=enemy_fly01;
		work_spr_spl->sprctl1=SKIP;
		work_spr_spl = (SCB_REHV_PAL *)work_spr_spl->next;
		enemy_list[y].status=ENEMY_OFF;
	}
	
	hero_spr.vpos=82;
	hero_spr.hpos=20;
	pause =0;
}



void physics(){
	// do physicals stuff here ;)
	unsigned int x, y;
	int xdiff, ydiff;

	work_spr = &skyline_bg01;
	work_spr1 = &skyline_bg11;
	work_spr3 = &city_11;
	for(y=0;y<6;y++){
		// plan 3
		work_spr->hpos-=1;
		if(work_spr->hpos==-32){
			work_spr->hpos=160;
			work_spr->data=get_skyline(rand()%10);
		}
		work_spr = (SCB_REHVST_PAL *)work_spr->next;
		// plan 2
		work_spr1->hpos-=2;
		if(work_spr1->hpos==-32){
			work_spr1->hpos=160;
			work_spr1->data=get_skyline(rand()%7);
		}
		work_spr1 = (SCB_REHVST_PAL *)work_spr1->next;
		// plan 1
		work_spr3->hpos-=3;
		if(work_spr3->hpos<-31){
			work_spr3->hpos=work_spr3->hpos+192;
		}
		work_spr3 = (SCB_REHVST_PAL *)work_spr3->next;
	}
	
	//update shoots
	work_spr_spl = &shoot_01;
	for(y=0;y<max_shoot;y++){
		if(shoot_status[y]!=SHOOT_OFF){
				switch(shoot_status[y]){
					case SHOOT_U:
						work_spr_spl->vpos-=2;
						break;
					case SHOOT_D:
						work_spr_spl->vpos+=2;
						break;
					case SHOOT_L:
						work_spr_spl->hpos-=2;
						break;
					case SHOOT_R:
						work_spr_spl->hpos+=2;
						break;
				}
			if(work_spr_spl->hpos>160 ||
			work_spr_spl->hpos<-6 ||
			work_spr_spl->vpos>102 ||
			work_spr_spl->vpos<-6){
				work_spr_spl->sprctl1=SKIP;
				shoot_status[y]=SHOOT_OFF;
			}
			work_spr_spl1 = &enemy_01;
			for(x=0;x<max_enemy;x++){
				if(enemy_list[x].status!=ENEMY_OFF && enemy_list[x].status!=ENEMY_EXPLO){
					xdiff = work_spr_spl->hpos - work_spr_spl1->hpos;
					ydiff = work_spr_spl->vpos - work_spr_spl1->vpos;
					if(xdiff>-enemy_list[x].hlhitbox && xdiff<enemy_list[x].hrhitbox && ydiff>-enemy_list[x].vuhitbox && ydiff<enemy_list[x].vdhitbox){
						inc_score(enemy_list[x].status);
						enemy_list[x].status=ENEMY_EXPLO;
						enemy_list[x].tic=0;
						enemy_list[x].explode_type=rand()%3;
						work_spr_spl->sprctl1=SKIP;
						difficulty++;
						max_enemy_speed=1+(difficulty/10);
						shoot_status[y]=SHOOT_OFF;
						start_crash();
					}
				}
				
				work_spr_spl1 = (SCB_REHV_PAL *)work_spr_spl1->next;
			}
		}
		work_spr_spl = (SCB_REHV_PAL *)work_spr_spl->next;
	}
	
	//player collisions
	work_spr_spl = &enemy_01;
	for(x=0;x<max_enemy;x++){
		if(enemy_list[x].status!=ENEMY_OFF && enemy_list[x].status!=ENEMY_EXPLO && hero_attr.status!=EXPLODE){
			xdiff = work_spr_spl->hpos - hero_spr.hpos;
			ydiff = work_spr_spl->vpos - hero_spr.vpos;
			if(xdiff>-10 && xdiff<10 && ydiff>-8 && ydiff<8){
				hero_attr.status=EXPLODE;
				hero_attr.tic=0;
				start_crash();
			}
		}
		work_spr_spl = (SCB_REHV_PAL *)work_spr_spl->next;
	}
	
	
}

void game_logic(){
	unsigned char joy;
	
	joy = joy_read(JOY_1);
	
	if(hero_attr.status!=EXPLODE){
		if (JOY_BTN_A(joy)&&!JOY_UP(joy)){
			hero_attr.status=SHOOT;
			make_shoot(SHOOT_R);
		}
		if (JOY_UP(joy)&&JOY_BTN_A(joy)){
			hero_attr.status=SHOOT_UP;
			make_shoot(SHOOT_U);
		}
		if (!JOY_BTN_A(joy)){
			hero_attr.status=RUN;
		}
	}
	
	if(hero_attr.status == RUN || hero_attr.status == SHOOT || hero_attr.status == SHOOT_UP){
		hero_tics_max = 4;
		if (JOY_LEFT(joy) && hero_spr.hpos > 0) {
			hero_spr.hpos--;
			hero_tics_max = 6;
		}
		if (JOY_RIGHT(joy) && hero_spr.hpos < 150) {
			hero_spr.hpos++;
			hero_tics_max = 2;
		}
	}
	if(shoot_gap!=0){
		shoot_gap--;
	}

}

void player_logic(){
	if(hero_attr.step>hero_tics_max ){
		hero_attr.tic++;
		hero_attr.step=0;
		if(hero_attr.tic>3 && hero_attr.status!=EXPLODE){
			hero_attr.tic=0;
		}
		if(hero_attr.tic>10 && hero_attr.status==EXPLODE){
			hero_attr.tic=0;
			hero_attr.status=DIE;
			playing = 0;
		}
		if(hero_attr.status==EXPLODE){
			hero_spr.hpos-=2;
		}
		hero_spr.data=get_hero();
	}
	
	hero_attr.step++;
}

void enemy_logic(){
	unsigned int y,x;
	signed int diff;
	work_spr_spl = &enemy_01;
	for(y=0;y<max_enemy;y++){
		if(enemy_list[y].status!=ENEMY_OFF){
			if(enemy_list[y].status==ENEMY_FLY){
				work_spr_spl->hpos+=enemy_list[y].hdir;
				work_spr_spl->vpos+=enemy_list[y].vdir;
				if(work_spr_spl->hpos<8){enemy_list[y].hdir=enemy_list[y].speed;}
				if(work_spr_spl->hpos>152){enemy_list[y].hdir=-enemy_list[y].speed;}
				if(work_spr_spl->vpos<8){enemy_list[y].vdir=1;}
				if(work_spr_spl->vpos>40){enemy_list[y].vdir=-1;}
				diff = work_spr_spl->hpos - hero_spr.hpos;
				if(enemy_list[y].wait_action<(200+difficulty) && diff>-2 && diff<2){
					make_enemyshoot(y,work_spr_spl->hpos,work_spr_spl->vpos);
				}
				if(enemy_list[y].wait_action==0){
					make_enemyshoot(y,work_spr_spl->hpos,work_spr_spl->vpos);
				}
				if(enemy_list[y].wait_action>0){enemy_list[y].wait_action--;}
			}
			if(enemy_list[y].status==ENEMY_SHT){
				work_spr_spl->vpos+=enemy_list[y].vdir;
				if(work_spr_spl->vpos>88){
					enemy_list[y].status=ENEMY_EXPLO;
					enemy_list[y].tic=0;
					start_crash();
				}
			}
			if(enemy_list[y].status==ENEMY_RUN || enemy_list[y].status==ENEMY_RUN2){
				work_spr_spl->hpos+=enemy_list[y].hdir;
				if(work_spr_spl->hpos<-8){
					enemy_list[y].status=ENEMY_OFF;
					work_spr_spl->sprctl1=SKIP;
				}
			}
			if(enemy_list[y].status==ENEMY_EXPLO){
				work_spr_spl->hpos-=2;
			}
			if(enemy_list[y].step>3){
				enemy_list[y].tic++;
				enemy_list[y].step=0;
				if(enemy_list[y].tic>3 && enemy_list[y].status==ENEMY_FLY){
					enemy_list[y].tic=0;
				}
				if(enemy_list[y].tic>2 && enemy_list[y].status==ENEMY_RUN){
					enemy_list[y].tic=0;
				}
				if(enemy_list[y].tic>1 && enemy_list[y].status==ENEMY_RUN2){
					enemy_list[y].tic=0;
				}
				if(enemy_list[y].tic>9 && enemy_list[y].status==ENEMY_EXPLO){
					enemy_list[y].tic=0;
					enemy_list[y].status=ENEMY_OFF;
					work_spr_spl->sprctl1=SKIP;
				}
				work_spr_spl->data=get_enemy(y);
			}
			enemy_list[y].step++;
		}
		work_spr_spl = (SCB_REHV_PAL *)work_spr_spl->next;
	}
	
	//generate enemies
	//for(y=0;y<max_enemy;y++){
	//	
	//}
	enemy_gap--;
	if(enemy_gap == 0){
		x=rand()%3;
		if(x==0){make_enemyfly(rand()%40);}
		if(x==1){make_enemyrun(ENEMY_RUN);}
		if(x==2){make_enemyrun(ENEMY_RUN2);}
		enemy_gap=64+rand()%64;
		if(enemy_gap>difficulty+10){
			enemy_gap-=difficulty;
		}else{
			enemy_gap=10;
		}
	}
	
	
}
 

void game(){
	char text[20];
	//int now, last_time, ticks_used;
	
	playing = 1;
	level = 0;
	score = 0;
	debug =0;
	difficulty = 0;
	shoot_gap = 0;
	enemy_gap = 60;
	max_enemy_speed =1;
	
	game_status = NORMAL;

	tgi_clear();

	init_level();
	
	// chiper init + start
	init_music();
	start_music();
	
	
	hero_attr.step = 0;
	hero_attr.tic = 0;
	hero_tics_max = 6;
	hero_attr.status = RUN;
	
	distance = 0;
	
	//now = clock(); 
	
	tgi_updatedisplay();
	
	
	while(playing){
		if (!tgi_busy())
		{
			tgi_clear();
			if(game_status == LEVEL_UP){
				level=distance;
				game_status = NORMAL;
				playing = 0;
			}
			if(!pause){
				tgi_sprite(&background);
				game_logic();
				player_logic();
				enemy_logic();
				physics();
			} else{
				tgi_sprite(&background);
				//tgi_outtextxy(36, 48, "GAME PAUSED");
				tgi_sprite(&pause_spr);
			}
			//score
			//tgi_setcolor(COLOR_RED);
			//itoa(score, text, 10);
			//tgi_outtextxy(8, 8, text);
			
			draw_score(8,8,6,0,score);
			
			//last_time = now;
			//now = clock();
			//ticks_used = now - last_time; 
			
			//debug
			//tgi_setcolor(COLOR_RED);
			//itoa(max_enemy_speed, text, 10);
			//tgi_outtextxy(8, 16, text);
			
			//work_spr_spl = &enemy_01;
			//for(i=0;i<max_enemy;i++){
			//	itoa(enemy_list[i].status, text, 10);
			//	//itoa(work_spr_spl->vpos, text, 10);
			//	tgi_outtextxy(144, 8*i, text);
			//	//work_spr_spl = (SCB_REHV_PAL *)work_spr_spl->next;
			//}
			
			//itoa(hero_attr.status, text, 10);
			//tgi_outtextxy(80, 8, text);

			
			std_functions();
			tgi_updatedisplay();
			//randomizator2++;
		}
	}	
	// chiper stop
	asm("jsr SndStopAll");
	//trick to correctly reset the screen
	while(tgi_busy()){
	}
	tgi_clear();

}