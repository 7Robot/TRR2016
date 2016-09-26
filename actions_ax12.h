/*
 * File:   actions_ax12.h
 * Author: zoltarius
 *
 * Created on 4 avril 2014, 18:41
 */

#ifndef ACTIONS_AX12_H
#define	ACTIONS_AX12_H

/*
 * NUMEROTATION DES ACTIONNEURS DU ROBOT :
   _______________
  /               /|
 /______________ / |
| \           / |  |
|  \ 1     2 /  |  |
|   °       °   |  |
| \           / |  |
|  \         /  |  |
|   \ 3   4 /   |  |
|    °     °    |  |
|       O       | 0'
| ______O______ |/
*/

#define Wing_1		6	// Bras haut - gauche pour tenir 3e etage de blocs de sable
#define Wing_2		12	// Bras haut - droite
#define Wing_3		13      // Bras bas - gauche pour tenir 1e et 2e etage de blocs de sable
#define Wing_4          10	// Bras bas - droite


// CLLC
// CLRO

#define NUM_ACTIONS_BUFFER 20


void Add_Action_AX12(char Action_ToDo);
void Faire_Actions_AX12(void);

#define AX12_INIT_AX12              1
void Init_ax12(void);
#define AX12_DEPLOY_WINGS_BAS       2		// Deploiement des ailes superieures
void Deploy_wings_B(void);
#define AX12_DEPLOY_WINGS_HAUT      3		// Deploiement des ailes inferieures
void Deploy_wings_H(void);
#define AX12_DEPLOY_WINGS           4           // Deploiement des 2 paires d'ailes en même temps + SendDone().
void Deploy_wings(void);
#define AX12_CLOSE_WINGS_BAS        5		// Fermeture des ailes superieures
void Close_wings_B(void);
#define AX12_CLOSE_WINGS_HAUT       6		// Fermeture des ailes inferieures
void Close_wings_H(void);
#define AX12_CLOSE_WINGS            7           // Fermeture des 2 paires d'ailes en même temps + SendDone().
void Close_wings(void);
#define AX12_IBICF                  8		// I Believe I Can Fly !!!
void I_Can_Fly(void);
#define POMPE_ACTIVER               9           // Active les pompes
 void Activer_Pompes(void);
#define POMPE_ACTIVER_BAS           10           // Active la pompe du bas uniquement
void Activer_Pompe_basse(void);
#define POMPE_ETEINDRE              11          // Eteint les pompes.
void Eteindre_Pompes(void);

#endif	/* ACTIONS_AX12_H */


 

 
