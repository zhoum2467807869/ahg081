#ifndef  __UPS_STATUS_TASK_H__
#define  __UPS_STATUS_TASK_H__


uint8_t ups_status_task_get_ups_status();
/*UPS任务*/
void ups_status_task(void const * argument);
extern osThreadId ups_status_task_hdl;


/*UPS任务*/
#define  UPS_STATUS_TASK_INTERVAL                         100/*UPS每隔100ms监视一次*/
/*UPS信号异常跳变过滤次数*/
#define  UPS_STATUS_TASK_HOLD_CNT_MAX                      5

#define  UPS_STATUS_TASK_STATUS_PWR_OFF                    0
#define  UPS_STATUS_TASK_STATUS_PWR_ON                     1
#define  UPS_STATUS_TASK_STATUS_INIT                       0xfe
#define  UPS_STATUS_TASK_STATUS_ERR                        0xff

#endif