#include "math.h"

float abs(float val){
    if (val<0){
        return -val;
    } else {
        return val;
    }
}

float signe(float val){
    if (val <0){
        return -1;
    } else {
        return 1;
    }
}

float linear_mapping(float value, float oldinf, float oldsup, float newinf, float newsup){
    return ((value-oldinf)/(oldsup-oldinf)*(newsup-newinf))+newinf;
}

