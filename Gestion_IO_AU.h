

#ifndef __GESTION_IO_H
#define __GESTION_IO_H


#define TRIS_DETECT_10V     _TRISA2
#define PIN_DETECT_10V      _RA2

#define PIN_TEAM _RA4       // ==> Pin 3.3V numero 2
#define TRIS_TEAM _TRISA4 

#define PIN_LAISSE _RC3     // ==> Pin 5V numero 4.
#define TRIS_LAISSE _TRISC3


void Gestion_IO_AU_Init(void);

void Gestion_IO_AU_Loop(void);


#endif

