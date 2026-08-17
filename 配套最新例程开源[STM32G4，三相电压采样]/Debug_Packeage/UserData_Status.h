#ifndef __USERDATA_STATUS_H
#define __USERDATA_STATUS_H
#include <stdint.h>
/* 电机控制User用户设置·状态管理 */
/* 用户自己的CODE BEGIN Includes */

/* 用户自己的CODE END Includes */


/* =================== 状态机任务信号(输入) ================== */
static inline uint8_t START_Signal(void){
    uint8_t START_num = 0;
    // 准备开始初始化信号(Preparing to start initializing the signal)
    // 输出0->待机 输出1->准备开始初始化
    return START_num;
}

static inline uint8_t EMERGENCY_STOP_Signal(void){
    uint8_t STOP_num = 0;
    // 急停信号(Emergency stop signal)
    // 输出0->正常运行 输出1->启用急停(锁定)
    return STOP_num;
}

static inline uint8_t DISABLED_Signal(void){
    uint8_t DISABLED_num = 0;
    // 失能信号(Disabling signal)
    // 输出0->正常运行 输出1->启用失能
    return DISABLED_num;
}


/* =================== 状态机任务处理(执行) ================== */
static inline void STANDBY_Loop(void){
    // Your code for 待机(未初始化，准备中) here
}

static inline void START_Loop(void){
    // Your code for 开始初始化 here
}

static inline void INITIALIZING_Loop(void){
    // Your code for 初始化中(参数加载、外设初始化) here
}

static inline void IDLE_Loop(void){
    // Your code for 空闲(已初始化，使能但零指令) here
}

static inline void ACCELERATING_Loop(void){
    // Your code for 加速中~速度模式(下时刻->恒速保持) here
}

static inline void DECELERATING_Loop(void){
    // Your code for 减速中~速度模式(下时刻->恒速保持) here
}

static inline void CONST_SPEED_Loop(void){
    // Your code for 恒速保持~速度模式(稳态) here
}

static inline void OVERVOLTAGE_Loop(void){
    // Your code for 过压保护(锁定->手动解除进待机) here
}

static inline void UNDERVOLTAGE_Loop(void){
    // Your code for 欠压保护(锁定->手动解除进待机) here
}

static inline void OVERTEMPERATURE_Loop(void){
    // Your code for 过温保护(锁定->手动解除进待机) here
}

static inline void UNDERTEMPERATURE_Loop(void){
    // Your code for 低温保护(锁定->手动解除进待机) here
}

static inline void OVERCURRENT_Loop(void){
    // Your code for 过流保护(稳态->电机电流限幅) here
}

static inline void EMERGENCY_STOP_Loop(void){
    // Your code for 急停(立即关闭PWM,会立即锁定->手动解除进待机) here
}

static inline void DISABLED_Loop(void){
    // Your code for 已失能(软关闭,会缓慢进入待机->自动进待机) here
}


#endif // USERDATA_STATUS_H
