#include"Timer_Interrupt.h"

int timer_counter = 0;
int timer_flag = 0;

int timer1_counter = 0;
int timer1_flag = 0;

int timer_Watchdog = 0;

void IRAM_ATTR resetModule() 
{
  ets_printf("reboot\n");
  esp_restart();
}


void setWatchDogTimer()
{
  timer_Watchdog = WatchDogTime;
}

void resetWatchDogTimer()
{
  timer_Watchdog = -282;
}

void setTimer(int duration)
{
  timer_counter = duration;
  timer_flag = 0;
}

void setTimer1(int duration)
{
  timer1_counter = duration;
  timer1_flag = 0;
}




void timerRun()
{
  if(timer_counter > 0)
  {
    timer_counter--;
    if(timer_counter == 0)
    {
      timer_flag = 1;
    }
  }
  
  if(timer1_counter > 0)
  {
    timer1_counter--;
    if(timer1_counter == 0)
    {
      timer1_flag = 1;
    }
  }

  if(timer_Watchdog > 0)
  {
    timer_Watchdog--;
    if(timer_Watchdog == 0)
    {
      resetModule();
    }
  }
}

