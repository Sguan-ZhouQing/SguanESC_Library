#ifndef __SGUANESC_H
#define __SGUANESC_H

/* USER CODE BEGIN Includes */
// 电机控制核心函数文件声明
#include "Sguan_MotorStatus.h"
#include "Sguan_Step.h"
/* USER CODE END Includes */

typedef struct{
    uint8_t status;
    
}SguanESC_System_STRUCT;


void SguanESC_High_Loop(void);
void SguanESC_Low_Loop(void);
void SguanESC_Printf_Loop(void);
void SguanESC_main_Loop(void);


#endif // SGUANESC_H
