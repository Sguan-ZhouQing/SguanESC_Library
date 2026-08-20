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

// ============================= SguanESC版本代码(仅声明) ============================
/**
 * @description: 2.Transfer传递函数的离散化运算，采用双线性变换
 * @param {LPF_STRUCT} *lpf (滤波)LPF二阶巴特沃斯低通滤波
 * @param {PLL_STRUCT} *pll (估算)PLL速度跟踪锁相环
 * @return {*}
 */
static void Transfer_LPF_RealTime_Loop(LPF_STRUCT *lpf,float input);
static void Transfer_LPF_Fixed_Loop(LPF_STRUCT *lpf,float input);
static void Transfer_PID_Loop(PID_STRUCT *pid,float Ref,float Fbk);
// [Sguan原代码] static void Transfer_PLL_Loop(PLL_STRUCT *pll,float input_Rad);
// [Claude修改] 参数改为直接传入sector与方向,差分解缠绕由函数内部完成
static void Transfer_PLL_Loop(PLL_STRUCT *pll,int8_t sector,int8_t dir);
static void Transfer_Init(SguanESC_System_STRUCT *sguan);
/**
 * @description: 3.Data...Get_Tick数据获取汇总
 * @param {SguanESC_System_STRUCT} *sguan
 * @return {*}
 */
static void Data_VBUS_Get_Tick(SguanESC_System_STRUCT *sguan);
static void Data_CURRENT_Get_Tick(SguanESC_System_STRUCT *sguan);
static void Data_Temp_Get_Tick(SguanESC_System_STRUCT *sguan);
/**
 * @description: 4.Status...Tick电调状态机函数汇总
 * @param {SguanESC_System_STRUCT} *sguan
 * @return {*}
 */
static void Status_Switch_STANDBY(SguanESC_System_STRUCT *sguan);
static void Status_Switch_DISABLED(SguanESC_System_STRUCT *sguan,uint32_t *count);
static void Status_Switch_IDLE(SguanESC_System_STRUCT *sguan,uint32_t *count);
static void Status_VBUS_OVERVOLTAGE(SguanESC_System_STRUCT *sguan,uint32_t *count);
static void Status_VBUS_UNDERVOLTAGE(SguanESC_System_STRUCT *sguan,uint32_t *count);
static void Status_Temp_OVERTEMPERATURE(SguanESC_System_STRUCT *sguan,uint32_t *count);
static void Status_Temp_UNDERTEMPERATURE(SguanESC_System_STRUCT *sguan,uint32_t *count);
static void Status_Current_OVERCURRENT(SguanESC_System_STRUCT *sguan,uint32_t *count);
static void Status_Switch_Tick(SguanESC_System_STRUCT *sguan);
static void Status_Run_Tick(SguanESC_System_STRUCT *sguan);
/**
 * @description: 5.Sguan...Loop电机运算汇总
 * @param {SguanESC_System_STRUCT} *sguan
 * @return {*}
 */
static void Sguan_Calculate_High_Loop(SguanESC_System_STRUCT *sguan);
static void Sguan_Calculate_Low_Loop(SguanESC_System_STRUCT *sguan);
static void Sguan_Calculate_main_Loop(SguanESC_System_STRUCT *sguan);


// 用于函数指针，启动电机
static void Function_Start(void){
    Sguan.status = MOTOR_STATUS_START_user;
}

// 用于函数指针，停止电机(缓停)
static void Function_Stop(void){
    Sguan.status = MOTOR_STATUS_DISABLED_user;
}

// 用于函数指针，设置电机电压
static void Function_SetUbus(float ud){
    Value_Limit(&ud,Sguan.esc.__Real_VBUS,-Sguan.esc.__Real_VBUS);
    Sguan.esc.Target_Ubus = ud;
}

// 用于函数指针，设置电机速度
static void Function_SetVelocity(float speed){
    Sguan.esc.Target_Speed = speed;
}

// 用于函数指针，设置串口打印通道
static void Function_SetTXdata(uint8_t ch,float data){
    // 注意ch数值不要越界
    Sguan.txdata.fdata[ch] = data;
}

// Transfer运算_低通滤波器
static void Transfer_LPF_RealTime_Loop(LPF_STRUCT *lpf,float input){
    lpf->go.Input = input;
    LPF_RealTime_Loop(lpf);
    // 输出lpf->filter.Output;
}

