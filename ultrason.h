
#ifndef __ULTRASON
#define __ULTRASON


void Init_Ultrasons (void);

#define NUMBER_OF_US                3
#define ULTRASON_THRESOLD           250      // seuil d'alarme en mm
#define ULTRASON_THRESOLD_TRIGGER   25      // zone du triger de schmitt


#define PIN_ULTRASON_D        _LATC4            // RC4 = connecteur 5V n3
#define TRIS_ULTRASON_D       _TRISC4
#define PIN_CN_ULTRASON_D_IE  _CN25IE
//#define PULL_UP_ULTRASON_D    _CN25PUE
#define ULTRASON_D_NUM_PIN     4

#define PIN_ULTRASON_G        _LATC5            // RC5 = connecteur 5V n2
#define TRIS_ULTRASON_G       _TRISC5
#define PIN_CN_ULTRASON_G_IE  _CN26IE
//#define PULL_UP_ULTRASON_G    _CN26PUE
#define ULTRASON_G_NUM_PIN     5

#define PIN_ULTRASON_H        _LATC3            // RC3 = connecteur 5V n4
#define TRIS_ULTRASON_H       _TRISC3
#define PIN_CN_ULTRASON_H_IE  _CN28IE
//#define PULL_UP_ULTRASON_H    _CN23PUE
#define ULTRASON_H_NUM_PIN     3


#define U_ETAT_OFF              0x00
#define U_ETAT_FOR_SEND1        0x01
#define U_ETAT_SEND1            0x02
#define U_ETAT_WAIT             0x04
//#define U_ETAT_WAIT0            0x08
#define U_ETAT_WAIT0_OVERSHOOT  0x10
#define U_ETAT_WAIT_FOR_RESTART 0x20


#define US1_IS_GAUCHE   1
#define US2_IS_GAUCHE   0
#define US3_IS_GAUCHE   0

#define US1_IS_DROITE   0
#define US2_IS_DROITE   1
#define US3_IS_DROITE   0

#define US1_IS_HAUT     0
#define US2_IS_HAUT     0
#define US3_IS_HAUT     1


void Init_Ultrasons(void);
void Init_CN(void);

void Start_Stop_Debug_Ultrason(void);

int Get_US_Sector(int US);

void Set_Threshold_US(int limit_mm);


#endif

