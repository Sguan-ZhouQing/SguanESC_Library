#ifndef __SGUAN_CONFIG_H
#define __SGUAN_CONFIG_H

#include "Sguan_math.h"
#include "UserData_Config.h"

// ============================ 系统配置 宏定义 ==============================
#define CONFIG_MODE         Define_Run_Mode             // (定义)电机运行模式
#define CONFIG_UMID         Switch_MOTOR_Umid_Get       // (选择)电压中性点获取方式


// ======================== 控制系统离散周期 宏定义 =========================
// 离散控制周期大小
#define BLDC_RUN_T          TIM_T                       // 系统离散运行时间



#endif // SGUAN_CONFIG_H
