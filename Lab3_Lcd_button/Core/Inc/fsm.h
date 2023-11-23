/*
 * fsm.h
 *
 *  Created on: Nov 20, 2023
 *      Author: Asus
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include <stdint.h>
#include "button.h"
#include "lcd.h"
#include "led_7seg.h"

#define RED_LED 0
#define GREEN_LED 1
#define YELLOW_LED 2

#define TIME_TICK 20

typedef enum
{
    NORMAL = 0,
    RED_MODIFIED,
    GREEN_MODIFIED,
    YELLOW_MODIFIED
} led_state;

typedef struct fsm_state
{
    led_state current_mode;
    led_state next_mode;

    int led_count[3];

    int temp;

} fsm_state;

void top_int(void);

void top_fsm(void);

void top_state_update(void);

fsm_state *get_fsm(void);

void trafficLight(void);

void normal_mode(void);

void top(void);

int cal_countdown(int);

#endif /* INC_FSM_H_ */
