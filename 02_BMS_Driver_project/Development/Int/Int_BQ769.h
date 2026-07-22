#ifndef __INT_BQ769_H__
#define __INT_BQ769_H__
#include "Com_Debug.h"
#include "i2c.h"
#include "gpio.h"
#include "Int_BQ769_BSP.h"
#include "FreeRTOS.h"
#include "task.h"

//定义BQ769 I2C从地址为0x10，地址在高7位，最低位是读写标志位，0为写，1为读
#define BQ769_I2C_ADDR_W 0x10
#define BQ769_I2C_ADDR_R 0x11

//声明全局变量
extern RegisterGroup g_BQ769_RegGroup;

/**
 * @brief 初始化BQ769
 * 
 */
void Int_BQ769_Init(void);


/**
 * @brief 写入BQ769寄存器
 * 
 * @param regaddr 寄存器地址
 * @param data 数据
 * @return uint8_t 0:成功 1:失败
 */
uint8_t Int_BQ769_WriteReg(uint8_t regaddr, uint8_t data);


/**
 * @brief 读取BQ769寄存器
 * 
 * @param regaddr 寄存器地址
 * @param datd 读取到的数据指针
 * @param len 读取数据的长度
 * @return uint8_t 0:成功 1:失败
 */
uint8_t Int_BQ769_ReadReg(uint8_t regaddr, uint8_t *data,uint16_t len);

#endif /* __INT_BQ769_H__ */
