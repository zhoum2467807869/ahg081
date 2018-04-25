#ifndef  __LOCK_DOOR_CACHE_TASK_H__
#define  __LOCK_DOOR_CACHE_TASK_H__

/*锁和门状态显示缓存任务*/
extern osThreadId ld_cache_task_hdl;
extern void ld_cache_task(void const * argument);

/*锁和门状态显示缓存*/
extern dis_num_t ld_dis_buff[];


#define  LOCK_DOOR_CACHE_TASK_INTERVAL                          50

#define  LOCK_DOOR_CACHE_TASK_LOCK_STATUS_LOCK_NUM              1
#define  LOCK_DOOR_CACHE_TASK_LOCK_STATUS_UNLOCK_NUM            0
#define  LOCK_DOOR_CACHE_TASK_LOCK_STATUS_INIT_NUM              DIGIT_LED_NEGATIVE_NUM


#define  LOCK_DOOR_CACHE_TASK_DOOR_STATUS_OPEN_NUM              0
#define  LOCK_DOOR_CACHE_TASK_DOOR_STATUS_CLOSE_NUM             1
#define  LOCK_DOOR_CACHE_TASK_DOOR_STATUS_INIT_NUM              DIGIT_LED_NEGATIVE_NUM




#endif