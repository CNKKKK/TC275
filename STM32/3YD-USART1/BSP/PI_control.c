/*用于位置闭环的PI控制*/
extern float angle_pu;
float angle_gi;
unsigned long lastTime;
u16 PIfunction(double kp,double ki,float angle_gi,float angle_pu)
{
		unsigned long now = millis(); 
		double timeChange = (double)(now - lastTime);
		double et = angle_gi - angle_pu;  //反馈值于输入值之间的差值 比例环节
		double etSum +=(et*timeChange);
		
		lastErr = et;  
		lastTime = now; 
		return	speed_gi = kp * et + ki * etSum + kd * dEt;
}