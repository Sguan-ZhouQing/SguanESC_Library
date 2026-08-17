#ifndef __SGUANESC_H
#define __SGUANESC_H

#include "Sguan_LPF.h"                      // LPF一阶低通滤波器
#include "Sguan_MotorStatus.h"              // MotorStatus电调状态机
#include "Sguan_PID.h"                      // PID传统闭环控制
#include "Sguan_PLL.h"                      // PLL锁相环跟踪角度
#include "Sguan_printf.h"                   // printf通信调试
#include "Sguan_SWPWM.h"                    // SWPWM六步换相PWM调制

// ╔═════════════════════════════════════════════════════════╗
// ║                       电调控制模式                       ║
// ╚═════════════════════════════════════════════════════════╝
#define MODE_Voltag_OPEN        0x00        // 电压开环控制（Sguan.esc.Ubus_in）
#define MODE_Velocity_Single    0x01        // 速度单环控制（Sguan.esc.Target_Speed）

typedef struct{
    #if !CONFIG_UMID
    LPF_STRUCT __LPF_Umid;                  // (同步动态LPF)Umid的低通滤波器
    #endif // CONFIG_UMID

    LPF_STRUCT __LPF_Ua;                    // (同步动态LPF)Ua的的低通滤波器
    LPF_STRUCT __LPF_Ub;                    // (同步动态LPF)Ub的的低通滤波器
    LPF_STRUCT __LPF_Uc;                    // (同步动态LPF)Uc的的低通滤波器
    
    LPF_STRUCT LPF_Sign;                    // (固定LPF)对电压信号/速度信号的输入限速
}__MOTOR_LPF_STRUCT;

typedef struct{
    float __Real_Speed;                     // (数据)电机实际机械角速度
    float __Real_We;                        // (数据)电机实际电子角速度

    float __LPF_Wc;                           // (截止频率)LPF_Uabc动态Wc
    float LPF_Max;                          // (限幅)LPF_Uabc的Wc上限限幅
    float LPF_Min;                          // (限幅)LPF_Uabc的Wc下限限幅
}__MOTOR_ENCODER_STRUCT;

typedef struct{
    float Target_Phase;                     // (输入数据)期望过零点相位延迟
    float Target_Speed;                     // (输入数据)期望电机速度
    float Target_Ubus;                      // (输入数据)期望电机输入电压
    
    float __Speed_in;                         // (中间数据)实际电机输入的Speed
    float __Ubus_in;                         // (中间数据)实际电机输入的Ubus

    int8_t __sector;                        // (数据)电机运行换相扇区

    float __Umid;                           // (数据)mid中性点电压
    float __Ua;                             // (数据)a相电压
    float __Ub;                             // (数据)b相电压
    float __Uc;                             // (数据)c相电压

    float __Real_VBUS;                      // (数据)电机实际母线电压
    float __Real_CURRENT;                   // (数据)电机母线电流
    float __Real_Temp;                      // (数据)电机实际运行温度
}__MOTOR_ESC_STRUCT;

typedef struct{
    uint8_t Poles;                          // (输入数据)电机极对数
    float VBUS;                             // (输入数据)电机工作母线电压

    int8_t Motor_Dir;                       // (输入数据)电机运行方向选择
    uint32_t Duty;                          // (输入数据)PWM满占空比

    float RC_Wc;                            // (输入数据)硬件RC滤波的截止频率
    float Phase_Limit;                      // (输入数据)软件RC相位延迟最小限制
    float Uabc_Gain;                        // (输入数据)相电压采样实际运算增益
}__MOTOR_QUANTIZE_STRUCT;

typedef struct{
    float VBUS_MAX;                         // (参数设计)母线电压值波动MAX阈值
    float VBUS_MIM;                         // (参数设计)母线电压值波动MIN阈值
    uint32_t VBUS_watchdog_limit;           // (参数设计)电压异常的警告周期
    // 如果有电机电压预警，电机正常运行...经过Sguan_Low_Loop()的VBUS_watchdog_limit此运行周期
    // 若2-10次，中间有一次再触发电压警告，电机停转进待机
    // 若首次后10次，都未再触发，电机以后都正常运行
    
    float Temp_MAX;                         // (参数设计)驱动器允许最大温度
    float Temp_MIN;                         // (参数设计)驱动器允许最小温度
    uint32_t Temp_watchdog_limit;           // (参数设计)温度异常的警告周期
    // 如果有驱动器温度预警，电机正常运行...经过Sguan_Low_Loop()的Temp_watchdog_limit此运行周期
    // 若2-10次，中间有一次再触发温度警告，电机停转进待机
    // 若首次后10次，都未再触发，电机以后都正常运行
 
    float CURRENT_MAX;                      // (参数设计)电机最大电流限制
    uint32_t CURRENT_watchdog_limit;        // (参数设计)过流保护的警告周期
    // 如果有电机过流预警，电机正常运行...经过Sguan_Low_Loop()的CURRENT_watchdog_limit此运行周期
    // 若2-10次，中间有一次再触发过流警告，电机停转进待机
    // 若首次后10次，都未再触发，电机以后都正常运行

    float Speed_limit;                      // (参数设计)状态机“加减速”判断裕度
}__MOTOR_SAFE_STRUCT;

typedef struct{
    // ============================= ①电机标识位 =================================
    // (status->状态机数值指示详情，请看Sguan_MotorStatus.h)
    // (error：0->相位延迟达预期，1->硬件RC限制，现在相位延迟已不满足条件)
    uint8_t status;                         // 【数据】电机电调状态机
    uint8_t error;                           // 【数据】相位处理标志位

   // ============================= ②嵌套结构体 =================================
    __MOTOR_LPF_STRUCT lpf;                 // 【有参数设计】低通滤波器
    __MOTOR_ENCODER_STRUCT encoder;         // 【有参数设计】解算电机速度
    __MOTOR_ESC_STRUCT esc;                 // 【数据】电调控制量设计
    __MOTOR_QUANTIZE_STRUCT motor;          // 【有参数设计】电机本体参数
    __MOTOR_SAFE_STRUCT safe;               // 【有参数设计】电机保护参数
    #if CONFIG_MODE
    PID_STRUCT pid;                         // 【有参数设计】速度环闭环控制
    #endif // CONFIG_MODE
    PLL_STRUCT pll;                         // 【有参数设计】锁相环预估速度
    PRINTF_STRUCT txdata;                   // 【数据】串口调试数据

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
