/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-01-26 22:38:09
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-08-10 15:45:47
 * @FilePath: \SguanESC_Debug\Debug_Packeage\Sguan_PID.c
 * @Description: SguanESC库的“闭环PID算法”实现
 * 
 * Copyright (c) 2026 by $星必尘Sguan, All Rights Reserved. 
 */
#include "Sguan_PID.h"

/**
 * @description: 闭环系统PID核心参数初始化
 * @reminder: (初始化相关系数float->double->float)
 * @reminder: (单浮点转double运算，提高系数精度)
 * @param {PID_STRUCT} *pid
 * @return {*}
 */
void PID_Init(PID_STRUCT *pid){
    double temp0 = ((double)pid->T)*((double)pid->Ki)/2.0;
    double temp1 = ((double)pid->T)*((double)pid->Wc);
    double temp2 = ((double)pid->Kd)*((double)pid->Wc);
    double den = -2.0+temp1;

    pid->run.__I_num = (float)temp0;
    pid->run.__D_num = (float)((2.0*temp2)/den);
    pid->run.__D_den = (float)((2.0+temp1)/den);

    // 初始化为零
    pid->run.__i[0] = 0.0f;
    pid->run.__i[1] = 0.0f;

    pid->run.Ref = 0.0f;
    pid->run.Fbk = 0.0f;
    pid->run.Output = 0.0f;
    pid->run.__IntegralFrozen_flag = 0;
}

/**
 * @description: 闭环控制运算的离散服务函数
 * @param {PID_STRUCT} *pid
 * @return {*}
 */
void PID_Loop(PID_STRUCT *pid){
    // 1.计算比例、积分、微分项
    pid->run.__i[0] = pid->run.Ref - pid->run.Fbk;
    if (pid->Ki){
        // 判断是否需要冻结积分
        if (pid->run.__IntegralFrozen_flag){
            // 如果积分已冻结，保持上次的积分值
            
            // 检查是否可以解除冻结
            // 情况1：误差反向（误差符号与积分输出符号相反）
            // 情况2：积分值回到限幅范围内
            if ((pid->run.__i[0]*pid->run.__Io < 0) ||  // 误差反向
                ((pid->run.__Io < pid->IntMax) && 
                (pid->run.__Io > pid->IntMin))){
                pid->run.__IntegralFrozen_flag = 0;
            }
        } else{
            // 正常计算积分
            pid->run.__Io += pid->run.__I_num*(pid->run.__i[0] + pid->run.__i[1]);
            
            // 检查是否达到限幅，达到则冻结积分
            if (pid->run.__Io > pid->IntMax){
                pid->run.__Io = pid->IntMax;
                pid->run.__IntegralFrozen_flag = 1;
            }
            else if (pid->run.__Io < pid->IntMin){
                pid->run.__Io = pid->IntMin;
                pid->run.__IntegralFrozen_flag = 1;
            }
        }
    }
    if (pid->Kd){
        pid->run.__Do = pid->run.__D_num*(pid->run.__i[0] + pid->run.__i[1]) -  
                    pid->run.__D_den*pid->run.__Do;
    }

    // 2.运算控制器输出量并输出限幅
    pid->run.Output = pid->run.__i[0]*pid->Kp + pid->run.__Io + pid->run.__Do;
    Value_Limit(&pid->run.Output, 
                pid->OutMax, 
                pid->OutMin);
    
    // 3.刷新历史输入和输出数值
    pid->run.__i[1] = pid->run.__i[0];
}

