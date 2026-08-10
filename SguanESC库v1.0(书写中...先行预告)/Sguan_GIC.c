/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-08-09 00:26:30
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-08-10 15:48:22
 * @FilePath: \SguanESC_Debug\Debug_Packeage\Sguan_GIC.c
 * @Description: SguanESC库的“增益分段控制器Gain”
 * 
 * Copyright (c) 2026 by $星必尘Sguan, All Rights Reserved. 
 */
#include "Sguan_GIC.h"

// 静态函数声明 - STA辅助函数
static float GIC_SignFunction(GIC_STRUCT *gic);

// 饱和函数_替代符号函数以减少抖振
static float GIC_SignFunction(GIC_STRUCT *gic){
    // 边界层内的线性区
    if(Value_fabsf(gic->run.__Error) < gic->boundary){
        return gic->run.__Error / gic->boundary;
    }
    // 边界层外的符号区
    else{
        return (gic->run.__Error > 0) ? 1.0f : -1.0f;
    }
}

/**
 * @description: 
 * @param {GIC_STRUCT} *gic
 * @return {*}
 */
void GIC_Init(GIC_STRUCT *gic){
    // 初始化为零
    gic->run.Ref = 0.0f;
    gic->run.Fbk = 0.0f;
    gic->run.Output = 0.0f;
    gic->run.__Error = 0.0f;
}

/**
 * @description: 
 * @param {GIC_STRUCT} *gic
 * @return {*}
 */
void GIC_Loop(GIC_STRUCT *gic){
    gic->run.__Error = gic->run.Ref - gic->run.Fbk;

    gic->run.Output += gic->K*GIC_SignFunction(gic);
    Value_Limit(&gic->run.Output,gic->OutMax,gic->OutMin);
}


