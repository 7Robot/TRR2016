#ifndef __TIMER_MS_H
#define __TIMER_MS_H

#define PIN_SERVO       _LATB7            // RB7 = connecteur 5V n4
#define TRIS_SERVO      _TRISB7
#define PIN_CN_SERVO_IE _CN23IE
//#define PULL_SERVO_G  _CN23PUE
#define SERVO_NUM_PIN   7

extern volatile char Delay_90_Over;

void Timer_ms_Init(int);

//unsigned int Get_Maxtime_Seq_AX12(void);
//void Set_Maxtime_Seq_AX12(unsigned int val);
//
//void Set_Maxtime(unsigned int val);
//char Get_Maxtime(void);

//ms
#define MAX_DELAY_WATCHDONE  7000   
void Raz_Delay_WatchDone(void);

#endif