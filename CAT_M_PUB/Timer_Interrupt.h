#include "M5Atom.h"

#define timeRead                60000
#define timeWaitSensor          1000
#define timeWaitResponse        120000
#define timeWaitRelay           5000
#define timeClearBuffer         60000
#define timeExecuteResponse     180000
#define timeSleep               420000
#define WatchDogTime            120000

extern int timer_flag;
extern int timer1_flag;
void setWatchDogTimer();
void resetWatchDogTimer();
void setTimer(int duration);
void setTimer1(int duration);
void timerRun();
void IRAM_ATTR resetModule();