// Transfer运算_低通滤波器
static void Transfer_LPF_Fixed_Loop(LPF_STRUCT *lpf,float input){
    lpf->go.Input = input;
    LPF_Fixed_Loop(lpf);
    // 输出lpf->filter.Output;
}

// Transfer运算_PID运算
static void Transfer_PID_Loop(PID_STRUCT *pid,float Ref,float Fbk){
    pid->run.Ref = Ref;
    pid->run.Fbk = Fbk;
    PID_Loop(pid);
    // 输出pid->run.Output;
}

// Transfer运算_速度锁相环
// [Sguan原代码] 绝对角度输入版:输入与OutRe各自独立回绕(输入5->0跳-300°、
// PLL内部OutRe独立归一化到[0,2π)),两个回绕时刻不同步时,误差经Value_Correct
// 折最短弧会出现错号,速度输出呈现阶梯状、正负交替
//static void Transfer_PLL_Loop(PLL_STRUCT *pll,
//                    float input_Rad){
//    // 非位置环模式：PLL输出归一化到[0, 2π)
//    pll->go.Error = input_Rad - pll->go.OutRe;
//
//    // 计算角度误差,始终归一化到[-π, π)范围
//    // (此处为绝对编码器提供角度运算)
//    Value_Correct(&pll->go.Error, pll->go.Error);
//
//    PLL_Loop(pll);
//    // 输出pll->go.OutWe;
//    // 输出pll->go.OutRe;
//}
// [Claude修改] 六步换相sector差分解缠绕版
// 原理:
// 1.sector差分:5->0回绕折算为+1(反向为-1),输入相位只含真实增量,无-300°跳变
//   (要求单次调用内电机转过<180°电角度,20kHz控制环下任何转速都满足);
// 2.共同平移回绕:theta_in折叠到[-π, π)时,OutRe平移相同的2π整数倍,
//   误差严格不变,且theta_in/OutRe永远有界——无需COUNT变量,不存在任何溢出;
// 3.误差取共动原始差:不再折最短弧,即使|误差|>π方向依旧正确,启动拉入可靠。
static void Transfer_PLL_Loop(PLL_STRUCT *pll,
                    int8_t sector, int8_t dir){
    static float theta_in = 0.0f;  // (Claude修改)解缠绕输入电角度,与OutRe共动、有界
    static int8_t sector_prev = -1;// (Claude修改)上一次sector,-1表示首次调用

    // 1.sector差分解缠绕(处理5->0 / 0->5回绕)
    int8_t ds;
    if (sector_prev < 0){
        ds = 0;                    // 首次调用:以当前sector为零点,避免启动大误差
    }
    else{
        ds = sector - sector_prev;
        if (ds > 3){
            ds -= 6;               // 5->0: 折算为+1
        }
        else if (ds < -3){
            ds += 6;               // 0->5(反向): 折算为-1
        }
    }
    sector_prev = sector;
    theta_in += (float)ds * (Value_2PI/6.0f) * (float)dir;

    // 2.共同平移回绕:theta_in折叠时,OutRe平移相同的2π整数倍
    // (误差严格不变,且两者永远有界,不会溢出)
    float shift = 0.0f;
    while (theta_in >= Value_PI){
        theta_in -= Value_2PI;
        shift -= Value_2PI;
    }
    while (theta_in < -Value_PI){
        theta_in += Value_2PI;
        shift += Value_2PI;
    }
    pll->go.OutRe += shift;

    // 3.误差=共动原始差(真实误差,不折最短弧,方向永远正确)
    pll->go.Error = theta_in - pll->go.OutRe;

    //Value_Limit(&pll->go.Error, Value_PI, -Value_PI);
    PLL_Loop(pll);
    // 输出pll->go.OutWe;
    // 输出pll->go.OutRe;
}

// Transfer运算_初始化函数
static void Transfer_Init(SguanESC_System_STRUCT *sguan){
    // [0]各种用户初始化函数
    User_StartMotor_Init();
    User_Motor_Init(sguan);
    User_Parameter_Init(sguan);

    // [1]低通滤波器
    #if !CONFIG_UMID
    sguan->lpf.__LPF_Umid.T = BLDC_RUN_T;
    LPF_Init(&sguan->lpf.__LPF_Umid);
    #endif // CONFIG_UMID

    sguan->lpf.__LPF_Ua.T = BLDC_RUN_T;
    LPF_Init(&sguan->lpf.__LPF_Ua);
    sguan->lpf.__LPF_Ub.T = BLDC_RUN_T;
    LPF_Init(&sguan->lpf.__LPF_Ub);
    sguan->lpf.__LPF_Uc.T = BLDC_RUN_T;
    LPF_Init(&sguan->lpf.__LPF_Uc);

    sguan->lpf.LPF_Sign.T = BLDC_RUN_T;
    LPF_Init(&sguan->lpf.LPF_Sign);

    // [2]闭环控制系统
    #if CONFIG_MODE
    sguan->pid.T = BLDC_RUN_T;
    PID_Init(&sguan->pid);
    #endif // CONFIG_MODE

    // [3]锁相环
    sguan->pll.T = BLDC_RUN_T;
    PLL_Init(&sguan->pll);

    (void)Transfer_PID_Loop;
}

