#include "App_Battery.h"

// 全局变量 保存gain
static double s_gain = 0.0;

// 全局变量 保存offset
static int8_t s_offset = 1;
//!========================================================================================================================================================================

// 静态函数，获取增益量和偏移量
static void App_Battery_GetGainAndOffset(void)
{

    // 1. 获取gain
    // 1.1 定义变量，保存读取的GAINADC值
    uint8_t gain_adc_val = 0;

    // 1.2 读取高位的GAINADC值
    uint8_t gain_adc_val_high = 0;
    Int_BQ769_ReadReg(BQ_ADCGAIN1, &gain_adc_val_high, 1);

    // 1.3 读取低位的GAINADC值
    uint8_t gain_adc_val_low = 0;
    Int_BQ769_ReadReg(BQ_ADCGAIN2, &gain_adc_val_low, 1);

    // 1.4 合并两个寄存器的高低位
    gain_adc_val = ((gain_adc_val_high & 0x0c) << 1) | (gain_adc_val_low >> 5);

    // 1.5 计算增益值，单位是mv/LSB
    s_gain = (gain_adc_val + 365) / 1000.0;

    // 2. 获取offset
    Int_BQ769_ReadReg(BQ_ADCOFFSET, (uint8_t *)&s_offset, 1);

    // 打印信息
    DEBUG_PRINTLN("s_gain = %.2f, s_offset = %d", s_gain, s_offset);
}
//!========================================================================================================================================================================

// 电池管理的初始化配置
static void App_Battery_InitConfig(void)
{

    // 1. 基本配置
    //CTRL_1
    // 1.1 开启ADC使能
    g_BQ769_RegGroup.SysCtrl1.SysCtrl1Bit.ADC_EN = 1;

    // 1.2 设置使用外部热敏电阻测温
    g_BQ769_RegGroup.SysCtrl1.SysCtrl1Bit.TEMP_SEL = 1;

    //1.3 写入寄存器CTRL_1
    Int_BQ769_WriteReg(BQ_SYS_CTRL1, g_BQ769_RegGroup.SysCtrl1.SysCtrl1Byte);

    //CTRL_2
    // 1.3 设置库仑计连续测电流
    g_BQ769_RegGroup.SysCtrl2.SysCtrl2Bit.CC_EN = 1;

    // 1.4 开启放电使能
    g_BQ769_RegGroup.SysCtrl2.SysCtrl2Bit.DSG_ON = 1;

    //1.5 开启充电使能
    g_BQ769_RegGroup.SysCtrl2.SysCtrl2Bit.CHG_ON = 1;

    //1.6 写入寄存器CTRL_2
    Int_BQ769_WriteReg(BQ_SYS_CTRL2, g_BQ769_RegGroup.SysCtrl2.SysCtrl2Byte);

    //2. 设置短路保护 PROTECT1寄存器
    //2.1 电流检测范围选择为高量程（检测流电阻是5mΩ）
    g_BQ769_RegGroup.Protect1.Protect1Bit.RSNS = 1;

    //2.2 延时为200us SCD_D[1:0]=0B10g
    g_BQ769_RegGroup.Protect1.Protect1Bit.SCD_DELAY = BMS_SCD_DELAY_200us;

    //2.3 设置阈值为200mv SCD_T[2:0]=0B111
    g_BQ769_RegGroup.Protect1.Protect1Bit.SCD_THRESH = 0x07;

    //2.4 写入寄存器PROTECT1
    Int_BQ769_WriteReg(BQ_PROTECT1, g_BQ769_RegGroup.Protect1.Protect1Byte);

    //3. 设置过流保护 PROTECT2寄存器
    //3.1 设置过流延时为最大延时 1280ms OCD_D[6:4]= 0x07
    g_BQ769_RegGroup.Protect2.Protect2Bit.OCD_DELAY = BMS_OCD_DELAY_1280ms;

    //3.2 设置过流阈值为100mv OCD_T[2:0]= 0x0f
    g_BQ769_RegGroup.Protect2.Protect2Bit.OCD_THRESH = 0x0f;

    //3.3 写入寄存器PROTECT2
    Int_BQ769_WriteReg(BQ_PROTECT2, g_BQ769_RegGroup.Protect2.Protect2Byte);
}

//!========================================================================================================================================================================

/**
 * @brief 初始化电池模块
 *
 */
void App_Battery_Init(void)
{
    // 1. 获取增益量和偏移量
    App_Battery_GetGainAndOffset();

    // 2. 初始化电池管理的配置
    App_Battery_InitConfig();
}
