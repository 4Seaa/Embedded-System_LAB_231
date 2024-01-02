/*
 * game.c
 *
 *  Created on: Jan 1, 2024
 *      Author: Asus
 */

#include "game.h"

// srand(static_cast<unsigned int>(time(NUL)));

int prey_x = 35;
int prey_y = 75;

// srand(time(NULL));
// srand(static_cast<unsigned int>(time(nullptr)));

// int prey_x = 15 + (rand() % ((225 - 15) / 5 + 1)) * 5;

uint8_t time_counter0 = 0;
uint8_t time_counter1 = 0;

int remain_prey = 3;

state fsm_state = INIT;
dir snake_dir;
dir next_dir;

uint16_t snake_x[100];
uint16_t snake_y[100];
int snake_len;

void snake_init(void) {
  snake_dir = RIGHT;
  next_dir = RIGHT;
  int i;
  snake_x[0] = 25;
  snake_y[0] = 55;
  snake_x[1] = 15;
  snake_y[1] = 55;
  for (i = 2; i < 100; i++) {
    snake_x[i] = -1;
    snake_y[i] = -1;
  }
  snake_len = 1;
}

uint8_t isStart(void) {
  if (!touch_IsTouched()) return 0;
  return touch_GetX() > 60 && touch_GetX() < 180 && touch_GetY() > 10 &&
         touch_GetY() < 60;
}

void update_fsm_state(void) {
  switch (fsm_state) {
    case START:
      if (isStart()) {
        fsm_state = PLAY;
        lcd_Clear(BLACK);
      }
      break;
    case PLAY:

      break;
    default:
      break;
  }
}

void top(void) {
  UI();
  // update_fsm_state();

  switch (fsm_state) {
    case INIT:
      fsm_state = START;
      break;
    case START:
      if (isStart()) {
        fsm_state = PLAY;
        lcd_Clear(WHITE);
        snake_init();
      }
      break;
    case PLAY:
      isNavigated();

      time_counter1 = (time_counter1 + 1) % 20;
      if (time_counter1 == 0) {
        eat_prey();
        handle_snake_dir();
      }

      switch (snake_dir) {
        case IDLE:
          // handle_snake_dir();
          break;
        case UP:
          if (next_dir != DOWN && next_dir != snake_dir) {
            snake_dir = next_dir;
          } else {
            if ((snake_dir == UP) && (snake_y[0] - HIT < LIMIT_UP)) {
              fsm_state = LOSE;
              lcd_Clear(WHITE);
            }
          }
          break;
        case DOWN:
          if (next_dir != UP && next_dir != snake_dir) {
            snake_dir = next_dir;
          } else {
            if ((snake_dir == DOWN) && (snake_y[0] + HIT > LIMIT_DOWN)) {
              fsm_state = LOSE;
              lcd_Clear(WHITE);
            }
          }
          break;
        case LEFT:
          if (next_dir != RIGHT && next_dir != snake_dir) {
            snake_dir = next_dir;
          } else {
            if ((snake_dir == LEFT) && (snake_x[0] - HIT < LIMIT_LEFT)) {
              fsm_state = LOSE;
              lcd_Clear(WHITE);
            }
          }
          break;
        case RIGHT:
          if (next_dir != LEFT && next_dir != snake_dir) {
            snake_dir = next_dir;
          } else {
            if ((snake_dir == RIGHT) && (snake_x[0] + HIT > LIMIT_RIGHT)) {
              fsm_state = LOSE;
              lcd_Clear(WHITE);
            }
          }
          break;
        default:
      }
      break;
    case LOSE:

      break;
    case WIN:

      break;
    default:
      break;
  }
}

void eat_prey(void) {
  if (prey_x == snake_x[0] && prey_y == snake_y[0]) {
    snake_len = snake_len + 1;
    remain_prey = remain_prey - 1;
    if (remain_prey == 0) {
      snake_dir = IDLE;
      fsm_state = WIN;
      lcd_Clear(WHITE);
    } else {
      prey_x = prey_x + HIT;
      prey_y = prey_y + HIT;
    }
  }
}

void snake_move(void) {
  lcd_Fill(10, 50, 230, 150, BLACK);

  lcd_DrawCircle(snake_x[0], snake_y[0], GREEN, 1, 1);

  int i;
  for (i = 0; i < snake_len; i++) {
    lcd_DrawLine(snake_x[i], snake_y[i], snake_x[i + 1], snake_y[i + 1], RED);
  }
}

void UI(void) {
  switch (fsm_state) {
    case INIT:
      lcd_Fill(60, 10, 180, 60, GBLUE);
      lcd_ShowStr(90, 20, "START", RED, BLACK, 24, 1);
      break;
    case PLAY:
      lcd_Fill(100, 260, 140, 280, BLUE);
      lcd_StrCenter(0, 260, "UP", RED, BLUE, 16, 1);

      lcd_Fill(100, 300, 140, 320, BLUE);
      lcd_StrCenter(0, 300, "DOWN", RED, BLUE, 16, 1);

      lcd_Fill(60, 280, 100, 300, BLUE);
      lcd_ShowStr(65, 280, "LEFT", RED, BLUE, 16, 1);

      lcd_Fill(140, 280, 180, 300, BLUE);
      lcd_ShowStr(142, 280, "RIGHT", RED, BLUE, 16, 1);

      lcd_StrCenter(0, 200, "REMAINING PREY:", BLACK, WHITE, 16, 1);
      lcd_ShowIntNum(180, 200, remain_prey, 1, BLUE, WHITE, 16);

      lcd_DrawCircle(prey_x, prey_y, YELLOW, 1, 1);

      snake_move();
      break;
    case LOSE:
      // lcd_Fill(10, 50, 230, 150, BLACK);

      lcd_StrCenter(0, 75, "LOSE!", RED, BLACK, 32, 1);
      break;

    case WIN:
      // time_counter0 = (time_counter0 + 1) % 60;
      // if (time_counter0 == 0) {

      // lcd_Fill(10, 50, 230, 150, BLACK);

      lcd_StrCenter(0, 75, "WIN!", RED, BLACK, 32, 1);
      // }
      break;
    default:
  }
}

void isNavigated(void) {
  if (!touch_IsTouched())
    ;
  else if (touch_GetX() > 100 && touch_GetX() < 140 && touch_GetY() > 260 &&
           touch_GetY() < 280)
    next_dir = UP;
  else if (touch_GetX() > 100 && touch_GetX() < 140 && touch_GetY() > 300 &&
           touch_GetY() < 320)
    next_dir = DOWN;
  else if (touch_GetX() > 60 && touch_GetX() < 100 && touch_GetY() > 280 &&
           touch_GetY() < 300)
    next_dir = LEFT;
  else if (touch_GetX() > 140 && touch_GetX() < 180 && touch_GetY() > 280 &&
           touch_GetY() < 300)
    next_dir = RIGHT;
}

void handle_snake_dir(void) {
  int i;
  for (i = snake_len; i > 0; i--) {
    snake_x[i] = snake_x[i - 1];
    snake_y[i] = snake_y[i - 1];
  }

  switch (snake_dir) {
    case UP:
      snake_y[0] = snake_y[1] - HIT;
      break;
    case DOWN:
      snake_y[0] = snake_y[1] + HIT;
      break;
    case LEFT:
      snake_x[0] = snake_x[1] - HIT;
      break;
    case RIGHT:
      snake_x[0] = snake_x[1] + HIT;
      break;
    default:
  }
}
