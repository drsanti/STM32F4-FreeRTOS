#STM32F4xx based on FreeRTOS

![vscode_snap](https://github.com/drsanti/STM32F4-FreeRTOS/blob/master/docs/vscode_snap.png)



**How to?**
* Copy the code in Src/examples/*.c
* Paste in Src/main.c
* Run make -j8
* Run make flash
* Debug or Check the result on the board


**main_ex00_init.c -- Getting Started**

```c
/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex00_init.c
 * Description: Getting Started 
 ********************************************************************
 * Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * 03 June, 2019
 * ****************************************************************** 
 */

#include "system_utils.h"

//!! Blue LED Blinking
static void Task1(void* pvParameters) {
	for (;;) {
		vTaskDelay(200/portTICK_PERIOD_MS);
        LED_Inv(LED_BLUE);
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
```

