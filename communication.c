/*
 * Template dsPIC33F
 * Compiler : Microchip xC16
 * µC : 33FJ128MC804
 * Avril 2016
 *
 *    ______ _____      _           _
 *   |___  /| ___ \    | |         | |
 *      / / | |_/ /___ | |__   ___ | |_
 *     / /  |    // _ \| '_ \ / _ \| __|
 *    / /   | |\ \ (_) | |_) | (_) | |_
 *   /_/    |_| \_\___/|____/ \___/'\__|
 *			      7robot.fr
 */

#include <uart.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "uart.h"

char ReceivedStringFromPi[50] = {0};
int CharFromPiNumber = 0;

void Init_Communication_RasPi(void)
{
        OpenUART1(UART_EN & UART_IDLE_CON & UART_IrDA_DISABLE & UART_MODE_FLOW
        & UART_UEN_00 & UART_DIS_WAKE & UART_DIS_LOOPBACK
        & UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & UART_BRGH_SIXTEEN
        & UART_NO_PAR_8BIT & UART_1STOPBIT,
          UART_INT_TX_BUF_EMPTY & UART_IrDA_POL_INV_ZERO
        & UART_SYNC_BREAK_DISABLED & UART_TX_ENABLE & UART_TX_BUF_NOT_FUL & UART_INT_RX_CHAR
        & UART_ADR_DETECT_DIS & UART_RX_OVERRUN_CLEAR,
          BRGVALRASPI);


    ConfigIntUART1(UART_RX_INT_PR6 & UART_RX_INT_EN
                 & UART_TX_INT_PR6 & UART_TX_INT_DIS);
    
        //Remapage uart 1
    _U1RXR = 18;
    _RP4R = 0b0011;  // RP4 = U1TX (p.167)
    
}


void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void){
    // Receive first Byte
    char b = ReadUART1();
    _U1RXIF = 0; // On baisse le FLAG
    AnalyzeCommandFromPi(b);
}

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void){
    _U1TXIF = 0; // clear TX interrupt flag
}



void AnalyzeCommandFromPi (char b)
{
    // If byte is "$" symbol, the string can be valid
    if (b == '$')
    {
        ReceivedStringFromPi[0] = b;
        CharFromPiNumber = 1;
    }
    // Here we are collecting all the char from the frame
    else if (CharFromPiNumber) {
        if (CharFromPiNumber == SIZE_BUFFER_COM) {
            CharFromPiNumber = 0;  // commande trop longue => on la drope, retourne attendre le prochain '$'
        } else if (b == ';') {     // si on a reçu le carractère de fin...
            ReceivedStringFromPi[CharFromPiNumber] = b;
            SelectActionFromPi();
        } else {
            ReceivedStringFromPi[CharFromPiNumber] = b;
            CharFromPiNumber++;
        }
    }
}

