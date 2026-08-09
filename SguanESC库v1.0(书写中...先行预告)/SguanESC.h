#ifndef __SGUANESC_H
#define __SGUANESC_H

#include "Sguan_GIC.h"
#include "Sguan_LPF.h"
#include "Sguan_MotorStatus.h"
#include "Sguan_SWPWM.h"

// ╔═════════════════════════════════════════════════════════╗
// ║                       电调控制模式                       ║
// ╚═════════════════════════════════════════════════════════╝
#define MODE_Voltag_OPEN        // 电压开环控制（Sguan.esc.Ubus_in）
#define MODE_Velocity_Single    // 速度单环控制（Sguan.esc.Target_Speed）


typedef struct{
    uint8_t status;
}SguanESC_System_STRUCT;

extern SguanESC_System_STRUCT Sguan;

void SguanESC_High_Loop(void);
void SguanESC_Low_Loop(void);
void SguanESC_Printf_Loop(uint8_t *data, uint16_t length);
void SguanESC_main_Loop(void);


#endif // SGUANESC_H
