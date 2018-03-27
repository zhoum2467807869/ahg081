#ifndef  __LOCK_CTRL_TASK_H__
#define  __LOCK_CTRL_TASK_H__
#include "app_common.h"

/*锁任务*/
void lock_ctrl_task(void const * argument);
extern osThreadId lock_ctrl_task_hdl;


#define  LOCK_CTRL_TASK_LOCK_SIGNAL                    (1<<0)
#define  LOCK_CTRL_TASK_UNLOCK_SIGNAL                  (1<<1)
#define  LOCK_CTRL_TASK_DOOR_STATUS_CLOSE_SIGNAL       (1<<2)
#define  LOCK_CTRL_TASK_DOOR_STATUS_OPEN_SIGNAL        (1<<3)
#define  LOCK_CTRL_TASK_LOCK_STATUS_LOCK_SIGNAL        (1<<4)
#define  LOCK_CTRL_TASK_LOCK_STATUS_UNLOCK_SIGNAL      (1<<5)
#define  LOCK_CTRL_TASK_DEBUG_LOCK_SIGNAL              (1<<6)
#define  LOCK_CTRL_TASK_DEBUG_UNLOCK_SIGNAL            (1<<7) 
#define  LOCK_CTRL_TASK_ALL_SIGNALS                    ((1<<8)-1)

#define  LOCK_CTRL_TASK_WAIT_TIMEOUT                   osWaitForever
#define  LOCK_CTRL_TASK_LOCK_TIMEOUT                   800
#define  LOCK_CTRL_TASK_UNLOCK_TIMEOUT                 800
#define  LOCK_CTRL_TASK_AUTO_LOCK_TIMEOUT              (10*1000)/*10秒钟无人开门自动上锁*/

#define  LOCK_CTRL_TASK_LOCK_TYPE_AUTO                  1/*自动上锁*/
#define  LOCK_CTRL_TASK_LOCK_TYPE_MAN                   2/*手动上锁*/

#define  LOCK_CTRL_TASK_LOCK_EXCEPTION_NONE             3
#define  LOCK_CTRL_TASK_LOCK_EXCEPTION_HAPPEN           4

#define  LOCK_CTRL_TASK_UNLCOK_EXCEPTION_CNT_MAX        10

/*获取锁的异常状态*/
uint8_t lock_ctrl_task_get_lock_exception();

#endif