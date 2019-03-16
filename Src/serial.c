/*
 * serial.c 
 *
 */

#include "serial.h"
#include "usbd_cdc_if.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "app_fifo.h"

extern TaskHandle_t xConsoleHandle;
extern struct kfifo cdcRxFifo;

xComPortHandle xSerialPortInitMinimal(int baudRate, int queueLength)
{
    baudRate = baudRate;
    queueLength = queueLength;

    return 0x00;
}

BaseType_t xSerialGetChar(xComPortHandle xPort, signed char *cRxedChar, uint32_t timeout)
{
    static uint32_t i = 0x00;

    xPort = xPort;

    if(i == 0x00)
    {
        ulTaskNotifyTake(pdTRUE, timeout);

        i = kfifo_len(&cdcRxFifo);
    }

    if(0x01 == kfifo_get(&cdcRxFifo, (unsigned char*) cRxedChar, 0x01))
    {
        i--;
    }

    return pdPASS;
}

BaseType_t vSerialPutString(xComPortHandle xPort, signed char *pMessage, unsigned short len)
{
    xPort = xPort;

retry:

   if(0x00 != CDC_Transmit_FS((uint8_t*)pMessage, len))
   {
       osDelay(10);
       goto retry;
   }

   return pdTRUE;
}

BaseType_t xSerialPutChar(xComPortHandle xPort, signed char c, uint32_t timeout)
{
    xPort = xPort;
    timeout = timeout;

    char tx = c;

retry:

    if(0x00 != CDC_Transmit_FS((uint8_t*)&tx, 0x01))
    {
        osDelay(10);
        goto retry;
    }

    return pdTRUE;
}

