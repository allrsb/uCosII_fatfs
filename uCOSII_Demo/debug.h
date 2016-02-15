#ifndef __DEBUG_H_
#define __DEBUG_H_

#include <stdio.h>
#include <stdarg.h>

#define DEBUG_ALL    1
#define DEBUG_ASSERT 1

#define DBG_LEVEL_ALL          0x00
#define DBG_ERROR              0x04      //��ӡ����
#define DBG_WARN	           0x03
#define DBG_INFO               0x02
#define DBG_DEBUG              0x01
#define DBG_LEVEL_MASK         0xFF      //����

#define LOG_WRITE              0x80      //���λ����SD����log��־

#define DBG_MIN_LEVEL DBG_LEVEL_ALL  //���ƴ�ӡ����





#if DEBUG_ALL

#define DBG_print(level, format, ...) \
do{\
	if ((level & DBG_LEVEL_MASK) >= DBG_MIN_LEVEL)\
	{\
		printf("%s, [%s, %d]:  ", #level, __FUNCTION__, __LINE__);\
		printf(format, ##__VA_ARGS__); \
		printf("\n");\
	}\
} while (0);
#else
#define Debug(level, fmt, args)
#endif



/*����*/
#if DEBUG_ASSERT

#define DBG_assert(assertion, format, args)\
do{\
	if (!(assertion))\
	{\
		printf("ASSERT, [%s, %d]:  ",  __FUNCTION__, __LINE__); \
		printf(format, ##args); \
		printf("\n"); \
		while (1);\
	}\
} while (0)
#else
#define DBG_assert(assertion, format, args) 

#endif


#endif


