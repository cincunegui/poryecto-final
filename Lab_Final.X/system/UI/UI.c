

#include "UI.h"
#include "../../framework/USB/USB_fwk.h"
#include <string.h>
#include <ctype.h>
#include <xc.h>
#include "string.h"
#include <time.h>
#include "events.h"
#include "USB_fwk.h"
#include "semphr.h"


//PALABRAS PARA DESPLEGAR
const uint8_t ui_welcomeText[] = "Bienvenido:\n Elija las siguientes opciones para continuar...\n";
const uint8_t ui_optionsText[] = "Indique la opción deseada:\n1) Setear hora del sistema\n2)Consultar hora del sistema\n3) Consultar eventos activos\n4) Agregar evento\n5) Quitar evento\n";
const uint8_t Error[]        = "opcion incorrecta, intente nuevamente...\n";
const uint8_t Ingresar1[]    = "Ingresar Año...\n";
const uint8_t Ingresar2[]    = "Ingresar Dia...\n";
const uint8_t Ingresar3[]    = "Ingresar Mes...\n";
const uint8_t Ingresar4[]    = "Ingresar Hora...\n";
const uint8_t Ingresar5[]    = "Ingresar min...\n";
const uint8_t Ingresar6[]    = "Ingresar Seg...\n";
const uint8_t Ingresar7[]    = "Ingresar lugar a borrar...\n";
const uint8_t Ingresar8[]    = "Ingresar comando...\n";
const uint8_t Ingresar9[]    = "Ingresar Parametro...\n";
const uint8_t Ingresar10[]   = "Ingresar color...\n";
const uint8_t Ingresar11[]   = "Ingresar time...\n";
const uint8_t Ingresar12[]   = "Ingresar la evento a Consultar...\n";
struct tm CurrentTime;// UTILIZO PARA GUARDAR LA HORA DEL RTCC
USB_checkStatus( );

void UI_Case( void ) {
    static ui_menu_states_t menuState;
    static uint8_t rxData[24];
    // si esta USB conectado, ENVIO mensaje Bienvenida
    // sino no hago nada, porque no hay nada para interactuar
    if( USB_getConnectedStatus() == true ) { 
        if( USB_send((uint8_t*) ui_welcomeText) == true ) {
            memset(rxData, 0, sizeof (rxData));
            menuState = UI_MENU_STATE_OPTIONS_SHOW;
        }
        switch( menuState ){             
            case( UI_MENU_STATE_OPTIONS_SHOW): //MUESTRO EL MENU
                if( USB_send((uint8_t*) ui_optionsText) == true ){
                    memset(rxData, 0, sizeof (rxData));
                    menuState = UI_MENU_STATE_OPTIONS_WAIT;
                }
            case( UI_MENU_STATE_OPTIONS_WAIT): //ESPERO ENTRADA
                if( UI_waitForInput(rxData) == true ) {
                    menuState = UI_MENU_STATE_OPTIONS_CHECK;
                }
            case( UI_MENU_STATE_OPTIONS_CHECK):
                if( (UI_checkValidOption(rxData, UI_OPTION_NUM, 5, 1)) == true ) {
                    menuState = UI_MENU_STATE_OPTIONS_ENTRADA;
                }
                else {// si hay error, indico que hay error y muestro el menu nuevamente
                    if( USB_send((uint8_t*) Error) == true ){
                        memset(rxData, 0, sizeof (rxData));
                        menuState = UI_MENU_STATE_OPTIONS_SHOW;
                    }
                }
           // despues de este case me muevo dependiendo de lo que haya elegito el usuario
           //luego de hacer lo pedido vuelvo a mostrar opciones     
            case( UI_MENU_STATE_OPTIONS_ENTRADA):
                int Entrada = UI_MENU_STATE_OPTIONS_ENTRADA + atoi(rxData);
                switch( Entrada ){
                    case( UI_MENU_STATE_SET_TIMEDATE ):
                        Set_Date();
                        menuState = UI_MENU_STATE_OPTIONS_SHOW;
                    case( UI_MENU_STATE_GET_TIMEDATE ):
                        if(Get_Date()){
                            menuState = UI_MENU_STATE_OPTIONS_SHOW;
                        }                        
                    case( UI_MENU_STATE_GET_EVENTS ): 
                        if( USB_send(EVT_isAvailable( )) == true ){
                            menuState = UI_MENU_STATE_OPTIONS_SHOW;
                         }
                    case( UI_MENU_STATE_SET_EVENTS ):  
                     // activo semaforo para trabajar con el arreglo de eventos[i]  
                        xSemaphoreTake(Mutex,portMAX_DELAY);                        
                        Add_Events();
                        menuState = UI_MENU_STATE_OPTIONS_SHOW;                        
                        xSemaphoreGive(Mutex);
                    case( UI_MENU_STATE_DELETE_EVENTS ):
                        // activo semaforo para trabajar con el arreglo de eventos[i]  
                        xSemaphoreTake(Mutex,portMAX_DELAY);                      
                        Delete_Events();
                        menuState = UI_MENU_STATE_OPTIONS_SHOW;                      
                        xSemaphoreGive(Mutex);
                                               
                }

        }
    }
}

