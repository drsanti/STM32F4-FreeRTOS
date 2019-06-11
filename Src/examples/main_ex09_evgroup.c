
#include "system_utils.h"

//!! Task Handles
TaskHandle_t TaskHandle_1, TaskHandle_2, TaskHandle_3;

//!! LEDs
uint16_t LEDs[] = { LED_RED, LED_GREEN, LED_ORANGE, LED_BLUE };


#define BIT_0	( 1 << 0 )
#define BIT_4	( 1 << 4 )

EventGroupHandle_t xEventGroup;



//!! Sends notify to Task2
static void Task1( void* pvParameters ) {
    for (;;) {

        /* Wait a maximum of 100ms for either bit 0 or bit 4 to be set within
        the event group.  Clear the bits before exiting. */
        EventBits_t uxBits = xEventGroupWaitBits(
                    xEventGroup,                /* The event group being tested. */
                    BIT_0 | BIT_4,              /* The bits within the event group to wait for. */
                    pdTRUE,                     /* BIT_0 & BIT_4 should be cleared before returning. */
                    pdFALSE,                    /* Don't wait for both bits, either bit will do. */
                    portMAX_DELAY );  /* Wait a maximum of 100ms for either bit to be set. */
        }
        //
}

//!! Waits notify from Task1
static void Task2( void* pvParameters ) {
    for (;;) {
        
        EventBits_t uxBits xEventGroupSetBits(
                xEventGroup,    /* The event group being updated. */
                BIT_0 );        /* The bits being set. */

        vTaskDelay( 1000/portTICK_PERIOD_MS );
	}
}


//!! Waits notify from ISR
static void Task3( void* pvParameters ) {
    for (;;) {
        
        //LED_Inv( LED_RED );
        vTaskDelay( 1000/portTICK_PERIOD_MS );
	}
}

//!! Sends notify to Task3
void EXTI0_IRQHandler( void ) {

    if( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 ) ) {
        //BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        //???
        //portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
}

int main(void) {

    //!! Initialize
    System_Init();

    xCreatedEvent = xEventGroupCreate();

    if( xCreatedEventGroup == NULL )
    {
        //
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
