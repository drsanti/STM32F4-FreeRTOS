
#include "system_utils.h"

static void Task1(void* pvParameters) {
	for (;;) {
		vTaskDelay(200/portTICK_PERIOD_MS);
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15); //!! BLUE LED
	}
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Create task
    xTaskCreate(Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, NULL);

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}
