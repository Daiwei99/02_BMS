#include "App_Main.h"
// 显示任务
void App_Main_Display(void *pvParameters);
#define APP_MAIN_DISPLAY_STACK_SIZE 512
#define APP_MAIN_DISPLAY_PRIORITY 5
#define APP_MAIN_DISPLAY_NAME "App_Main_Display"
TaskHandle_t dis_task_handle = NULL;

/**
 * @brief 函数启动
 *
 */
void App_Main_Start(void)
{
    // 进入临界区
    taskENTER_CRITICAL();

    // 创建显示任务
    BaseType_t res = xTaskCreate(App_Main_Display, APP_MAIN_DISPLAY_NAME, APP_MAIN_DISPLAY_STACK_SIZE, NULL, APP_MAIN_DISPLAY_PRIORITY, &dis_task_handle);
    if (res == pdPASS)
    {
        DEBUG_PRINTLN("App_Main_Display Task Create Success");
        /* code */
    }
    else
    {
        DEBUG_PRINTLN("App_Main_Display Task Create Failed");
    }

    // 退出临界区
    taskEXIT_CRITICAL();

    // 启动任务调度器
    vTaskStartScheduler();
}

// 显示任务的回调函数
void App_Main_Display(void *pvParameters)
{

    DEBUG_PRINTLN("App_Main_Display Task Start");

    // 初始化显示
    App_Display_Init();
    while (1)
    {

        // 进入临界区
        taskENTER_CRITICAL();
        // 刷新显示
        App_Display_Run();

        // 退出临界区
        taskEXIT_CRITICAL();

        // 延时100ms
        vTaskDelay(100 / portTICK_PERIOD_MS);

        /* code */
    }
}
