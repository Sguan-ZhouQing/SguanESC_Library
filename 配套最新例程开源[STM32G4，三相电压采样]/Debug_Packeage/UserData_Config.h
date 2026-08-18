#ifndef __USERDATA_CONFIG_H
#define __USERDATA_CONFIG_H

/**
 * @description: 宏定义0-10决定“电机的控制模式”(默认使用“电调电压开环控制”模式)
 * @reminder: 0->MODE_Voltag_OPEN       电调电压开环控制        (仅角度闭环)
 * @reminder: 1->MODE_Velocity_Single   速度单环控制           (速度闭环控制)
 * @return {*}
 */
#define Define_Run_Mode 0

/**
 * @description: 宏定义数值决定“电机的静止时启动卡死的重置时间”(默认0.3s刷新一次)
 * @reminder: （如果电机有电压，但是电机静止...电机存在通电锁轴的嫌疑）
 * @reminder: （此时电机换相__sector会朝目标方向手动换相，以打破平衡）
 * @reminder: （数值不宜过小，会干扰电机从负速度跨越零速到正速度...或从正跨越零速到负）
 * @return {*}
 */
#define Define_Run_WaitingTime 0.3f

/**
 * @description: 宏定义数值决定“上一个WaitingTime的判断条件”(默认We小于1.0f,Ubus大于0.02f)
 * @reminder: （如果电机有电压，但是电机静止...电机存在通电锁轴的嫌疑）
 * @reminder: Define_We_Min->电子角速度的判断条件
 * @reminder: Define_Ubus_Max->电调输入电压的判断条件
 * @return {*}
 */
#define Define_We_Min 1.0f
#define Define_Ubus_Max 0.02f

/**
 * @description: 宏定义0-1决定“电机速度环”的控制方式(默认使用软件获取)
 * @reminder: 0->使用硬件ADC采样虚拟中性“BLDC电压点”
 * @reminder: 1->使用软件求平均，得到三相相电压的虚拟中性电压点
 * @return {*}
 */
#define Switch_MOTOR_Umid_Get 1


// 定时器中断参数设计
#define TIM_T 5e-5f                         // 最大频率的控制周期


#endif // USERDATA_CONFIG_H