// 实时电压数据获取
static void Data_VBUS_Get_Tick(SguanESC_System_STRUCT *sguan){
    float VBUS = User_VBUS_DataGet();
    if (VBUS != Value_N_INF){
        sguan->esc.__Real_VBUS = VBUS;
    }
    else{
        sguan->esc.__Real_VBUS = sguan->motor.VBUS;
    }
}

// 实时电流数据获取
static void Data_CURRENT_Get_Tick(SguanESC_System_STRUCT *sguan){
    float CURRENT = User_CURRENT_DataGet();
    if (CURRENT != Value_N_INF){
        sguan->esc.__Real_CURRENT = CURRENT;
    }
}

// 实时温度数据获取
static void Data_Temp_Get_Tick(SguanESC_System_STRUCT *sguan){
    float Temp = User_Temperature_DataGet();
    if (Temp != Value_N_INF){
        sguan->esc.__Real_Temp = Temp;
    }
}

// Status判断DISABLED状态机的切换
static void Status_Switch_STANDBY(SguanESC_System_STRUCT *sguan){
    sguan->status = MOTOR_STATUS_STANDBY;
}

// Status判断去切换DISABLED状态机
static void Status_Switch_DISABLED(SguanESC_System_STRUCT *sguan,uint32_t *count){
    sguan->status = MOTOR_STATUS_DISABLED_user;
    *count = 0;
    // 已失能状态持续“自定义”次控制周期后，自动切换退出已失能状态
}

// Status判断IDLE状态机的切换
static void Status_Switch_IDLE(SguanESC_System_STRUCT *sguan,uint32_t *count){
    sguan->status = MOTOR_STATUS_IDLE;
    *count = 0;
}

// Status判断过压保护状态
static void Status_VBUS_OVERVOLTAGE(SguanESC_System_STRUCT *sguan,uint32_t *count){
    if (sguan->esc.__Real_VBUS > sguan->safe.VBUS_MAX){
        Status_Switch_DISABLED(sguan,count);
    }
}

// Status判断欠压保护状态
static void Status_VBUS_UNDERVOLTAGE(SguanESC_System_STRUCT *sguan,uint32_t *count){
    if (sguan->esc.__Real_VBUS < sguan->safe.VBUS_MIM){
        Status_Switch_DISABLED(sguan,count);
    }
}

// Status判断过温保护状态
static void Status_Temp_OVERTEMPERATURE(SguanESC_System_STRUCT *sguan,uint32_t *count){
    if (sguan->esc.__Real_Temp > sguan->safe.Temp_MAX){
        Status_Switch_DISABLED(sguan,count);
    }
}

// Status判断低温保护状态
static void Status_Temp_UNDERTEMPERATURE(SguanESC_System_STRUCT *sguan,uint32_t *count){
    if (sguan->esc.__Real_Temp < sguan->safe.Temp_MIN){
        Status_Switch_DISABLED(sguan,count);
    }
}

// Status判断过流保护状态
static void Status_Current_OVERCURRENT(SguanESC_System_STRUCT *sguan,uint32_t *count){
    if (sguan->esc.__Real_CURRENT > sguan->safe.CURRENT_MAX){
        Status_Switch_DISABLED(sguan,count);
    }
}

