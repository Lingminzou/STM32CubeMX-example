
/* 
 *  serial.h  
 *
 */
#include "FreeRTOS.h"
#include "queue.h"

typedef int xComPortHandle;

xComPortHandle xSerialPortInitMinimal(int baudRate, int queueLength);

BaseType_t xSerialGetChar(xComPortHandle xPort, signed char *cRxedChar, uint32_t timeout);

BaseType_t vSerialPutString(xComPortHandle xPort, signed char *pcWelcomeMessage, unsigned short len);

BaseType_t xSerialPutChar(xComPortHandle xPort, signed char c, uint32_t timeout);

