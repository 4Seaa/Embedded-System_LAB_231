/*
 * fsm.c
 *
 *  Created on: Nov 20, 2023
 *      Author: Asus
 */
#include "fsm.h"

int count_led;
int count_clearLCD;

fsm_state state;

fsm_state *get_fsm(void)
{
    return &state;
}

void top_state_update(void)
{
    fsm_state *fsm = get_fsm();

    if (button_count[0] > 0)
    {
        fsm->next_mode = (fsm->current_mode + 1) % 4;

        if (fsm->next_mode != NORMAL)
        {
            fsm->temp = fsm->led_count[fsm->next_mode - 1];
        }
    }
}

void top_init(void)
{
    fsm_state *fsm = get_fsm();

    fsm->current_mode = NORMAL;
    fsm->next_mode = NORMAL;

    fsm->temp = 0;

    fsm->led_count[RED_LED] = 6;
    fsm->led_count[GREEN_LED] = 4;
    fsm->led_count[YELLOW_LED] = 2;

    count_led = 0;
    count_clearLCD = 0;
}

void top(void)
{
    fsm_state *fsm = get_fsm();

    fsm->current_mode = fsm->next_mode;

    switch (fsm->current_mode)
    {
    case NORMAL:
        normal_mode();

        break;
    case RED_MODIFIED:
        if (button_count[1] > 0)
        {
            if (fsm->temp == 99)
            {
                // Display ERROR
            }
            else
            {
                fsm->temp += 1;
            }
        }
        else if (button_count[2] > 0)
        {
            if (fsm->temp == 1)
            {
                // Display ERROR
            }
            else
            {
                fsm->temp -= 1;
            }
        }
        else if (button_count[3] > 0)
        {
            fsm->led_count[RED_LED] = fsm->temp;
        }
        break;
    case GREEN_MODIFIED:
        if (button_count[1] > 0)
        {
            if (fsm->temp == 99)
            {
                // Display ERROR
            }
            else
            {
                fsm->temp += 1;
            }
        }
        else if (button_count[2] > 0)
        {
            if (fsm->temp == 1)
            {
                // Display ERROR
            }
            else
            {
                fsm->temp -= 1;
            }
        }
        else if (button_count[3] > 0)
        {
            fsm->led_count[GREEN_LED] = fsm->temp;
        }
        break;
    case YELLOW_MODIFIED:
        if (button_count[1] > 0)
        {
            if (fsm->temp == 99)
            {
                // Display ERROR
            }
            else
            {
                fsm->temp += 1;
            }
        }
        else if (button_count[2] > 0)
        {
            if (fsm->temp == 1)
            {
                // Display ERROR
            }
            else
            {
                fsm->temp -= 1;
            }
        }
        else if (button_count[3] > 0)
        {
            fsm->led_count[YELLOW_LED] = fsm->temp;
        }
        break;
    default:
    }
}

void trafficLight(void)
{
    lcd_StrCenter(0, 0, "Traffic Light", RED, BLUE, 16, 0);

    lcd_DrawCircle(100, 120, RED, 16, 1);
    lcd_DrawCircle(130, 120, GREEN, 16, 1);
    lcd_DrawCircle(160, 120, YELLOW, 16, 1);

    // lcd_ShowIntNum(160, 120, 99, 2, RED, GRAY, 12);
}

void normal_mode(void)
{
    fsm_state *fsm = get_fsm();

    // count_led = (count_led + 1) % ((fsm->led_count[RED] + fsm->led_count[GREEN] + fsm->led_count[YELLOW]) * TIME_TICK);
    count_led = (count_led + 1) % 240;

    if (count_led < 6 * 20)
    {
        // RED ON
        if (count_led % 20 == 0)
        {
            led7_SetDigit((5 - (count_led / TIME_TICK)) / 10, 0, 0);
            led7_SetDigit((5 - (count_led / TIME_TICK)) % 10, 1, 0);
        }
    }
    else if (count_led < 4 * TIME_TICK)
    {
        // GREEN ON
        if (count_led % 20 == 0)
        {
            led7_SetDigit((9 - (count_led / TIME_TICK)) / 10, 0, 0);
            led7_SetDigit((9 - (count_led / TIME_TICK)) % 10, 1, 0);
        }
    }
    else
    {
        // YELLOW ON
        if (count_led % 20 == 0)
        {
            led7_SetDigit((11 - (count_led / TIME_TICK)) / 10, 0, 0);
            led7_SetDigit((11 - (count_led / TIME_TICK)) % 10, 1, 0);
        }
    }

    // if (count_led < (fsm->led_count[RED] * 20))
    // {
    //     // RED ON
    //     if (count_led % 20 == 0)
    //     {
    //         led7_SetDigit((fsm->led_count[RED] - (count_led / TIME_TICK)) / 10, 0, 0);
    //         led7_SetDigit((fsm->led_count[RED] - (count_led / TIME_TICK)) % 10, 1, 0);
    //     }
    // }
    // else if (count_led < ((fsm->led_count[RED] + fsm->led_count[GREEN]) * TIME_TICK))
    // {
    //     // GREEN ON
    //     if (count_led % 20 == 0)
    //     {
    //         led7_SetDigit((fsm->led_count[RED] + fsm->led_count[GREEN] - (count_led / TIME_TICK)) / 10, 0, 0);
    //         led7_SetDigit((fsm->led_count[RED] + fsm->led_count[GREEN] - (count_led / TIME_TICK)) % 10, 1, 0);
    //     }
    // }
    // else
    // {
    //     // YELLOW ON
    //     if (count_led % 20 == 0)
    //     {
    //         led7_SetDigit((fsm->led_count[RED] + fsm->led_count[GREEN] + fsm->led_count[YELLOW] - (count_led / TIME_TICK)) / 10, 0, 0);
    //         led7_SetDigit((fsm->led_count[RED] + fsm->led_count[GREEN] + fsm->led_count[YELLOW] - (count_led / TIME_TICK)) % 10, 1, 0);
    //     }
    // }
}

int cal_countdown(int curr_time)
{
    fsm_state *fsm = get_fsm();
}