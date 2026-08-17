#ifndef __SGUAN_MATH_H
#define __SGUAN_MATH_H

/* 外部函数声明 */
#include <stdint.h>
#include <stdio.h>

// 常量宏定义
#define Value_PI            3.141592653589793f      // 圆周率数值
#define Value_PI_2          1.570796326794896f      // 二分之一pi
#define Value_2PI           6.283185307179586f      // 2pi的数值
#define Value_SQRT2         1.414213562373095f      // 根号二数值
#define Value_N_INF         0xFF800000              // (负无穷数)

float Value_fabsf(float x);
float Value_sqrtf(float x);
void Value_Limit(float *val, float max, float min);
int8_t Value_set(int8_t val, int8_t max, int8_t min);

// 快速正余弦求解float版本
#define fast_cos(x) fast_sin(Value_PI_2 - x);
float fast_sin(float x);
void fast_sin_cos(float x, float *sin_x, float *cos_x);
float fast_tan(float x);
float fast_atan(float x);


#endif // SGUAN_MATH_H
