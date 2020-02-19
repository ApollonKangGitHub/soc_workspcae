#include <geometry.h>
#include <tools.h>
#include <frameBuffer.h>

/* 画点 */
int geometry_draw_point
(
	draw_point_coordinate point,
	uint32 color
)
{
	return frameBuffer_set_point(point.x, point.y, color);
}

#if 1

/* 画环,环内部填充 */
int geometry_draw_annulus_full
(
	draw_point_coordinate center_point,
	uint32 innerRadius,
	uint32 outerRadius,
	uint32 color
)
{
	int x_tmp = 0;
	int y_tmp = 0;
	int x_index = 0;
	int y_index = 0;
	int x_start = center_point.x - outerRadius;
	int y_start = center_point.y - outerRadius;
	int x_end = center_point.x + outerRadius;
	int y_end = center_point.y + outerRadius;
	uint32 squareInner = innerRadius * innerRadius;
	uint32 squareOuter = outerRadius * outerRadius;
	int squareSum = 0;

	if (outerRadius < innerRadius)
	{
		return ERROR;
	}

	for (x_index = x_start; x_index < x_end; x_index++)
	{
		x_tmp = x_index - center_point.x;
		for (y_index = y_start; y_index < y_end; y_index++)
		{
			y_tmp = y_index - center_point.y;
			squareSum = (x_tmp * x_tmp) + (y_tmp  * y_tmp );
			if ((squareSum >= squareInner) && (squareSum <= squareOuter))
			{
				(void)frameBuffer_set_point(x_index, y_index, color);
			}
		}
	}

	return OK;
}

/* 画圆,只画边,圆内部不填充 */
int geometry_draw_circle_empty
(
	draw_point_coordinate center_point,
	uint32 radius,
	uint32 color
)
{
	return geometry_draw_annulus_full(center_point, radius-1, radius, color);
}

/* 画圆,圆内部填充 */
int geometry_draw_circle_full
(
	draw_point_coordinate center_point,
	uint32 radius,
	uint32 color
)
{
	return geometry_draw_annulus_full(center_point, 0, radius, color);
}

#else
/* 
 * 画圆,只画边,圆内部不填充,画圆代码来自于
 * https://blog.csdn.net/p1126500468/article/details/50428613
 */
int geometry_draw_circle_empty
(
	draw_point_coordinate center_point,
	uint32 radius,
	uint32 color
)
{
    int xTmp, yTmp, num;
    xTmp = 0;
    yTmp = radius;
	
    while((2 * yTmp * yTmp) >= (radius * radius))	/* 八分之一圆 */
    {
        frameBuffer_set_point(center_point.x + xTmp, center_point.y - yTmp, color);
        frameBuffer_set_point(center_point.x - xTmp, center_point.y - yTmp, color);
        frameBuffer_set_point(center_point.x - xTmp, center_point.y + yTmp, color);
        frameBuffer_set_point(center_point.x + xTmp, center_point.y + yTmp, color);
 
        frameBuffer_set_point(center_point.x + yTmp, center_point.y + xTmp, color);
        frameBuffer_set_point(center_point.x + yTmp, center_point.y - xTmp, color);
        frameBuffer_set_point(center_point.x - yTmp, center_point.y - xTmp, color);
        frameBuffer_set_point(center_point.x - yTmp, center_point.y + xTmp, color);
        
        xTmp++;
        num = (xTmp * xTmp + yTmp * yTmp) - radius * radius;
        if (num > 0)
        {
            yTmp--;
            xTmp--;
        }
    }

	
}

/* 画环,环内部填充 */
int geometry_draw_annulus_full
(
	draw_point_coordinate center_point,
	uint32 innerRadius,
	uint32 outerRadius,
	uint32 color
)
{
	int inner = innerRadius;
	int outer = outerRadius;
	
	if (outerRadius < innerRadius)
	{
		return ERROR;
	}

	while (inner <= outer)
	{
		(void)geometry_draw_circle_empty(center_point, inner, color);
		inner++;
	}

	return OK;
}

/* 画圆,圆内部填充 */
int geometry_draw_circle_full
(
	draw_point_coordinate center_point,
	uint32 radius,
	uint32 color
)
{
	return geometry_draw_annulus_full(center_point, 0, radius, color);
}

#endif

/* 画线 */
int geometry_draw_line
(
	draw_point_coordinate start_point,
	draw_point_coordinate end_point,
	uint32 color
)
{
	double dx = (double)(start_point.x - end_point.x);
	double dy = (double)(start_point.y - end_point.y);
	double slope = dy / dx;	/* 斜率 */

	return OK;
}

