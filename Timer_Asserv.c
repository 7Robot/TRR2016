


#include <xc.h>
#include "main.h"

#include <timer.h>
#include "Timer_ms.h"

volatile int direction = 0;  //doit �tre compris entre -45� et 45�
    
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
    
    // activation du Timer5 : retour de la pin du servo moteur..
    // pour l'instant OFF
    OpenTimer5(T5_OFF &
                T5_IDLE_STOP &
                T5_GATE_OFF &
                T5_PS_1_64 &
                T5_SOURCE_INT, 500 );  // pour l'instant inutile...
    _T5IF = 0;
    // configuration des interruptions
    ConfigIntTimer2(T5_INT_PRIOR_2 & T5_INT_ON);
    
}


// 20 ms
void __attribute__((interrupt,auto_psv)) _T2Interrupt(void) {
    
   /*
    // compteurs QEI gauche et droit
    static int tics_g, tics_d;
    // commandes gauches et droite
    static float commande_g, commande_d;

    // r�cup�ration des donn�es des compteurs qei gauche et droit
    tics_g = (int)POS1CNT;
    tics_d = (int)POS2CNT;
    // effectuer un pas de d�placement
    motion_step(tics_g,tics_d, &commande_g, &commande_d);
    // mettre ici les pwm gauche et droit
    PWM_Moteurs(commande_g, commande_d); */

    float pulse = 1.5;
<<<<<<< HEAD
    
=======
    pulse = (direction + 90)/90 + 0.5;
    int pulse_tic = 625*pulse;

>>>>>>> b0d071ea41b37e855369ea6b72f2b5426ad3bc4d
    PIN_SERVO = 1;
    
    Demarrage_T5(pulse);
    
   _T2IF = 0;   // on baisse le flag
}

void Demarrage_T5(float temps_ms) {
    long Var_Timer = 0;
    // 625 = 1 ms;
    temps_ms = temps_ms * 625;
    
    Var_Timer = temps_ms ;
    if (Var_Timer > 0xFFFF) {
        Var_Timer = 0xFFFF;
    } else if (Var_Timer < 10) {
        Var_Timer = 10;
    }
    
    TMR5 = 0;   // RAZ du timer
    PR5 = Var_Timer;    // var de fin du timer
    T5CONbits.TON = 1;  // d�marrage du Timer..
    
}

void __attribute__((interrupt,auto_psv)) _T5Interrupt(void) {
    T5CONbits.TON = 0;
    PIN_SERVO = 0;
    _T5IF = 0;
}

