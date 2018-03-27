#ifndef  __LOCK_STATUS_TASK_H__ 
#define  __LOCK_STATUS_TASK_H__




extern osThreadId lock_status_task_hdl;
extern void lock_status_task(void const * argument);

/*锁状态任务运行间隔*/
#define  LOCK_STATUS_TASK_INTERVAL                    50

/*锁状态任务状态定义*/
#define  LOCK_STATUS_TASK_LOCK_STATUS_LOCK            1
#define  LOCK_STATUS_TASK_LOCK_STATUS_UNLOCK          2
#define  LOCK_STATUS_TASK_LOCK_STATUS_INIT            3









#endif