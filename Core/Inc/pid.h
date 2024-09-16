#ifndef __PID_H__
#define __PID_H__

#include "main.h"
typedef struct{
    float P;
    float I;
    float D;

    float PrevError;
    float SumError;
}Pid_t;


int PID_wheelspeed_calc(Pid_t *, int16_t _usMeasure, int16_t _usTarget);
void PID_Param_SetZero(Pid_t *);

// extern Pid_t tPidSpeedA;
// extern Pid_t tPidSpeedB;
// extern Pid_t tPidSpeedC;
// extern Pid_t tPidSpeedD;
#endif /*__PID_H__*/
