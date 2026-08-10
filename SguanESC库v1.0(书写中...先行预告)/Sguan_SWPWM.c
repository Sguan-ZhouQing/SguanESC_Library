/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-08-09 00:35:59
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-08-10 17:23:46
 * @FilePath: \SguanESC_Debug\Debug_Packeage\Sguan_SWPWM.c
 * @Description: SguanESC库的“六步换相的PWM调制策略实现”
 * 
 * Copyright (c) 2026 by $星必尘Sguan, All Rights Reserved. 
 */
#include "Sguan_SWPWM.h"

/* UserData外部文件声明 */
#include "UserData_Function.h"

/**
 * @description: 六步换相PWM切换调制
 * @param {uint8_t} *sector
 * @param {float} Umid
 * @param {float} Ua
 * @param {float} Ub
 * @param {float} Uc
 * @param {uint8_t} flag
 * @return {*}
 */
void SWPWM_Tick(uint8_t *sector, 
                float Umid, 
                float Ua, 
                float Ub, 
                float Uc, 
                uint8_t flag){
    if (flag){
        switch (*sector){
        case 0:
            if (Uc >= Umid){
                *sector++;
            }
            break;
        case 1:
            if (Ub <= Umid){
                *sector++;
            }
            break;
        case 2:
            if (Ua >= Umid){
                *sector++;
            }
            break;
        case 3:
            if (Uc <= Umid){
                *sector++;
            }
            break;
        case 4:
            if (Ub >= Umid){
                *sector++;
            }
            break;
        case 5:
            if (Ua <= Umid){
                *sector++;
            }
            break;
        
        default:
            break;
        }
    }
    

	// *sector限幅
	if (*sector >= 6){
		*sector = 0;
	}

	switch (*sector){
	case 0:
		Sguan_SetDuty(0,1);
		Sguan_SetDuty(1,0);
		Sguan_SetDuty(2,5);
		break;
	case 1:
		Sguan_SetDuty(0,1);
		Sguan_SetDuty(1,5);
		Sguan_SetDuty(2,0);
		break;
	case 2:
		Sguan_SetDuty(0,5);
		Sguan_SetDuty(1,1);
		Sguan_SetDuty(2,0);
		break;
	case 3:
		Sguan_SetDuty(0,0);
		Sguan_SetDuty(1,1);
		Sguan_SetDuty(2,5);
		break;
	case 4:
		Sguan_SetDuty(0,0);
		Sguan_SetDuty(1,5);
		Sguan_SetDuty(2,1);
		break;
	case 5:
		Sguan_SetDuty(0,5);
		Sguan_SetDuty(1,0);
		Sguan_SetDuty(2,1);
		break;
	
	default:
		break;
	}
}

