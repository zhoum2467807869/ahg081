#ifndef  __FAN_CTRL_TASK_H__
#define  __FAN_CTRL_TASK_H__



/*灯带任务*/
void fan_ctrl_task(void const * argument);
extern osThreadId fan_ctrl_task_hdl;



#define  FAN_CTRL_TASK_INTERVAL                      osWaitForever/*运行间隔*/

/*灯条任务信号*/
#define  FAN_CTRL_TASK_FAN_TURN_ON_SIGNAL             (1<<0)
#define  FAN_CTRL_TASK_FAN_TURN_OFF_SIGNAL            (1<<1)

#define  FAN_CTRL_TASK_DOOR_STATUS_OPEN_SIGNAL        (1<<2)
#define  FAN_CTRL_TASK_DOOR_STATUS_CLOSE_SIGNAL       (1<<3)

#define  FAN_CTRL_TASK_UPS_PWR_ON_SIGNAL              (1<<4)
#define  FAN_CTRL_TASK_UPS_PWR_OFF_SIGNAL             (1<<5)

#define  FAN_CTRL_TASK_DEBUG_FAN_TURN_ON_SIGNAL       (1<<6)
#define  FAN_CTRL_TASK_DEBUG_FAN_TURN_OFF_SIGNAL      (1<<7)

#define  FAN_CTRL_TASK_ALL_SIGNALS                    ((1<<8)-1)











#endif