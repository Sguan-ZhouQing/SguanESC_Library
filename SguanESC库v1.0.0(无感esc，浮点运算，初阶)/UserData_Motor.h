#ifndef __USERDATA_MOTOR_H
#define __USERDATA_MOTOR_H
#include "SguanESC.h"
/* 电机控制User用户设置 */

/**
 * @description: 实体参数填写函数的初始化代码
 * @reminder: (此方函数->填写电机实际物理参数)
 * @param {SguanFOC_System_STRUCT} *user
 * @return {*}
 */
static inline void User_Motor_Init(SguanESC_System_STRUCT *user){
    // +---------------------------------------------------------+
    // |                  电机实体参数Motor填写                   |
    // +---------------------------------------------------------+
    // 1.Sguan.foc参数设计(最初始的Target数值)
    user->esc.Target_Phase = 30.0f;                 // (float)期望的过零点相位延迟
    user->esc.Target_Speed = 0.0f;                  // (float)期望的机械角速度
    user->esc.Target_Ubus = 0.0f;                   // (float)期望的电机电压数值

    /* ====================================== 分割线 =================================== */
    user->motor.Poles = 7;                          // (uint8_t)电机的极对数
    user->motor.VBUS = 12.0f;                       // (float)标定的母线电压
    
    user->motor.Motor_Dir = 1;                      // (int8_t)电机方向1->正向，负1->负向
    user->motor.Duty = 4249;                        // (uint16_t)PWM满占空比数值

    user->motor.RC_Wc = 10000.0f;                   // (float)硬件RC滤波截止频率
    user->motor.Phase_Limit = 0.018f;               // (float)软件RC相位延迟最小限制
    user->motor.Uabc_Gain = 1.0f;                   // (float)相线电压的ADC计算增益

    // 3.Sguan.safe参数设计(维护驱动器安全)
    user->safe.VBUS_MAX = 14.0f;                    // (float)母线电压值波动MAX阈值
    user->safe.VBUS_MIM = 10.0f;                    // (float)母线电压值波动MIN阈值
    user->safe.VBUS_watchdog_limit = 1000;          // (uint32_t)看门狗

    user->safe.Temp_MAX = 60.0f;                    // (float)驱动器允许最大温度
    user->safe.Temp_MIN = -20.0f;                   // (float)驱动器允许最小温度
    user->safe.Temp_watchdog_limit = 1000;          // (uint32_t)看门狗

    user->safe.CURRENT_MAX = 60.0f;                 // (float)电机最大电流D轴限制
    user->safe.CURRENT_watchdog_limit = 1000;       // (uint32_t)看门狗

    user->safe.Speed_limit = 5.0f;                  // (float)电机->速度状态机判断的速度范围
} 


#endif // USERDATA_MOTOR_H
