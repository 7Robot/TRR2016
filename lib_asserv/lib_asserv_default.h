#ifndef _LIB_ASSERV_DEFAULT_H_
#define _LIB_ASSERV_DEFAULT_H_

#include "../communication.h"

#ifndef NULL
#define NULL 0
#endif


/*############################################################################*/
/*                                  Motion                                    */
/*############################################################################*/

#ifndef DEFAULT_CONSTRAINT_V_MAX // {v, vt}
#define DEFAULT_CONSTRAINT_V_MAX {0.8, 2} // crabot: {0.8, 4}
#endif

#ifndef DEFAULT_CONSTRAINT_A_MAX // {a, at, v_vt}
#define DEFAULT_CONSTRAINT_A_MAX {1, 7, 0.7} // crabot: {0.7, 20, 0.8}
#endif

#ifndef BLOCK_LIMIT
#define BLOCK_LIMIT 500000
#endif

/*############################################################################*/
/*                                    Odo                                     */
/*############################################################################*/

#ifndef DEFAULT_ODO_COEFS
#define DEFAULT_ODO_COEFS {53833, 0.000018576, 0.19}    // { , , 0.19 }
#endif

#ifndef DEFAULT_PERIOD
#define DEFAULT_PERIOD 0.005
#endif

/*############################################################################*/
/*                                  Asserv                                    */
/*############################################################################*/

#ifndef DEFAULT_ASSERV_MODE
#define DEFAULT_ASSERV_MODE ASSERV_MODE_OFF
#endif
#ifndef DEFAULT_STOP_DISTANCE
#define DEFAULT_STOP_DISTANCE 0.008
#endif

/*############################################################################*/
/*                                   PID                                      */
/*############################################################################*/

// 2 PID lies a l'asserve en vitesse (delta: vitesse absolue, alpha: vitesse angulaire)
#ifndef DEFAULT_PID_COEFS_DELTA // {kp, ki, kd, mu_p}
#define DEFAULT_PID_COEFS_DELTA {160, 16, 0, 1} // crabot: {160,20,0,0.2}
#endif
#ifndef DEFAULT_PID_COEFS_ALPHA // {kp, ki, kd, mu_p}
//#define DEFAULT_PID_COEFS_ALPHA {20, 2, 2, 1} // crabot: {32,8,0,0.8} // asserv costaude
#define DEFAULT_PID_COEFS_ALPHA {15, 1, 0, 1} // crabot: {32,8,0,0.8}   // asserv pataude, mais beaucoup plus fluide...
#endif

// valeurs max de l'intégrale en vitesse et vitesse angulaire
#ifndef DEFAULT_PID_MAX_INT_DELTA
#define DEFAULT_PID_MAX_INT_DELTA 4 // crabot: 4
#endif
#ifndef DEFAULT_PID_MAX_INT_ALPHA
#define DEFAULT_PID_MAX_INT_ALPHA 30 // crabot: 4
#endif

// conditions d'arrêt des PID en vitesse et vitesse angulaire
#ifndef DEFAULT_PID_EPS_DELTA
#define DEFAULT_PID_EPS_DELTA {0.01,0.01}
#endif
#ifndef DEFAULT_PID_EPS_ALPHA
#define DEFAULT_PID_EPS_ALPHA {0.02,0.02}
#endif

/*############################################################################*/
/*                                  DEBUG                                     */
/*############################################################################*/

#ifndef DEFAULT_DEBUG_MODE
#define DEFAULT_DEBUG_MODE 1
#endif

#endif // _LIB_ASSERV_DEFAULT_H_
