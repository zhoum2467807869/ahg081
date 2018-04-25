#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "app_common.h"
#include "ABDK_AHG081_ZK.h"
#include "lock_ctrl_task.h"
#include "ups_status_task.h"
#include "light_ctrl_task.h"
#include "glass_pwr_task.h"
#include "fan_ctrl_task.h"
#define APP_LOG_MODULE_NAME   "[lock_ctrl]"
#define APP_LOG_MODULE_LEVEL   APP_LOG_LEVEL_DEBUG    
#include "app_log.h"
#include "app_error.h"

/*
 * 主要是测试开锁按键的功能，包括自动关锁、关门关锁、开门开锁等 
*/



/*任务句柄*/
osThreadId lock_ctrl_task_hdl;

osTimerId lock_timer_id;
osTimerId unlock_timer_id;
osTimerId auto_lock_timer_id;

static void lock_timer_init();
static void lock_timer_start();
static void lock_timer_stop();
static void lock_timer_expired(void const * argument);

static void auto_lock_timer_init();
static void auto_lock_timer_start();
static void auto_lock_timer_stop();
static void auto_lock_timer_expired(void const * argument);

static void unlock_timer_init();
static void unlock_timer_start();
static void unlock_timer_stop();
static void unlock_timer_expired(void const * argument);

static void lock_ctrl_task_turn_on_lock_led();
static void lock_ctrl_task_turn_on_unlock_led();
/*电磁锁吸住门*/
static void lock_ctrl_task_lock_door();
/*电磁锁释放门*/
static void lock_ctrl_task_unlock_door();

static void lock_ctrl_task_lock_lock();
static void lock_ctrl_task_unlock_lock();

/*调试调用*/
static void lock_ctrl_task_debug_lock_lock();
static void lock_ctrl_task_debug_unlock_lock();



static uint8_t lock_type=LOCK_CTRL_TASK_LOCK_TYPE_AUTO;
static uint8_t lock_exception=LOCK_CTRL_TASK_LOCK_EXCEPTION_NONE;
static uint8_t unlock_exception_cnt;

static uint8_t is_enable_lock_lock=APP_TRUE;

/*获取锁的异常状态*/
uint8_t lock_ctrl_task_get_lock_exception()
{
 return lock_exception;
}

static void lock_timer_init()
{
 osTimerDef(lock_timer,lock_timer_expired);
 lock_timer_id=osTimerCreate(osTimer(lock_timer),osTimerOnce,0);
 APP_ASSERT(lock_timer_id);
}

static void lock_timer_start()
{
 APP_LOG_DEBUG("关锁定时器开始.\r\n");
 osTimerStart(lock_timer_id,LOCK_CTRL_TASK_LOCK_TIMEOUT);
}
static void lock_timer_stop()
{
 APP_LOG_DEBUG("关锁定时器停止.\r\n");
 osTimerStop(lock_timer_id);
}




static void lock_timer_expired(void const * argument)
{
 APP_LOG_DEBUG("关锁定时器到达.\r\n");
 APP_LOG_DEBUG("关锁失败.\r\n");
}

static void auto_lock_timer_init()
{
 osTimerDef(auto_lock_timer,auto_lock_timer_expired);
 auto_lock_timer_id=osTimerCreate(osTimer(auto_lock_timer),osTimerOnce,0);
 APP_ASSERT(auto_lock_timer_id);
}

static void auto_lock_timer_start()
{
 APP_LOG_DEBUG("自动关锁定时器开始.\r\n");
 osTimerStart(auto_lock_timer_id,LOCK_CTRL_TASK_AUTO_LOCK_TIMEOUT);
}
static void auto_lock_timer_stop()
{
 APP_LOG_DEBUG("自动关锁定时器停止.\r\n");
 osTimerStop(auto_lock_timer_id);
}

