
/* 
 *  serial.h  
 *
 */
#include "FreeRTOS.h"
#include "queue.h"

typedef int xComPortHandle;

extern QueueHandle_t vSerialRxQueue;

xComPortHandle xSerialPortInitMinimal(int baudRate, int queueLength);

int xSerialGetChar(xComPortHandle xPort, signed char *cRxedChar, int timeout);

int vSerialPutString(xComPortHandle xPort, signed char *pcWelcomeMessage, unsigned short len);

int xSerialPutChar(xComPortHandle xPort, signed char c, int timeout);

