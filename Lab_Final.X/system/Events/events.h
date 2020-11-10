
#include <machine/int_types.h>
#include "stdint.h"
#include "stdbool.h"
#ifndef EVENTS_H    /* Guard against multiple inclusion */
#define EVENTS_H


    #define EVENT_MAX               8
    #define EVENT_PARAM_ON          1
    #define EVENT_PARAM_OFF         0
    #define EVENT_PARAM_NOT_CONFIG  0xFF

    typedef struct
    {
        uint8_t command;
        uint8_t param;
        uint8_t color;
        uint32_t time;
    } app_event_t;


    void EVT_checkAndExecute( void );
    
    int8_t EVT_isAvailable( void );
    
    bool EVT_add( app_event_t p_event );

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* EVENTS_H */