static void auto_lock_timer_expired(void const * argument)
{
 APP_LOG_DEBUG("自动关锁定时器到达.\r\n");
 lock_type=LOCK_CTRL_TASK_LOCK_TYPE_AUTO;
 /*立刻上锁*/
 APP_LOG_DEBUG("准备自动关锁.lock_type:%d.\r\n",lock_type);
 osSignalSet(lock_ctrl_task_hdl,LOCK_CTRL_TASK_LOCK_SIGNAL);
}

static void unlock_timer_init()
{
 osTimerDef(unlock_timer,unlock_timer_expired);
 unlock_timer_id=osTimerCreate(osTimer(unlock_timer),osTimerOnce,0);
 APP_ASSERT(unlock_timer_id);
}
static void unlock_timer_start()
{
 APP_LOG_DEBUG("开锁定时器开始.\r\n");
 osTimerStart(unlock_timer_id,LOCK_CTRL_TASK_LOCK_TIMEOUT);
}
static void unlock_timer_stop()
{
 APP_LOG_DEBUG("开锁定时器停止.\r\n");
 osTimerStop(unlock_timer_id);
}
 
static void unlock_timer_expired(void const * argument)
{
 APP_LOG_DEBUG("开锁定时器到达.\r\n");
 unlock_exception_cnt++;
 APP_LOG_ERROR("开锁失败.lock_exception_cnt:%d\r\n",unlock_exception_cnt);
 
 /*开锁失败次数超限*/
 if(unlock_exception_cnt > LOCK_CTRL_TASK_UNLCOK_EXCEPTION_CNT_MAX)
 {
 APP_LOG_DEBUG("开锁次数超限失败.unlock_exception_cnt=%d.\r\n",unlock_exception_cnt); 
 unlock_exception_cnt=0;
 /*置为锁异常状态*/
 lock_exception=LOCK_CTRL_TASK_LOCK_EXCEPTION_HAPPEN;
 return;
 }
 
 /*再次等待开锁*/
 unlock_timer_start();
}

/*电磁锁吸住门*/
static void lock_ctrl_task_lock_door()
{
 BSP_DOOR_CTL(DOOR_CTL_LOCK);
}
/*电磁锁释放门*/
static void lock_ctrl_task_unlock_door()
{
 BSP_DOOR_CTL(DOOR_CTL_UNLOCK); 
}

static void lock_ctrl_task_lock_lock()
{
 BSP_LOCK_CTL(LOCK_CTL_LOCK); 
}
static void lock_ctrl_task_unlock_lock()
{
 BSP_LOCK_CTL(LOCK_CTL_UNLOCK); 
}
/*关锁成功时 打开绿色led*/
static void lock_ctrl_task_turn_on_lock_led()
{
 APP_LOG_DEBUG("打开绿色门灯.\r\n");
 BSP_LED_TURN_ON_OFF(DOOR_GREEN_LED,LED_CTL_ON); 
 BSP_LED_TURN_ON_OFF(DOOR_ORANGE_LED,LED_CTL_OFF);
}
/*开锁成功时 打开绿色led*/
static void lock_ctrl_task_turn_on_unlock_led()
{
 APP_LOG_DEBUG("打开橙色门灯.\r\n");
 BSP_LED_TURN_ON_OFF(DOOR_ORANGE_LED,LED_CTL_ON); 
 BSP_LED_TURN_ON_OFF(DOOR_GREEN_LED,LED_CTL_OFF); 
}

/*调试调用*/
static void lock_ctrl_task_debug_lock_lock()
{
  BSP_LOCK_CTL(LOCK_CTL_LOCK);  
}
/*调试调用*/
static void lock_ctrl_task_debug_unlock_lock()
{
  BSP_LOCK_CTL(LOCK_CTL_UNLOCK);  
}

