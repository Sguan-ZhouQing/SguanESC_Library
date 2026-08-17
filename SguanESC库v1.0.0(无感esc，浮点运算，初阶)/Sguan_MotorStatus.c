/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-08-09 00:27:12
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-08-09 00:45:35
 * @FilePath: \SguanESC_Library\Sguan_MotorStatus.c
 * @Description: SguanESC库的“电调状态机管理”
 * 
 * Copyright (c) 2026 by $星必尘Sguan, All Rights Reserved. 
 */
#include "Sguan_MotorStatus.h"

// 函数指针数组（按枚举顺序排列）
static void (*const status_handlers[])(void) = {
    // 初始化与运行状态
    STANDBY_Loop,
    START_Loop,
    INITIALIZING_Loop,
    
    // 运行状态
    IDLE_Loop,

    ACCELERATING_Loop,
    DECELERATING_Loop,
    CONST_SPEED_Loop,
    
    // 硬件错误
    OVERVOLTAGE_Loop,
    UNDERVOLTAGE_Loop,
    OVERTEMPERATURE_Loop,
    UNDERTEMPERATURE_Loop,
    OVERCURRENT_Loop,
    
    // 安全状态
    EMERGENCY_STOP_Loop,
    DISABLED_Loop
};

/**
 * @description: 核心函数MotorStatus_Loop函数
 * @param {uint8_t} *status
 * @return {*}
 */
void MotorStatus_Loop(uint8_t *status){
    // 运行状态机任务指示函数
    // 带“输入参数”数值限定
    status_handlers[Value_set(*status,
        MOTOR_STATUS_DISABLED_user,0)]();
}

