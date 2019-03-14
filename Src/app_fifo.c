/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>

#include "app_fifo.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
#define kfifo_Debug(format, args...)    printf(format, ##args)

#define min(x,y) (((x) < (y)) ? (x) : (y))

#define max(x,y) (((x) > (y)) ? (x) : (y))

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void kfifo_init(struct kfifo *fifo, unsigned char *buffer, unsigned int size)
{
	if(size & (size - 1))
	{
		kfifo_Debug("kfifo_init error!!!\n");
	}

	fifo->size = size;

	fifo->buffer = buffer;
	fifo->in = fifo->out = 0;
}

unsigned int kfifo_put(struct kfifo *fifo, const unsigned char *buffer, unsigned int len)
{
	unsigned int l;

	len = min(len, fifo->size - fifo->in + fifo->out);
	/*
	* Ensure that we sample the fifo->out index -before- we
	* start putting bytes into the kfifo.
	*/
	//smp_mb();
	/* first put the data starting from fifo->in to buffer end */
	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
	memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);
	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(fifo->buffer, buffer + l, len - l);

	/*
	* Ensure that we add the bytes to the kfifo -before-
	* we update the fifo->in index.
	*/
	//smp_wmb();
	fifo->in += len;
	
	return len;
}

unsigned int kfifo_get(struct kfifo *fifo, unsigned char *buffer, unsigned int len)
{
	unsigned int l;

	len = min(len, fifo->in - fifo->out);
	/*
	* Ensure that we sample the fifo->in index -before- we
	* start removing bytes from the kfifo.
	*/
	//smp_rmb();
	/* first get the data from fifo->out until the end of the buffer */
	l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
	memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
	/* then get the rest (if any) from the beginning of the buffer */
	memcpy(buffer + l, fifo->buffer, len - l);
	/*
	* Ensure that we remove the bytes from the kfifo -before-
	* we update the fifo->out index.
	*/
	//smp_mb();
	fifo->out += len; 
	
	return len;
}

unsigned int kfifo_peek(struct kfifo *fifo, unsigned char *buffer, unsigned int len)
{
	unsigned int l;

	len = min(len, fifo->in - fifo->out);
	/*
	* Ensure that we sample the fifo->in index -before- we
	* start removing bytes from the kfifo.
	*/
	//smp_rmb();
	/* first get the data from fifo->out until the end of the buffer */
	l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
	memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
	/* then get the rest (if any) from the beginning of the buffer */
	memcpy(buffer + l, fifo->buffer, len - l);
	/*
	* Ensure that we remove the bytes from the kfifo -before-
	* we update the fifo->out index.
	*/
	//smp_mb();
	
	return len;    
}

unsigned int kfifo_len(struct kfifo *fifo)
{
    return fifo->in - fifo->out;
}