// 状态机判断并切换
static void Status_Switch_Tick(SguanESC_System_STRUCT *sguan){
    if (sguan->status < MOTOR_STATUS_IDLE){
        return;
    }

    // ====== 安全状态(状态) ======
    if ((sguan->status != MOTOR_STATUS_EMERGENCY_STOP_user) && 
        EMERGENCY_STOP_Signal()){
        sguan->status = MOTOR_STATUS_EMERGENCY_STOP_user;
    }
    if ((sguan->status != MOTOR_STATUS_DISABLED_user) && 
        DISABLED_Signal()){
        sguan->status = MOTOR_STATUS_DISABLED_user;
    }

    // ====== 初始化与运行状态(状态) ======
    if ((sguan->status == MOTOR_STATUS_EMERGENCY_STOP_user) || 
        (sguan->status == MOTOR_STATUS_DISABLED_user) || 
        (sguan->status < MOTOR_STATUS_IDLE)){
        return; // 紧急停止和失能状态优先级最高, 直接返回不执行后续状态判断
    }
    if ((sguan->status != MOTOR_STATUS_START_user) && 
        START_Signal()){ // [重要]接收到开始信号后，切换到未初始化状态...准备初始化
        sguan->status = MOTOR_STATUS_START_user;
    }

    // ====== 硬件相关错误(状态) ======
    // 1.电机母线电压VBUS状态机
    if (User_VBUS_DataGet() != Value_N_INF){
        if ((sguan->status != MOTOR_STATUS_OVERVOLTAGE) && 
            sguan->esc.__Real_VBUS > sguan->safe.VBUS_MAX){
            sguan->status = MOTOR_STATUS_OVERVOLTAGE;
        }
        else if ((sguan->status != MOTOR_STATUS_UNDERVOLTAGE) && 
            sguan->esc.__Real_VBUS < sguan->safe.VBUS_MIM){
            sguan->status = MOTOR_STATUS_UNDERVOLTAGE;
        }
    }
    // 2.驱动器物理温度Temp状态机
    if (User_Temperature_DataGet() != Value_N_INF){        
        if ((sguan->status != MOTOR_STATUS_OVERTEMPERATURE) && 
            sguan->esc.__Real_Temp > sguan->safe.Temp_MAX){
            sguan->status = MOTOR_STATUS_OVERTEMPERATURE;
        }
        else if ((sguan->status != MOTOR_STATUS_UNDERTEMPERATURE) && 
            sguan->esc.__Real_Temp < sguan->safe.Temp_MIN){
            sguan->status = MOTOR_STATUS_UNDERTEMPERATURE;
        }
    }
    // 3.过流保护
    if ((sguan->status != MOTOR_STATUS_OVERCURRENT) && 
        (sguan->esc.__Real_CURRENT > sguan->safe.CURRENT_MAX)){
        sguan->status = MOTOR_STATUS_OVERCURRENT;
    }
    
    // ====== 运行状态(当前反馈) ======
    // 1.速度模式检测
    if (CONFIG_MODE && 
        (sguan->status != MOTOR_STATUS_CONST_SPEED) && 
        (sguan->encoder.__Real_Speed > sguan->esc.Target_Speed - sguan->safe.Speed_limit) && 
        (sguan->encoder.__Real_Speed < sguan->esc.Target_Speed + sguan->safe.Speed_limit)){
        sguan->status = MOTOR_STATUS_CONST_SPEED;
    }
    if (CONFIG_MODE && 
        (sguan->status != MOTOR_STATUS_ACCELERATING) && 
        (sguan->encoder.__Real_Speed < sguan->esc.Target_Speed + sguan->safe.Speed_limit)){
        sguan->status = MOTOR_STATUS_ACCELERATING;
    }
    if (CONFIG_MODE && 
        (sguan->status != MOTOR_STATUS_DECELERATING) && 
        (sguan->encoder.__Real_Speed > sguan->esc.Target_Speed - sguan->safe.Speed_limit)){
        sguan->status = MOTOR_STATUS_DECELERATING;
    }

}

