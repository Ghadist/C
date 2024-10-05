#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_conf.h"
#include "system_stm32f4xx.h"
#include "misc.h"
#include "stm32f4xx_exti.h"


RTC_InitTypeDef rtc;
RTC_TimeTypeDef time;
RTC_TimeTypeDef alarmTime;
RTC_AlarmTypeDef alarm;
EXTI_InitTypeDef exti;
NVIC_InitTypeDef NVIC_InitStructure;
int i;

void initRTC()
{
		/*Vkliuchaem taktirovanie (PWR - Power Control)*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		/*Razreshaem dostup k rezervnim dannim*/
    PWR_BackupAccessCmd(ENABLE);
		/*Sbrasivaem RTC-modul'*/
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
		/*Vkiuchaem nizkochastotnii generator i vibiraem ego v kachestve istochniks taktirovania*/
    RCC_LSICmd(ENABLE);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);
		/*Ukazivaem chasovoi format i chastotu*/
    RTC_StructInit(&rtc);
    rtc.RTC_HourFormat = RTC_HourFormat_24;
    rtc.RTC_SynchPrediv = 0x7FFF;
    RTC_Init(&rtc);
}

/*Nastroika obrabotchika prerivania*/
void RTC_Alarm_IRQHandler()
{
    if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_ALRA);
        EXTI_ClearITPendingBit(EXTI_Line17);
    }
}

int main()
{
		__enable_irq();
    initRTC();
		time.RTC_H12 = RTC_HourFormat_24;
		time.RTC_Hours = 20;
		time.RTC_Minutes = 00;
		time.RTC_Seconds = 00;
		RTC_SetTime(RTC_Format_BIN, &time);
		EXTI_ClearITPendingBit(EXTI_Line17);
		exti.EXTI_Line = EXTI_Line17;
		exti.EXTI_Mode = EXTI_Mode_Interrupt;
		exti.EXTI_Trigger = EXTI_Trigger_Rising;
		exti.EXTI_LineCmd = ENABLE;
		EXTI_Init(&exti);
	
		NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		/*Ustanavlivaem vremia srabativania budilnika*/
		RTC_AlarmStructInit(&alarm);
		alarmTime.RTC_H12 = RTC_HourFormat_24;
		alarmTime.RTC_Hours = 20;
		alarmTime.RTC_Minutes = 00;
		alarmTime.RTC_Seconds = 15;
		alarm.RTC_AlarmTime = alarmTime;
		alarm.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
		RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &alarm);
		RTC_OutputConfig(RTC_Output_AlarmA, RTC_OutputPolarity_High);
		RTC_ITConfig(RTC_IT_ALRA, ENABLE);
		RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
		RTC_ClearFlag(RTC_FLAG_ALRAF);
		
    while(1)
		{
			
		}
}

