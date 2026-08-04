#include "vofa.h"
#include <string.h>

// 定义共用体：float 和 uint32_t 共享内存，方便拆解字节[reference:13]
typedef union {
    float fdata;
    uint32_t udata;
} FloatUnion;

// 发送缓冲区
static uint8_t tx_buffer[VOFA_MAX_FLOATS * 4 + 4];

/**
 * @brief 阻塞方式发送多个 float 到 VOFA+
 * @param huart 串口句柄（如 &huart1）
 * @param data  float 数组指针
 * @param count 要发送的 float 数量
 */
void VOFA_SendFloats_Blocking(UART_HandleTypeDef *huart, float *data, uint16_t count) {
    if (count > VOFA_MAX_FLOATS) count = VOFA_MAX_FLOATS;
    
    FloatUnion fu;
    uint16_t idx = 0;
    
    // 1. 将所有 float 按小端顺序填入缓冲区[reference:15]
    for (uint16_t i = 0; i < count; i++) {
        fu.fdata = data[i];
        tx_buffer[idx++] = (uint8_t)(fu.udata & 0xFF);
        tx_buffer[idx++] = (uint8_t)((fu.udata >> 8) & 0xFF);
        tx_buffer[idx++] = (uint8_t)((fu.udata >> 16) & 0xFF);
        tx_buffer[idx++] = (uint8_t)((fu.udata >> 24) & 0xFF);
    }
    
    // 2. 添加帧尾：0x00, 0x00, 0x80, 0x7F
    uint32_t tail = VOFA_TAIL_WORD;
    tx_buffer[idx++] = (uint8_t)(tail & 0xFF);
    tx_buffer[idx++] = (uint8_t)((tail >> 8) & 0xFF);
    tx_buffer[idx++] = (uint8_t)((tail >> 16) & 0xFF);
    tx_buffer[idx++] = (uint8_t)((tail >> 24) & 0xFF);
    
    // 3. 通过串口发送
    HAL_UART_Transmit(huart, tx_buffer, idx, 100);
}

/**
 * @brief 发送单个 float（封装便捷函数）
 */
void VOFA_SendFloat(UART_HandleTypeDef *huart, float data) {
    VOFA_SendFloats_Blocking(huart, &data, 1);
}

/**
 * @brief 发送多个 float（封装便捷函数）
 */
void VOFA_SendFloats(UART_HandleTypeDef *huart, float *data, uint16_t count) {
    VOFA_SendFloats_Blocking(huart, data, count);
}