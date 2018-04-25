#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "app_common.h"
#include "digit_led.h"
#include "display_task.h"
#include "switch_task.h"
#include "lock_status_task.h"
#include "door_status_task.h"
#include "lock_door_cache_task.h"
#define APP_LOG_MODULE_NAME   "[ld_cache]"
#define APP_LOG_MODULE_LEVEL   APP_LOG_LEVEL_DEBUG    
#include "app_log.h"
#include "app_error.h"

/*锁和门状态显示缓存*/
 dis_num_t ld_dis_buff[DIGIT_LED_POS_CNT];

/*锁和门状态显示缓存任务*/
osThreadId ld_cache_task_hdl;

void ld_cache_task(void const * argument)
{
 APP_LOG_INFO("@锁和门显示缓存任务开始.\r\n");
 ld_dis_buff[0].dp=APP_FALSE;
 ld_dis_buff[1].dp=APP_FALSE;
 while(1)
 {
 osDelay(LOCK_DOOR_CACHE_TASK_INTERVAL);
 
 if(lock_status_task_get_lock_status()==LOCK_STATUS_TASK_LOCK_STATUS_LOCK)
 {
 ld_dis_buff[1].num=LOCK_DOOR_CACHE_TASK_LOCK_STATUS_LOCK_NUM;  
 }
 else if(lock_status_task_get_lock_status()==LOCK_STATUS_TASK_LOCK_STATUS_UNLOCK)
 {
 ld_dis_buff[1].num=LOCK_DOOR_CACHE_TASK_LOCK_STATUS_UNLOCK_NUM;  
 }
 else
 {
 ld_dis_buff[1].num=LOCK_DOOR_CACHE_TASK_LOCK_STATUS_INIT_NUM;  
 }
 if(door_task_get_door_status()==DOOR_TASK_DOOR_STATUS_OPEN)
 {
 ld_dis_buff[0].num=LOCK_DOOR_CACHE_TASK_DOOR_STATUS_OPEN_NUM;  
 }
 else if(door_task_get_door_status()==DOOR_TASK_DOOR_STATUS_CLOSE)
 {
 ld_dis_buff[0].num=LOCK_DOOR_CACHE_TASK_DOOR_STATUS_CLOSE_NUM;  
 }
 else
 {
 ld_dis_buff[0].num=LOCK_DOOR_CACHE_TASK_DOOR_STATUS_INIT_NUM;  
 }
 }
}