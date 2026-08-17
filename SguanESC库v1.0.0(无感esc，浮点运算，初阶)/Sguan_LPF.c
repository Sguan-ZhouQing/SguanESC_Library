/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-08-09 00:28:19
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-08-11 10:10:35
 * @FilePath: \SguanESC_Library\Sguan_LPF.c
 * @Description: SguanESC库的“LPF一阶低通滤波器实现”
 * 
 * Copyright (c) 2026 by $星必尘Sguan, All Rights Reserved. 
 */
#include "Sguan_LPF.h"

/**
 * @description: 低通滤波器lpf函数初始化
 * @reminder: (初始化相关系数float->double->float)
 * @reminder: (单浮点转double运算，提高系数精度)
 * @param {lpf_STRUCT} *lpf
 * @return {*}
 */
void LPF_Init(LPF_STRUCT *lpf){
    // 固定截止频率
    double temp1 = ((double)lpf->T)*((double)lpf->Wc);
    double den = 2.0+temp1;

    lpf->go.__num = (float)(temp1/den);
    lpf->go.__den = (float)((-2.0+temp1)/den);

    // 初始化为零
    lpf->go.Input = 0.0f;
    lpf->go.Output = 0.0f;
    lpf->go.__Last_i = 0.0f;
}

/**
 * @description: 低通滤波器lpf的运行函数(实时更新截止频率)
 * @param {lpf_STRUCT} *lpf
 * @return {*}
 */
void LPF_RealTime_Loop(LPF_STRUCT *lpf){
    // 1.动态计算截止频率
    float temp1 = lpf->T*lpf->Wc;
    float den = 2.0f+temp1;

    lpf->go.__num = temp1/den;
    lpf->go.__den = (-2.0f+temp1)/den;

    // 2.带入差分方程，计算输出
    lpf->go.Output = lpf->go.__num*(lpf->go.Input + lpf->go.__Last_i) - 
                    lpf->go.__den*lpf->go.Output;

    // 3.更新历史输入数值
    lpf->go.__Last_i = lpf->go.Input;
}

/**
 * @description: 低通滤波器lpf的运行函数(固定截止频率运算)
 * @param {lpf_STRUCT} *lpf
 * @return {*}
 */
void LPF_Fixed_Loop(LPF_STRUCT *lpf){
    // 1.带入差分方程，计算输出
    lpf->go.Output = lpf->go.__num*(lpf->go.Input + lpf->go.__Last_i) - 
                    lpf->go.__den*lpf->go.Output;

    // 2.更新历史输入数值
    lpf->go.__Last_i = lpf->go.Input;
}
