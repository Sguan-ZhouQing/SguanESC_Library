/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-08-09 00:28:19
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-08-10 14:57:44
 * @FilePath: \SguanESC_Debug\Debug_Packeage\Sguan_LPF.c
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
 * @description: 低通滤波器lpf的运行函数
 * @reminder: https://github.com/Sguan-ZhouQing/SguanFOC_Library/blob/main/%E9%85%8D%E5%A5%97Simulink%E6%A8%A1%E5%9E%8B%E5%BC%80%E6%BA%90%E2%91%A1%5B%E7%AE%97%E6%B3%95%E5%8E%9F%E7%90%86%E5%9B%BE%5D/Sguan_lpf.png
 * @reminder: (上方链接是此Sguan_lpf模块Simulink原理仿真图)
 * @param {lpf_STRUCT} *lpf
 * @return {*}
 */
void LPF_Loop(LPF_STRUCT *lpf){
    // 1.带入差分方程，计算输出
    lpf->go.Output = lpf->go.__num*(lpf->go.Input + lpf->go.__Last_i) - 
                    lpf->go.__den*lpf->go.Output;

    // 2.更新历史输入数值
    lpf->go.__Last_i = lpf->go.Input;
}

/**
 * @description: 更新历史数值(特殊函数)
 * @param {LPF_STRUCT} *lpf
 * @param {float} data_in
 * @param {float} data_out
 * @return {*}
 */
void LPF_Enable(LPF_STRUCT *lpf, float data_in,float data_out){
    lpf->go.Output = data_out;
    lpf->go.__Last_i = data_in;
}

