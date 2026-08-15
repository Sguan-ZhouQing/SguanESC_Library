#ifndef __SGUAN_PLL_H
#define __SGUAN_PLL_H

/* SguanFOC配置文件声明 */
#include "Sguan_Config.h"

typedef struct{
    float Error;            // (输入数据)Error真实反馈数据
    float OutWe;            // (输出数据)OutWe电角速度输出
    float OutRe;            // (输出数据)OutRe电角度输出

    float __Kp;             // (数据)Kp比例项增益
    float __Ki;             // (数据)Ki积分项增益

    float __We_i;           // (数据)数据历史输入值
    float __Re_i;           // (数据)数据历史输入值
    float __X_num[2];       // (中间量)PI传递函数分子系数
    float __Y_num;          // (中间量)积分项分母系数
}__PLL_GO_STRUCT;

typedef struct{
    __PLL_GO_STRUCT go;       // (结构体)PLL运算结构体

    float T;                // (系统时钟)T运算离散周期
    float Wc;               // (参数设计)锁相环截止频率
}PLL_STRUCT;

void PLL_Init(PLL_STRUCT *pll);
void PLL_Loop(PLL_STRUCT *pll);


#endif // SGUAN_PLL_H
