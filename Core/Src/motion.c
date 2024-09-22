#include "motion.h"
#include "motor.h"
#include "pid.h"
#include "stdio.h"

extern motor_info_t motor_A1;
extern motor_info_t motor_A2;
extern motor_info_t motor_B1;
extern motor_info_t motor_B2;

// 转向方向，转向速度，运动方向，运动速度
void car_motion_control(uint8_t _cAngularDirection, uint16_t _cAngularSpeed, uint8_t _car_speed_dir, uint16_t _car_speed_tar)
{
    int16_t car_speed_target = 0;
    int16_t _cAngularSpeed_M = 0;

    int16_t motorA1_speed_target = 0;
    int16_t motorA2_speed_target = 0;
    int16_t motorB1_speed_target = 0;
    int16_t motorB2_speed_target = 0;

    int16_t _sMotorA_PWM = 0;
    int16_t _sMotorB_PWM = 0;
    int16_t _sMotorC_PWM = 0;
    int16_t _sMotorD_PWM = 0;


    int16_t _usLinearSpeed = 0; //根据角速度计算的线速度

    if((_cAngularSpeed == 0) && (_car_speed_tar == 0)&&(motor_A1.speed_current == 0))//���趨�Ľ��ٶȻ����˶��ٶ�Ϊ0ʱ ��PID���ۼ���������0
    {
        PID_Param_SetZero(&(motor_A1.pid_para));
        PID_Param_SetZero(&(motor_A2.pid_para));
        PID_Param_SetZero(&(motor_B1.pid_para));
        PID_Param_SetZero(&(motor_B2.pid_para));
    }

    if(_car_speed_tar >= AUTO_SPEED_MAX) _car_speed_tar = AUTO_SPEED_MAX;

    if(_cAngularDirection == 0)
    {
        _cAngularSpeed_M = _cAngularSpeed;
    }
    else
    {
        _cAngularSpeed_M = -_cAngularSpeed;
    }
    if(_car_speed_dir == 0)
    {
        car_speed_target = _car_speed_tar;
    }
    else
    {
        car_speed_target = -_car_speed_tar;
    }

    //差速计算，计算每个轮子的目标转速
    motorA1_speed_target = car_speed_target - 0.5 * _cAngularSpeed_M * 0.01 * TIRE_SPACE;
    motorA2_speed_target = car_speed_target - 0.5 * _cAngularSpeed_M * 0.01 * TIRE_SPACE;
    motorB1_speed_target = car_speed_target + 0.5 * _cAngularSpeed_M * 0.01 * TIRE_SPACE;
    motorB2_speed_target = car_speed_target + 0.5 * _cAngularSpeed_M * 0.01 * TIRE_SPACE;
    // 更新目标值缓存 
    motor_A1.speed_target = motorA1_speed_target;
    motor_A2.speed_target = motorA2_speed_target;
    motor_B1.speed_target = motorB1_speed_target;
    motor_B2.speed_target = motorB2_speed_target;

    //PID计算，计算得到这个转速所需的pwm值
    _sMotorA_PWM = PID_wheelspeed_calc(&motor_A1.pid_para, -motor_A1.speed_current,  motorA1_speed_target);
    _sMotorB_PWM = PID_wheelspeed_calc(&motor_A2.pid_para, -motor_A2.speed_current,  motorA2_speed_target);
    _sMotorC_PWM = PID_wheelspeed_calc(&motor_B1.pid_para, motor_B1.speed_current,  motorB1_speed_target);
    _sMotorD_PWM = PID_wheelspeed_calc(&motor_B2.pid_para, motor_B2.speed_current,  motorB2_speed_target);

    motor_set_speed_pwm(&motor_A1, _sMotorA_PWM);
    motor_set_speed_pwm(&motor_A2, _sMotorB_PWM);
    motor_set_speed_pwm(&motor_B1, _sMotorC_PWM);
    motor_set_speed_pwm(&motor_B2, _sMotorD_PWM);
}

void car_motion_control_test_motor()
{
    if((motor_A1.speed_target == 0)&&(motor_A1.speed_current == 0))    PID_Param_SetZero(&(motor_A1.pid_para));
    if((motor_A2.speed_target == 0)&&(motor_A2.speed_current == 0))    PID_Param_SetZero(&(motor_A2.pid_para));
    if((motor_B1.speed_target == 0)&&(motor_B1.speed_current == 0))    PID_Param_SetZero(&(motor_B1.pid_para));
    if((motor_B2.speed_target == 0)&&(motor_B2.speed_current == 0))    PID_Param_SetZero(&(motor_B2.pid_para));
    
    int16_t _sMotorA_PWM = 0;
    int16_t _sMotorB_PWM = 0;
    int16_t _sMotorC_PWM = 0;
    int16_t _sMotorD_PWM = 0;

    _sMotorA_PWM = PID_wheelspeed_calc(&motor_A1.pid_para, motor_A1.speed_current,  motor_A1.speed_target);
    _sMotorB_PWM = PID_wheelspeed_calc(&motor_A2.pid_para, motor_A2.speed_current,  motor_A2.speed_target);
    _sMotorC_PWM = PID_wheelspeed_calc(&motor_B1.pid_para, motor_B1.speed_current,  motor_B1.speed_target);
    _sMotorD_PWM = PID_wheelspeed_calc(&motor_B2.pid_para, motor_B2.speed_current,  motor_B2.speed_target);

    // printf("PID_wheelspeed_calc: cur: %d, tar: %d, pwm: %d\r\n", motor_A1.speed_current, motor_A1.speed_target, _sMotorA_PWM);
    // printf("PID_wheelspeed_calc: cur: %d, tar: %d, pwm: %d\r\n", motor_A2.speed_current, motor_A2.speed_target, _sMotorB_PWM);
    // printf("PID_wheelspeed_calc: cur: %d, tar: %d, pwm: %d\r\n", motor_B1.speed_current, motor_B1.speed_target, _sMotorC_PWM);
    // printf("PID_wheelspeed_calc: cur: %d, tar: %d, pwm: %d\r\n", motor_B2.speed_current, motor_B2.speed_target, _sMotorD_PWM);


    motor_set_speed_pwm(&motor_A1, _sMotorA_PWM);
    motor_set_speed_pwm(&motor_A2, _sMotorB_PWM);
    motor_set_speed_pwm(&motor_B1, _sMotorC_PWM);
    motor_set_speed_pwm(&motor_B2, _sMotorD_PWM);
}

// 蓝牙控制
void motion_control_ble(uint8_t* b)
{
  // todo  
}

