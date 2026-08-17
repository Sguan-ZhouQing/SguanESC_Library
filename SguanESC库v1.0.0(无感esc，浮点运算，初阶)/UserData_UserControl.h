#ifndef __USERDATA_USERCONTROL_H
#define __USERDATA_USERCONTROL_H
#include "SguanESC.h"
/* 电机控制User用户设置·实时参数控制页面 */

/**
 * @description: 1.用户的通信发送接口
 * @reminder: (此方函数->填写你需要修改的串口发送的数据)
 * @return {*}
 */
static inline void User_UserTX(void){
    // 传入串口printf要发送的数据，如txdata.fdata[0]，最多默认16个数值
    // 如需传入更多数值，请修改Sguan_printf中的参数
    // Sguan.txdata.fdata[0] = Sguan.pll.go.OutRe;
    // Sguan.txdata.fdata[1] = Sguan.pll.go.OutWe;
    // Sguan.txdata.fdata[2] = Sguan.pll.go.Error;
    // Sguan.txdata.fdata[3] = Sguan.esc.__sector;
    // Sguan.txdata.fdata[4] = Sguan.esc.__Umid;
    // Sguan.txdata.fdata[5] = Sguan.esc.__Ua;
    // Sguan.txdata.fdata[6] = Sguan.esc.__Ub;
    // Sguan.txdata.fdata[7] = Sguan.esc.__Uc;
    // Sguan.txdata.fdata[8] = Sguan.encoder.__LPF_Wc;
}

/**
 * @description: 2.用户的通信接收接口
 * @reminder: (此方函数->填写浮点数赋值对象)
 * @reminder: (如果需要其他指令，可在Sguan_printf.c中添加)
 * @param {float} data
 * @return {*}
 */
static inline void Handle_User0_Adjust(float data){
    /* Your code for Parameter set */
    // 接收到串口或者CAN的数据是User0=xx?
    // 收到指令后，会把数据赋值到data
}

/**
 * @description: 3.用户的通信接收接口
 * @reminder: (此方函数->填写浮点数赋值对象)
 * @param {float} data
 * @return {*}
 */
static inline void Handle_User1_Adjust(float data){
    /* Your code for Parameter set */
    // 接收到串口或者CAN的数据是User1=xx?
    // 收到指令后，会把数据赋值到data
}

/**
 * @description: 4.用户的通信接收接口
 * @reminder: (此方函数->填写浮点数赋值对象)
 * @param {float} data
 * @return {*}
 */
static inline void Handle_User2_Adjust(float data){
    /* Your code for Parameter set */
    // 接收到串口或者CAN的数据是User2=xx?
    // 收到指令后，会把数据赋值到data
}


#endif // USERDATA_USERCONTROL_H
