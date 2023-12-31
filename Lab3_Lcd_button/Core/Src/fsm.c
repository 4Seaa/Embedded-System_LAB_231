/*
 * fsm.c
 *
 *  Created on: Dec 30, 2023
 *      Author: Asus
 */

#include "fsm.h"

fsm_state f_state = normal;

int count0 = 0;
int count1 = 0;
int green_count = 2;
int yellow_count = 1;
int red_count = 3;
int tick_count = 0;
int temp_count = 0;
int toggle = 0;

void traffic_init(void) {

	count0 = 0;
	count1 = 0;

	green_count = 2;
	yellow_count = 1;
	red_count = 3;

	tick_count = 0;
}

void update_time(void) {
	tick_count = (tick_count + 1)
			% ((green_count + yellow_count + red_count) * 20);
	if (tick_count % 20 == 0) {
		count0 = tick_count / 20;
		count1 = tick_count / 20;
	}
}

void show_count(void) {
	int temp;
	if (count0 < green_count) {
		temp = green_count - count0;
	} else if (count0 < green_count + yellow_count) {
		temp = green_count + yellow_count - count0;
	} else {
		temp = green_count + yellow_count + red_count - count0;
	}
	lcd_ShowIntNum(90, 50, temp, 2, RED, WHITE, 16);

	int temp0;
	if (count0 < red_count) {
		temp0 = red_count - count0;
	} else if (count0 < green_count + red_count) {
		temp0 = green_count + red_count - count0;
	} else {
		temp0 = green_count + yellow_count + red_count - count0;
	}
	lcd_ShowIntNum(210, 50, temp0, 2, RED, WHITE, 16);
}

void traffic_fsm(void) {
	show_count();
	if (count0 == green_count) {
		lcd_DrawCircle(50, 100, WHITE, 24, 1);
		lcd_DrawCircle(50, 170, WHITE, 24, 1);
		lcd_DrawCircle(50, 240, YELLOW, 24, 1);
	} else if (count0 == green_count + yellow_count) {
		lcd_DrawCircle(50, 100, RED, 24, 1);
		lcd_DrawCircle(50, 170, WHITE, 24, 1);
		lcd_DrawCircle(50, 240, WHITE, 24, 1);
	} else if (count0 == 0) {
		lcd_DrawCircle(50, 100, WHITE, 24, 1);
		lcd_DrawCircle(50, 170, GREEN, 24, 1);
		lcd_DrawCircle(50, 240, WHITE, 24, 1);
	}

	if (count1 == red_count) {
		lcd_DrawCircle(170, 100, WHITE, 24, 1);
		lcd_DrawCircle(170, 170, GREEN, 24, 1);
		lcd_DrawCircle(170, 240, WHITE, 24, 1);
	} else if (count1 == green_count + red_count) {
		lcd_DrawCircle(170, 100, WHITE, 24, 1);
		lcd_DrawCircle(170, 170, WHITE, 24, 1);
		lcd_DrawCircle(170, 240, YELLOW, 24, 1);
	} else if (count1 == 0) {

		lcd_DrawCircle(170, 100, RED, 24, 1);
		lcd_DrawCircle(170, 170, WHITE, 24, 1);
		lcd_DrawCircle(170, 240, WHITE, 24, 1);
	}
}

void traffic_UI(void) {
	lcd_StrCenter(0, 10, "TRAFFIC LIGHT", RED, BLUE, 16, 1);

	lcd_ShowStr(30, 50, "Lane I:", BLUE, RED, 16, 1);

	lcd_ShowStr(140, 50, "Lane II:", BLUE, RED, 16, 1);

}

void update_fsm_state(void) {
	if (button_count[0] == 1) {
		f_state = (f_state + 1) % 4;
		lcd_Clear(WHITE);
		if (f_state == red) {
			temp_count = red_count;
		} else if (f_state == green) {
			temp_count = green_count;
		} else if (f_state == yellow) {
			temp_count = yellow_count;
		}
	}
}

void mod_UI(void) {
	toggle = (toggle + 1) % 20;

	update_temp_count();
	switch (f_state) {
	case red:
		lcd_StrCenter(0, 10, "RED MODIFICATION", RED, BLUE, 16, 1);
		lcd_ShowStr(30, 50, "RED Period:", BLUE, RED, 16, 1);
		if (toggle == 0)
			lcd_DrawCircle(50, 100, RED, 24, 1);
		else if (toggle == 10)
			lcd_DrawCircle(50, 100, WHITE, 24, 1);
		break;
	case green:
		lcd_StrCenter(0, 10, "GREEN MODIFICATION", RED, BLUE, 16, 1);
		lcd_ShowStr(30, 50, "GREEN Period:", BLUE, RED, 16, 1);
		if (toggle == 0)
			lcd_DrawCircle(50, 170, GREEN, 24, 1);
		else if (toggle == 10)
			lcd_DrawCircle(50, 170, WHITE, 24, 1);
		break;
	case yellow:
		lcd_StrCenter(0, 10, "YELLOW MODIFICATION", RED, BLUE, 16, 1);
		lcd_ShowStr(30, 50, "YELLOW Period:", BLUE, RED, 16, 1);
		if (toggle == 0)
			lcd_DrawCircle(50, 240, YELLOW, 24, 1);
		else if (toggle == 10)
			lcd_DrawCircle(50, 240, WHITE, 24, 1);
		break;
	default:
	}
	lcd_ShowIntNum(180, 50, temp_count, 2, RED, WHITE, 16);
}

void update_temp_count(void) {
	if ((button_count[1] == 1) && (temp_count < 99))
		temp_count = temp_count + 1;
}

void fsm_no_return(void) {
	update_fsm_state();
	switch (f_state) {
	case normal:
		update_time();
		traffic_UI();
		traffic_fsm();
		break;
	case red:
		mod_UI();
		if (button_count[2] == 1)
			red_count = temp_count;
		break;
	case green:
		mod_UI();
		if (button_count[2] == 1)
			green_count = temp_count;
		break;
	case yellow:
		mod_UI();
		if (button_count[2] == 1)
			yellow_count = temp_count;
		break;
	default:

	}
}
