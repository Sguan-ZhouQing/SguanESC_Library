#ifndef __USERDATA_CONFIG_H
#define __USERDATA_CONFIG_H

/**
 * @description: 宏定义0-10决定“电机的控制模式”(默认使用“速度-电流串级闭环控制”模式)
 * @reminder: 0->MODE_Voltag_OPEN       电调电压开环控制        (仅角度闭环)
 * @reminder: 1->MODE_Velocity_Single   速度单环控制           (速度闭环控制)
 * @return {*}
 */
#define Define_Run_Mode 0

/**
 * @description: 宏定义0-3决定“电机速度环”的控制方式(默认使用GIC控制)
 * @reminder: 0->Control_PID            转速环“PI控制”
 * @reminder: 1->Control_GIC            转速环“GIC增益分段控制”
 * @return {*}
 */
#define Switch_MOTOR_Control_Vel 1




#endif // USERDATA_CONFIG_H
