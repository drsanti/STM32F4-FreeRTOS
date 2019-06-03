#include "system_utils.h"

static void Task1(void* pvParameters) {
	for (;;) {
		vTaskDelay(200/portTICK_PERIOD_MS);
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15); //!! BLUE LED
	}
}

void EXTI0_IRQHandler(void) {
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14); //!! RED LED
    }
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

//!! Called by the HAL_GPIO_EXTI_IRQHandler()
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13); //!! ORANGE LED   
}


int main(void) {

    //!! Initialize
    System_Init();

    //!! Initial External Interupt (User button)
    ExInt_Init();

    //!! Create task
    xTaskCreate(Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, NULL);

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}

