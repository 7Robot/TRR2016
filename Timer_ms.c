


#include <xc.h>
#include "main.h"

#include <timer.h>

volatile char Active_Delay_90 = 0;
volatile long Delay_90 = 0;

volatile char Delay_90_Over = 0;
volatile long Delay_WatchDone = 0;

volatile int Voiture_Must_Run  = 0;
extern volatile uint8_t cpt_arrivee;
extern volatile uint8_t fin_course;

void Timer_ms_Init(void)
{
	// activation du Timer3
    OpenTimer3(T3_ON &
                T3_IDLE_CON &
                T3_GATE_OFF &
                T3_PS_1_64 &
                T3_SOURCE_INT, 625 ); // 625 pour 1ms
    // configuration des interruptions
    ConfigIntTimer3(T3_INT_PRIOR_1 & T3_INT_ON);
    //IPC2bits.T3IP = 7
	
}



//volatile unsigned int Maxtime_Seq_AX12 = 0;
//volatile unsigned int Loc_Maxtime = 0;
//
//unsigned int Get_Maxtime_Seq_AX12(void)
//{   return Maxtime_Seq_AX12;    }
//
//void Set_Maxtime_Seq_AX12(unsigned int val)
//{   Maxtime_Seq_AX12 = val;    }
//
//void Set_Maxtime(unsigned int val)
//{    Loc_Maxtime = val; }
//
//char Get_Maxtime(void)
//{
//    if (Loc_Maxtime)
//        return 1;
//    else
//        return 0;
//}

void Raz_Delay_WatchDone (void)
{   Delay_WatchDone = 0;   }


// every ms
void __attribute__((interrupt,auto_psv)) _T3Interrupt(void) 
{


    static uint8_t Etat_Laisse = 0;
    static uint8_t Count_Laisse = 0;
    uint8_t Etat_Pin_Laisse = PIN_LAISSE;
    //static char count_Evit = 10;

    if (Etat_Pin_Laisse) {
        if (Count_Laisse < 30)
            Count_Laisse ++;
    } else {
        if (Count_Laisse)
            Count_Laisse --;
    }

    if (Etat_Laisse) {
        if (!Count_Laisse) {
            Etat_Laisse = 0;
            Active_Delay_90 = 1;
            Voiture_Must_Run = 1;
            cpt_arrivee = 0;
            fin_course = 0;
            //IPC2bits.T3IP = 7;  // passage de cette IT en haute priorit�, pour ne pas perdre le compte
            // pour la TRR2016 pas besoin de pr�cision sur le d�lais max...
        }
    } else {
        if (Count_Laisse == 30) {
            Etat_Laisse = 1;
            Active_Delay_90 = 0;
            Delay_90 = 0;
            Voiture_Must_Run = 0;
            IPC2bits.T3IP = 1;
        }
    }

////    count_Evit--;
////    if (!count_Evit) {
////        Must_do_Gestion_Evitement();
////        count_Evit = 10;
////    }
//
////    if (Delay_TimeOut_AX12) {
////        Delay_TimeOut_AX12 --;
////    }
////    if (Maxtime_Seq_AX12) {
////        Maxtime_Seq_AX12 --;
////    }
////    if (Loc_Maxtime)
////        Loc_Maxtime--;
//
//	if (asserv_mode != ASSERV_MODE_OFF) {
//		if (Delay_WatchDone >= MAX_DELAY_WATCHDONE){
//            motion_free();
//			SendDone();
//		} else {
//			Delay_WatchDone ++;
//		}
//	}
//

    if (Delay_90 < 90000) {
        if (Active_Delay_90) {
            Delay_90 ++;
        } else {
            Delay_90 = 0;
        }
        Delay_90_Over = 0;
    } else if (Delay_90 == 90000) {
        Delay_90 ++;
        // IPC2bits.T3IP = 1;      // repassage low priority...
        // Delay_90_Over = 1;
    } else {
        Delay_90_Over = 1;
        if (!Active_Delay_90) {
            Delay_90 = 0;
        }
    }



   _T3IF = 0;   // on baisse le flag
}

