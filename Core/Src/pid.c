#include "pid.h"

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

    if(_tempI > 2000)
    {
        _tpid->SumError = 2000 / _tpid->I;
    }
    if(_tempI < -2000)
    {
        _tpid->SumError = -2000 / _tpid->I;
    }

    _dError = _error - _tpid->PrevError;/*΢�����*/

    _tempD  = _tpid->D * _dError;/*����D���ֵ*/

    _tpid->PrevError = _error ;

    _tempPID = _tempP + _tempI + _tempD;

    if(_tempPID > 2000)
    {
        _tempPID = 2000;
    }
    if(_tempPID < -2000)
    {
        _tempPID  = -2000;
    }

    return _tempPID;
}

void PID_Param_SetZero(Pid_t * p)
{
    p->PrevError = 0;
    p->SumError = 0;
}
