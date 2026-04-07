#ifndef __USERDATA_FUNCTION_H
#define __USERDATA_FUNCTION_H
/* 电机控制User用户设置·功能接口 */
/* 用户自己的CODE BEGIN Includes */
#include "main.h"

extern UART_HandleTypeDef huart1;
/* 用户自己的CODE END Includes */

static inline void User_InitialInit(void){
    /* Your code for initing TIM and gate driver and encoder and ADC here */
    // // 开启SD使能栅极驱动器
    // HAL_GPIO_WritePin(SD1_GPIO_Port,SD1_Pin,GPIO_PIN_SET);
    // HAL_GPIO_WritePin(SD2_GPIO_Port,SD2_Pin,GPIO_PIN_SET);
    // HAL_GPIO_WritePin(SD3_GPIO_Port,SD3_Pin,GPIO_PIN_SET);
    // // 开启PWM输出
    // HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    // HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    // HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    // HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    // // 设置TIM3计数器初始值
    // __HAL_TIM_SET_COUNTER(&htim3, 0);
}

static inline void User_Delay(unsigned int ms){
    /* Your code for Delay_ms here */
    // HAL_Delay(ms);
}

static inline signed int User_ReadADC_Raw(unsigned char Current_CH){
    signed int ADC_num = 0;
    switch (Current_CH){
    case 0:
        /* Your code for Motor CH0 raw */
        break;
    case 1:
        /* Your code for Motor CH1 raw */
        break;
    case 2:
        /* Your code for Motor CH2 raw */
        break;
    default:
        break;
    }
    return ADC_num;
}

/* ================= 驱动代码(驱动层) ================= */
static inline void User_CorrespondSet(unsigned char *ch, unsigned short int size){
    /* Your code for UART or CAN Signal Transmit Driver */
    HAL_UART_Transmit(&huart1, ch, size, 0xFFFF);
}


#endif // USERDATA_FUNCTION_H
