#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "ring_queue_test.h"
#include "ring_queue.h"

#define RING_QUEUE_TEST_BYTE   TRUE

#define BUFFER_SIZE 8

//单位2ms
#define RING_QUEUE_WRITE_CYCLE      100
#define RING_QUEUE_READ_CYCLE       100


RingQueue_t RingQueue;
uint8_t RingQueueBuffer[BUFFER_SIZE + 1];
RingQueueBase_t RingQueueFreeSpace = BUFFER_SIZE;

bool RingQueueWriteTest()
{
    RingQueueBase_t writeSize;
    RingQueueBase_t writtenSize;
    uint8_t writeData[BUFFER_SIZE + 1];
    static uint8_t writeCount = 0;

    srand((uint32_t)clock());

    writeSize = (rand() % BUFFER_SIZE) + 1;

    for(int i = 0;i < writeSize;++i)
    {
        writeData[i] = writeCount + i;
    }

#if RING_QUEUE_TEST_BYTE
    for(writtenSize = 0;writtenSize < writeSize;++writtenSize)
    {
        if(FALSE == RingQueueWriteByte(&RingQueue, writeData[writtenSize]))
        {
            break;
        }
    }
#else
    writtenSize = RingQueueWriteData(&RingQueue, writeData, writeSize);
#endif

    writeCount += writtenSize;
    RingQueueFreeSpace -= writtenSize;

    printf("已写入数据:");
    for(int i = 0;i < writtenSize;++i)
    {
        printf("%d ", writeData[i]);
    }
    printf("(执行写入%d字节,剩余空间%d字节,实际写入%d字节)\n", writeSize, RingQueueFreeSpace, writtenSize);

    if(writeSize != writtenSize && (RingQueueFreeSpace != 0u))
    {
        printf("队列写入实现有误,执行写入%d字节,剩余空间%d字节,仅写入了%d字节\n",writeSize, RingQueueFreeSpace, writtenSize);

        return FALSE;
    }
    
    return TRUE;
}

bool RingQueueReadTest()
{
    RingQueueBase_t readSize;
    RingQueueBase_t readingSize;
    uint8_t readData[BUFFER_SIZE + 1];
    static RingQueueBase_t readCount = 0; 

    srand((uint32_t)clock());

    readSize = (rand() % BUFFER_SIZE) + 1;

#if RING_QUEUE_TEST_BYTE
    for(readingSize = 0;readingSize < readSize;++readingSize)
    {
        if(FALSE == RingQueueReadByte(&RingQueue, readData+readingSize))
        {
            break;
        }
    }
#else
    readingSize = RingQueueReadData(&RingQueue, readData, readSize);
#endif

    RingQueueFreeSpace += readingSize;

    printf("已读取数据:");
    for(int i = 0;i < readingSize;++i)
    {
        printf("%d ", readData[i]);
        if(readCount != readData[i])
        {
            printf("\n队列实现有误,数据不是递增的\n");
            return FALSE;
        }
        ++readCount;
    }
    printf("(执行读取%d字节,剩余空间%d字节,实际读取到%d字节)\n", readSize, RingQueueFreeSpace, readingSize);

    if(readSize != readingSize && (RingQueueFreeSpace != BUFFER_SIZE))
    {
        printf("队列读取实现有误,执行读取%d字节,剩余空间%d字节,仅读取到%d字节\n",readSize, RingQueueFreeSpace, readingSize);

        return FALSE;
    }

    return TRUE;
}

void RingQueueLoopTest()
{
    uint32_t timeCount = 0;
    RingQueueInit(&RingQueue, RingQueueBuffer, sizeof(RingQueueBuffer));

    for (;;)
    {
        ++timeCount;

        if((timeCount % RING_QUEUE_WRITE_CYCLE) == 0)
        {
            if(FALSE == RingQueueWriteTest())
            {
                break;
            }
        }

        //sleep(1);
        usleep(1000);

        if((timeCount % RING_QUEUE_READ_CYCLE) == 0)
        {
            if(FALSE == RingQueueReadTest())
            {
                break;
            }
        }

        //sleep(1);
        usleep(1000);
    }
}