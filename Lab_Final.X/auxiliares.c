/* Funciones del programa princial */

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include "string.h"
#include <stdlib.h>
#include <xc.h>
#include "../time.h"
#include <stdint.h>
#include "auxiliares.h"
#include "../Lab_Final.X/framework/Accelerometer/Accelerometer.h"
#include "../Lab_Final.X/mcc_generated_files/usb/usb.h"
#include "../Lab_Final.X/mcc_generated_files/usb/usb_device_config.h"
#include "../Lab_Final.X/mcc_generated_files/system.h"
#include "../Lab_Final.X/mcc_generated_files/tmr2.h"
#include "utils/utils.h"
#include "../Lab_Final.X/framework/USB/USB_fwk.h"
#include "../Lab_Final.X/platform/LEDs_RGB/WS2812.h"
#include <time.h>

/* +++ LISTADO DE PALABRAS +++ */

const uint8_t msj_bienvenida[] = "Bienvenido al juego:\n Elija las siguientes opciones para continuar\n";
const uint8_t msj_opciones[] = "Indique la opción deseada:\n1) Jugar\n2)Ver registro de juegos\n3)Setear hora\n";
const uint8_t Error[] = "Error, no se ingreso una opcion valida. No te hagas el boludo";
const uint8_t Estadisticas[] = "Estas son las estadisticas del juego\n:";

// Ver texto de la sensibilidad

const uint8_t OPCIONES5[] = "Ingrese la hora";
const uint8_t OPCIONES6[] = "Ingrese el minuto";
const uint8_t OPCIONES7[] = "Ingrese el segundos";
const uint8_t OPCIONES8[] = "Datos de la partida";

estadisticas_partida Max_Partidas[10]; // Cantidad de partidas que se pueden jugar. Por ej pueden ser 10. 

ws2812_t arreglo[8]; //Arreglo de leds
uint8_t cant_led;

estado_juego partida; //Variable que controla el estado de la partida que se esta jugando.

estadisticas_partida datos_partida; //Almacenamiento de numero y tiempo de la partida.

//Inicializacion de variables

int tiempo_juego = 0;
int contador = 0;
int tiempo_delay = 0;
int i = 1;

pelota pelota_roja;
pelota pelota_azul;

void Interfaz_Usuario(void) {

    static ui_menu_states_t estado_programa;
    static uint8_t rxData[24];

    // si esta USB conectado, ENVIO mensaje Bienvenida
    // sino no hago nada, porque no hay nada para interactuar

    if (USB_getConnectedStatus() == true) {

        if (USB_send((uint8_t*) msj_bienvenida) == true) {
            memset(rxData, 0, sizeof (rxData));
            estado_programa = UI_MENU_STATE_OPTIONS_SHOW; // Se muestra la bienvenida y se va a las opciones
        }
        switch (estado_programa) {

            case(UI_MENU_STATE_OPTIONS_SHOW): //MUESTRO EL MENU

                if (USB_send((uint8_t*) msj_opciones) == true) {
                    memset(rxData, 0, sizeof (rxData));
                    estado_programa = UI_MENU_STATE_OPTIONS_WAIT;
                }

            case(UI_MENU_STATE_OPTIONS_WAIT): //ESPERO ENTRADA

                if (UI_waitForInput(rxData) == true) {
                    estado_programa = UI_MENU_STATE_OPTIONS_CHECK; //Si se recibio, se chequea que este correcto
                }

            case(UI_MENU_STATE_OPTIONS_CHECK):

                if ((UI_checkValidOption(rxData, UI_OPTION_NUM, 4, 1)) == true) {
                    estado_programa = UI_MENU_STATE_OPTIONS_ENTRADA;
                } else { // si hay error, indico que hay error y muestro el menu nuevamente
                    if (USB_send((uint8_t*) Error) == true) {
                        memset(rxData, 0, sizeof (rxData));
                        estado_programa = UI_MENU_STATE_OPTIONS_SHOW;
                    }
                }

                // despues de este case me muevo dependiendo de lo que haya ingresado el usuario
                //luego de hacer lo pedido vuelvo a mostrar opciones  

            case(UI_MENU_STATE_OPTIONS_ENTRADA):

                int Entrada;
                Entrada = UI_MENU_STATE_OPTIONS_ENTRADA + atoi(rxData);

                switch (Entrada) { //Segun lo que ingreso el usuario, se juega, muestra log o se setea hora.

                    case( UI_MENU_STATE_PLAY):

                        jugar();
                        estado_programa = UI_MENU_STATE_OPTIONS_SHOW; //Revisar el flujo aca.

                    case( UI_MENU_STATE_GET_LOG):

                        mostrar_estadisticas();
                        estado_programa = UI_MENU_STATE_OPTIONS_SHOW;

                    case( UI_MENU_STATE_SET_TIMEDATE):

                        Set_Date();
                        estado_programa = UI_MENU_STATE_OPTIONS_SHOW;
                }
        }
    }
} //End de la funcion interfaz usuario

void jugar(void) {

    partida = inicial;

    switch (partida) {
        case (inicial):

            inicio_juego(&pelota_roja, &pelota_azul, &partida);

        case (jugando):

            juego();

        case (fin_juego):

            mostrar_estadisticas();

    } // End swich case del juego
} // End funcion jugar.

