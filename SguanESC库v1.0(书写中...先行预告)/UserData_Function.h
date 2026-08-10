#ifndef __USERDATA_FUNCTION_H
#define __USERDATA_FUNCTION_H
#include <stdint.h>
/* 电机控制User用户设置·功能接口 */


/**
 * @description: 1.电机上电即初始化的函数接口
 * @reminder: (此方函数->填入一些最先初始化的代码)
 * @reminder: (比如定时器中断开启、串口接收开启)
 * @return {*}
 */
static inline void User_Initial_Init(void){
    /* Your code for Initializing immediately upon powering on here */

    // User profile is like:
    // // 初始化定时器中断
    // HAL_TIM_Base_Start_IT(&htim1);
    // HAL_TIM_Base_Start_IT(&htim2);
    // // 启用串口DMA接收
    // HAL_UARTEx_ReceiveToIdle_DMA(&huart1, Sguan_PrintfBuff, sizeof(Sguan_PrintfBuff));
    // __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}

/**
 * @description: 2.有启动信号后才初始化的函数
 * @reminder: (此方函数->填入电机启动后才初始化的代码)
 * @reminder: (比如使能驱动、开启PWM输出)
 * @return {*}
 */
static inline void User_StartMotor_Init(void){
    /* Your code for initing TIM and gate driver and encoder and ADC here */

    // User profile is like:
    // // 开启PWM输出
    // HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    // HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    // HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
}

/**
 * @description: 3.用户系统的Delay函数对接
 * @reminder: (此方函数->填入你自己单片机的延时函数)
 * @reminder: (用于某些初始化场景的时机等待)
 * @param {unsigned int} ms
 * @return {*}
 */
static inline void User_Delay(uint32_t ms){
    /* Your code for Delay_ms here */

    // User profile is like:
    // HAL_Delay(ms);
}

/**
 * @description: 4.用户读取的ADC原始数据传入函数
 * @reminder: (此方函数->填入你电机采样的ADC原始数据)
 * @reminder: (比如12位数据就是0-4095)
 * @reminder: (0是中性点数据，若无，UserData_Config.h中定义后可软件计算)
 * @reminder: (123分别对应电机UVW三相的相电压采集数据)
 * @param {int32_t} Current_CH
 * @return {*}
 */
static inline int32_t User_ReadADC_Raw(int32_t Current_CH){
    int32_t ADC_num = 0;
    switch (Current_CH){
    case 0:
        /* Your code for Motor Umid raw */

        // User profile is like:
        // ADC_num = (int32_t)ADC_InjectedValues[0];
        break;
    case 1:
        /* Your code for Motor Ua raw */

        // User profile is like:
        // ADC_num = (int32_t)ADC_InjectedValues[1];
        break;
    case 2:
        /* Your code for Motor Ub raw */

        // User profile is like:
        // ADC_num = (int32_t)ADC_InjectedValues[2];
        break;
    case 3:
        /* Your code for Motor Uc raw */

        // User profile is like:
        // ADC_num = (int32_t)ADC_InjectedValues[3];
        break;
    default:
        break;
    }
    return ADC_num;
}

/**
 * @description: 5.用户的PWM驱动接口函数
 * @reminder: (此方函数->填写你自己的驱动器PWM占空比)
 * @return {*}
 */
static inline void User_PwmDuty_SetU(uint8_t Duty_CH,
                                uint32_t Duty_uvw){
    /* Your code for Motor PWM_CH0~2 duty set */

    switch (Duty_CH){
    case 0:
        // User profile is like:
        // __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Duty_uvw);
        break;
    case 1:
        // User profile is like:
        // __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,Duty_uvw);
        break;
    case 2:
        // User profile is like:
        // __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,Duty_uvw);
        break;
    
    default:
        break;
    }
}

/**
 * @description: 6.用户的PWM使能与失能函数
 * @reminder: (此方函数->填写你自己的驱动器PWM使能或者失能设置)
 * @return {*}
 */
static inline void User_PWM_SWitch(uint8_t Duty_CH,
                                uint8_t Enable){
    /* Your code for Motor PWM_CH0~2 duty enable */
    
    switch (Duty_CH){
    case 0:
        if (Enable){
            // User profile is like:
            // HAL_GPIO_WritePin(SD1_GPIO_Port,SD1_Pin,GPIO_PIN_SET);
        }
        else{
            // User profile is like:
            // HAL_GPIO_WritePin(SD1_GPIO_Port,SD1_Pin,GPIO_PIN_RESET);
        }
        break;
    case 1:
        if (Enable){
            // User profile is like:
            // HAL_GPIO_WritePin(SD2_GPIO_Port,SD2_Pin,GPIO_PIN_SET);
        }
        else{
            // User profile is like:
            // HAL_GPIO_WritePin(SD2_GPIO_Port,SD2_Pin,GPIO_PIN_RESET);
        }
        break;
    case 3:
        if (Enable){
            // User profile is like:
            // HAL_GPIO_WritePin(SD3_GPIO_Port,SD3_Pin,GPIO_PIN_SET);
        }
        else{
            // User profile is like:
            // HAL_GPIO_WritePin(SD3_GPIO_Port,SD3_Pin,GPIO_PIN_RESET);
        }
        break;
    
    default:
        break;
    }
}

/**
 * @description: 10.用户的驱动器母线电压读取接口
 * @reminder: (此方函数->填写驱动器母线电压滤波后的数值)
 * @return {*}
 */
static inline float User_VBUS_DataGet(void){
    // float VBUS_num = 0.0f;
    /* Your code for motor VBUS_Voltage Data return if you use it */
    
    // 如果不使用电压功能，返回0xFF800000（正常数值不会是负无穷）
    return 0xFF800000;
}

/**
 * @description: 11.用户的驱动器温度读取接口
 * @reminder: (此方函数->填写驱动器温度滤波后的数值)
 * @return {*}
 */
static inline float User_Temperature_DataGet(void){
    // float Temp_num = 0.0f;
    /* Your code for motor Temperature Data return if you use it */
    
    // 如果不使用温度功能，返回0xFF800000（正常数值不会是负无穷）
    return 0xFF800000;
}

/**
 * @description: 8.用户的通信接口设计
 * @reminder: (此方函数->填写串口或者CAN的对应接口函数)
 * @param {unsigned char} *ch
 * @param {unsigned short int} size
 * @return {*}
 */
static inline void User_CorrespondSet(uint8_t *ch, uint16_t size){
    /* Your code for UART or CAN Signal Transmit Driver */

    // User profile is like:
    // HAL_UART_Transmit(&huart1, ch, size, 0xFFFF);
}


#endif // USERDATA_FUNCYION_H
