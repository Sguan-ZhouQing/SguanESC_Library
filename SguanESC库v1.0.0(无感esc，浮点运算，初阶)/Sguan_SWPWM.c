/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-08-09 00:35:59
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-08-11 01:00:07
 * @FilePath: \SguanESC_Library\Sguan_SWPWM.c
 * @Description: SguanESC库的“六步换相的PWM调制策略实现”
 * 
 * Copyright (c) 2026 by $星必尘Sguan, All Rights Reserved. 
 */
#include "Sguan_SWPWM.h"

/* UserData外部文件声明 */
#include "UserData_Function.h"

#define MOTOR_CH_a      0x00        // 宏定义A相的设置通道定义
#define MOTOR_CH_b      0x01        // 宏定义B相的设置通道定义
#define MOTOR_CH_c      0x02        // 宏定义C相的设置通道定义

#define MOTOR_DISENABLE 0x00        // PWM失能信号
#define MOTOR_ENABLE    0x01        // PWM使能信号

/**
 * @description: 六步换相PWM切换调制
 * @param {int8_t} *sector 电机换相扇区
 * @param {uint32_t} Duty   满占空比
 * @param {int8_t} D_ubus_in控制端电压信号
 * @param {float} Umid      中性点电压
 * @param {float} Ua        A相相电压
 * @param {float} Ub        B相相电压
 * @param {float} Uc        C相相电压
 * @return {*}
 */
