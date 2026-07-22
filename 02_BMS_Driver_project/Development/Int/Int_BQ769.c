#include "Int_BQ769.h"

// 定义BQ769寄存器的映射结构体变量
RegisterGroup g_BQ769_RegGroup;

//!========================================================================================================================================================================

// 静态函数：计算CRC8校验和
static uint8_t Int_BQ769_CRC8(const uint8_t *data, size_t length)
{
    uint8_t crc = 0x00;
    for (size_t i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ 0x07;
            }
            else
            {
                crc <<= 1;
            }
        }
        crc &= 0xFF;
    }
    return crc;
}

//!========================================================================================================================================================================

/**
 * @brief 写入BQ769寄存器
 *
 * @param regaddr 寄存器地址
 * @param data 数据
 * @return uint8_t 0:成功 1:失败
 */
uint8_t Int_BQ769_WriteReg(uint8_t regaddr, uint8_t data)
{

    // 计算CRC8校验和
    uint8_t crc8_data[3] = {BQ769_I2C_ADDR_W, regaddr, data};
    uint8_t crc8_byte = Int_BQ769_CRC8(crc8_data, 3);

    // 使用i2c发送数据
    uint8_t send_data[2] = {data, crc8_byte};

    // 进入临界区
    taskENTER_CRITICAL();

    uint8_t res = HAL_I2C_Mem_Write(&hi2c2, BQ769_I2C_ADDR_W, regaddr, I2C_MEMADD_SIZE_8BIT, send_data, 2, 1000) == HAL_OK ? 0 : 1;

    // 退出临界区
    taskEXIT_CRITICAL();

    return res;
}

//!========================================================================================================================================================================
/**
 * @brief 读取BQ769寄存器
 *
 * @param regaddr 寄存器地址
 * @param datd 读取到的数据指针
 * @param len 读取数据的长度
 * @return uint8_t 0:成功 1:失败
 */
uint8_t Int_BQ769_ReadReg(uint8_t regaddr, uint8_t *data, uint16_t len)
{

    // 定义变量，保存读取到的数据
    uint8_t read_data[len * 2];

    // 读取I2C数据
    //  进入临界区
    taskENTER_CRITICAL();

    HAL_StatusTypeDef res = HAL_I2C_Mem_Read(&hi2c2, BQ769_I2C_ADDR_R, regaddr, I2C_MEMADD_SIZE_8BIT, read_data, len * 2, 1000);

    // 退出临界区
    taskEXIT_CRITICAL();

    if (res != HAL_OK)
    {
        DEBUG_PRINTLN("BQ769 ReadReg Failed");
        return 1;
    }

    // 对读取到的数据进行CRC8校验
    for (uint8_t i = 0; i < len; i++)
    {
        uint8_t crc8_byte = 0;
        // 处理第一个数据字节
        if (i == 0)
        {
            // 计算CRC校验和
            uint8_t crc_data[2] = {BQ769_I2C_ADDR_R | 0x01,read_data[i]};
            crc8_byte = Int_BQ769_CRC8(crc_data, 2);
        }
        else
        {
            // 处理其他数据字节
            crc8_byte = Int_BQ769_CRC8(&read_data[i * 2], 1);
        }

        if (crc8_byte != read_data[i*2+1])
        {
            DEBUG_PRINTLN(" %d CRC8 Check Failed",i);
            return 1;
            /* code */
        }

        //将数据放入data数组
        data[i] = read_data[i*2];
        
    }
    return 0;
}

//!========================================================================================================================================================================

// 进入ship模式
static void Int_BQ769_EnterShipMode(void)
{

    // 连续两次写入

    // 第一次写入SHUT_A = 0 , SHUT_B = 1
    g_BQ769_RegGroup.SysCtrl1.SysCtrl1Bit.SHUT_A = 0;
    g_BQ769_RegGroup.SysCtrl1.SysCtrl1Bit.SHUT_B = 1;
    Int_BQ769_WriteReg(BQ_SYS_CTRL1, g_BQ769_RegGroup.SysCtrl1.SysCtrl1Byte);

    // 第二次写入SHUT_A = 1 , SHUT_B = 0
    g_BQ769_RegGroup.SysCtrl1.SysCtrl1Bit.SHUT_A = 1;
    g_BQ769_RegGroup.SysCtrl1.SysCtrl1Bit.SHUT_B = 0;
    Int_BQ769_WriteReg(BQ_SYS_CTRL1, g_BQ769_RegGroup.SysCtrl1.SysCtrl1Byte);
}

//!========================================================================================================================================================================

// 进入Normal模式
static void Int_BQ769_EnterNormalMode(void)
{

    // 拉高BQ769_WKP引脚,保持至少10us，实际保持1ms
    HAL_GPIO_WritePin(BQ769_WKP_GPIO_Port, BQ769_WKP_Pin, GPIO_PIN_SET);

    // 保持至少1ms
    HAL_Delay(1);

    // 拉低BQ769_WKP引脚
    HAL_GPIO_WritePin(BQ769_WKP_GPIO_Port, BQ769_WKP_Pin, GPIO_PIN_RESET);
}

//!========================================================================================================================================================================

/**
 * @brief 初始化BQ769
 *
 */
void Int_BQ769_Init(void)
{

    // 进入ship模式
    Int_BQ769_EnterShipMode();
    HAL_Delay(1000);
    DEBUG_PRINTLN("BQ769 Enter Ship Mode");

    // 进入Normal模式
    Int_BQ769_EnterNormalMode();
    HAL_Delay(1000);
    DEBUG_PRINTLN("BQ769 Enter Normal Mode");

    DEBUG_PRINTLN("BQ769 Init Done");
}
