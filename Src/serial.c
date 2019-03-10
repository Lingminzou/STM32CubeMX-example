/*
 * serial.c 
 *
 */

#include "serial.h"
#include "usbd_cdc_if.h"

QueueHandle_t vSerialRxQueue = NULL;

xComPortHandle xSerialPortInitMinimal(int baudRate, int queueLength)
{
    baudRate = baudRate;

    vSerialRxQueue = xQueueCreate(queueLength, sizeof(char));

    return 0x00;
}

int xSerialGetChar(xComPortHandle xPort, signed char *cRxedChar, int timeout)
{
    xPort = xPort;

    if(NULL != vSerialRxQueue)
    {
        if(xQueueReceive(vSerialRxQueue, cRxedChar, timeout))
        {

        }
    }
}

int vSerialPutString(xComPortHandle xPort, signed char *pcWelcomeMessage, unsigned short len)
{
    xPort = xPort;

   if(0x00 != CDC_Transmit_FS(pcWelcomeMessage, len))
   {

   }
}

int xSerialPutChar(xComPortHandle xPort, signed char c, int timeout)
{
    xPort = xPort;
    timeout = timeout;

    char tx = c;

    if(0x00 != CDC_Transmit_FS(&tx, 0x01))
    {

    }
}

