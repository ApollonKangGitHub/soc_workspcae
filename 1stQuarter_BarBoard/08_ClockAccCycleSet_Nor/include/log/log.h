#ifndef _LOG_H_
#define _LOG_H_

#include <tools.h>

typedef enum soc_debug_level{
	SOC_DBG_NORMAL,
	SOC_DBG_WARN,
	SOC_DBG_ERROR,
	SOC_DBG_LOG,
	SOC_DBG_MAX
}SOC_DBG_LEV;

#define SOC_DBG_ERR_STR		"Error"
#define SOC_DBG_WARN_STR 	"Warnning"
#define SOC_DBG_NOR_STR 	"Debug"
#define SOC_DBG_LOG_STR 	"Logging"

#define SOC_DEBUG_PRINT_LEVEL(lev, format, ...){\
		print_screen(lev"[%s-%d]:"format, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	}

/* 当前就error、warning、normal可能会使用到，后期考虑动态注册，全局标志来控制 */
#define SOC_DEBUG_PRINT(lev, format, ...){\
		if(SOC_DBG_ERROR == lev)\
			SOC_DEBUG_PRINT_LEVEL(SOC_DBG_ERR_STR, format, ##__VA_ARGS__)\
		else if(SOC_DBG_WARN == lev)\
			SOC_DEBUG_PRINT_LEVEL(SOC_DBG_WARN_STR, format, ##__VA_ARGS__)\
		else if(SOC_DBG_NORMAL == lev)\
			SOC_DEBUG_PRINT_LEVEL(SOC_DBG_NOR_STR, format, ##__VA_ARGS__)\
		else if(SOC_DBG_LOG == lev){\
			SOC_DEBUG_PRINT_LEVEL(SOC_DBG_LOG_STR, format, ##__VA_ARGS__)\
		}\
	}

/* 系统debug打印，由debug开关控制，开机时中断置flag位 */
extern BOOL gSysDebugFlag;

#define SYS_DEBUG_PRINT(lev, format, ...){\
		if(gSysDebugFlag){\
			SOC_DEBUG_PRINT(lev, format, ##__VA_ARGS__);\
		}\
	}
#endif
