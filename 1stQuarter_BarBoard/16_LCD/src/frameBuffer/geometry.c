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

/* 画圆 */
int geometry_draw_circle
(
	draw_point_coordinate center_point,
	uint32 radius,
	uint32 color
)
{
	int x_tmp = 0;
	int y_tmp = 0;
	int x_index = 0;
	int y_index = 0;
	int x_start = center_point.x - radius;
	int y_start = center_point.y - radius;
	int x_end = center_point.x + radius;
	int y_end = center_point.y + radius;
	uint32 squareInner = (radius-1) * (radius-1);
	uint32 squareOuter = radius * radius;
	int squareSum = 0;

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

/* 画环 */
int geometry_draw_annulus
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
			if ((squareSum > squareInner) && (squareSum <= squareOuter))
			{
				(void)frameBuffer_set_point(x_index, y_index, color);
			}
		}
	}

	return OK;
}

/* 画线 */
int geometry_draw_line
(
	draw_point_coordinate start_point,
	draw_point_coordinate end_point,
	uint32 color
)
{
	return OK;
}

