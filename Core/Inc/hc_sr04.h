#ifndef __HC_SR04_H__
#define __HC_SR04_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


void HC_SR04_init(void);
uint32_t HC_SR04_sonar_mm(void);
void HC_SR04_TIM_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif 

