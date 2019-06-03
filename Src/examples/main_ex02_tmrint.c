#include "system_utils.h"

static void Task1(void* pvParameters) {
	for (;;) {
		vTaskDelay(200/portTICK_PERIOD_MS);
        // HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15); //!! BLUE LED
	}
}

//!! Callbed by CPU
void TIM2_IRQHandler(void) {
  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);       // Red LED
  HAL_TIM_IRQHandler(&htim2);
}

//!! Callbed by CPU
void TIM4_IRQHandler(void) {
  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);       //!! BLUE LED
  HAL_TIM_IRQHandler(&htim4);
}


//!! Called by HAL_TIM_IRQHandler()
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);     //!! Green LED
  }
  else if (htim->Instance == TIM4) {
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);     //!! Orange LED
  }
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Initial Timer2 and Timer4
    Timer2_Init();
    Timer4_Init();

    //!! Create task
    xTaskCreate(Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, NULL);

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}