void SWPWM(int8_t *sector, 
        uint32_t Duty,
        float D_ubus_in,
        float Umid, 
        float Ua, 
        float Ub, 
        float Uc, 
        float abs_Speed){
    switch (*sector){
    case 0:
        // AB通电->C悬空
        if ((Uc >= Umid) && (D_ubus_in >= 0.0f)){
            (*sector)++;
            break;
        }
        if((Uc <= Umid) && (D_ubus_in < 0.0f)){
            (*sector)--;
            break;
        }
        break;
    case 1:
        // AC通电->B悬空
        if ((Ub <= Umid) && (D_ubus_in >= 0.0f)){
            (*sector)++;
            break;
        }
        if((Ub >= Umid) && (D_ubus_in < 0.0f)){
            (*sector)--;
            break;
        }
        break;
    case 2:
        // BC通电->A悬空
        if ((Ua >= Umid) && (D_ubus_in >= 0.0f)){
            (*sector)++;
            break;
        }
        if((Ua <= Umid) && (D_ubus_in < 0.0f)){
            (*sector)--;
            break;
        }
        break;
    case 3:
        // BA通电->C悬空
        if ((Uc <= Umid) && (D_ubus_in >= 0.0f)){
            (*sector)++;
            break;
        }
        if((Uc >= Umid) && (D_ubus_in < 0.0f)){
            (*sector)--;
            break;
        }
        break;
    case 4:
        // CA通电->B悬空
        if ((Ub >= Umid) && (D_ubus_in >= 0.0f)){
            (*sector)++;
            break;
        }
        if((Ub <= Umid) && (D_ubus_in < 0.0f)){
            (*sector)--;
            break;
        }
        break;
    case 5:
        // CB通电->A悬空
        if ((Ua <= Umid) && (D_ubus_in >= 0.0f)){
            (*sector)++;
            break;
        }
        if((Ua >= Umid) && (D_ubus_in < 0.0f)){
            (*sector)--;
            break;
        }
        break;
    
    default:
        break;
    }

    float abs_ubus = Value_fabsf(D_ubus_in);
    if ((abs_Speed <= (float)CONFIG_We) && (abs_ubus >= (float)CONFIG_UBUS)){
        static uint32_t num = 0;
        num++;
        if (num >= (uint32_t)(CONFIG_WAIT/BLDC_RUN_T)){   // 50us中断中，每250ms刷新一下角度位置
            if (D_ubus_in >= 0.0f){
                (*sector)++;
            }
            else{
                (*sector)--;
            }
            num = 0;
        }
    }
    

	// *sector限幅
    if ((*sector) <= -1){
        (*sector) = 5;
    }
	else if((*sector) >= 6){
		(*sector) = 0;
	}

	switch (*sector){
	case 0:
        // AB通电
        User_PwmDuty_Set(MOTOR_CH_a, (uint16_t)(Duty*(0.5f + abs_ubus*0.5f)));
        User_PwmDuty_Set(MOTOR_CH_b, (uint16_t)(Duty*(0.5f - abs_ubus*0.5f)));
        User_PWM_SWitch(MOTOR_CH_a, MOTOR_ENABLE);
        User_PWM_SWitch(MOTOR_CH_b, MOTOR_ENABLE);
        // C悬空
        User_PWM_SWitch(MOTOR_CH_c, MOTOR_DISENABLE);
		break;
	case 1:
        // AC通电
        User_PwmDuty_Set(MOTOR_CH_a, (uint16_t)(Duty*(0.5f + abs_ubus*0.5f)));
        User_PwmDuty_Set(MOTOR_CH_c, (uint16_t)(Duty*(0.5f - abs_ubus*0.5f)));
        User_PWM_SWitch(MOTOR_CH_a, MOTOR_ENABLE);
        User_PWM_SWitch(MOTOR_CH_c, MOTOR_ENABLE);
        // B悬空
        User_PWM_SWitch(MOTOR_CH_b, MOTOR_DISENABLE);
		break;
	case 2:
        // BC通电
        User_PwmDuty_Set(MOTOR_CH_b, (uint16_t)(Duty*(0.5f + abs_ubus*0.5f)));
        User_PwmDuty_Set(MOTOR_CH_c, (uint16_t)(Duty*(0.5f - abs_ubus*0.5f)));
        User_PWM_SWitch(MOTOR_CH_b, MOTOR_ENABLE);
        User_PWM_SWitch(MOTOR_CH_c, MOTOR_ENABLE);
        // A悬空
        User_PWM_SWitch(MOTOR_CH_a, MOTOR_DISENABLE);
		break;
	case 3:
        // BA通电
        User_PwmDuty_Set(MOTOR_CH_b, (uint16_t)(Duty*(0.5f + abs_ubus*0.5f)));
        User_PwmDuty_Set(MOTOR_CH_a, (uint16_t)(Duty*(0.5f - abs_ubus*0.5f)));
        User_PWM_SWitch(MOTOR_CH_b, MOTOR_ENABLE);
        User_PWM_SWitch(MOTOR_CH_a, MOTOR_ENABLE);
        // C悬空
        User_PWM_SWitch(MOTOR_CH_c, MOTOR_DISENABLE);
		break;
	case 4:
        // CA通电
        User_PwmDuty_Set(MOTOR_CH_c, (uint16_t)(Duty*(0.5f + abs_ubus*0.5f)));
        User_PwmDuty_Set(MOTOR_CH_a, (uint16_t)(Duty*(0.5f - abs_ubus*0.5f)));
        User_PWM_SWitch(MOTOR_CH_c, MOTOR_ENABLE);
        User_PWM_SWitch(MOTOR_CH_a, MOTOR_ENABLE);
        // B悬空
        User_PWM_SWitch(MOTOR_CH_b, MOTOR_DISENABLE);
		break;
	case 5:
        // CB通电
        User_PwmDuty_Set(MOTOR_CH_c, (uint16_t)(Duty*(0.5f + abs_ubus*0.5f)));
        User_PwmDuty_Set(MOTOR_CH_b, (uint16_t)(Duty*(0.5f - abs_ubus*0.5f)));
        User_PWM_SWitch(MOTOR_CH_c, MOTOR_ENABLE);
        User_PWM_SWitch(MOTOR_CH_b, MOTOR_ENABLE);
        // A悬空
        User_PWM_SWitch(MOTOR_CH_a, MOTOR_DISENABLE);
		break;
	
	default:
		break;
	}
}