void SelectActionFromPi()
{
    int cursorPosition, floatLength;
    float ANGLE;
    float valf;
    Position MOVE;
    Speed VITESSE;
    uint8_t val8;
    char valc;
    
    if (!Delay_90_Over) {
        // INIT
        if(ReceivedStringFromPi[1]=='I'
	&& ReceivedStringFromPi[2]=='N'
	&& ReceivedStringFromPi[3]=='I'
	&& ReceivedStringFromPi[4]=='T')
        {
            Init_All(1);
        }

        // MOVE
        if(ReceivedStringFromPi[1]=='M' 
        && ReceivedStringFromPi[2]=='O'
	&& ReceivedStringFromPi[3]=='V' 
	&& ReceivedStringFromPi[4]=='E')
        {
            cursorPosition=6;
            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=',';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            MOVE.x = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ',';

            cursorPosition+=floatLength+1;
            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=',';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            MOVE.y = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ',';

            cursorPosition+=floatLength+1;
            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=';';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            MOVE.t = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ';';

            motion_pos(MOVE);
        }
        
        // PUSH
        if(ReceivedStringFromPi[1]=='P' 
	&& ReceivedStringFromPi[2]=='U' 
	&& ReceivedStringFromPi[3]=='S' 
	&& ReceivedStringFromPi[4]=='H')
        {
            cursorPosition=6;
            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=',';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            MOVE.x = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ',';

            cursorPosition+=floatLength+1;
            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=',';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            MOVE.y = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ',';

            cursorPosition+=floatLength+1;
            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=',';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            MOVE.t = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ',';
            cursorPosition+=floatLength+1;
            
            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=';';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            valf = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ';';

            motion_push(MOVE, valf);
        }
        
        // SPED
        if(ReceivedStringFromPi[1]=='S' 
		&& ReceivedStringFromPi[2]=='P' 
		&& ReceivedStringFromPi[3]=='E' 
		&& ReceivedStringFromPi[4]=='D')
        {
            cursorPosition=6;
            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=',';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            VITESSE.v = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ',';
            
            cursorPosition+=floatLength+1;
            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=',';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            VITESSE.vt = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ';';
            
            
            motion_linear_speed(VITESSE);
        }

        // ANGL
        if(ReceivedStringFromPi[1]=='A' 
	&& ReceivedStringFromPi[2]=='N' 
	&& ReceivedStringFromPi[3]=='G' 
	&& ReceivedStringFromPi[4]=='L')
        {
            cursorPosition=6;

            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=';';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            ANGLE = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ';';

            motion_angle(ANGLE);
        }

        // FREE
        if(ReceivedStringFromPi[1]=='F'
	&& ReceivedStringFromPi[2]=='R'
	&& ReceivedStringFromPi[3]=='E'
	&& ReceivedStringFromPi[4]=='E')
        {
            motion_free();
        }
        
        // SETX
        if(ReceivedStringFromPi[1]=='S' 
	&& ReceivedStringFromPi[2]=='E' 
	&& ReceivedStringFromPi[3]=='T' 
	&& ReceivedStringFromPi[4]=='X')
        {
            cursorPosition=6;

            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=';';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            valf = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ';';

            set_position_x(valf);
        }
        
        // SETY
        if(ReceivedStringFromPi[1]=='S' 
	&& ReceivedStringFromPi[2]=='E' 
	&& ReceivedStringFromPi[3]=='T' 
	&& ReceivedStringFromPi[4]=='Y')
        {
            cursorPosition=6;

            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=';';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            valf = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ';';

            set_position_y(valf);
        }
        
        // SETA
        if(ReceivedStringFromPi[1]=='S' 
	&& ReceivedStringFromPi[2]=='E' 
	&& ReceivedStringFromPi[3]=='T' 
	&& ReceivedStringFromPi[4]=='A')
        {
            cursorPosition=6;

            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=';';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            valf = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ';';

            set_position_t(valf);
        }

    //**************************************************************************//
    //************** PARTIE LIMITES DE VITESSES, ACCELERATIONS... **************//
    //**************************************************************************//

    // VMAX     choisi une vitesse max
    if(ReceivedStringFromPi[1]=='V'
    && ReceivedStringFromPi[2]=='M'
    && ReceivedStringFromPi[3]=='A'
    && ReceivedStringFromPi[4]=='X')
    {
        cursorPosition=6;
        for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=';';floatLength++); // Return the number of char taken by the float in the command line
        ReceivedStringFromPi[cursorPosition+floatLength] = 0;
        valf = atof(&ReceivedStringFromPi[cursorPosition]);
        ReceivedStringFromPi[cursorPosition+floatLength] = ';';
        set_Constraint_vitesse_max(valf);
    }

            // VTMA     choisi une vitesse angulaire max
    if(ReceivedStringFromPi[1]=='V'
    && ReceivedStringFromPi[2]=='T'
    && ReceivedStringFromPi[3]=='M'
    && ReceivedStringFromPi[4]=='A')
    {
        cursorPosition=6;
        for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=';';floatLength++); // Return the number of char taken by the float in the command line
        ReceivedStringFromPi[cursorPosition+floatLength] = 0;
        valf = atof(&ReceivedStringFromPi[cursorPosition]);
        ReceivedStringFromPi[cursorPosition+floatLength] = ';';
        set_Constraint_vt_max(valf);
    }

    // AMAX     choisi une acceleration max pour des mouvements 'doux'
    if(ReceivedStringFromPi[1]=='A'
    && ReceivedStringFromPi[2]=='M'
    && ReceivedStringFromPi[3]=='A'
    && ReceivedStringFromPi[4]=='X')
    {
        cursorPosition=6;
        for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=',';floatLength++); // Return the number of char taken by the float in the command line
        ReceivedStringFromPi[cursorPosition+floatLength] = 0;
        float al_max = atof(&ReceivedStringFromPi[cursorPosition]);
        ReceivedStringFromPi[cursorPosition+floatLength] = ',';

        cursorPosition+=floatLength+1;
        for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=',';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
        float at_max = atof(&ReceivedStringFromPi[cursorPosition]);
        ReceivedStringFromPi[cursorPosition+floatLength] = ',';

        cursorPosition+=floatLength+1;
        for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=',';floatLength++); // Return the number of char taken by the float in the command line
        ReceivedStringFromPi[cursorPosition+floatLength] = 0;
        float a_max = atof(&ReceivedStringFromPi[cursorPosition]);
        ReceivedStringFromPi[cursorPosition+floatLength] = ',';
        cursorPosition+=floatLength+1;

        set_Constraint_acceleration_max(al_max, at_max, a_max);
    }

    //**************************************************************************//
    //************** PARTIE ACTIONS AX12 ***************************************//
    //**************************************************************************//

        // AXIN
        if(ReceivedStringFromPi[1]=='A' 
	&& ReceivedStringFromPi[2]=='X' 
	&& ReceivedStringFromPi[3]=='I' 
	&& ReceivedStringFromPi[4]=='N')
        {
            Add_Action_AX12(AX12_INIT_AX12);
        }



        // OWIH
        if(ReceivedStringFromPi[1]=='O'
	&& ReceivedStringFromPi[2]=='W'
	&& ReceivedStringFromPi[3]=='I'
	&& ReceivedStringFromPi[4]=='H')
        {
            Add_Action_AX12(AX12_DEPLOY_WINGS_HAUT);
        }

        // OWIB
        if(ReceivedStringFromPi[1]=='O'
	&& ReceivedStringFromPi[2]=='W'
	&& ReceivedStringFromPi[3]=='I'
	&& ReceivedStringFromPi[4]=='B')
        {
            Add_Action_AX12(AX12_DEPLOY_WINGS_BAS);
        }

        // WINA
        if(ReceivedStringFromPi[1]=='O'
	&& ReceivedStringFromPi[2]=='W'
	&& ReceivedStringFromPi[3]=='I'
	&& ReceivedStringFromPi[4]=='A')
        {
            Add_Action_AX12(AX12_DEPLOY_WINGS);
        }

        // CWIH
        if(ReceivedStringFromPi[1]=='C'
	&& ReceivedStringFromPi[2]=='W'
	&& ReceivedStringFromPi[3]=='I'
	&& ReceivedStringFromPi[4]=='H')
        {
            Add_Action_AX12(AX12_CLOSE_WINGS_HAUT);
        }

        // CWIB
        if(ReceivedStringFromPi[1]=='C'
	&& ReceivedStringFromPi[2]=='W'
	&& ReceivedStringFromPi[3]=='I'
	&& ReceivedStringFromPi[4]=='B')
        {
            Add_Action_AX12(AX12_CLOSE_WINGS_BAS);
        }

        // CINA
        if(ReceivedStringFromPi[1]=='C'
	&& ReceivedStringFromPi[2]=='W'
	&& ReceivedStringFromPi[3]=='I'
	&& ReceivedStringFromPi[4]=='A')
        {
            Add_Action_AX12(AX12_CLOSE_WINGS);
        }

        // IFLY
        if(ReceivedStringFromPi[1]=='I'
	&& ReceivedStringFromPi[2]=='F'
	&& ReceivedStringFromPi[3]=='L'
	&& ReceivedStringFromPi[4]=='Y')
        {
            Add_Action_AX12(AX12_IBICF);
        }

        //POMA
        if(ReceivedStringFromPi[1]=='P'
	&& ReceivedStringFromPi[2]=='O'
	&& ReceivedStringFromPi[3]=='M'
	&& ReceivedStringFromPi[4]=='A')
        {
            Add_Action_AX12(POMPE_ACTIVER);
        }

        //POMD
        if(ReceivedStringFromPi[1]=='P'
	&& ReceivedStringFromPi[2]=='O'
	&& ReceivedStringFromPi[3]=='M'
	&& ReceivedStringFromPi[4]=='D')
        {
            Add_Action_AX12(POMPE_ETEINDRE);
        }

        //POAB
        if(ReceivedStringFromPi[1]=='P'
	&& ReceivedStringFromPi[2]=='O'
	&& ReceivedStringFromPi[3]=='A'
	&& ReceivedStringFromPi[4]=='B')
        {
            Add_Action_AX12(POMPE_ACTIVER_BAS);
        }

    //**************************************************************************//
    //************** PARTIE ULTRASONS ******************************************//
    //**************************************************************************//

        // ENUS         // active ou pas le motion_free des Ultrasons  à l'unitée
        if(ReceivedStringFromPi[1]=='E'
        && ReceivedStringFromPi[2]=='N'
        && ReceivedStringFromPi[3]=='U'
        && ReceivedStringFromPi[4]=='S')
        {
            // l'utilisateur a juste droit à de 0 à F
            valc = ReceivedStringFromPi[6];
            if (valc >= '0' && valc <= '3') {
                valc -= '0';
                Choose_Enabled_US(valc);
            }
        }

        // ULS?			// demande status ultrasons
        if(ReceivedStringFromPi[1]=='U'
	&& ReceivedStringFromPi[2]=='L'
	&& ReceivedStringFromPi[3]=='S'
	&& ReceivedStringFromPi[4]=='?')
        {
            SendUltrason_Status();
        }

        // DBUS			// start/stop debug ultrason
        if(ReceivedStringFromPi[1]=='D'
	&& ReceivedStringFromPi[2]=='B'
	&& ReceivedStringFromPi[3]=='U'
	&& ReceivedStringFromPi[4]=='S')
        {
            Start_Stop_Debug_Ultrason();
        }
        
        // DTUS			// change la distance de detection des ultrasons
        if(ReceivedStringFromPi[1]=='D'
        && ReceivedStringFromPi[2]=='T'
        && ReceivedStringFromPi[3]=='U'
        && ReceivedStringFromPi[4]=='S')
        {
            cursorPosition = 6;
            for(floatLength=0;ReceivedStringFromPi[cursorPosition+floatLength]!=';';floatLength++); // Return the number of char taken by the float in the command line
            ReceivedStringFromPi[cursorPosition+floatLength] = 0;
            valf = atof(&ReceivedStringFromPi[cursorPosition]);
            ReceivedStringFromPi[cursorPosition+floatLength] = ';';
            Set_Threshold_US(valf*1000);
        }
        
        

    //**************************************************************************//
    //************** PARTIE AUTRES MOUVEMENTS **********************************//
    //**************************************************************************//

        // TEAM
        if(ReceivedStringFromPi[1]=='T' 
	&& ReceivedStringFromPi[2]=='E' 
	&& ReceivedStringFromPi[3]=='A'
	&& ReceivedStringFromPi[4]=='M')
        {
            SendTeam(PIN_TEAM);
        }

        // SIK?			// demande status sick
        if(ReceivedStringFromPi[1]=='S' 
	&& ReceivedStringFromPi[2]=='I' 
	&& ReceivedStringFromPi[3]=='K' 
	&& ReceivedStringFromPi[4]=='?')
        {
            val8 = ReceivedStringFromPi[6] -48; // traduction décimal - ascii : 48 en ascii  = 0
            if (val8 >= NUMBER_OF_SICK) {
                    val8 = 0;
            }
            //__delay_ms(50);
            SendSick_Status(val8);
            //__delay_ms(50);
        }

        // DBSI			// start/stop debug sick
        if(ReceivedStringFromPi[1]=='D' 
	&& ReceivedStringFromPi[2]=='B' 
	&& ReceivedStringFromPi[3]=='S' 
	&& ReceivedStringFromPi[4]=='I')
        {
            Start_Stop_Debug_Sick();
        }

        // ENSI         // active ou pas le motion_free des sicks  à l'unitée
        if(ReceivedStringFromPi[1]=='E' 
	&& ReceivedStringFromPi[2]=='N' 
	&& ReceivedStringFromPi[3]=='S' 
	&& ReceivedStringFromPi[4]=='I')
        {
            // l'utilisateur a juste droit à de 0 à F
            valc = ReceivedStringFromPi[6];
            if (valc >= '0' && valc <= '9') {
                valc -= '0';
            } else if (valc >= 'A' && valc <= 'F') {
                valc = valc + 10 - 'A';
            } else {
                valc = 0x0F;
            }
            Choose_Enabled_Sicks(valc);
        }
    
		
        // VBAT			// récupère la tension baterie
        if(ReceivedStringFromPi[1]=='V' 
	&& ReceivedStringFromPi[2]=='B' 
	&& ReceivedStringFromPi[3]=='A' 
	&& ReceivedStringFromPi[4]=='T')
        {
            //__delay_ms(50);
            printf("$VBAT,%d;", Get_VBat());
            //__delay_ms(50);
        }

    }
}