// 状态机运行函数
static void Status_Run_Tick(SguanESC_System_STRUCT *sguan){
    // 1.状态机失能DISABLED安全状态处理
    if (sguan->status == MOTOR_STATUS_EMERGENCY_STOP_user){
        sguan->esc.Target_Speed = 0.0f;
        sguan->esc.Target_Ubus = 0.0f;
        return;
    }
    if (sguan->status == MOTOR_STATUS_DISABLED_user){
        User_StopMotor_DeInit();
        Status_Switch_STANDBY(sguan);
        return;
    }

    // 2.电机电压异常_错误处理
    static uint32_t count = 0;
    count++;
    if (sguan->status == MOTOR_STATUS_OVERVOLTAGE){
        if (count % sguan->safe.VBUS_watchdog_limit == 0){
            Status_VBUS_OVERVOLTAGE(sguan,&count);
        }
        if (count > sguan->safe.VBUS_watchdog_limit*10){ 
            Status_Switch_IDLE(sguan,&count);
            // 持续自定义的周期后，自动切换退出过压保护，并进入STANDBY
        }
    }
    if (sguan->status == MOTOR_STATUS_UNDERVOLTAGE){
        if (count % sguan->safe.VBUS_watchdog_limit == 0){
            Status_VBUS_UNDERVOLTAGE(sguan,&count);
        }
        if (count > sguan->safe.VBUS_watchdog_limit*10){ 
            Status_Switch_IDLE(sguan,&count);
            // 持续自定义的周期后，自动切换退出过压保护，并进入STANDBY
        }
    }
    // 3.电机驱动器温度异常_错误处理
    if (sguan->status == MOTOR_STATUS_OVERTEMPERATURE){
        if (count % sguan->safe.Temp_watchdog_limit == 0){
            Status_Temp_OVERTEMPERATURE(sguan,&count);
        }
        if (count > sguan->safe.Temp_watchdog_limit*10){ 
            Status_Switch_IDLE(sguan,&count);
            // 持续自定义的周期后，自动切换退出过压保护，并进入STANDBY
        }
    }
    if (sguan->status == MOTOR_STATUS_UNDERTEMPERATURE){
        if (count % sguan->safe.Temp_watchdog_limit == 0){
            Status_Temp_UNDERTEMPERATURE(sguan,&count);
        }
        if (count > sguan->safe.Temp_watchdog_limit*10){ 
            Status_Switch_IDLE(sguan,&count);
            // 持续自定义的周期后，自动切换退出过压保护，并进入STANDBY
        }
    }
    // 4.电机过流保护OVERCURRENT错误处理
    if (sguan->status == MOTOR_STATUS_OVERCURRENT){
        if (count % sguan->safe.CURRENT_watchdog_limit == 0){
            Status_Current_OVERCURRENT(sguan,&count);
        }
        if (count > sguan->safe.CURRENT_watchdog_limit*10){ 
            Status_Switch_IDLE(sguan,&count);
            // 持续自定义的周期后，自动切换退出过压保护，并进入STANDBY
        }
    }
    MotorStatus_Loop(&sguan->status);
}

