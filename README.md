# SguanESC - 轻量BLDC无感电调控制库
![Version](https://img.shields.io/badge/Version-1.0-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Language](https://img.shields.io/badge/C-00599C)
![Platform](https://img.shields.io/badge/ARM_/_MCU-支持-orange)

---

## 项目简介
**SguanESC** 是一款专为无刷直流电机 (BLDC) 设计的开源无感控制算法库，完全基于纯 C 语言编写，适配各类嵌入式微控制器。该库核心采用三相分压电阻电压检测方案，内置六步换相逻辑与无刷电机过零检测功能，结合精细化的电机状态机设计，实现对 BLDC 电机的精准控制与灵活调节。

## 核心特性
- 三相分压电阻电压采样
- 无感BLDC六步换相控制
- 电机过零检测
- 电机状态机管理
- 纯逻辑层、易移植

## 现有文件
- `SguanESC.c/h`：核心入口
- `Sguan_MotorStatus.c/h`：电机状态机
- `Sguan_Step.c/h`：六步换相
- `Sguan_math.c/h`：数学运算
- `UserData_*`：用户配置与硬件接口

## 开发状态
当前为**初始框架版本**，功能正在逐步实现中。

## 说明
本项目为轻量化无感BLDC电调驱动库，专注简单可靠、开箱即用。

---
⭐ 欢迎Star，持续更新中～

要不要我帮你把这份**极简README**直接生成可复制的纯文本？
