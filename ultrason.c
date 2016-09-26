/*
* UltraSon
* Compiler : Microchip xC16
* µC : 33FJ64MC804
* Mai 2015
*    ____________      _           _
*   |___  /| ___ \    | |         | |
*      / / | |_/ /___ | |__   ___ | |_
*     / /  |    // _ \| '_ \ / _ \| __|
*    / /   | |\ \ (_) | |_) | (_) | |_
*   /_/    |_| \_\___/|____/ \___/'\__|
*			      7robot.fr
*/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdio.h>
#include <stdbool.h>       /* Includes true/false definition                  */

#include "main.h"
#include <timer.h>
#include <math.h>

// principe : petit spike sur la pin (qq usecs) pin en sortie
// puis attente, avec mesure du temps à 1       pin en entrée
// attente 50 ms entre 2 spikes
volatile uint8_t cpt_arrivee;

volatile uint8_t Etat_Ultrason = 0;

volatile uint16_t Mesure_Timer_Ultrason_G_Start = 0;
volatile uint16_t Mesure_Timer_Ultrason_G_End = 0;
volatile uint16_t Mesure_Timer_Ultrason_D_Start = 0;
volatile uint16_t Mesure_Timer_Ultrason_D_End = 0;
volatile uint16_t Mesure_Timer_Ultrason_H_Start = 0;
volatile uint16_t Mesure_Timer_Ultrason_H_End = 0;

volatile uint16_t Mesure_Distance_Ultrason_G = 3000;  // distance en mm
volatile uint16_t Mesure_Distance_Ultrason_D = 3000;  // distance en mm
volatile uint16_t Mesure_Distance_Ultrason_H = 3000;  // distance en mm

uint8_t Ultrason_G_Detect = 0;
uint8_t Ultrason_D_Detect = 0;
uint8_t Ultrason_H_Detect = 0;

volatile int Threshold_US = ULTRASON_THRESOLD;

// debug
//volatile uint8_t Debug_Ultrason = 0;
//volatile uint16_t count_Debug_Ultrason = 0;

volatile uint16_t Last_For_Degug_Mesure_Timer_Ultrason_G_Start = 0;
volatile uint16_t Last_For_Degug_Mesure_Timer_Ultrason_G_End = 0;
volatile uint16_t Last_For_Degug_Mesure_Timer_Ultrason_D_Start = 0;
volatile uint16_t Last_For_Degug_Mesure_Timer_Ultrason_D_End = 0;
volatile uint16_t Last_For_Degug_Mesure_Timer_Ultrason_H_Start = 0;
volatile uint16_t Last_For_Degug_Mesure_Timer_Ultrason_H_End = 0;

// pas en externe
volatile uint8_t nb_Coups_Timers = 0;

void Init_Ultrasons (void)
{
    PIN_CN_ULTRASON_G_IE = 0;     // desactivation de l'IT
    PIN_CN_ULTRASON_D_IE = 0;     // desactivation de l'IT
    PIN_CN_ULTRASON_H_IE = 0;     // desactivation de l'IT

    TRIS_ULTRASON_G = 1;          // mise en input de la pin
    TRIS_ULTRASON_D = 1;          // mise en input de la pin
    TRIS_ULTRASON_H = 1;          // mise en input de la pin
    
    CloseTimer4();
    __delay_ms(5); // attente de 5 ms pour bien tuer l'IT
    Etat_Ultrason = U_ETAT_OFF;
    nb_Coups_Timers = 0;
    
    Ultrason_G_Detect = 0;
    Ultrason_D_Detect = 0;
    Ultrason_H_Detect = 0;
    Threshold_US = ULTRASON_THRESOLD;
    
    OpenTimer4(T4_ON & T4_GATE_OFF & T4_PS_1_8 & T4_SOURCE_INT, 0xFFFF );
    // FCY = 40Meg   prescaler à 8 donc F timer = 5Meg
    // 1 coup = 200 ns = 0.2us
    // max = 65535 => 13.107 ms     // ce qui correspondra à de l'overshoot

    // configuration des interruptions
    ConfigIntTimer4(T4_INT_PRIOR_2 & T4_INT_ON); 
    Init_CN();
    Etat_Ultrason = U_ETAT_FOR_SEND1;

}

