

/* Kernel includes. */
#include "utils/utils.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "freeRtos/include/FreeRTOS.h"
#include "freeRtos/include/task.h"
#include "freeRtos/include/semphr.h"
#include "system/UI/UI.h"
#include "math.h"
#include "freeRtos/include/projdefs.h"
#include "framework/Accelerometer/Accelerometer.h"
#include "WS2812.h"
#include "framework/LEDs_RGB/LEDs_RGB_fwk.h"
#include "utils.h"
#include "auxiliares.h"

xSemaphoreHandle xSemaphore_calendario;

void InterfazUser(void *pvParameters) {

}

// decidir si teta se hacer afuera de la funcion ver el uso de los semaforos.
uint8_t Mover_Pelota(uint8_t *pvParameters) {
    // en esta funcion entro con el led que estoy parado actualmente y decido a donde me voy
    // si a la derecha o izquierda del led actual
    Accel_t Aceleracion;
    float x,y,z,Teta,Sencibilidad;
    #define PI 3.141592654;
    static ws2812_t Leds[LED_RGB_SIZE];
    if(ACCEL_init( )&& ACCEL_GetAccel (Accel_t &Aceleracion)){
        x = Aceleracion.Accel_X;
        y = Aceleracion.Accel_Y;
        z = Aceleracion.Accel_Z;
    }
    
    Teta = atan2f(y,x); // paso de cartecianas a radianes
    Teta = Teta* 180.0/PI; // paso de radianes a grados
    switch(pvParameters) { // led en el que estoy parado actualmente
        case(1)://LED 1 PRENDIDO
            RGB_setLedColor( 1, RGB_BLUE );
            RGB_showLeds( Leds,LED_RGB_SIZE );
            while(0 < Teta < 44){
                vTaskDelay(150);//con esto suspende la tarea, para ceder el procesador    
            }
            if(Teta> 44 ){ //45 grados       // Dceido a que led me voy       
                pvParameters = 2;  
            }
            if(Teta < 360 ){
                pvParameters = 8;
            }
            break;
        case(2)://LED 2 PRENDIDO
            RGB_setLedColor( 2, RGB_BLUE );
            RGB_showLeds( Leds,LED_RGB_SIZE );
            while(45 < Teta < 89){
                vTaskDelay(150);//con esto suspende la tarea, para ceder el procesador    
            }            
            if(89>Teta){  // Dceido a que led me voy       
                pvParameters = 3;  
            }
            if(Teta<45){
                pvParameters = 1; 
            }
            break;            
        case(3)://LED 3 PRENDIDO
            RGB_setLedColor( 3, RGB_BLUE );
            RGB_showLeds( Leds,LED_RGB_SIZE );
            while(90 < Teta < 134){
                vTaskDelay(150);//con esto suspende la tarea, para ceder el procesador    
            }
            if(134>Teta ){  // Dceido a que led me voy       
                pvParameters = 4;  
            }
            if(Teta<90){
                pvParameters = 2; 
            }             
            break;
        case(4)://LED 4 PRENDIDO
            RGB_setLedColor( 4, RGB_BLUE );
            RGB_showLeds( Leds,LED_RGB_SIZE );
            while(135 < Teta < 179){
                vTaskDelay(150);//con esto suspende la tarea, para ceder el procesador    
            }
            if(179>Teta ){  // Dceido a que led me voy       
                pvParameters = 5;  
            }           
            if(Teta<135){
                pvParameters = 3; 
            }            
            break;
        case(5)://LED 5 PRENDIDO
            RGB_setLedColor( 5, RGB_BLUE );
            RGB_showLeds( Leds,LED_RGB_SIZE );
            while(180 < Teta < 244){
                vTaskDelay(150);//con esto suspende la tarea, para ceder el procesador    
            }
            if(224>Teta ){  // Dceido a que led me voy       
                pvParameters = 6;  
            }           
            if(Teta<180){
                pvParameters = 4; 
            }            
            break;
        case(6)://LED 6 PRENDIDO
            RGB_setLedColor( 6, RGB_BLUE );
            RGB_showLeds( Leds,LED_RGB_SIZE );
            while(225 < Teta < 269){
                vTaskDelay(150);//con esto suspende la tarea, para ceder el procesador    
            }
            if(269>Teta ){  // Dceido a que led me voy       
                pvParameters = 7;  
            }           
            if(Teta<225){
                pvParameters = 5; 
            }            
            break;
        case(7)://LED 7 PRENDIDO
            RGB_setLedColor( 7, RGB_BLUE );
            RGB_showLeds( Leds,LED_RGB_SIZE );
            while(270 < Teta < 314){
                vTaskDelay(150);//con esto suspende la tarea, para ceder el procesador    
            }
            if(314>Teta ){  // Dceido a que led me voy       
                pvParameters = 8;  
            }           
            if(Teta<270){
                pvParameters = 6; 
            }            
            break; 
        case(8)://LED 8 PRENDIDO
            RGB_setLedColor( 8, RGB_BLUE );
            RGB_showLeds( Leds,LED_RGB_SIZE );
            while(315 < Teta < 360){ // espero a que cambie la aceleracion 
                vTaskDelay(150);    //con esto suspende la tarea, para ceder el procesador    
            }
            if(Teta > 0 ){  // Dceido a que led me voy       
                pvParameters = 1;  
            }           
            if(Teta<315){
                pvParameters = 7; 
            }            
            break;
    }
    //dependiendo de la sencibilidad analogica y la inclinacion de la placa
    // respecto al versor z aumento o disminuyo la rapides del cambio de led
    //Sencibilidad = (nivel* z)*k;  // hallar k para  pasar a ms
    //vTaskDelay(Sencibilidad);

}

