#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

//#define BUFFER_MAX 100//���建������С

typedef struct{
	unsigned int headPosition;//������ͷ��ָ��
	unsigned int tailPosition;
	const int ringBuf[35];//��������С
	
}ringBuffer_t;

void RingBuf_Write(const float data);//�򻺳���д��һ���ֽ�
unsigned char RingBuf_Read(float* pData);//��ȡ������һ���ֽ�

#endif
