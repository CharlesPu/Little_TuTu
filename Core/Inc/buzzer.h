#ifndef __BUZZER_H__
#define __BUZZER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define BEEP_INTERVAL 150 // ms

void BUZZER_init(void);
void BUZZER_beep_once(void);
void BUZZER_beep_twice(void);
void BUZZER_beep_long_on(void);
void BUZZER_beep_long_off(void);


#ifdef __cplusplus
}
#endif

#endif 

