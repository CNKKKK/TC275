#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#define BUFFER_MAX 800//���建������С

typedef struct{
	unsigned int headPosition;//������ͷ��ָ��
	unsigned int tailPosition;
	float ringBuf[BUFFER_MAX];//��������С
	
}ringBuffer_t;

void RingBuf_Write(float data);//�򻺳���д��һ���ֽ�
unsigned char RingBuf_Read( float* pData);//��ȡ������һ���ֽ�
void RingBuf_Write_A(float data);//�򻺳���д��һ���ֽ�
unsigned char RingBuf_Read_A( float* pData);//��ȡ������һ���ֽ�

#endif
