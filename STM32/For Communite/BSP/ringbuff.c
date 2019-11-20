#include "ringbuff.h"

ringBuffer_t buffer = {0,0,{0}};//初始化为0

void RingBuf_Write(float data){//向缓冲区写入一个字节
	buffer.ringBuf[buffer.tailPosition] = data;//从尾部追加
//	printf("writeok\r\n");
	if(++buffer.tailPosition >= BUFFER_MAX)//尾结点偏移
		buffer.tailPosition = 0;//大于数组长度，最大数组长度归零，形成环形数组
	//若尾部节点追到头部节点，则修改头节点偏移位置丢弃早期数据
	if(buffer.tailPosition == buffer.headPosition)
		if(++buffer.headPosition >= BUFFER_MAX)
			buffer.headPosition=0;
}


unsigned char RingBuf_Read(float* pData){//读取缓存区一个字节数据
	if(buffer.headPosition == buffer.tailPosition){//头尾相接表示缓冲区为空
		return 0;//读取失败 返回0
	}
	else{
		*pData = buffer.ringBuf[buffer.headPosition];//如果缓冲区为空则取头节点并偏移头节点
		if(++buffer.headPosition>=BUFFER_MAX)
			buffer.headPosition = 0;
		return 1;//读取成功返回1
	}
}
