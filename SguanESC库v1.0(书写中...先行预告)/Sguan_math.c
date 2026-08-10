/*
 * @Author: 星必尘Sguan
 * @GitHub: https://github.com/Sguan-ZhouQing
 * @Date: 2026-08-09 00:27:55
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-08-10 17:07:07
 * @FilePath: \SguanESC_Debug\Debug_Packeage\Sguan_math.c
 * @Description: SguanESC库的“数学算法库”
 * 
 * Copyright (c) 2026 by $星必尘Sguan, All Rights Reserved. 
 */
#include "Sguan_math.h"

// 数值fabsf绝对值函数
float Value_fabsf(float x){
  union{
    float f;
    uint32_t i;
  }u;
  u.f = x;
  u.i &= 0x7FFFFFFF;
  return u.f;
}

// 数值限幅float函数
void Value_Limit(float *val, float max, float min){
    if (*val > max) *val = max;
    if (*val < min) *val = min;
}


