

#ifndef __GESTION_IO_H
#define __GESTION_IO_H


#define TRIS_DETECT_10V     _TRISA2
#define PIN_DETECT_10V      _RA2

#define PIN_TEAM _RA4       // ==> Pin 3.3V numero 2
#define TRIS_TEAM _TRISA4 

#define PIN_LAISSE _RB7     // ==> Pin 5V numero 1 
#define TRIS_LAISSE _TRISB7

#define PIN_SERVO       _LATC6            // RC6 = encodeur 1 A
#define TRIS_SERVO      _TRISC6

//2	ENCODEUR 1 DATA A	RP22/RC6
//3	ENCODEUR 2 DATA A 	RP23/RC7
//4	ENCODEUR 1 DATA B	RP24/RC8
//5	ENCODEUR 2 DATA B	RP25/RC9


void Gestion_IO_AU_Init(void);

void Gestion_IO_AU_Loop(void);


#endif

