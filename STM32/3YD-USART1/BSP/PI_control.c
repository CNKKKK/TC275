/*����λ�ñջ���PI����*/
extern float angle_pu;
float angle_gi;
unsigned long lastTime;
u16 PIfunction(double kp,double ki,float angle_gi,float angle_pu)
{
		unsigned long now = millis(); 
		double timeChange = (double)(now - lastTime);
		double et = angle_gi - angle_pu;  //����ֵ������ֵ֮��Ĳ�ֵ ��������
		double etSum +=(et*timeChange);
		
		lastErr = et;  
		lastTime = now; 
		return	speed_gi = kp * et + ki * etSum + kd * dEt;
}