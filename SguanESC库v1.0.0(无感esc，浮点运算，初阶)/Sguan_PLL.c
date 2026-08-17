/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-01-26 22:50:37
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-06-05 03:49:39
 * @FilePath: \SguanESC_Library\Sguan_PLL.c
 * @Description: SguanESC库的“开环PLL锁相环”实现
 * 
 * Copyright (c) 2026 by $星必尘Sguan, All Rights Reserved. 
 */
#include "Sguan_PLL.h"

/**
 * @description: 锁相环PLL核心参数初始化
 * @reminder: (初始化相关系数float->double->float)
 * @reminder: (单浮点转double运算，提高系数精度)
 * @param {PLL_STRUCT} *pll
 * @return {*}
 */
void PLL_Init(PLL_STRUCT *pll){
    // 1.动态计算截止频率
    double Ki = ((double)pll->Wc)*((double)pll->Wc);
    double Kp = ((double)Value_SQRT2)*((double)Value_sqrtf(Ki));

    double temp0 = ((double)pll->T)*Ki;
    pll->go.__X_num[0] = (float)((2.0*Kp+temp0)/2.0);
    pll->go.__X_num[1] = (float)((-2.0*Kp+temp0)/2.0);
    pll->go.__Y_num = (pll->T)/2.0f;

    // 初始化为零
    pll->go.__We_i = 0.0f;
    pll->go.__Re_i = 0.0f;

    pll->go.OutWe = 0.0f;
    pll->go.OutRe = 0.0f;
    pll->go.Error = 0.0f;
}

/**
 * @description: 锁相环运算的离散函数
 * @param {PLL_STRUCT} *pll
 * @return {*}
 */
void PLL_Loop(PLL_STRUCT *pll){
    // 1.计算PI控制器(并输出We)
    pll->go.OutWe += pll->go.__X_num[0]*pll->go.Error + 
                    pll->go.__X_num[1]*pll->go.__We_i;

    // 2.计算积分器(并输出Re)
    pll->go.OutRe += pll->go.__Y_num*(pll->go.OutWe + pll->go.__Re_i);

    // 3.更新历史输入和输出数值
    pll->go.__We_i = pll->go.Error;
    pll->go.__Re_i = pll->go.OutWe;
}

