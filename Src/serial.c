/*
 * serial.c 
 *
 */

#include "serial.h"
#include "usbd_cdc_if.h"
#include "task.h"
#include "app_fifo.h"

extern TaskHandle_t xConsoleHandle;
extern struct kfifo cdcRxFifo;

xComPortHandle xSerialPortInitMinimal(int baudRate, int queueLength)
{
    baudRate = baudRate;
    queueLength = queueLength;

    return 0x00;
}

int xSerialGetChar(xComPortHandle xPort, signed char *cRxedChar, int timeout)
{
    static uint32_t i = 0x00;

    xPort = xPort;

    if(i == 0x00)
    {
        ulTaskNotifyTake(pdTRUE, timeout);

        i = kfifo_len(&cdcRxFifo);
    }

    if(0x01 == kfifo_get(&cdcRxFifo, cRxedChar, 0x01))
    {
        i--;
    }

    return pdPASS;
}

int vSerialPutString(xComPortHandle xPort, signed char *pcWelcomeMessage, unsigned short len)
{
    xPort = xPort;

retry:

   if(0x00 != CDC_Transmit_FS(pcWelcomeMessage, len))
   {
       osDelay(10);
       goto retry;
   }
}

int xSerialPutChar(xComPortHandle xPort, signed char c, int timeout)
{
    xPort = xPort;
    timeout = timeout;

    char tx = c;

retry:

    if(0x00 != CDC_Transmit_FS(&tx, 0x01))
    {
        osDelay(10);
        goto retry;
    }
}

