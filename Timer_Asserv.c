


#include <xc.h>
#include "main.h"

#include <timer.h>
#include "Timer_ms.h"


void Timer_Asserv_Init(void)
{
    
    // activation du Timer2
    OpenTimer2(T2_ON &
                T2_IDLE_CON &
                T2_GATE_OFF &
                T2_PS_1_64 &
                T2_SOURCE_INT, 12500 ); // 12500 pour 20ms
    // configuration des interruptions
    ConfigIntTimer2(T2_INT_PRIOR_4 & T2_INT_ON);
}


// 5 ms
void __attribute__((interrupt,auto_psv)) _T2Interrupt(void) {
    
   /*
    // compteurs QEI gauche et droit
    static int tics_g, tics_d;
    // commandes gauches et droite
    static float commande_g, commande_d;

    // récupération des données des compteurs qei gauche et droit
    tics_g = (int)POS1CNT;
    tics_d = (int)POS2CNT;
    // effectuer un pas de déplacement
    motion_step(tics_g,tics_d, &commande_g, &commande_d);
    // mettre ici les pwm gauche et droit
    PWM_Moteurs(commande_g, commande_d); */

    float pulse = 1.5;
    int pulse_tic = 625*pulse;

    PIN_SERVO = 1;

    Timer_ms_Init(pulse_tic);

   _T2IF = 0;   // on baisse le flag
}
