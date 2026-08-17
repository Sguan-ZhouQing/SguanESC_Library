#ifndef __USERDATA_PARAMETER_H
#define __USERDATA_PARAMETER_H
#include "SguanESC.h"
/* 电机控制User用户设置 */

/**
 * @description: 控制器参数设计的初始化代码
 * @reminder: (此方函数->填写你设定好的控制器参数)
 * @param {SguanFOC_System_STRUCT} *user
 * @return {*}
 */
static inline void User_Parameter_Init(SguanESC_System_STRUCT *user){
    // +---------------------------------------------------------+
    // |                控制器参数Parameter设计                    |
    // |              （默认数值，动态变化的不记）                  |
    // +---------------------------------------------------------+
    // 1.低通滤波器LPF相关
    user->lpf.LPF_Sign.Wc = 3000.0f;                // 输入信号速率限幅

    // 2.无感速度解算相关
    user->encoder.LPF_Max = 1e10f;                  // LPF截止频率动态Wc变化上限限幅
    user->encoder.LPF_Min = 618.8f;                 // LPF截止频率动态Wc变化下限限幅

    // 2.速度环PID参数
    #if CONFIG_MODE
    user->pid.Wc = 100.0f;                          // 速度环speed的PID->微分滤波
    user->pid.Kp = 0.18f;                           // 速度环speed的PID->Kp
    user->pid.Ki = 1.5f;                            // 速度环speed的PID->Ki
    user->pid.Kd = 0.0f;                            // 速度环speed的PID->Kd
    user->pid.OutMax = 20.2f;                       // 速度环speed的PID->最大限幅
    user->pid.OutMin = -20.2f;                      // 速度环speed的PID->最小限幅
    user->pid.IntMax = 15000.0f;                    // 速度环speed的PID->积分项上限
    user->pid.IntMin = -15000.0f;                   // 速度环speed的PID->积分项下限
    #endif // CONFIG_MODE

    // 3.锁相环自然频率设计
    user->pll.Wc = 3.14f;                           // 锁相环截止频率设计
}

static inline void User_RealTime_Parameter_Loop(SguanESC_System_STRUCT *user){
    // 在这里填入你期望动态变化的数据
    // (可以是各种增益或者参数变化)
}


#endif // USERDATA_PARAMETER_H
