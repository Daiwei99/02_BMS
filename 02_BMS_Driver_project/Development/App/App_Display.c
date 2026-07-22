#include "App_Display.h"

/* OLED 显示参数 */
#define DISP_FONT_SIZE   16                    // 字号(中文 16x16, ASCII 16x8)
#define DISP_MODE        1                     // 1:正常显示  0:反色
#define DISP_LINE_Y      0                     // 首行 Y 坐标
#define CN_CHAR_WIDTH    DISP_FONT_SIZE        // 中文字符宽度(像素)
#define ASCII_CHAR_WIDTH (DISP_FONT_SIZE / 2)  // ASCII 字符宽度(像素)

/**
 * @brief 初始化显示: "BMS 项目"
 *
 */
void App_Display_Init(void)
{
    uint8_t x = 0;  // 横向光标, 逐个元素累加宽度, 避免硬编码坐标

    // 初始化
    Int_OLED_Init();

    // 前半段中文 "尚硅谷" (字库索引 5~7)
    for (uint8_t i =0 ; i < 3; i++, x += CN_CHAR_WIDTH)
    {
        Int_OLED_ShowChinese(x, DISP_LINE_Y, i, DISP_FONT_SIZE, DISP_MODE);
    }

    // 字符串 "sb"
    // Int_OLED_ShowString(x, DISP_LINE_Y, "sb", DISP_FONT_SIZE, DISP_MODE);
    // x += 2 * ASCII_CHAR_WIDTH;

    // 字符串 "BMS"
    Int_OLED_ShowString(x, DISP_LINE_Y, "BMS", DISP_FONT_SIZE, DISP_MODE);
    x += 3 * ASCII_CHAR_WIDTH;

    // 后半段中文 "项目" (字库索引 3~4)
    for (uint8_t i = 3; i < 5; i++, x += CN_CHAR_WIDTH)
    {
        Int_OLED_ShowChinese(x, DISP_LINE_Y, i, DISP_FONT_SIZE, DISP_MODE);
    }
}

void App_Display_Run(void){

    //刷新显示
    Int_OLED_Refresh();
}
