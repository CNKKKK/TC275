#include "ringbuff.h"

ringBuffer_t buffer = {0,0,{0}};//��ʼ��Ϊ0

void RingBuf_Write(float data){//�򻺳���д��һ���ֽ�
	buffer.ringBuf[buffer.tailPosition] = data;//��β��׷��
//	printf("writeok\r\n");
	if(++buffer.tailPosition >= BUFFER_MAX)//β���ƫ��
		buffer.tailPosition = 0;//�������鳤�ȣ�������鳤�ȹ��㣬�γɻ�������
	//��β���ڵ�׷��ͷ���ڵ㣬���޸�ͷ�ڵ�ƫ��λ�ö�����������
	if(buffer.tailPosition == buffer.headPosition)
		if(++buffer.headPosition >= BUFFER_MAX)
			buffer.headPosition=0;
}


unsigned char RingBuf_Read(float* pData){//��ȡ������һ���ֽ�����
	if(buffer.headPosition == buffer.tailPosition){//ͷβ��ӱ�ʾ������Ϊ��
		return 0;//��ȡʧ�� ����0
	}
	else{
		*pData = buffer.ringBuf[buffer.headPosition];//���������Ϊ����ȡͷ�ڵ㲢ƫ��ͷ�ڵ�
		if(++buffer.headPosition>=BUFFER_MAX)
			buffer.headPosition = 0;
		return 1;//��ȡ�ɹ�����1
	}
}