uint16_t Conv_ACD(void *pvParameters) {
// convierte el lo adquirido por el conversor ACD, lo divide en 3 y descrimina el nivel
   uint16_t Salida; 
   uint16_t Dato = ANALOG_getResult( );
   uint16_t nivel = Dato/3;  //se puede sacar nivel y trabajar solo con dato, asi me queda mas claro
   
   if (nivel > 341)
       Salida = 1;
   if(315<nivel<682)
       Salida = 2;
   if(683<nivel)
       Salida = 3;
   
   return Salida;

}



/*
const uint8_t Error[] = "opcion incorrecta, intente nuevamente...\n";
const uint8_t ui_welcomeText[] = "Bienvenido al calendario de eventos de Sistemas Embebidos\nPor favor presione una tecla para continuar...\n";
const uint8_t ui_optionsText[] = "Indique la opción deseada:\n1) Setear hora del sistema\n2)Consultar hora del sistema\n3) Consultar eventos activos\n4) Control de eventos\n";

xSemaphoreHandle xSemaphore_calendario;
 */

/*
void blinkLED(void *pvParameters) {

    ut_tmrDelay_t TIMER_LED;
    UT_LED_STATE State_led = UT_LED_ON;

    while (1) {
        LEDA_SetHigh();
        vTaskDelay(pdMS_TO_TICKS(400));
        LEDA_SetLow();
        vTaskDelay(pdMS_TO_TICKS(800));
    }
}*/

void main(void) {

    // initialize the device
    SYSTEM_Initialize();
    USB_checkStatus();                
    ANALOG_convert( ); 
    
    xSemaphoreHandle xSemaphore_calendario = xSemaphoreCreateMutex();

    if (xSemaphore_calendario != NULL) {

        /* Create the tasks defined within this file. */

        xTaskCreate(&Mover_Pelota, "Mover_Pelota", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
        xTaskCreate(&Interfaz_Usuario, "Interfaz_Usuario", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
        xTaskCreate(&Conv_ACD, "Conv_ACD", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
        xTaskCreate(jugar, "task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
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
