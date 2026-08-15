#ifndef __SGUAN_MOTORSTATUS_H
#define __SGUAN_MOTORSTATUS_H

/* SguanESC配置文件声明 */
#include "UserData_Status.h"
#include "Sguan_Config.h"

// +---------------------------------------------------------+
// |   重要运行事件的说明：                                    |
// |   MOTOR_STATUS_STANDBY        NULL(用户无控制权)         |
// |   MOTOR_STATUS_START          (用户无控制权)             |
// |   MOTOR_STATUS_INITIALIZING   (用户无控制权)             |
// |   MOTOR_STATUS_IDLE           (控制权交接完成)           |
// +---------------------------------------------------------+

// ====== 初始化与运行状态(状态) ======
#define MOTOR_STATUS_STANDBY                0x00    // 待机(未初始化，准备中)
#define MOTOR_STATUS_START_user             0x01    // 开始初始化(若进入此状态，电机开始初始化)
#define MOTOR_STATUS_INITIALIZING           0x02    // 初始化中(参数加载、外设初始化)

// ====== 运行状态(当前反馈) ======
#define MOTOR_STATUS_IDLE                   0x03    // 空闲(电机空闲可用，使能但零指令)

#define MOTOR_STATUS_ACCELERATING           0x04    // 加速中~速度模式(下时刻->恒速保持)
#define MOTOR_STATUS_DECELERATING           0x05    // 减速中~速度模式(下时刻->恒速保持)
#define MOTOR_STATUS_CONST_SPEED            0x06    // 恒速保持~速度模式(稳态)

// ====== 硬件相关错误(状态) ======
#define MOTOR_STATUS_OVERVOLTAGE            0x07    // 过压保护(锁定->手动解除进待机)
#define MOTOR_STATUS_UNDERVOLTAGE           0x08    // 欠压保护(锁定->手动解除进待机)
#define MOTOR_STATUS_OVERTEMPERATURE        0x09    // 过温保护(锁定->手动解除进待机)
#define MOTOR_STATUS_UNDERTEMPERATURE       0x0A    // 低温保护(锁定->手动解除进待机)
#define MOTOR_STATUS_OVERCURRENT            0x0B    // 过流保护(稳态->电机电流限幅)

// ====== 安全状态(状态) ======
#define MOTOR_STATUS_EMERGENCY_STOP_user    0x0C    // 急停(立即关闭PWM,会立即锁定->手动解除进待机)
#define MOTOR_STATUS_DISABLED_user          0x0D    // 已失能(软关闭,会缓慢进入待机->自动进待机)

// 函数定义声明
void MotorStatus_Loop(uint8_t *status);


#endif // SGUAN_MOTORSTATUS_H