// 高频率运行函数，电调主要运算任务
static void Sguan_Calculate_High_Loop(SguanESC_System_STRUCT *sguan){
    if (sguan->status >= MOTOR_STATUS_IDLE){        
        // 1.用户实时动态任务计算
        User_RealTime_Parameter_Loop(sguan);
    
        // 2.锁相环速度计算
        Transfer_PLL_Loop(&sguan->pll,
            sguan->esc.__sector,
            sguan->motor.Motor_Dir);
    
        sguan->encoder.__Real_Speed = sguan->pll.go.OutWe/(float)sguan->motor.Poles;
        sguan->encoder.__Real_We = sguan->pll.go.OutWe;
    
        // 3.LPF动态截止频率计算
        float abs_Speed = Value_fabsf(sguan->encoder.__Real_We);
        float RC_Phase = fast_atan(abs_Speed/sguan->motor.RC_Wc);
        if (sguan->esc.Target_Phase >= (RC_Phase + sguan->motor.Phase_Limit)){
            sguan->encoder.__LPF_Wc = abs_Speed/fast_tan(sguan->esc.Target_Phase - RC_Phase);
            if (sguan->error != 0){
                sguan->error = 0;
            }
        }
        else{
            sguan->encoder.__LPF_Wc = abs_Speed/fast_tan(sguan->motor.Phase_Limit);
            if (sguan->error != 1){
                sguan->error = 1;
            }
        }
        Value_Limit(&sguan->encoder.__LPF_Wc, 
            sguan->encoder.LPF_Max, 
            sguan->encoder.LPF_Min);
    
        // 4.低通滤波器计算(动态and固定)
        sguan->lpf.__LPF_Ua.Wc = sguan->encoder.__LPF_Wc;
        sguan->lpf.__LPF_Ub.Wc = sguan->encoder.__LPF_Wc;
        sguan->lpf.__LPF_Uc.Wc = sguan->encoder.__LPF_Wc;
        Transfer_LPF_RealTime_Loop(&sguan->lpf.__LPF_Ua,User_ReadADC_Raw(1)*sguan->motor.Uabc_Gain);
        sguan->esc.__Ua = sguan->lpf.__LPF_Ua.go.Output;
        Transfer_LPF_RealTime_Loop(&sguan->lpf.__LPF_Ub,User_ReadADC_Raw(2)*sguan->motor.Uabc_Gain);
        sguan->esc.__Ub = sguan->lpf.__LPF_Ub.go.Output;
        Transfer_LPF_RealTime_Loop(&sguan->lpf.__LPF_Uc,User_ReadADC_Raw(3)*sguan->motor.Uabc_Gain);
        sguan->esc.__Uc = sguan->lpf.__LPF_Uc.go.Output;
    
        #if !CONFIG_UMID
        sguan->lpf.__LPF_Umid.Wc = sguan->encoder.__LPF_Wc;
        Transfer_LPF_RealTime_Loop(&sguan->lpf.__LPF_Umid,User_ReadADC_Raw(0));
        sguan->esc.__Umid = sguan->lpf.__LPF_Umid.go.Output;
        #else // CONFIG_UMID
        sguan->esc.__Umid = (sguan->esc.__Ua+sguan->esc.__Ub+sguan->esc.__Uc)/3.0f;
        #endif // CONFIG_UMID
    
        #if CONFIG_MODE
        Transfer_LPF_Fixed_Loop(&sguan->lpf.LPF_Sign,sguan->esc.Target_Speed);
        sguan->esc.__Speed_in = sguan->lpf.LPF_Sign.go.Output;
        #else // CONFIG_MODE
        Transfer_LPF_Fixed_Loop(&sguan->lpf.LPF_Sign,sguan->esc.Target_Ubus);
        sguan->esc.__Ubus_in = sguan->lpf.LPF_Sign.go.Output;
        #endif // CONFIG_MODE
    
        // 5.闭环控制算法实现
        #if CONFIG_MODE
        Transfer_PID_Loop(&sguan->pid, 
            sguan->esc.__Speed_in, 
            sguan->encoder.__Real_Speed);
        sguan->esc.__Ubus_in = sguan->pid.run.Output;
        #endif // CONFIG_MODE
    
        // 6.运行SWPWM六步换相PWM方波调制策略
        float D_ubus_in = sguan->esc.__Ubus_in*
                        sguan->motor.Motor_Dir/
                        sguan->esc.__Real_VBUS;
        SWPWM(&sguan->esc.__sector, 
            sguan->motor.Duty, 
            D_ubus_in, 
            sguan->esc.__Umid, 
            sguan->esc.__Ua, 
            sguan->esc.__Ub, 
            sguan->esc.__Uc,
            abs_Speed);
    }
    
}

// 低频率运行函数，电调状态机判断和切换函数
static void Sguan_Calculate_Low_Loop(SguanESC_System_STRUCT *sguan){
    // 1.获取实时电机数据
    Data_VBUS_Get_Tick(sguan);
    Data_CURRENT_Get_Tick(sguan);
    Data_Temp_Get_Tick(sguan);

    // 2.电调状态机切换
    Status_Switch_Tick(sguan);

    // 3.电调状态机运行指示任务
    Status_Run_Tick(sguan);
}

// 主循环运算函数，初始化和串口打印数据
static void Sguan_Calculate_main_Loop(SguanESC_System_STRUCT *sguan){
    if (sguan->status == MOTOR_STATUS_START_user){
        // 1.更新目前的电调状态机
        sguan->status = MOTOR_STATUS_INITIALIZING;
        
        // 2.传递函数初始化
        Transfer_Init(sguan);

        // 3.初始化完成，电机进入正常运行状态
        sguan->status = MOTOR_STATUS_IDLE;
    }
}


/**
 * @description: SguanESC核心文件，定时中断服务函数(高频率电机载波)
 * @reminder: 10Khz或者更高定时中断中调用，任务优先级“最高”
 * @return {*}
 */
void SguanESC_High_Loop(void){
    Sguan_Calculate_High_Loop(&Sguan);
}

/**
 * @description: SguanESC核心文件，定时中断服务函数(1ms周期数据更新)
 * @reminder: 1Khz或者更低定时中断中调用，任务优先级“中”
 * @return {*}
 */
void SguanESC_Low_Loop(void){
    // 执行电机状态机切换函数和任务
    Sguan_Calculate_Low_Loop(&Sguan);
}

/**
 * @description: SguanESC核心文件，UART或者CAN接收完成中断服务函数
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
    if (Sguan.status >= MOTOR_STATUS_IDLE){
        Printf_TX_Loop(&Sguan.txdata);
    }
}


