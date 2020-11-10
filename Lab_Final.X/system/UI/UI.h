

#ifndef UI_H    /* Guard against multiple inclusion */
#define UI_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_EVENTOS     10
    
typedef enum{
    UI_OPTION_NUM,
    UI_OPTION_ALPHANUM,
    UI_OPTION_ALPHA,
}ui_options_t;

typedef enum{
    UI_MENU_STATE_INIT_SHOW,
    UI_MENU_STATE_INIT_WAIT,
    UI_MENU_STATE_OPTIONS_SHOW,
    UI_MENU_STATE_OPTIONS_WAIT,
    UI_MENU_STATE_OPTIONS_CHECK,
    UI_MENU_STATE_OPTIONS_ENTRADA,
    UI_MENU_STATE_SET_TIMEDATE,
    UI_MENU_STATE_GET_TIMEDATE,
    UI_MENU_STATE_GET_EVENTS,
    UI_MENU_STATE_SET_EVENTS,
    UI_MENU_STATE_DELETE_EVENTS,
}ui_menu_states_t;
    
typedef struct {
    uint8_t command;
    uint8_t param;
    uint8_t color;
    uint32_t time;
} app_event_t;


    void Consultar_evento(void);
    void Delete_Events(void);
    bool Get_Date(void);
    void Set_Date(void);
    void Add_Events(void);
    void UI_Case( void );
    
    bool UI_waitForInput( uint8_t *p_dest );
    
    bool UI_checkValidOption( uint8_t *p_src, ui_options_t p_type, uint32_t p_max, uint32_t p_min );

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* UI_H */

/* *****************************************************************************
 End of File
 */
