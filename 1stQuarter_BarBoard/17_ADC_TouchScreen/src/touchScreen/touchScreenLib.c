#include <touchScreen.h>
#include <timer.h>
#include <tools.h>
#include <geometry.h>
#include <lcd_common.h>

#define TOUCH_SCREEN_CALIBRATION_WIDTH	(10)

static double gKx = 0;
static double gKy = 0;
static int gLcdCorex, gLcdCorey;
static int gTsCorex, gTsCorey;
static BOOL gIsSwap = FALSE;
	
static uint32 touchScreen_calc_x(uint32 ts_x)
{
	return (gKx * ((int)ts_x - gTsCorex) + gLcdCorex);
}
static uint32 touchScreen_calc_y(uint32 ts_y)
{
	return (gKy * ((int)ts_y - gTsCorey) + gLcdCorey);
}

/* 触摸屏校准点 */
static touchScreen_calibration_point_data
(
	draw_point_coordinate lcdPoint,
	draw_point_coordinate * tsPoint
)
{
	BOOL pressure = FALSE;
	uint32 ts_x, ts_y;
	uint32 width = TOUCH_SCREEN_CALIBRATION_WIDTH;
	uint32 cnt = 0;
	uint32 x_sum = 0;
	uint32 y_sum = 0;

	/* 十字画线 */
	(void)geometry_draw_cross(lcdPoint , width, paletee_256_type_White_SYSTEM);

	do {
		(void)touchScreen_get_contact_coordinates(&ts_x, &ts_y, &pressure);
	}while(FALSE == pressure); /* 一直读取直到有数据 */
	
	do {	
		/* 多次测量求平均值 */
		if (cnt < 128)
		{
			x_sum += ts_x;
			y_sum += ts_y;
			cnt++;
		}
		(void)touchScreen_get_contact_coordinates(&ts_x, &ts_y, &pressure);
	}while(pressure);	/* 直到松开，单点取值才算完成 */
	
	if (cnt > 0) {
		tsPoint->x = x_sum / cnt;
		tsPoint->y = y_sum / cnt;
	}
	print_screen("\r\ntsPoint->x:%d, tsPoint->y:%d.", tsPoint->x, tsPoint->y);
	(void)geometry_draw_cross(lcdPoint , width, paletee_256_type_Black_SYSTEM);
}

/* 触摸屏x、y和lcd是否坐标反转判断 */
BOOL touchScreen_swap_check
(
	draw_point_coordinate level_left, 
	draw_point_coordinate level_right
)
{
	int dx = level_right.x - level_left.x;
	int dy = level_right.y - level_left.y;

	if (dx < 0) {
		dx = 0 - dx;
	}
	if (dy < 0) {
		dy = 0 - dy;
	}
	if (dx > dy) {
		return FALSE;	/* 未反转 */
	}
	else {
		return TRUE;	/* 反转 */
	}
}

/* 触摸屏坐标反转矫正 */
void touchScreen_swap_point(uint32 *x, uint32 *y)
{
	uint32 tmp = *x;
	*x = *y;
	*y = tmp;
}