void SendDone(void)
{
    //__delay_ms(50);
    Raz_Delay_WatchDone();
    printf("$DONE;");
    //__delay_ms(50);
}

void SendStart(void)
{
    //__delay_ms(50);
    printf("$STRT;");
    //__delay_ms(50);
}

void SendEnd (void)
{
    //__delay_ms(10);
    printf("$END9;");
    //__delay_ms(10);
}

void SendFailAX12(void)
{
    //__delay_ms(50);
    printf("$FAAX;");
    //__delay_ms(50);
}

void DetectSick(int channel)
{
    //__delay_ms(2);
    switch(channel){
        case 1 : printf("$DSI1;");  break;
        case 2 : printf("$DSI2;");  break;
        case 3 : printf("$DSI3;");  break;
        case 4 : printf("$DSI4;");  break;
    }
   //__delay_ms(2);
}

void ReleaseSick (int channel)
{
    //__delay_ms(50);
	switch(channel){
        case 1 : printf("$RSI1;");  break;
        case 2 : printf("$RSI2;");  break;
        case 3 : printf("$RSI3;");  break;
        case 4 : printf("$RSI4;");  break;
    }
    //__delay_ms(50);
}

void SendSick_Status(int val8)
{
    //__delay_ms(50);
    printf("$SICK,%d,%d,%d;", val8, Get_Sick(val8), Get_Sick_Sector(val8) );
	//__delay_ms(50);
}

void DetectUltrason(int channel)
{
    switch(channel){
        case 1 : printf("$DUS1;"); break;
        case 2 : printf("$DUS2;"); break;
    }

}

void ReleaseUltrason(int channel)
{
    switch(channel){
    case 1 : printf("$RUS1;"); break;
    case 2 : printf("$RUS2;"); break;
    }
}

void SendUltrason_Status(void)
{
    //__delay_ms(50);
    //printf("$SULS,%d,%d,%d;", Sector_Ultrason, Mesure_Distance_Ultrason, Mesure_Timer_Ultrason);
    printf("$SULS,%d,%d,%d;", 0, 0, 0);
    //__delay_ms(50);
}

void SendTeam (int team)
{
    //__delay_ms(50);
    switch(team) {
        case 1 : printf("$VERT;");  break;
        case 0 : printf("$VIOL;");  break;
    }
    //__delay_ms(50);
}

void SendUrge(void){
    printf ("$URGE;");
}