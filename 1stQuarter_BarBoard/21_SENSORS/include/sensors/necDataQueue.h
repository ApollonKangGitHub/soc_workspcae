#ifndef _NEC_DATA_QUEUE_H
#define _NEC_DATA_QUEUE_H

typedef struct {
	int pol; 		/* 极性 */
	int duration;  	/* 脉冲宽度, us */
}necDataQueue_t;

typedef enum {
	nec_read_ret_ok,		/* 读取数据成功 */
	nec_read_ret_timeout, 	/* 读取超时 */
	nec_read_ret_crc,		/* 校验失败 */
	nec_read_ret_unknow,	/* 未知电平值，可能是脉冲宽度超出可容忍范围 */
	nec_read_ret_repl,		/* 重复码 */
	nec_read_ret_error,		/* 其他错误 */
	nec_read_ret_max
}necReadRet_t;

#define NEC_READ_RET_STR_ARR \
	"ok", \
	"timeout", \
	"check error", \
	"unknow data", \
	"repl data", \
	"other error"

extern int necDataQUeue_clear_data(void);
extern int necDataQUeue_put_data(necDataQueue_t * eventData);
extern int necDataQUeue_get_data(necDataQueue_t * eventData);
extern int necDataQUeue_get_data_timeout(necDataQueue_t * eventData, int timeout_us);

#endif /* _NEC_DATA_QUEUE_H */