void Init_CN()
{
    IPC4bits.CNIP = 6;      // Interrupt level 6 
    IFS1bits.CNIF = 0;      // Reset CN interrupt
    IEC1bits.CNIE = 1;      // Enable CN interrupts
}


void __attribute__((interrupt,auto_psv)) _T4Interrupt(void) {
    // attente pour envoi : on envoie et on passe à l'état suivant
    if (Etat_Ultrason & U_ETAT_FOR_SEND1) {
        //count_Debug_Ultrason++;
        //if (count_Debug_Ultrason == 50) {
        //    count_Debug_Ultrason = 0;
        //}
        //if (!count_Debug_Ultrason && Debug_Ultrason) { printf("$START_SPIKE;"); }
        PIN_ULTRASON_G = 1;
        TRIS_ULTRASON_G = 0;      // pin en sortie
        PIN_ULTRASON_D = 1;
        TRIS_ULTRASON_D = 0;      // pin en sortie
        PIN_ULTRASON_H = 1;
        TRIS_ULTRASON_H = 0;      // pin en sortie
        PR4 = 200;              // 40 us pour le spike de début
        Etat_Ultrason = U_ETAT_SEND1;
    } else if (Etat_Ultrason & U_ETAT_SEND1) {      // spike en cours => il faut le finir + lancer le truc de mesure
        //if (!count_Debug_Ultrason && Debug_Ultrason) { printf("$END_SPIKE;"); }
        PIN_ULTRASON_G = 0;
        PIN_ULTRASON_D = 0;         // Pins en entree
        PIN_ULTRASON_H = 0;
        Etat_Ultrason = U_ETAT_WAIT;
        PR4 = 0xFFFF;           // 13.107 ms (overshoot)
        TMR4 = 0;   // reset du timer
        //nb_Coups_Timers = 0;
        IFS1bits.CNIF = 0; // Clear CN interrupt Flag
        PIN_CN_ULTRASON_G_IE = 1; // activation IT sur PIN
        TRIS_ULTRASON_G = 1;      // on relache la pin
        PIN_CN_ULTRASON_D_IE = 1; // activation IT sur PIN
        TRIS_ULTRASON_D = 1;      // on relache la pin
        PIN_CN_ULTRASON_H_IE = 1; // activation IT sur PIN
        TRIS_ULTRASON_H = 1;      // on relache la pin
    } else if (Etat_Ultrason & U_ETAT_WAIT) {
        // on a attendu 13.107 ms, on vient lire les résultats
        PIN_CN_ULTRASON_G_IE = 0;
        PIN_CN_ULTRASON_D_IE = 0;
        PIN_CN_ULTRASON_H_IE = 0;
        Last_For_Degug_Mesure_Timer_Ultrason_G_Start = Mesure_Timer_Ultrason_G_Start;
        Last_For_Degug_Mesure_Timer_Ultrason_D_Start = Mesure_Timer_Ultrason_D_Start;
        Last_For_Degug_Mesure_Timer_Ultrason_H_Start = Mesure_Timer_Ultrason_H_Start;
        Last_For_Degug_Mesure_Timer_Ultrason_H_End = Mesure_Timer_Ultrason_H_End;
        Last_For_Degug_Mesure_Timer_Ultrason_G_End = Mesure_Timer_Ultrason_G_End;
        Last_For_Degug_Mesure_Timer_Ultrason_D_End = Mesure_Timer_Ultrason_D_End;
        
        // à base de vitesse du son (/2 pour l'aller-retour)  340.29 m/s
        // => 1 coup = 34 us 
        // pour avoir distance en mm, il faut diviser par 29.39
        // donc multiplication par 1115 puis division par 32768 (2^15)
        // passage obligé en 32 bits
        uint32_t val32 = 0;
        if (Mesure_Timer_Ultrason_G_End) {
            val32 = Mesure_Timer_Ultrason_G_End - Mesure_Timer_Ultrason_G_Start;
            val32 = (val32 * 1115) >> 15;
            Mesure_Distance_Ultrason_G = (uint16_t)(val32);
        } else {
            Mesure_Distance_Ultrason_G = 2000;
        }
        if (Mesure_Timer_Ultrason_D_End) {
            val32 = Mesure_Timer_Ultrason_D_End - Mesure_Timer_Ultrason_D_Start;
            val32 = (val32 * 1115) >> 15;
            Mesure_Distance_Ultrason_D = (uint16_t)(val32);
        } else {
            Mesure_Distance_Ultrason_D = 2000;
        }

        ///
        /// Calcul de la commande angulaire du servo-moteur
        ///
        int delta_d = Mesure_Distance_Ultrason_D - Mesure_Distance_Ultrason_G;
        if (delta_d > DELTA_D_MAX)
            delta_d = DELTA_D_MAX;
        else if (delta_d < -DELTA_D_MAX)
            delta_d = -DELTA_D_MAX;
        else if (delta_d > -DELTA_D_MIN && delta_d < DELTA_D_MIN)
            delta_d = 0;
        direction = 0.5625 * delta_d;       // Coef de pente pour passer de [-80:80] à [-45:45]
        

        if (Mesure_Timer_Ultrason_H_End) {
            val32 = Mesure_Timer_Ultrason_H_End - Mesure_Timer_Ultrason_H_Start;
            val32 = (val32 * 1115) >> 15;
            Mesure_Distance_Ultrason_H = (uint16_t)(val32);
        } else {
            Mesure_Distance_Ultrason_H = 2000;
        }

        if (Ultrason_H_Detect) {
            if (Mesure_Distance_Ultrason_H > (Threshold_US + ULTRASON_THRESOLD_TRIGGER)) {
                Ultrason_H_Detect = 0;    // passage en zone ok
            }
        } else {
            if (Mesure_Distance_Ultrason_H < (Threshold_US - ULTRASON_THRESOLD_TRIGGER)) {
                Ultrason_H_Detect = 1; // passage en zone occupé
                cpt_arrivee++;
            }
        }

        /*
        if (Ultrason_G_Detect) {
            if (Mesure_Distance_Ultrason_G > (Threshold_US + ULTRASON_THRESOLD_TRIGGER)) {
                Ultrason_G_Detect = 0;    // passage en zone ok
                // ReleaseUltrason(1);         // on previent la PI
            }
        } else {
            if (Mesure_Distance_Ultrason_G < (Threshold_US - ULTRASON_THRESOLD_TRIGGER)) {
                Ultrason_G_Detect = 1; // passage en zone occupé
                // DetectUltrason(1);		// on previent la PI
            }
        }
        if (Ultrason_D_Detect) {
            if (Mesure_Distance_Ultrason_D > (Threshold_US + ULTRASON_THRESOLD_TRIGGER)) {
                Ultrason_D_Detect = 0;    // passage en zone ok
                // ReleaseUltrason(2);         // on previent la PI
            }
        } else {
            if (Mesure_Distance_Ultrason_D < (Threshold_US - ULTRASON_THRESOLD_TRIGGER)) {
                Ultrason_D_Detect = 1; // passage en zone occupé
                // DetectUltrason(2);		// on previent la PI
            }
        }
        */
        
        
        Mesure_Timer_Ultrason_G_Start = 0;
        Mesure_Timer_Ultrason_G_End = 0;
        Mesure_Timer_Ultrason_D_Start = 0;
        Mesure_Timer_Ultrason_D_End = 0;
        Mesure_Timer_Ultrason_H_Start = 0;
        Mesure_Timer_Ultrason_H_End = 0;
        
        nb_Coups_Timers = 1;
        Etat_Ultrason = U_ETAT_WAIT_FOR_RESTART;
    } else if (Etat_Ultrason & U_ETAT_WAIT_FOR_RESTART) {
        if (nb_Coups_Timers > 5) {
            //if (!PIN_ULTRASON_AV && !PIN_ULTRASON_AR)
                Etat_Ultrason = U_ETAT_FOR_SEND1;
            //if (!count_Debug_Ultrason && Debug_Ultrason) { printf("meas %dmm;", Mesure_Distance_Ultrason); }
        } else {
            nb_Coups_Timers ++;
        }
    }

    _T4IF = 0;  // baisse du flag
}