bool UI_waitForInput( uint8_t *p_dest ) {
    uint8_t bytesReceived;

    bytesReceived = USB_receive(p_dest);
    if( bytesReceived > 0 ) {
        p_dest[bytesReceived] = '\0';
        return true;
    }
    return false;
}

bool UI_checkValidOption( uint8_t *p_src, ui_options_t p_type, uint32_t p_max, uint32_t p_min ) {
    uint32_t intValue;
    uint32_t i;

    switch( p_type ) {
        case UI_OPTION_NUM:
            for( i = 0; i < strlen(p_src); i++ ) {
                if( isdigit(p_src[i]) == 0 ) {
                    return false;
                }
            }
            intValue = atoi(p_src);
            if( (intValue < p_min) || (intValue > p_max) ) {
                return false;
            }
            break;

        case UI_OPTION_ALPHANUM:
            for( i = 0; i < strlen(p_src); i++ ) {
                if( isalnum(p_src[i]) == 0 ) {
                    return false;
                }
            }
            if( strlen(p_src) > p_max ) {
                return false;
            }
            break;

        case UI_OPTION_ALPHA:
            for( i = 0; i < strlen(p_src); i++ ) {
                if( isalpha(p_src[i]) == 0 ) {
                    return false;
                }
            }
            if( strlen(p_src) > p_max ) {
                return false;
            }
            break;
    }
    return true;
}


void Consultar_evento(void){
    uint8_t buff;
    uint8_t i = 0;
    uint8_t buff_salida[10];
    if( USBGetDeviceState() >= CONFIGURED_STATE && !USBIsDeviceSuspended()){
        putsUSBUSART((uint8_t*)OPCIONES4);
        //putUSBUSART(Ingresar12, strlen(Ingresar12));
        CDCTxService();
        buff = getsUSBUSART(buff_salida, sizeof (buff_salida));
        CDCTxService();
        if (buff>0){
            
            while ( i <= EVENT_MAX) {
                buff = atoi(buff_salida);
                if (eventos[i].command == buff) { //podria no ir                    
                    itoa(buff_salida,eventos[i].command,2);
                    putsUSBUSART(buff_salida);
                    //putUSBUSART(buff_salida, strlen(buff_salida)); 
                    CDCTxService();
                    itoa(buff_salida,eventos[i].param,2);
                    putsUSBUSART(buff_salida);
                    //putUSBUSART(buff_salida, strlen(buff_salida)); 
                    CDCTxService();
                    itoa(buff_salida,eventos[i].color,2);
                    putsUSBUSART(buff_salida);
                    //putUSBUSART(buff_salida, strlen(buff_salida));
                    CDCTxService();
                    itoa(buff_salida,eventos[i].time,2);
                    putsUSBUSART(buff_salida);
                    //putUSBUSART(buff_salida, strlen(buff_salida)); 
                    CDCTxService();                   
                }
            i++;
            }    
        }
    }        
}
    

