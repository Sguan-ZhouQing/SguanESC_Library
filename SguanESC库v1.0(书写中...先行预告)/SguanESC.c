/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-08-09 00:26:48
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-08-10 17:06:12
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



SguanESC_System_STRUCT Sguan;



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









void SguanESC_High_Loop(void){

}

void SguanESC_Low_Loop(void){

}

void SguanESC_Printf_Loop(uint8_t *data, uint16_t length){

}

void SguanESC_main_Loop(void){

}


