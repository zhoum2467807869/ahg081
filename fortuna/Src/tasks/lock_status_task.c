#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "app_common.h"
#include "ABDK_AHG081_ZK.h"
#include "lock_status_task.h"
#include "lock_ctrl_task.h"
#define APP_LOG_MODULE_NAME   "[lock_status]"
#define APP_LOG_MODULE_LEVEL   APP_LOG_LEVEL_DEBUG    
#include "app_log.h"
#include "app_error.h"

osThreadId lock_status_task_hdl;

static uint8_t lock_status=LOCK_STATUS_TASK_LOCK_STATUS_INIT;

uint8_t lock_status_task_get_lock_status()
{
 return lock_status;
}

void lock_status_task(void const * argument)
{
 bsp_status_t status;
 APP_LOG_INFO("@锁状态任务开始.\r\n");
 
 while(1)
 {
  osDelay(LOCK_STATUS_TASK_INTERVAL);
  status=bsp_get_lock_status(); 
  if(status==LOCK_STATUS_UNLOCK)
  {
    /*锁从关闭状态变化成开启状态*/
    if(lock_status!=LOCK_STATUS_TASK_LOCK_STATUS_UNLOCK)
    {
      lock_status=LOCK_STATUS_TASK_LOCK_STATUS_UNLOCK;
      /*向锁任务发送锁开启信号*/
      osSignalSet(lock_ctrl_task_hdl,LOCK_CTRL_TASK_LOCK_STATUS_UNLOCK_SIGNAL);
    }
  }
  if(status==LOCK_STATUS_LOCK)
  {
     /*锁从开启状态变化成关闭状态*/
    if(lock_status!=LOCK_STATUS_TASK_LOCK_STATUS_LOCK)
    {
      lock_status=LOCK_STATUS_TASK_LOCK_STATUS_LOCK;
      /*向锁任务发送门关闭信号*/
      osSignalSet(lock_ctrl_task_hdl,LOCK_CTRL_TASK_LOCK_STATUS_LOCK_SIGNAL);
    } 
  } 
 }  
}