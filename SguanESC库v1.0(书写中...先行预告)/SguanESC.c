/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-08-09 00:26:48
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-08-10 17:14:25
 * @FilePath: \SguanESC_Debug\Debug_Packeage\SguanESC.c
 * @Description: SguanESC库的“核心代码实现”
 * 
 * Copyright (c) 2026 by $星必尘Sguan, All Rights Reserved. 
 */
#include "SguanESC.h"

/* USER CODE BEGIN Includes */
// 电机控制User用户设置声明
#include "UserData_Function.h"
#include "UserData_Motor.h"
#include "UserData_Parameter.h"
#include "UserData_UserControl.h"
/* USER CODE END Includes */

/**
 * @description: 1.电机控制函数实现(Sguan.*使用)
 * @reminder: Sguan控制函数接口的使用方法，如下：
 * @reminder: 启动电机  Sguan.Func_Start()
 * @reminder: 停止电机  Sguan.Func_Stop()
 * @reminder: 设计电压  Sguan.Func_Set_Uq(1.68f)
 * @reminder: 设计电流  Sguan.Func_Set_Iq(2.68f)
 * @reminder: 设计转速  Sguan.Func_Set_Velocity(16.8f)
 * @reminder: 设计位置  Sguan.Func_Set_Position(26.8f)
 * @reminder: 数据打印  Sguan.Func_Set_TXdata(0,Sguan.status)
 * @param {void} *ctrl
 * @return {*}
 */
static void Function_Start(void);
static void Function_Stop(void);
static void Function_SetUbus(float ud);
static void Function_SetVelocity(float speed);
static void Function_SetTXdata(uint8_t ch,float data);



SguanESC_System_STRUCT Sguan = {
    .Func_Start = Function_Start,
    .Func_Stop = Function_Stop,
    .Func_Set_Ubus = Function_SetUbus,
    .Func_Set_Velocity = Function_SetVelocity,
    .Func_Set_TXdata = Function_SetTXdata
};



static void Function_Start(void){
    Sguan.status = 0x01;
}

static void Function_Stop(void){
    Sguan.status = 0x01;
}

static void Function_SetUbus(float ud){
    Value_Limit(&ud,Sguan.esc.Real_VBUS,0.0f);
    Sguan.esc.Ubus_in = ud;
}

static void Function_SetVelocity(float speed){
    Sguan.esc.Target_Speed = speed;
}

static void Function_SetTXdata(uint8_t ch,float data){
    // 注意ch数值不要越界
    Sguan.txdata.fdata[ch] = data;
}





/**
 * @description: SguanFOC核心文件，定时中断服务函数(高频率电机载波)
 * @reminder: 10Khz或者更高定时中断中调用，任务优先级“最高”
 * @return {*}
 */
void SguanESC_High_Loop(void){

}

/**
 * @description: SguanFOC核心文件，定时中断服务函数(1ms周期数据更新)
 * @reminder: 1Khz或者更低定时中断中调用，任务优先级“中”
 * @return {*}
 */
void SguanESC_Low_Loop(void){
    // 执行电机状态机切换函数和任务
    Sguan_Calculate_Low_Loop(&Sguan);
}

/**
 * @description: SguanFOC核心文件，UART或者CAN接收完成中断服务函数
 * @reminder: 主循环函数调用，任务优先级“低”
 * @param {uint8_t} *data 接收到的数据
 * @param {uint16_t} length 数据长度
 * @return {*}
 */
void SguanESC_Printf_Loop(uint8_t *data, uint16_t length){
    // 微控制器接收来自上位机的消息
    // 解析数据的格式like：AO=16.8?
    Printf_RX_Loop(data,length);
}

/**
 * @description: SguanESC核心文件，主循环服务函数(主循环TXdata数据更新)
 * @reminder: 主循环函数调用，任务优先级“最低”
 * @return {*}
 */
void SguanESC_main_Loop(void){
    // 1.上电即初始化的函数
    // (包含printf的收发初始化和Initial_Init)
    static uint8_t count = 0;
    if (!count){
        Printf_TX_Init(&Sguan.txdata);
        Printf_RX_Init();
        User_Initial_Init();
        count = !count;
    }

    // 2.接收到开启电机才初始化
    // (即电机状态机为Sguan.status==1)
    Sguan_Calculate_main_Loop(&Sguan);

    // 3.正常运行时串口打印数据，或Cogging数据打印
    if ((Sguan.status >= MOTOR_STATUS_IDLE) && 
        (Sguan.status < MOTOR_STATUS_ENCODER_ERROR)){
        #if CONFIG_Printf==0
        Printf_Normal_Loop(&Sguan);
        #elif CONFIG_Printf==2
        Printf_Cogging_Loop();
        #endif // CONFIG_Printf
    }
}


