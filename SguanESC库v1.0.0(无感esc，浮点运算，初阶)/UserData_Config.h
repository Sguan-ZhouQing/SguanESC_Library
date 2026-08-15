#ifndef __USERDATA_CONFIG_H
#define __USERDATA_CONFIG_H

/**
 * @description: 宏定义0-10决定“电机的控制模式”(默认使用“电调电压开环控制”模式)
 * @reminder: 0->MODE_Voltag_OPEN       电调电压开环控制        (仅角度闭环)
 * @reminder: 1->MODE_Velocity_Single   速度单环控制           (速度闭环控制)
 * @return {*}
 */
#define Define_Run_Mode 1

/**
 * @description: 宏定义0-1决定“电机速度环”的控制方式(默认使用软件获取)
 * @reminder: 0->使用硬件ADC采样虚拟中性“BLDC电压点”
 * @reminder: 1->使用软件求平均，得到三相相电压的虚拟中性电压点
 * @return {*}
 */
#define Switch_MOTOR_Umid_Get 1


// 定时器中断参数设计
#define TIM_T 5e-5                          // 最大频率的控制周期


#endif // USERDATA_CONFIG_H