/**********************************************/
// CN interrupt for ultrasson 

void __attribute__ ((__interrupt__, no_auto_psv)) _CNInterrupt(void)
{
    // baisse le flag puis récup des etats de pins, 
    // si les pins rebougent durant ce laps tres court, ça redéclenchera une IT directe apres,
    uint8_t Etat_Port_C = PORTC;
    IFS1bits.CNIF = 0; // Clear CN interrupt
    uint8_t Etat_Pin_Ultrason_G = (Etat_Port_C & (1 << ULTRASON_G_NUM_PIN));
    uint8_t Etat_Pin_Ultrason_D = (Etat_Port_C & (1 << ULTRASON_D_NUM_PIN));
    uint8_t Etat_Pin_Ultrason_H = (Etat_Port_C & (1 << ULTRASON_D_NUM_PIN));
    

    // si Etat_Ultrason mérite que l'on s'occupe de lui
    if (Etat_Ultrason == U_ETAT_WAIT) {
        if (Etat_Pin_Ultrason_G) {
            if (!Mesure_Timer_Ultrason_G_Start) {
                Mesure_Timer_Ultrason_G_Start = TMR4;
            }
        } else if (!Mesure_Timer_Ultrason_G_End && Mesure_Timer_Ultrason_G_Start) {
            Mesure_Timer_Ultrason_G_End = TMR4;
            if (Mesure_Timer_Ultrason_G_End < Mesure_Timer_Ultrason_G_Start)
                Mesure_Timer_Ultrason_G_End = 0xFFFF;
            //PIN_CN_ULTRASON_AV_IE = 0;     // desactivation de cette IT
        }
        if (Etat_Pin_Ultrason_D) {
            if (!Mesure_Timer_Ultrason_D_Start) {
                Mesure_Timer_Ultrason_D_Start = TMR4;
            }
        } else if (!Mesure_Timer_Ultrason_D_End && Mesure_Timer_Ultrason_D_Start) {
            Mesure_Timer_Ultrason_D_End = TMR4;
            if (Mesure_Timer_Ultrason_D_End < Mesure_Timer_Ultrason_D_Start)
                Mesure_Timer_Ultrason_D_End = 0xFFFF;
            //PIN_CN_ULTRASON_AR_IE = 0;     // desactivation de cette IT
        }
        if (Etat_Pin_Ultrason_H) {
            if (!Mesure_Timer_Ultrason_H_Start) {
                Mesure_Timer_Ultrason_H_Start = TMR4;
            }
        } else if (!Mesure_Timer_Ultrason_H_End && Mesure_Timer_Ultrason_H_Start) {
            Mesure_Timer_Ultrason_H_End = TMR4;
            if (Mesure_Timer_Ultrason_H_End < Mesure_Timer_Ultrason_H_Start)
                Mesure_Timer_Ultrason_H_End = 0xFFFF;
            //PIN_CN_ULTRASON_AR_IE = 0;     // desactivation de cette IT
        }
    }
}

void Start_Stop_Debug_Ultrason(void)
{
//    if (Debug_Ultrason) {
//        Debug_Ultrason = 0;
//    } else {
//        Debug_Ultrason = 1;
//    }
}


// return 1 si rien devant
// return 0 si detection
int Get_US_Sector(int US)
{
    if (US == 0) {
        return (1-Ultrason_G_Detect);
    } else if (US == 1) {
        return (1-Ultrason_D_Detect);
    }
    else return 1;
}

void Set_Threshold_US(int limit_mm)
{
    Threshold_US = limit_mm;
}