void inicio_juego(pelota *pelota_roja, pelota *pelota_azul, estado_juego *partida) {

    pelota_roja->posicion = random;
    pelota_azul->posicion = random;

    tablero(pelota_roja, pelota_azul);

    partida = jugando;

    datos_partida.tiempo = 0;
    datos_partida.Puntuacion = 0;
    datos_partida.num_Partida = 0;

}

void juego(void) {

    //    tiempo_juego = TMR2_Start(); //esto no puede ser asi porque TMR2_Start() es void - void.

    Max_Partidas[i] = datos_partida;

    while (partida == jugando) {

        pelota_azul.posicion = mover_pelota(&pelota_azul);

        tablero(&pelota_roja, &pelota_azul); // Revisar en la placa si esto va aca o al final

        if (pelota_azul.posicion == pelota_roja.posicion) {
           
            contador = contador + 1;
            pelota_roja.posicion = random;
        }
        if (contador == 10) {
            finalizar_juego(&partida);
            i = i + 1; //Si se termino el juego, se incrementa en uno el contenido del arreglo Max_Partidas. 
        }
    }
}

void Set_Date(void) {

    struct tm Ext_Time;
    struct tm *Init_Time;
    struct tm *formato_fecha;
    uint8_t buff[10];
    time_t tiempo_t;
    uint8_t tiempo;

    if (USBGetDeviceState() == CONFIGURED_STATE && USBIsDeviceSuspended() == false) {
        putUSBUSART(OPCIONES5, strlen(OPCIONES5)); //ingrese hora
        CDCTxService();
        tiempo = getsUSBUSART(buff, sizeof (buff));
        Ext_Time.tm_hour = tiempo;
        CDCTxService();

        putUSBUSART(OPCIONES6, strlen(OPCIONES6)); //ingrese min
        CDCTxService();
        tiempo = getsUSBUSART(buff, sizeof (buff));
        Ext_Time.tm_min = tiempo;
        CDCTxService();

        putUSBUSART(OPCIONES7, strlen(OPCIONES7)); //ingrese seg
        CDCTxService();
        tiempo = getsUSBUSART(buff, sizeof (buff));
        Ext_Time.tm_sec = tiempo;
        CDCTxService();

        Init_Time = &Ext_Time;

        tiempo_t = mktime(Init_Time); // paso a time_t para cargar los campos restantes

        formato_fecha = gmtime(&tiempo_t);

        RTCC_TimeSet(formato_fecha);
    }
}

void mostrar_estadisticas(void) {

    //REVISAR TODA ESTA FUNCION.
	// La idea en esta funcion es desplegar la estructura que cargas las estadistias.
	// debe ser la misma que se pone en cero en el inicio
	// usar semaforo para usar esa estructura

    uint8_t buff;
    uint8_t buff_salida[100];
    int i = 1;

    putUSBUSART(Estadisticas, strlen(Estadisticas)); //Estadisticas del juego
    CDCTxService();

    while (i <= 10) { //10 es un ejemplo. Puede guardarse tantas partidas como se quiera. 

        buff = atoi(buff_salida); //Chequear esto que onda. 

        itoa(buff_salida, Max_Partidas[i].num_Partida, 2);
        putsUSBUSART(buff_salida);
        CDCTxService();
        itoa(buff_salida, Max_Partidas[i].tiempo, 2);
        putsUSBUSART(buff_salida);
        CDCTxService();
        i++;
    }
}

int sensibilidad() {

}

int mover_pelota(pelota * pelota_azul) {

}

void tablero(pelota *pelota_roja, pelota *pelota_azul) {
	// COMO PELOTA SE USA SEGUIDO, PONER SEMAFORO  CADA VEZ QUE SE LLAMA PELOTA 
	//DONDE SE CARGA EL SOLOR DE LAS PELOTAS??

    arreglo[8] = 0; // apago todos los led
		   // cargar el color de las pelotas elejidas, llamar a las funciones det color para eso
    arreglo[pelota_azul.posicion] = 1;
    arreglo[pelota_roja.posicion] = 1;

    WS2812_send(arreglo, cant_led);
}

void finalizar_juego(estado_juego *partida) {

    partida = fin_juego;
    //Se podria mostrar las estadisticas aca. 

}

bool UI_checkValidOption(uint8_t *p_src, ui_options_t p_type, uint32_t p_max, uint32_t p_min) {

    uint32_t intValue;
    uint32_t i;

    switch (p_type) {
        case UI_OPTION_NUM:
            for (i = 0; i < strlen(p_src); i++) {
                if (isdigit(p_src[i]) == 0) {
                    return false;
                }
            }
            intValue = atoi(p_src);
            if ((intValue < p_min) || (intValue > p_max)) {
                return false;
            }
            break;

        case UI_OPTION_ALPHANUM:
            for (i = 0; i < strlen(p_src); i++) {
                if (isalnum(p_src[i]) == 0) {
                    return false;
                }
            }
            if (strlen(p_src) > p_max) {
                return false;
            }
            break;

        case UI_OPTION_ALPHA:
            for (i = 0; i < strlen(p_src); i++) {
                if (isalpha(p_src[i]) == 0) {
                    return false;
                }
            }
            if (strlen(p_src) > p_max) {
                return false;
            }
            break;
    }
    return true;
}

bool UI_waitForInput(uint8_t *p_dest) {

    uint8_t bytesReceived;

    bytesReceived = USB_receive(p_dest);
    if (bytesReceived > 0) {
        p_dest[bytesReceived] = '\0';
        return true;
    }
    return false;
}

/*
 * End of File
 */
