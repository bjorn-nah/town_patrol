#include <tgi.h>

extern unsigned char font_0[];
extern unsigned char font_1[];
extern unsigned char font_2[];
extern unsigned char font_3[];
extern unsigned char font_4[];
extern unsigned char font_5[];
extern unsigned char font_6[];
extern unsigned char font_7[];
extern unsigned char font_8[];
extern unsigned char font_9[];

static unsigned char *fontSprite[] = {
	font_0,
	font_1,
	font_2,
	font_3,
	font_4,
	font_5,
	font_6,
	font_7,
	font_8,
	font_9
};

static SCB_REHV_PAL sprite_base = {
	BPP_4 | TYPE_NORMAL, 
	REHV,
	0x01,
	0x0000,
	font_0,
	0, 0,
	0x0100, 0x0100,
	{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF}
};
static SCB_REHV_PAL* sprite = &(sprite_base);

unsigned int get_digit_nb(const unsigned int num){
	unsigned int value = num;
	unsigned int digits = 0;
	// count number of digits in number
	while (value) {
		value /= 10;
		digits++;
	}
	return digits;
}

void draw_score(unsigned char x, unsigned char y, unsigned char wx, unsigned char wy, const unsigned int num) {
	if (num == 0) {
		sprite->data = fontSprite[0];
		sprite->hpos = x;
		sprite->vpos = y;
		tgi_sprite(sprite);
	}
	else {
		unsigned int value = num;
		unsigned int digit, digits;
		digits = get_digit_nb(num);

		// draw digits in reverse order
		value = num;
		y += (digits - 1) * wy;
		x += (digits - 1) * wx;
		while (value) {
			digit = value % 10;
			sprite->data = fontSprite[digit];
			sprite->hpos = x;
			sprite->vpos = y;
			tgi_sprite(sprite);
			value /= 10;
			y -= wy;
			x -= wx;
		}
	}
}