/*锁任务*/
void lock_ctrl_task(void const * argument)
{
  osEvent sig;
  APP_LOG_INFO("@锁任务开始.\r\n"); 
  lock_timer_init();
  auto_lock_timer_init();
  unlock_timer_init(); 
  
  while(1)
  {
  sig=osSignalWait(LOCK_CTRL_TASK_ALL_SIGNALS,LOCK_CTRL_TASK_WAIT_TIMEOUT);
  if(sig.status==osEventSignal)
  {
   /*收到关锁信号*/
   if(sig.value.signals & LOCK_CTRL_TASK_LOCK_SIGNAL)
   {
    APP_LOG_DEBUG("收到关锁信号.关锁.\r\n");
    /*首先判断是否可以上锁*/
    if(is_enable_lock_lock==APP_TRUE)
    {
    auto_lock_timer_stop();
    unlock_timer_stop();
    lock_timer_start();
    lock_ctrl_task_lock_lock();
    }
    else
    {
     APP_LOG_WARNING("锁状态关闭非法.因为门状态不匹配.\r\n"); 
    }
   }
   /*收到开锁信号*/
   if(sig.value.signals & LOCK_CTRL_TASK_UNLOCK_SIGNAL)
   {
    APP_LOG_DEBUG("收到开锁信号.开锁.\r\n");
    auto_lock_timer_stop();
    lock_timer_stop();
    unlock_timer_start();
    lock_ctrl_task_unlock_lock();
   }
   /*收到门的状态变为打开*/
   if(sig.value.signals & LOCK_CTRL_TASK_DOOR_STATUS_OPEN_SIGNAL)
   {
    APP_LOG_DEBUG("门状态变化->打开.\r\n");
    is_enable_lock_lock=APP_FALSE;
    /*收到门打开后 停止自动关锁定时器*/
    lock_type=LOCK_CTRL_TASK_LOCK_TYPE_MAN;/*只要有人打开门 手动关闭*/
    osSignalSet(lock_ctrl_task_hdl,LOCK_CTRL_TASK_UNLOCK_SIGNAL);
   }
   /*收到门的状态变为关闭*/
   if(sig.value.signals & LOCK_CTRL_TASK_DOOR_STATUS_CLOSE_SIGNAL)
   {
    APP_LOG_DEBUG("门状态变化->关闭.\r\n");
    is_enable_lock_lock=APP_TRUE;
    osSignalSet(lock_ctrl_task_hdl,LOCK_CTRL_TASK_LOCK_SIGNAL);/*马上尝试关锁*/
   }   
   if(sig.value.signals & LOCK_CTRL_TASK_LOCK_STATUS_LOCK_SIGNAL)
   {
    APP_LOG_DEBUG("锁状态变化->关锁.\r\n");

    /*取消锁异常状态*/
    lock_exception=LOCK_CTRL_TASK_LOCK_EXCEPTION_NONE;
    lock_timer_stop();
    /*显示对应的灯光*/
    lock_ctrl_task_turn_on_lock_led();
    lock_ctrl_task_lock_door();
   } 
   if(sig.value.signals & LOCK_CTRL_TASK_LOCK_STATUS_UNLOCK_SIGNAL)
   {
    APP_LOG_DEBUG("锁状态变化->开锁.\r\n");
    /*开锁异常次数清零*/
    unlock_exception_cnt=0;
    /*取消锁异常状态*/
    lock_exception=LOCK_CTRL_TASK_LOCK_EXCEPTION_NONE;
    lock_type=LOCK_CTRL_TASK_LOCK_TYPE_AUTO;
    unlock_timer_stop();
    /*显示对应的灯光*/
    lock_ctrl_task_turn_on_unlock_led();
    lock_ctrl_task_unlock_door();
  
    auto_lock_timer_start();
   } 
   /*收到调试开锁信号 打开*/
   if(sig.value.signals & LOCK_CTRL_TASK_DEBUG_UNLOCK_SIGNAL)
   {
    APP_LOG_DEBUG("调试开锁.\r\n");
    lock_ctrl_task_debug_unlock_lock(); 
   }
   /*收到调试关锁信号 关锁*/
   if(sig.value.signals & LOCK_CTRL_TASK_DEBUG_LOCK_SIGNAL)
   {
    APP_LOG_DEBUG("调试关锁.\r\n");
    lock_ctrl_task_debug_lock_lock();
   }     
  }
 }
}
 
 
 