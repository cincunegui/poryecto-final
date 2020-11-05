
#include "events.h"
#include "../app.h"
#include "../../framework/LEDs_RGB/LEDs_RGB_fwk.h"


app_event_t eventos[EVENT_MAX];


static void EVT_execute( app_event_t p_event ){
    if(p_event.param==EVENT_PARAM_ON){
        RGB_setLedColor( p_event.param, p_event.color );
    }else if(p_event.param==EVENT_PARAM_OFF){
        RGB_setLedColor( p_event.param, RGB_BLACK );
    }
    RGB_showLeds( LED_RGB_SIZE );
}


void EVT_checkAndExecute( void ){
    uint8_t i;

    for(i=0; i<EVENT_MAX; i++){
        if((eventos[i].command!=EVENT_PARAM_NOT_CONFIG) && (eventos[i].time>=APP_getTimestamp( ))){
            EVT_execute( eventos[i] );
            eventos[i].command=EVENT_PARAM_NOT_CONFIG; // Borro el evento
        }
    }
}

int8_t EVT_isAvailable( void ){
    uint8_t i;

    for(i=0; i<EVENT_MAX; i++){
        if(eventos[i].command==EVENT_PARAM_NOT_CONFIG){
            return i;
        }
    }
    return -1;
}

bool EVT_add( app_event_t p_event ){
    uint8_t i;

    for(i=0; i<EVENT_MAX; i++){
        if(eventos[i].command==EVENT_PARAM_NOT_CONFIG){
            eventos[i]=p_event;
            return true;
        }
    }
    return false;
}

