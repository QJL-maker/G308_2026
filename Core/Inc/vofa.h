#ifndef __VOFA_H
#define __VOFA_H

#include "main.h"
#include <stdint.h>

// 定义最大可发送 float 数量（可自行调整）
#define VOFA_MAX_FLOATS 16

// 帧尾：+Inf 的小端表示
#define VOFA_TAIL_WORD 0x7F800000

void VOFA_SendFloat(UART_HandleTypeDef *huart, float data);
void VOFA_SendFloats(UART_HandleTypeDef *huart, float *data, uint16_t count);
void VOFA_SendFloats_Blocking(UART_HandleTypeDef *huart, float *data, uint16_t count);

#endif