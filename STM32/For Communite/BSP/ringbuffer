#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#define BUFFER_MAX 6000//定义缓冲区大小

typedef struct{
	unsigned int headPosition;//缓冲区头部指针
	unsigned int tailPosition;
	float ringBuf[BUFFER_MAX];//缓冲区大小
	
}ringBuffer_t;

void RingBuf_Write(float data);//向缓冲区写入一个字节
unsigned char RingBuf_Read(float* pData);//读取缓冲区一个字节
unsigned char VoltageSend(void);
void stop(void);
#endif
