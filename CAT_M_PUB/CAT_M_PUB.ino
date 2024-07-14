#include <M5Atom.h>
#include "Timer_Interrupt.h"
#include "Config.h"
#include "CAT_M.h"

hw_timer_t* timer = NULL; //khơi tạo timer
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


// hàm xử lý ngắt
void IRAM_ATTR onTimer() 
{   
  portENTER_CRITICAL_ISR(&timerMux); //vào chế độ tránh xung đột
  timerRun();
  portEXIT_CRITICAL_ISR(&timerMux); // thoát 
}

void setup() 
{
  // put your setup code here, to run once:
  M5.begin(true, false, true);
  delay(50);
  M5.dis.fillpix(0x00ff00);

  SerialMon.begin(Monitor_baudrate, SERIAL_8N1, Monitor_RX, Monitor_TX);
  SerialCATM.begin(CATM_baudrate, SERIAL_8N1, CATM_RX, CATM_TX);
  Serial485.begin(RS485_baudrate, SERIAL_8N1, RS485_RX, RS485_TX);
  delay(1000);

  timer = timerBegin(0, 80, true);
  //khởi tạo hàm xử lý ngắt ngắt cho Timer
  timerAttachInterrupt(timer, &onTimer, true);
  //khởi tạo thời gian ngắt cho timer là 1ms (1000 us)
  timerAlarmWrite(timer, 1000, true);
  //bắt đầu chạy timer  
  timerAlarmEnable(timer);


  CATM_Init();
  CATM_CheckConnection();
  CATM_ConnectMQTT();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  CATM_publishData("/innovation/airmonitoring/WSNs", "Test_PUB");
  delay(30000);
}
