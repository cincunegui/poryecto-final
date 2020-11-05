

/* Kernel includes. */
//#include "freeRtos/FreeRTOS.h"
#include "utils/utils.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "freeRtos/include/FreeRTOS.h"
#include "freeRtos/include/task.h"
#include "freeRtos/include/semphr.h"
#include "system/UI/UI.h"
#include "system/Events/events.h"
#include "math.h"



xSemaphoreHandle xSemaphore_calendario;

void InterfazUser(void *pvParameters) {

}


void InterfazUser(void *pvParameters) {

}


void main(void) {

    // initialize the device
    SYSTEM_Initialize();

    xSemaphoreHandle xSemaphore_calendario = xSemaphoreCreateMutex();

    if (xSemaphore_calendario != NULL) {

        /* Create the tasks defined within this file. */

        //xTaskCreate(blinkLED, "BlinkLed", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
        xTaskCreate(InterfazUser, "InterfazUser", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
       // xTaskCreate(Eventos, "Eventos", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);

        /* Finally start the scheduler. */

        vTaskStartScheduler();
    }
    /* If all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
    for (;;);
}