void Add_Events(void){
    app_event_t datos;
    uint32_t buff;
    uint8_t i = 0;
    uint8_t buff_salida[10];    
    if( USB_getConnectedStatus( )){
        putsUSBUSART((uint8_t*)Ingresar8);
        CDCTxService();
        buff = getsUSBUSART(buff_salida, sizeof (buff_salida));
        CDCTxService();
        if (buff>0){
            buff = atoi(buff_salida);
            datos.command = buff ;
            putsUSBUSART((uint8_t*)Ingresar9);
            CDCTxService();
            buff = getsUSBUSART(buff_salida, sizeof (buff_salida));
            CDCTxService();
        }
        if (buff>0){
            buff = atoi(buff_salida);
            datos.param = buff ;
             putsUSBUSART((uint8_t*)Ingresar10);
            CDCTxService();
            buff = getsUSBUSART(buff_salida, sizeof (buff_salida));
            CDCTxService();
        }
        if (buff>0){
            buff = atoi(buff_salida);
            datos.color = buff ;
            putsUSBUSART((uint8_t*)Ingresar11);
            CDCTxService();
            buff = getsUSBUSART(buff_salida, sizeof (buff_salida));
            CDCTxService();
        }    
            if (buff>0){
            buff = atoi(buff_salida);
            datos.time = buff ;
        }
    }
    while ( i <= EVENT_MAX) {

        if (eventos[i].command == 0xFF) {
            eventos[i].command = datos.command; // 0 es apagar los led
            eventos[i].param = datos.param; //El led que se debe prender o apagar (0 a 7)
            eventos[i].color = datos.color; //que color se enciende en el led
            eventos[i].time = datos.time; //cuando se ejecuta el evento
        }
        i++;
    }
}

void Delete_Events(void){
    uint8_t buff;
    uint8_t buffer[10];
    if( USB_getConnectedStatus( )){
        putsUSBUSART((uint8_t*)Ingresar7);
        CDCTxService();
        buff = getsUSBUSART(buffer, sizeof (buffer));
        if (buff>0)
        buff = atoi(buffer);
        eventos[buff].command = 0xFF; //Simplemente borro el lugar que me dicen
    }
}

bool Get_Date(void) {  // trabaja con la estructura CurrentTime definida global a esta carpeta
    
    char buff[10];
    bool ret= false;
    ret = RTCC_TimeGet(&CurrentTime);
        if( USB_getConnectedStatus( )){           
            itoa(buff,CurrentTime.tm_hour,2);           
            putsUSBUSART(buff); //Envio hora            
            CDCTxService();
            itoa(buff,CurrentTime.tm_min,2);
            putsUSBUSART(buff); //Envio minuto
            CDCTxService();
            itoa(buff,CurrentTime.tm_sec,2);
            putsUSBUSART(buff); //Envio segundo
            CDCTxService();
           ret= true;  
        }
    return  ret;
}

void Set_Date(void) {
    struct tm Ext_Time;                                                        
    struct tm *Init_Time;       
    struct tm *formato_fecha;
    uint8_t buff[10];
    time_t tiempo_t;
    uint8_t tiempo;
    if( USB_getConnectedStatus( )){
        
        putsUSBUSART((uint8_t*)Ingresar4);  //ingrese hora           
        CDCTxService();
        tiempo = getsUSBUSART(buff, sizeof (buff));
        Ext_Time.tm_hour = tiempo;
        CDCTxService();
        
        putsUSBUSART((uint8_t*)Ingresar5); //ingrese min
        CDCTxService();
        tiempo = getsUSBUSART(buff, sizeof (buff));
        Ext_Time.tm_min = tiempo;
        CDCTxService();

        putsUSBUSART((uint8_t*)Ingresar6); //ingrese seg
        CDCTxService();
        tiempo= getsUSBUSART(buff, sizeof (buff));
        Ext_Time.tm_sec = tiempo;
        CDCTxService();
        
        Init_Time = &Ext_Time;
        
       tiempo_t =  mktime(Init_Time);         // paso a time_t para cargar los campos restantes

        formato_fecha = gmtime(&tiempo_t);

        RTCC_TimeSet(formato_fecha);
    }
}

