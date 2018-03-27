#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "app_common.h"
#include "ABDK_AHG081_ZK.h"
#include "fan_ctrl_task.h"

#define APP_LOG_MODULE_NAME   "[fan_ctrl]"
#define APP_LOG_MODULE_LEVEL   APP_LOG_LEVEL_DEBUG    
#include "app_log.h"
#include "app_error.h"

osThreadId fan_ctrl_task_hdl;

/*风扇相关的ups和门状态*/
app_bool_t is_fan_turn_on_enable_ups=APP_TRUE;
app_bool_t is_fan_turn_on_enable_door=APP_TRUE;

static void fan_ctrl_task_fan_turn_on()
{
 if(is_fan_turn_on_enable_ups==APP_TRUE && is_fan_turn_on_enable_door==APP_TRUE)
 {
  APP_LOG_DEBUG("打开直流风扇.\r\n");
  BSP_FAN_TURN_ON_OFF(FAN_CTL_ON);
 }
 else
 {
  APP_LOG_WARNING("UPS断开市电或者门是打开状态.无法打开直流风扇.\r\n");  
 }
}
static void fan_ctrl_task_fan_turn_off()
{
 APP_LOG_DEBUG("关闭直流风扇.\r\n");
 BSP_FAN_TURN_ON_OFF(FAN_CTL_OFF);
}

static void fan_ctrl_task_debug_fan_turn_on()
{
 APP_LOG_DEBUG("调试打开直流风扇.\r\n");
 BSP_FAN_TURN_ON_OFF(FAN_CTL_ON); 
}
static void fan_ctrl_task_debug_fan_turn_off()
{
 APP_LOG_DEBUG("调试关闭直流风扇.\r\n");
 BSP_FAN_TURN_ON_OFF(FAN_CTL_OFF); 
}



void fan_ctrl_task(void const * argument)
{
 osEvent signal;
 APP_LOG_INFO("@直流风扇任务开始.\r\n");

 while(1)
 {
 signal=osSignalWait(FAN_CTRL_TASK_ALL_SIGNALS,FAN_CTRL_TASK_INTERVAL);
 if(signal.status==osEventSignal)
 {
 if(signal.value.signals & FAN_CTRL_TASK_FAN_TURN_ON_SIGNAL)
 { 
  fan_ctrl_task_fan_turn_on();
 }
 if(signal.value.signals & FAN_CTRL_TASK_FAN_TURN_OFF_SIGNAL)
 {
  fan_ctrl_task_fan_turn_off();
 }
 if(signal.value.signals & FAN_CTRL_TASK_DEBUG_FAN_TURN_ON_SIGNAL)
 {
  fan_ctrl_task_debug_fan_turn_on();
 }
 if(signal.value.signals & FAN_CTRL_TASK_DEBUG_FAN_TURN_OFF_SIGNAL)
 {
  fan_ctrl_task_debug_fan_turn_off();
 }
 /*根据UPS状态设置风扇可控使能*/
 if(signal.value.signals & FAN_CTRL_TASK_UPS_PWR_ON_SIGNAL)
 {
  APP_LOG_DEBUG("直流风扇任务收到UPS变化.\r\n");
  is_fan_turn_on_enable_ups=APP_TRUE;
  fan_ctrl_task_fan_turn_on(); 
 }
 if(signal.value.signals & FAN_CTRL_TASK_UPS_PWR_OFF_SIGNAL)
 {
  APP_LOG_DEBUG("直流风扇任务收到UPS变化.\r\n");
  is_fan_turn_on_enable_ups=APP_FALSE;
  fan_ctrl_task_fan_turn_off();
 }
 
 /*根据门状态设置风扇状态*/
 if(signal.value.signals & FAN_CTRL_TASK_DOOR_STATUS_OPEN_SIGNAL)
 {
  APP_LOG_DEBUG("直流风扇任务收到门变化.\r\n");
  is_fan_turn_on_enable_door=APP_FALSE;
  fan_ctrl_task_fan_turn_off(); 
 }
 if(signal.value.signals & FAN_CTRL_TASK_DOOR_STATUS_CLOSE_SIGNAL)
 {
  APP_LOG_DEBUG("直流风扇任务收到门变化.\r\n");
  is_fan_turn_on_enable_door=APP_TRUE;
  fan_ctrl_task_fan_turn_on();
 }
 
 }
 } 
}