#include "buzzer.h"

void BUZZER_init(void)
{
  BUZZER_beep_long_off();
}

void BUZZER_beep_once(void)
{
  BUZZER_beep_long_on();
  HAL_Delay(BEEP_INTERVAL);
  BUZZER_beep_long_off();
}

void BUZZER_beep_twice(void)
{
  BUZZER_beep_long_on();
  HAL_Delay(BEEP_INTERVAL);
  BUZZER_beep_long_off();
  HAL_Delay(BEEP_INTERVAL);
  BUZZER_beep_long_on();
  HAL_Delay(BEEP_INTERVAL);
  BUZZER_beep_long_off();
}

void BUZZER_beep_long_on(void)
{
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
}

void BUZZER_beep_long_off(void)
{
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
}
