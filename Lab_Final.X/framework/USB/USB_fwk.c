
#include "USB_fwk.h"
#include "../../mcc_generated_files/usb/usb.h"
#include "../../mcc_generated_files/pin_manager.h"
#include <string.h>


static uint8_t usb_readBuffer[64];
static uint8_t usb_writeBuffer[64];
static bool usb_connected = false;


 
void USB_checkStatus( void ) {
    if( (USBGetDeviceState() >= CONFIGURED_STATE) && !USBIsDeviceSuspended() ) {
        CDCTxService();
        LEDB_SetHigh();
        usb_connected = true;
    }
    else {
        LEDB_SetLow();
        usb_connected = false;
    }
}

bool USB_getConnectedStatus( void ) {
    return usb_connected;
}

void USB_setConnectedStatus( bool p_status ) {
    usb_connected = p_status;
}

uint8_t USB_receive( uint8_t *p_dest ) {
    return getsUSBUSART(p_dest, CDC_DATA_IN_EP_SIZE);
}

bool USB_send( uint8_t *p_src ) {
    if( USBUSARTIsTxTrfReady() ){
        putsUSBUSART(p_src);
        return true;
    }
    else {
        return false;
    }
}



/* *****************************************************************************
 End of File
 */
