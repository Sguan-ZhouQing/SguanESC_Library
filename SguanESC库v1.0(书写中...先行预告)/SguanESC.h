#ifndef __SGUANESC_H
#define __SGUANESC_H

#include "Sguan_GIC.h"                      // GIC增益分段控制器
#include "Sguan_LPF.h"                      // LPF一阶低通滤波器
#include "Sguan_MotorStatus.h"              // MotorStatus电调状态机
#include "Sguan_PID.h"                      // PID传统闭环控制
#include "Sguan_printf.h"                   // printf通信调试
#include "Sguan_SWPWM.h"                    // SWPWM六步换相PWM调制

// ╔═════════════════════════════════════════════════════════╗
// ║                       电调控制模式                       ║
// ╚═════════════════════════════════════════════════════════╝
#define MODE_Voltag_OPEN        0x00        // 电压开环控制（Sguan.esc.Ubus_in）
#define MODE_Velocity_Single    0x01        // 速度单环控制（Sguan.esc.Target_Speed）

typedef struct{
    LPF_STRUCT LPF_Umid;
    LPF_STRUCT LPF_Ua;
    LPF_STRUCT LPF_Ub;
    LPF_STRUCT LPF_Uc;
}__MOTOR_LPF_STRUCT;

typedef struct{
    float Target_Speed;
    float Ubus_in;

    uint8_t sector;

    float Umid;
    float Ua;
    float Ub;
    float Uc;

    float Real_VBUS;
    float Real_Temp;
}__MOTOR_ESC_STRUCT;

typedef struct{
    uint8_t Poles;
    float VBUS;

    int8_t Motor_Dir;
    uint32_t Duty;

    uint32_t ADC_Precision;                 // (参数设计)ADC采样精度,如12位精度为4096
    float MCU_Voltage;                      // (参数设计)DSP/单片机的ADC基准电压
    float Uabc_Gain;
}__MOTOR_QUANTIZE_STRUCT;

typedef struct{
    // ============================= ①电机标识位 =================================
    uint8_t status;

   // ============================= ②嵌套结构体 =================================
    __MOTOR_LPF_STRUCT lpf;
    __MOTOR_ESC_STRUCT esc;
    __MOTOR_QUANTIZE_STRUCT motor;
    PRINTF_STRUCT txdata;

    // =========================== ③简易控制函数 =================================
    void (*Func_Start)(void);               // 【函数】control控制接口->启动电机
    void (*Func_Stop)(void);                // 【函数】control控制接口->停止电机
    void (*Func_Set_Ubus)(float);           // 【函数】control控制接口->设计目标电压
    void (*Func_Set_Velocity)(float);       // 【函数】control控制接口->设计目标转速
    void (*Func_Set_TXdata)(uint8_t,float); // 【函数】control控制接口->数据打印
}SguanESC_System_STRUCT;

extern SguanESC_System_STRUCT Sguan;

void SguanESC_High_Loop(void);
void SguanESC_Low_Loop(void);
void SguanESC_Printf_Loop(uint8_t *data, uint16_t length);
void SguanESC_main_Loop(void);


#endif // SGUANESC_H
