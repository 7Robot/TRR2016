#ifndef _ASSERV_H_
#define _ASSERV_H_

#include "motion.h"
#include "pid.h"
#include "odo.h"

// mode de l'asservissement
#define ASSERV_MODE_OFF 0
#define ASSERV_MODE_POS 1
#define ASSERV_MODE_SPEED 2
#define ASSERV_MODE_ANGLE 3
#define ASSERV_MODE_SEQUENCE 4
#define ASSERV_MODE_LINEAR_SPEED 5

/*****************************    Structures    *******************************/

typedef struct {
    float d;
    float dt;
} Distance;

// Un asservissement en position connait les asserv en vitesse des roues gauche et droite
typedef struct {
    Position pos_order;
    Distance distance;
    float stop_distance;
    float kp; // coef proportionnel qui doit être plus petit que l'accélération max
    MotionState *state;
    MotionConstraint *constraint;
    int done;
} PositionAsserv;

// Un asservissement en vitesse
typedef struct {
    Speed speed_order;
    Speed speed_order_constrained;
    Pid pid_delta;
    Pid pid_alpha;
    MotionState *state;
    MotionConstraint *constraint;
    int done;
} SpeedAsserv;

// Un asservissement en angle
typedef struct {
    float angle_order;
    Pid pid_alpha;
    MotionState *state;
    MotionConstraint *constraint;
    int done;
} AngleAsserv;


/******************************    Fonctions    *******************************/

// initialiser le mode et les différents asservissements
void asserv_init();

// choisir le mode d'asservissement (désactivé, en position, en vitesse)
void set_asserv_off();
void set_asserv_pos_mode();
void set_asserv_speed_mode();
void set_asserv_linear_speed_mode();
void set_asserv_angle_mode();
void set_asserv_seq_mode();

// contraindre les vitesses et accélérations autorisées
void constrain_speed(
        float v, float vt,
        float *v_constrained, float *vt_constrained,
        float v_max, float vt_max,
        float a_max, float at_max, float v_vt_max);
// contraint la consigne de vitesse avec la fonction precedente constrain_speed
void constrain_speed_order();

// effectue un pas d'asservissement
void asserv_step(Odo *odo, float *commande_g, float *commande_d);
void speed_asserv_step(Odo *odo, float *commande_g, float *commande_d);
void linear_speed_asserv_step(Odo *odo, float *commande_g, float *commande_d);
void pos_asserv_step(Odo *odo, float *commande_g, float *commande_d);
void angle_asserv_step(Odo *odo, float *commande_g, float *commande_d);
void seq_asserv_step(Odo *odo, float *commande_g, float *commande_d);

// indique si l'asservissement en cours a terminé
int asserv_done();


#endif // _ASSERV_H_
