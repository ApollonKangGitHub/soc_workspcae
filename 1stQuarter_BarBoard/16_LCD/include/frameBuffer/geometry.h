#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <tools.h>

typedef struct
{
	uint32 x;
	uint32 y;
}draw_point_coordinate;

/* 画点 */
extern int geometry_draw_point
(
	draw_point_coordinate point,
	uint32 color
);

/* 画圆，内部不填充 */
extern int geometry_draw_circle_empty
(
	draw_point_coordinate center_point,
	uint32 radius,
	uint32 color
);

/* 画圆，内部填充 */
extern int geometry_draw_circle_full
(
	draw_point_coordinate center_point,
	uint32 radius,
	uint32 color
);

/* 画环 */
extern int geometry_draw_annulus_full
(
	draw_point_coordinate center_point,
	uint32 innerRadius,
	uint32 outerRadius,
	uint32 color
);

/* 划线 */
extern int geometry_draw_line
(
	draw_point_coordinate start_point,
	draw_point_coordinate end_point,
	uint32 color
);

#endif	/* _GEOMETRY_H_ */