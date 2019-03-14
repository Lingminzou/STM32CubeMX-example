#ifndef APP_FIFO_H_
#define APP_FIFO_H_

struct kfifo {
    unsigned char *buffer;     /* the buffer holding the data */
    unsigned int size;         /* the size of the allocated buffer */
    unsigned int in;           /* data is added at offset (in % size) */
    unsigned int out;          /* data is extracted from off. (out % size) */
    //spinlock_t *lock;        /* protects concurrent modifications */
};

/*********************************************************************************

#define MY_FIFO_LEN   64
struct kfifo myFifo;
static unsigned char fifoBuffer[MY_FIFO_LEN] = {0x00};

kfifo_init(&myFifo, fifoBuffer, MY_FIFO_LEN);

*********************************************************************************/
void kfifo_init(struct kfifo *fifo, unsigned char *buffer, unsigned int size);

unsigned int kfifo_put(struct kfifo *fifo, const unsigned char *buffer, unsigned int len);

unsigned int kfifo_get(struct kfifo *fifo, unsigned char *buffer, unsigned int len);

unsigned int kfifo_peek(struct kfifo *fifo, unsigned char *buffer, unsigned int len);

unsigned int kfifo_len(struct kfifo *fifo);

#endif

