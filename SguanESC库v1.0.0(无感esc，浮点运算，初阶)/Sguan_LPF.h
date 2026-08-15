#ifndef __SGUAN_LPF_H
#define __SGUAN_LPF_H

/* SguanESC配置文件声明 */
#include "Sguan_Config.h"

typedef struct{
    float Input;            // (输入数据)阶跃输入的数值
    float Output;           // (输出数据)输出平滑的数值

    float __num;            // (中间量)传递函数的分子系数
    float __den;            // (中间量)传递函数的分母系数
    float __Last_i;         // (数据)历史的输入数值
}__LPF_GO_STRUCT;

typedef struct{
    __LPF_GO_STRUCT go;     // (结构体)LTD运输数据

    float T;                // (系统时钟)T离散运行的时间周期
    float Wc;               // (参数设计)Wc低通滤波器截止频率
}LPF_STRUCT;

void LPF_Init(LPF_STRUCT *lpf);
void LPF_RealTime_Loop(LPF_STRUCT *lpf);
void LPF_Fixed_Loop(LPF_STRUCT *lpf);

#endif // SGUAN_LPF_H
