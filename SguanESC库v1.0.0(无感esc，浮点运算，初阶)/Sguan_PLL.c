/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-01-26 22:50:37
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-06-05 03:49:39
 * @FilePath: \SguanFOC_Debug\SguanFOC\Sguan_PLL.c
 * @Description: SguanFOC库的“开环PLL锁相环”实现
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
    // 初始化为零
    pll->go.__We_i = 0.0f;
    pll->go.__Re_i = 0.0f;

    pll->go.OutWe = 0.0f;
    pll->go.OutRe = 0.0f;
    pll->go.Error = 0.0f;
}

/**
 * @description: 锁相环运算的离散函数
 * @reminder: https://github.com/Sguan-ZhouQing/SguanFOC_Library/blob/main/%E9%85%8D%E5%A5%97Simulink%E6%A8%A1%E5%9E%8B%E5%BC%80%E6%BA%90%E2%91%A1%5B%E7%AE%97%E6%B3%95%E5%8E%9F%E7%90%86%E5%9B%BE%5D/Sguan_PLL.png
 * @reminder: (上方链接是此Sguan_PLL模块Simulink原理仿真图)
 * @param {PLL_STRUCT} *pll
 * @return {*}
 */
void PLL_Loop(PLL_STRUCT *pll){
    // 1.动态计算截止频率
    pll->go.__Ki = pll->Wc*pll->Wc;
    pll->go.__Kp = Value_2_SQRT2*Value_sqrtf(pll->go.__Ki);

    float temp0 = pll->T*pll->go.__Ki;
    pll->go.__X_num[0] = (2.0f*pll->go.__Kp+temp0)/2.0f;
    pll->go.__X_num[1] = (-2.0f*pll->go.__Kp+temp0)/2.0f;
    pll->go.__Y_num = (pll->T)/2.0f;

    // 2.计算PI控制器(并输出We)
    pll->go.OutWe += pll->go.__X_num[0]*pll->go.Error + 
                    pll->go.__X_num[1]*pll->go.__We_i;

    // 3.计算积分器(并输出Re)
    pll->go.OutRe += pll->go.__Y_num*(pll->go.OutWe + pll->go.__Re_i);

    // 4.更新历史输入和输出数值
    pll->go.__We_i = pll->go.Error;
    pll->go.__Re_i = pll->go.OutWe;
}

