#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <tools.h>
#include <paletee.h>

typedef struct
{
	uint32 x;
	uint32 y;
}draw_point_coordinate;

/* 画点 */
extern int geometry_draw_point
(
	draw_point_coordinate point,
	paletee_256_type_t color
);

/* 画圆，内部不填充 */
extern int geometry_draw_circle_empty
(
	draw_point_coordinate center_point,
	uint32 radius,
	paletee_256_type_t color
);

/* 画圆，内部填充 */
extern int geometry_draw_circle_full
(
	draw_point_coordinate center_point,
	uint32 radius,
	paletee_256_type_t color
);

/* 画环 */
extern int geometry_draw_annulus_full
(
	draw_point_coordinate center_point,
	uint32 innerRadius,
	uint32 outerRadius,
	paletee_256_type_t color
);

/* 画线 */
extern int geometry_draw_line
(
	draw_point_coordinate start_point,
	draw_point_coordinate end_point,
	paletee_256_type_t color
);

/* 画十字 */
extern void geometry_draw_cross
(
	draw_point_coordinate core, 
	uint32 width,
	paletee_256_type_t color
);
#endif	/* _GEOMETRY_H_ */