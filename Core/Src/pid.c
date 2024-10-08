#include "pid.h"
#include "motor.h"

// 输出的是电机pwm值
int PID_wheelspeed_calc(Pid_t *pid_para, int16_t val_curr, int16_t val_target)
{
    float _dError = 0;
    float _error  = 0;
    float _tempP  = 0;
    float _tempI  = 0;
    float _tempD  = 0;
    float _tempPID = 0;

    Pid_t *_tpid = pid_para;

    _error = val_target - val_curr;  /*���㵱ǰ���*/

    _tempP = _tpid->P * _error;/*����P���ֵ*/

    _tpid->SumError += _error;  /*�ۼ����*/
    _tempI = _tpid->I * _tpid->SumError;   /*����I���ֵ*/

    if(_tempI > MOTOR_SPEED_PWM_MAX)
    {
        _tpid->SumError = MOTOR_SPEED_PWM_MAX / _tpid->I;
    }
    if(_tempI < -MOTOR_SPEED_PWM_MAX)
    {
        _tpid->SumError = -MOTOR_SPEED_PWM_MAX / _tpid->I;
    }

    _dError = _error - _tpid->PrevError;/*΢�����*/

    _tempD  = _tpid->D * _dError;/*����D���ֵ*/

    _tpid->PrevError = _error ;

    _tempPID = _tempP + _tempI + _tempD;

    if(_tempPID > MOTOR_SPEED_PWM_MAX)
    {
        _tempPID = MOTOR_SPEED_PWM_MAX;
    }
    if(_tempPID < -MOTOR_SPEED_PWM_MAX)
    {
        _tempPID  = -MOTOR_SPEED_PWM_MAX;
    }

    return _tempPID;
}

void PID_Param_SetZero(Pid_t * p)
{
    p->PrevError = 0;
    p->SumError = 0;
}
