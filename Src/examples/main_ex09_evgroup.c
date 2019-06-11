<<<<<<< HEAD
=======
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
>>>>>>> b704437058f3be329a6770383ba802686a1a9312

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


<<<<<<< HEAD
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
=======

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
>>>>>>> b704437058f3be329a6770383ba802686a1a9312
}

//!! Set bits
static void Task2( void* pvParameters ) {
<<<<<<< HEAD
    for (;;) {
        
        EventBits_t uxBits xEventGroupSetBits(
                xEventGroup,    /* The event group being updated. */
                BIT_0 );        /* The bits being set. */

        vTaskDelay( 1000/portTICK_PERIOD_MS );
=======
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
>>>>>>> b704437058f3be329a6770383ba802686a1a9312
	}
}


//!! Waits notify from ISR
static void Task3( void* pvParameters ) {
<<<<<<< HEAD
    for (;;) {
        
        //LED_Inv( LED_RED );
        vTaskDelay( 1000/portTICK_PERIOD_MS );
=======

    for (;;) {
        vTaskDelay( 1000/portTICK_PERIOD_MS );    
>>>>>>> b704437058f3be329a6770383ba802686a1a9312
	}
}

//!! Sends notify to Task3
void EXTI0_IRQHandler( void ) {
<<<<<<< HEAD

    if( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 ) ) {
        //BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        //???
        //portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
=======
    
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
>>>>>>> b704437058f3be329a6770383ba802686a1a9312
    }
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
}

int main(void) {

    //!! Initialize
    System_Init();

<<<<<<< HEAD
    xCreatedEvent = xEventGroupCreate();

    if( xCreatedEventGroup == NULL )
    {
        //
    }
=======
    //!! Create Event Group
    EventGroupHandle = xEventGroupCreate();

    if( EventGroupHandle == NULL ) {
        //!! The event group was not created
    }

>>>>>>> b704437058f3be329a6770383ba802686a1a9312
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
