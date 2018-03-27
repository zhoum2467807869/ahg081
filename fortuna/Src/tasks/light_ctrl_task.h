#ifndef  __LIGHT_CTRL__TASK_H__
#define  __LIGHT_CTRL__TASK_H__


/*灯带任务*/
void light_ctrl_task(void const * argument);
extern osThreadId light_ctrl_task_hdl;



#define  LIGHT_CTRL_TASK_INTERVAL                       osWaitForever/*运行间隔*/

/*灯条任务信号*/
#define  LIGHT_CTRL_TASK_LIGHT_TURN_ON_SIGNAL            (1<<0)
#define  LIGHT_CTRL_TASK_LIGHT_TURN_OFF_SIGNAL           (1<<1)
#define  LIGHT_CTRL_TASK_DEBUG_LIGHT_TURN_ON_SIGNAL      (1<<2)
#define  LIGHT_CTRL_TASK_DEBUG_LIGHT_TURN_OFF_SIGNAL     (1<<3)
#define  LIGHT_CTRL_TASK_UPS_PWR_ON_SIGNAL               (1<<4)
#define  LIGHT_CTRL_TASK_UPS_PWR_OFF_SIGNAL              (1<<5)
#define  LIGHT_CTRL_TASK_ALL_SIGNALS                     ((1<<6)-1)








#endif