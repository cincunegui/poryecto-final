
#include "app.h"
#include "../utils/utils.h"
#include "../mcc_generated_files/rtcc.h"


static ut_tmrDelay_t app_update_tmr;
static struct tm app_timeDate;
static time_t app_timestamp;


void APP_updateTime( void ){
    if(UT_delayms( &app_update_tmr, 1000 )==true){
        if(RTCC_TimeGet( &app_timeDate )==true){
            app_timestamp=mktime( &app_timeDate );
        }
    }
}

time_t APP_getTimestamp( void ){
    return app_timestamp;
}
