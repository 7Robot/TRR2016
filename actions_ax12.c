#include <p33Fxxxx.h>      /* Includes device header file                     */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <stdio.h>
#include <uart.h>
#include <delay.h>

#include "main.h"


#define maxtork_ax 850
#define maxspeed_ax 400
#define delay_min_ax 30

/******************************************************************************/
/*************************** Arm Specification ********************************/
/******************************************************************************/

// PIN TEAM = COTE D'EQUIPE VIOLET !!

/******************************************************************************/
/****************************** Init Position *********************************/
/******************************************************************************/

volatile int Num_Action_Done = 0;
volatile int Num_Action_ToDo = 0;
volatile char Tab_Actions_ToDo [NUM_ACTIONS_BUFFER] = {0};



void Add_Action_AX12(char Action_ToDo)
{
    int num = Num_Action_ToDo;
    num++;
    if (num == NUM_ACTIONS_BUFFER) {
        num = 0;
    }
    Tab_Actions_ToDo[num] = Action_ToDo;
    Num_Action_ToDo = num;
}

void Faire_Actions_AX12(void)
{
    int num = Num_Action_Done;
    char Action_ToDo;
    if (num != Num_Action_ToDo && !Delay_90_Over) {
        num++;
        if (num == NUM_ACTIONS_BUFFER) {
            num = 0;
        }
        Action_ToDo = Tab_Actions_ToDo[num];
        switch (Action_ToDo)
        {
            case AX12_INIT_AX12 :
                Init_ax12();
                break;
            case AX12_DEPLOY_WINGS_BAS :
                Deploy_wings_B();
                break;
            case AX12_DEPLOY_WINGS_HAUT :
                Deploy_wings_H();
                break;
            case AX12_DEPLOY_WINGS :
                Deploy_wings();
                break;
            case AX12_IBICF:
                I_Can_Fly();
                break;
            case POMPE_ACTIVER:
                Activer_Pompes();
                break;
            case POMPE_ACTIVER_BAS:
                Activer_Pompe_basse();
                break;
            case POMPE_ETEINDRE:
                Eteindre_Pompes();
                break;
            case AX12_CLOSE_WINGS_BAS :
                Close_wings_B();
                break;
            case AX12_CLOSE_WINGS_HAUT :
                Close_wings_H();
                break;
            case AX12_CLOSE_WINGS :
                Close_wings();
                break;
        }
        Num_Action_Done = num;
    }
}

void Init_ax12() {

    PutAX(Wing_1, 	AX_TORQUE_LIMIT, maxtork_ax);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_2, 	AX_TORQUE_LIMIT, maxtork_ax);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_3, 	AX_TORQUE_LIMIT, maxtork_ax);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_4, 	AX_TORQUE_LIMIT, maxtork_ax);
                        __delay_ms(delay_min_ax);

    PutAX(Wing_1, 	AX_MOVING_SPEED, maxspeed_ax);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_2, 	AX_MOVING_SPEED, maxspeed_ax);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_3, 	AX_MOVING_SPEED, maxspeed_ax);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_4, 	AX_MOVING_SPEED, maxspeed_ax);
                        __delay_ms(delay_min_ax);

                     
    PutAX(Wing_1,       AX_GOAL_POSITION, 280);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_2,       AX_GOAL_POSITION, 740);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_3,       AX_GOAL_POSITION, 270);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_4,       AX_GOAL_POSITION, 740);
                        __delay_ms(delay_min_ax);

}

// PIN TEAM = 1 pour violet, 0 pour vert

 void Deploy_wings_H(void)		// Deploie les ailes du haut.
 {
    PutAX(Wing_1,       AX_GOAL_POSITION, 800);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_2,       AX_GOAL_POSITION, 205);
                        __delay_ms(delay_min_ax);
 }

 void Deploy_wings_B(void)		// Deploie les ailes du bas.
 {
    PutAX(Wing_3,       AX_GOAL_POSITION, 819);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_4,       AX_GOAL_POSITION, 205);
                        __delay_ms(delay_min_ax);
 }


 void Deploy_wings(void)                // Deplois toutes les ailes.
 {
    PutAX(Wing_1,       AX_GOAL_POSITION, 800);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_2,       AX_GOAL_POSITION, 220);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_3,       AX_GOAL_POSITION, 814);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_4,       AX_GOAL_POSITION, 210);
                        __delay_ms(delay_min_ax);
    SendDone();
 }

  void Close_wings_B(void)                // Range toutes les ailes.
 {
    PutAX(Wing_3,       AX_GOAL_POSITION, 270);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_4,       AX_GOAL_POSITION, 740);
                        __delay_ms(delay_min_ax);
 }

   void Close_wings_H(void)                // Range toutes les ailes.
 {
    PutAX(Wing_1,       AX_GOAL_POSITION, 280);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_2,       AX_GOAL_POSITION, 740);
                        __delay_ms(delay_min_ax);
 }

 void Close_wings(void)                // Range toutes les ailes.
 {
    PutAX(Wing_1,       AX_GOAL_POSITION, 280);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_2,       AX_GOAL_POSITION, 740);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_3,       AX_GOAL_POSITION, 270);
                        __delay_ms(delay_min_ax);
    PutAX(Wing_4,       AX_GOAL_POSITION, 740);
                        __delay_ms(delay_min_ax);
    SendDone();
 }

 void I_Can_Fly(void)		// A vous de jouer !!! ;)
{
    PutAX(Wing_1, AX_GOAL_POSITION, 800);
    __delay_ms(delay_min_ax);

    PutAX(Wing_3, AX_GOAL_POSITION, 819);
    __delay_ms(500);
    
    PutAX(Wing_1, AX_GOAL_POSITION, 280);
    __delay_ms(delay_min_ax);

    PutAX(Wing_3, AX_GOAL_POSITION, 270);
    __delay_ms(delay_min_ax);
    
    PutAX(Wing_2, AX_GOAL_POSITION, 205);
    __delay_ms(delay_min_ax);

    PutAX(Wing_4, AX_GOAL_POSITION, 210);
    __delay_ms(500)

    PutAX(Wing_2, AX_GOAL_POSITION, 740);
    __delay_ms(delay_min_ax);

    PutAX(Wing_4, AX_GOAL_POSITION, 740);
    __delay_ms(delay_min_ax);
 }

 void Activer_Pompes(void)
 {
        POMPE_HAUTE = 1;
        __delay_ms(50);
        POMPE_BASSE = 1;
        __delay_ms(50);
 }

  void Eteindre_Pompes(void)
 {
        POMPE_HAUTE = 0;
        __delay_ms(50);
        POMPE_BASSE = 0;
        __delay_ms(50);
 }

 void Activer_Pompe_basse(void)
 {
        POMPE_BASSE = 1;
        __delay_ms(50);
 }


 

/******************************************************************************/