#ifndef  __HOST_COMM_TASK_H__
#define  __HOST_COMM_TASK_H__

/*主机通信任务*/
void host_comm_task(void const * argument);
/*主机通信任务任务handle*/
extern osThreadId host_comm_task_hdl;


/*主机通信任务信号*/
#define  HOST_COMM_TASK_BUFF_OVERFLOW_SIGNAL                    (1<<0)
#define  HOST_COMM_TASK_RECV_FSM_SIGNAL                         (1<<1)
#define  HOST_COMM_TASK_PARSE_PROTOCOL_SIGNAL                   (1<<2)
#define  HOST_COMM_TASK_SEND_FSM_SIGNAL                         (1<<3)
#define  HOST_COMM_TASK_SEND_FSM_OVER_SIGNAL                    (1<<4)
/*通信任务子函数信号*/
#define  HOST_COMM_TASK_CLEAR_SCALE_TARE_WEIGHT_OK_SIGNAL       (1<<5)
#define  HOST_COMM_TASK_CLEAR_SCALE_TARE_WEIGHT_ERR_SIGNAL      (1<<6)
#define  HOST_COMM_TASK_CLEAR_ZERO_OK_SIGNAL                    (1<<7)
#define  HOST_COMM_TASK_CLEAR_ZERO_ERR_SIGNAL                   (1<<8)
#define  HOST_COMM_TASK_CALIBRATE_SCALE_CODE_OK_SIGNAL          (1<<9)
#define  HOST_COMM_TASK_CALIBRATE_SCALE_CODE_ERR_SIGNAL         (1<<10)
#define  HOST_COMM_TASK_CALIBRATE_SCALE_MEASUREMENT_OK_SIGNAL   (1<<11)
#define  HOST_COMM_TASK_CALIBRATE_SCALE_MEASUREMENT_ERR_SIGNAL  (1<<12)
#define  HOST_COMM_TASK_CALIBRATE_SCALE_WEIGHT_OK_SIGNAL        (1<<13)
#define  HOST_COMM_TASK_CALIBRATE_SCALE_WEIGHT_ERR_SIGNAL       (1<<14)
#define  HOST_COMM_TASK_LOCK_LOCK_SUCCESS_SIGNAL                (1<<15)
#define  HOST_COMM_TASK_LOCK_LOCK_FAIL_SIGNAL                   (1<<16)
#define  HOST_COMM_TASK_UNLOCK_LOCK_SUCCESS_SIGNAL              (1<<17)
#define  HOST_COMM_TASK_UNLOCK_LOCK_FAIL_SIGNAL                 (1<<18)

#define  HOST_COMM_TASK_ALL_SIGNALS                        ((1<<19)-1)

/*主机通信任务同步事件*/
#define  HOST_COMM_TASK_SYNC_EVT                           (1<<3)








#endif