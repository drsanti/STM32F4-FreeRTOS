/*
 ********************************************************************
 *                     STM32F4xx based on FreeRTOS
 ********************************************************************
 * FileName:    main_ex09_evgroup.c
 * Description: Using event group
 ********************************************************************
 * Dr.Santi Nuratch
 * Embedded Computing and Control Laboratory | INC@KMUTT
 * 04 June, 2019
 * ****************************************************************** 
 */

#include "system_utils.h"

//!! Task Handles
TaskHandle_t TaskHandle_1, TaskHandle_2, TaskHandle_3;

//!! Event Group Handle
EventGroupHandle_t EventGroupHandle;

//!! Bits
#define BIT_0	( 1 << 0 )
#define BIT_1	( 1 << 1 )
#define BIT_2	( 1 << 2 )
#define BIT_3	( 1 << 3 )

//!! LEDs
uint16_t LEDs[] = { LED_RED, LED_GREEN, LED_ORANGE, LED_BLUE };



//!! Wait bits
static void Task1( void* pvParameters ) {

    for (;;) {

        EventBits_t uxBits = xEventGroupWaitBits(
            EventGroupHandle,               //!! The event group being tested. 
            BIT_0 | BIT_1 | BIT_2 | BIT_3,  //!! The bits within the event group to wait for. 
            pdTRUE,                         //!! BIT_0 - BIT_3 should be cleared before returning. 
            pdFALSE,                        //!! Don't wait for both bits, either bit will do. 
            1000/portTICK_PERIOD_MS         //!! Wait a maximum of 100ms for either bit to be set. 
        );

        //!! Task2 sets BIT_0
        if( uxBits & BIT_0 ) {
            LED_Inv( LED_RED );
        }
        //!! Task2 sets BIT_1
        if( uxBits & BIT_1 ) {
            LED_Inv( LED_GREEN );
        }
        //!! ISR sets BIT_2 and BIT_3
        if( uxBits & (BIT_2 | BIT_3) ) {
            LED_Inv( LED_BLUE );
        }
	}
}

//!! Set bits
static void Task2( void* pvParameters ) {
    int k =0;
    for (;;) {
        EventBits_t uxBits = xEventGroupSetBits(
            EventGroupHandle,   //!! The event group being updated.
            k?BIT_0:BIT_1       //!! The bits being set.
        );
        if( uxBits ) {
            //!! Check if required
        }
        //!! k=0x00->0x01->0x00->...
        k ^= 0x01;

        vTaskDelay( 500/portTICK_PERIOD_MS );
	}
}


//!! Waits notify from ISR
static void Task3( void* pvParameters ) {

    for (;;) {
        vTaskDelay( 1000/portTICK_PERIOD_MS );    
	}
}

//!! Sends notify to Task3
void EXTI0_IRQHandler( void ) {
    
    if( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 ) ) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE; 
        BaseType_t xResult = xEventGroupSetBitsFromISR(
            EventGroupHandle,   //!! The event group being updated.
            BIT_2 | BIT_3,      //!! The bits being set.
            &xHigherPriorityTaskWoken 
        );
        if( xResult != pdFAIL ) {
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );   
        }
    }
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
}

int main(void) {

    //!! Initialize
    System_Init();

    //!! Create Event Group
    EventGroupHandle = xEventGroupCreate();

    if( EventGroupHandle == NULL ) {
        //!! The event group was not created
    }

    //!! Create tasks
    xTaskCreate(Task1, "Task_1", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_1);
    xTaskCreate(Task2, "Task_2", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_2);
    xTaskCreate(Task3, "Task_3", 128, NULL, tskIDLE_PRIORITY+1, &TaskHandle_3);

    //!! Initial External Interupt (User button)
    ExInt_Init();

    //!! Start scheduler
    vTaskStartScheduler();

    //!!
    while(1);
}
