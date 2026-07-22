#ifndef __COM_DEBUG_H__
#define __COM_DEBUG_H__

#include <stdio.h>
#include <string.h>
#include "usart.h"

#define DEBUG_MODE 1

#if DEBUG_MODE == 1

// ���·������ /, ȡ / ����ģ����û�� /, ֱ��ȡȫ��
#define BASENAME_PRE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
// ���·���� \, ȡ \ ����ģ����û�� \, ֱ��ȡȫ��
#define BASENAME strrchr(BASENAME_PRE, '\\') ? strrchr(BASENAME_PRE, '\\') + 1 : BASENAME_PRE
#define DEBUG_PRINT(fmt, ...) printf("[%s:%d] " fmt, BASENAME, __LINE__, ##__VA_ARGS__)
#define DEBUG_PRINTLN(fmt, ...) printf("[%s:%d] " fmt "\n", BASENAME, __LINE__, ##__VA_ARGS__)

#else

#define BASENAME_PRE
#define BASENAME
#define DEBUG_PRINT(fmt, ...)
#define DEBUG_PRINTLN(fmt, ...)

#endif

#endif /* __COM_DEBUG_H__ */
