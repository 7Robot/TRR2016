


#include <xc.h>
#include "main.h"

#include <timer.h>
#include "Timer_ms.h"

volatile int direction = 0;  //doit être compris entre -45° et 45°
extern volatile int Voiture_Must_Run;

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
    ConfigIntTimer5(T5_INT_PRIOR_2 & T5_INT_ON);
    
}


// 20 ms
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

    float pulse = (direction + 90)/90 + 0.5;
    
    // 2 ms => tourne à gauche
    // 1 ms => tourne à droite (gentillement)
    // 0.9 ms => tourne à droite, bien...
   
    
    if (Voiture_Must_Run) {
        PIN_SERVO = 1;
        PWM_Moteurs (0.1, 0);   //  max à 0.5...
        Demarrage_T5(pulse);
    }
    else {
         PWM_Moteurs (0, 0);
         PIN_SERVO = 0;
    }
    
   _T2IF = 0;   // on baisse le flag
}

void Demarrage_T5(float temps_ms) {
    long Var_Timer = 0;
    // 625 = 1 ms;
    temps_ms = temps_ms * 625;
    
    Var_Timer = temps_ms ;
    if (Var_Timer > 0xFFFF) {       // max matos à 13 ms environ...
        Var_Timer = 0xFFFF;
    } else if (Var_Timer < 10) {
        Var_Timer = 10;
    }
    
    TMR5 = 0;   // RAZ du timer
    PR5 = Var_Timer;    // var de fin du timer
    T5CONbits.TON = 1;  // démarrage du Timer..
    
}

void __attribute__((interrupt,auto_psv)) _T5Interrupt(void) {
    T5CONbits.TON = 0;
    PIN_SERVO = 0;
    _T5IF = 0;
}

