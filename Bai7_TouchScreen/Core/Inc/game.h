/*
 * game.h
 *
 *  Created on: Jan 1, 2024
 *      Author: Asus
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

// #include <stdio.h>  
// #include <stdlib.h>
// #include <time.h>

// #include <stdio.h>      
// #include <conio.h>  
// #include <stdlib.h>  

// int x = rand() % 100;

#include "touch.h"
#include "lcd.h"

#define HIT 10

#define LIMIT_UP 50
#define LIMIT_DOWN 150
#define LIMIT_LEFT 10
#define LIMIT_RIGHT 230

extern uint16_t snake_x[100];
extern uint16_t snake_y[100];
extern uint8_t time_counter0, time_counter1;
extern int remain_prey;
extern int snake_len;

extern int prey_x, prey_y;


typedef enum { INIT = 0, START, PLAY, LOSE, WIN } state;

typedef enum { IDLE = 0, UP, DOWN, LEFT, RIGHT } dir;

void UI(void);

void top(void);

void update_fsm_state(void);

uint8_t isStart(void);

void snake_move(void);

void snake_init(void);

void isNavigated(void);

void handle_snake_dir(void);

void eat_prey(void);

#endif /* INC_GAME_H_ */