/* 触摸屏校准 */
void touchScreen_calibration(void)
{
	uint32 width = TOUCH_SCREEN_CALIBRATION_WIDTH;
	lcd_common_para_t lcdPara;
	
	draw_point_coordinate lcdLeftUp;
	draw_point_coordinate lcdLeftDown;
	draw_point_coordinate lcdCore;
	draw_point_coordinate lcdRightUp;
	draw_point_coordinate lcdRightDown;

	draw_point_coordinate tsLeftUp;
	draw_point_coordinate tsLeftDown;
	draw_point_coordinate tsCore;
	draw_point_coordinate tsRightUp;
	draw_point_coordinate tsRightDown;

	uint32 ts_up_len;
	uint32 ts_down_len;
	uint32 ts_left_len;
	uint32 ts_right_len;
	uint32 lcd_x_len;
	uint32 lcd_y_len;

	lcd_common_para_get(&lcdPara);

	/* 根据LCD参数进行四角和中心坐标计算 */
	lcdLeftUp.x = 50;
	lcdLeftUp.y = 50;
	lcdLeftDown.x = 50;
	lcdLeftDown.y = lcdPara._y_res - 50;
	lcdRightUp.x = lcdPara._x_res - 50;
	lcdRightUp.y = 50;
	lcdRightDown.x = lcdPara._x_res - 50;
	lcdRightDown.y = lcdPara._y_res - 50;
	lcdCore.x = lcdPara._x_res / 2;
	lcdCore.y = lcdPara._y_res / 2;

	/* 
	 * LCD上显示四个角和中心点十字标志，
	 * 并根据触摸屏点击十字中心获取LCD对应触摸屏坐标
	 */
	(void)touchScreen_calibration_point_data(lcdLeftUp, &tsLeftUp);
	(void)touchScreen_calibration_point_data(lcdLeftDown, &tsLeftDown);
	(void)touchScreen_calibration_point_data(lcdCore, &tsCore);
	(void)touchScreen_calibration_point_data(lcdRightUp, &tsRightUp);
	(void)touchScreen_calibration_point_data(lcdRightDown, &tsRightDown);

	/* 坐标反转校准 */
	gIsSwap = touchScreen_swap_check(tsLeftUp, tsRightUp);
	if (gIsSwap) 
	{
		(void)touchScreen_swap_point(&tsLeftUp.x, &tsLeftUp.y);
		(void)touchScreen_swap_point(&tsLeftDown.x, &tsLeftDown.y);
		(void)touchScreen_swap_point(&tsRightDown.x, &tsRightDown.y);
		(void)touchScreen_swap_point(&tsRightUp.x, &tsRightUp.y);
		(void)touchScreen_swap_point(&tsCore.x, &tsCore.y);
	}

	/* 计算 */
	ts_up_len = tsRightUp.x - tsLeftUp.x;
	ts_down_len = tsRightDown.x - tsLeftDown.x;
	ts_left_len = tsLeftDown.y - tsLeftUp.y;
	ts_right_len = tsRightDown.y - tsRightUp.y;
	lcd_x_len = lcdRightUp.x - lcdLeftUp.x;
	lcd_y_len = lcdLeftDown.y - lcdLeftUp.y;
	
	gKx = ((double)(2 * lcd_x_len)) / (ts_up_len + ts_down_len);
	gKy = ((double)(2 * lcd_y_len)) / (ts_left_len + ts_right_len);
	gTsCorex = tsCore.x;
	gTsCorey = tsCore.y;
	gLcdCorex = lcdPara._x_res / 2;
	gLcdCorey = lcdPara._y_res / 2;
#if 0
	print_screen("\r\n ----------------------------------------------------------------");

	print_screen("\r\n lux:%d, luy :%d", tsLeftUp.x, tsLeftUp.y);
	print_screen("\r\n ldx:%d, ldy :%d", tsLeftDown.x, tsLeftDown.y);
	print_screen("\r\n tcx:%d, tcy :%d", tsCore.x, tsCore.y);
	print_screen("\r\n rux:%d, ruy :%d", tsRightUp.x, tsRightUp.y);
	print_screen("\r\n rdx:%d, rdy :%d", tsRightDown.x, tsRightDown.y);
	
	print_screen("\r\n lcd_x_len   :%d", lcd_x_len);
	print_screen("\r\n lcd_y_len   :%d", lcd_y_len);
	
	print_screen("\r\n ts_up_len   :%d", ts_up_len);
	print_screen("\r\n ts_down_len :%d", ts_down_len);
	print_screen("\r\n ts_left_len :%d", ts_left_len);
	print_screen("\r\n ts_right_len:%d", ts_right_len);
	print_screen("\r\n gTsCorex    :%d", gTsCorex);
	print_screen("\r\n gTsCorey    :%d", gTsCorey);
	print_screen("\r\n gLcdCorex   :%d", gLcdCorex);
	print_screen("\r\n gLcdCorey   :%d", gLcdCorey);
	print_screen("\r\n lu.x:%d lu.y:%d", touchScreen_calc_x(tsLeftUp.x), touchScreen_calc_y(tsLeftUp.y));
	print_screen("\r\n ld.x:%d ld.y:%d", touchScreen_calc_x(tsLeftDown.x), touchScreen_calc_y(tsLeftDown.y));
	print_screen("\r\n lc.x:%d lc.y:%d", touchScreen_calc_x(tsCore.x), touchScreen_calc_y(tsCore.y));
	print_screen("\r\n ru.x:%d ru.y:%d", touchScreen_calc_x(tsRightUp.x), touchScreen_calc_y(tsRightUp.y));
	print_screen("\r\n rd.x:%d rd.y:%d", touchScreen_calc_x(tsRightDown.x), touchScreen_calc_y(tsRightDown.y));
#endif
}

/* 触摸屏初始化 */
void touchScreen_init(void)
{
	/* 设置触摸屏相关寄存器 */
	(void)touchScreen_adc_ts_reg_init();

	/* 设置中断 */
	(void)touchScreen_adc_ts_interrupt_init();

	/* 让触摸屏控制器进入等待中断模式 */
	(void)touchScreen_wait_pen_down();
		
	/* 注册定时器回掉函数, 启动定时器 */
	(void)timer_register("touch screen timer 0", touchScreen_timer_handle);
}

int touchScreen_get_point
(
	uint32 * lcd_x, 
	uint32 * lcd_y,
	BOOL * pPressure
)
{
	uint32 ts_x, ts_y;
	BOOL pressure;
	
	/* 获取触点原始坐标 */
	(void)touchScreen_get_contact_coordinates(&ts_x, &ts_y, &pressure);
	if (gIsSwap) 
	{
		/* 如果坐标有反转则先修正 */
		(void)touchScreen_swap_point(&ts_x, &ts_y);
	}

	/* 计算lcd坐标 */
	*lcd_x = touchScreen_calc_x(ts_x);
	*lcd_y = touchScreen_calc_y(ts_y);
	*pPressure = pressure;

	/* 计算的点不在范围内可能是波动，忽略该值 */
	if ((*lcd_x < 0) || (*lcd_x > 2*gLcdCorex)
		|| (*lcd_y < 0) || (*lcd_y > 2*gLcdCorey))
	{	
		return ERROR;
	}
		
	return OK;
}