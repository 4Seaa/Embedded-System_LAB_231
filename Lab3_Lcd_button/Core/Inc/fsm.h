/*
 * fsm.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Asus
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "button.h"
#include "lcd.h"
#include "led_7seg.h"

extern int count0, count1, tick_count;

typedef enum {
	normal = 0, red, yellow, green
} fsm_state;

extern int green_count, yellow_count, red_count;

extern int temp_count, toggle;

void traffic_init(void);

void traffic_fsm(void);

void update_time(void);

void traffic_UI(void);

void show_count(void);

void fsm_no_return(void);

void update_fsm_state(void);

void mod_UI(void);

void update_temp_count(void);

#endif /* INC_FSM_H_ */
