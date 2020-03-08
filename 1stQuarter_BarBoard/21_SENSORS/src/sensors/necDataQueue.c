#include <tools.h>
#include <necDataQueue.h>

#define NEC_DATA_QUEUE_LEN				(1024)
#define NEC_DATA_QUEUE_NEXT_PLACE(pos) 	((pos + 1) % NEC_DATA_QUEUE_LEN)

static int gReadPos = 0;
static int gWritePos = 0;
static necDataQueue_t * gNecEventData[NEC_DATA_QUEUE_LEN];

/* 循环队列判满 */
static int necDataQUeue_is_empty(void)
{
	return (gReadPos == gWritePos);
}

/* 循环队列判空 */
static int necDataQUeue_is_full(void)
{
	return (NEC_DATA_QUEUE_NEXT_PLACE(gWritePos) == gReadPos);
}

/* 初始化清除循环队列，应用在使用前clear一下，保证清除垃圾数据 */
int necDataQUeue_clear_data(void)
{
	gReadPos = gWritePos;
}

/* 循环队列存数据 */
int necDataQUeue_put_data(necDataQueue_t * eventData)
{
	if (necDataQUeue_is_full())
	{
		return ERROR;
	}

	gNecEventData[gWritePos]->pol = eventData->pol;
	gNecEventData[gWritePos]->duration = eventData->duration;
	gWritePos = NEC_DATA_QUEUE_NEXT_PLACE(gWritePos);
	
	return OK;
}

/* 循环队列取数据 */
int necDataQUeue_get_data(necDataQueue_t * eventData)
{
	if (necDataQUeue_is_empty())
	{
		return ERROR;
	}

	eventData->pol = gNecEventData[gReadPos]->pol;
	eventData->duration = gNecEventData[gReadPos]->duration;
	gReadPos = NEC_DATA_QUEUE_NEXT_PLACE(gReadPos);
	
	return OK;
}

/* 循环队列取数据，指定等待超时时间 */
int necDataQUeue_get_data_timeout(necDataQueue_t * eventData, int timeout_us)
{
	while(timeout_us--)
	{
		if (OK == necDataQUeue_get_data(eventData))
		{
			return OK;
		}
	}

	return ERROR;
}

