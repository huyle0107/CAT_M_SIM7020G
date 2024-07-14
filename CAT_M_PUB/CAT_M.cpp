#include "CAT_M.h"

bool isListen = false;
bool getResponse = false;

void sendATCommand(const char* command) 
{
  setWatchDogTimer();
  SerialMon.println(command);

  SerialCATM.write(command);
  SerialCATM.write("\r"); 

  String result = waitAndReadResponse(); 
  if(result.indexOf("OK") != -1 && isListen)
  {
    getResponse = true;
  }
  if(!(result.indexOf("OK") != -1 || result.indexOf("ER") != -1))
  {
    result = waitAndReadResponse();
  }
  resetWatchDogTimer();
}

void CATM_ListenCallback()
{
  String receivedData = "";
  while (SerialCATM.available()) 
  {
    char c = SerialCATM.read();
    receivedData += c;
  }
  if(receivedData.indexOf("+CMQPUB: 0,") != -1) getResponse = true;
}

String waitAndReadResponse() 
{
  while (!SerialCATM.available()) {}
  //delay(2000);

  String receivedData = "";
  while (SerialCATM.available()) 
  {
    char c = SerialCATM.read();
    receivedData += c;
  }
  SerialMon.println(receivedData);
  return receivedData;
}


void CATM_Init()
{
  // Init CATM
  SerialMon.begin(Monitor_baudrate, SERIAL_8N1, Monitor_RX, Monitor_TX);
  SerialCATM.begin(CATM_baudrate, SERIAL_8N1, CATM_RX,CATM_TX);
  Serial485.begin(RS485_baudrate, SERIAL_8N1, RS485_RX, RS485_TX);
  sendATCommand("AT");
  sendATCommand("ATZ");
  sendATCommand("AT+CPIN?");
  sendATCommand("AT+CCID");
  sendATCommand("AT+COPS?");
  sendATCommand("AT+CEREG?");
  
  //Attach network
  sendATCommand("AT+CBAND=3");
  sendATCommand("AT+CGACT=1");
  sendATCommand("AT+CGDCONT=1,\"IP\",\"cat-m\"");
  sendATCommand("AT+CGPADDR=?");
  sendATCommand("AT+COPS=0");
}

void CATM_CheckConnection()
{
  sendATCommand("AT+CGREG=0");
  sendATCommand("AT+COPS?");
  sendATCommand("AT+CSQ");
  sendATCommand("AT+CENG?");
}

void CATM_ConnectMQTT()
{
  sendATCommand("AT+SMCONF=\"CLIENTID\",\"SoilAir\"");
  sendATCommand("AT+SMCONF=\"KEEPTIME\",120");
  sendATCommand("AT+SMCONF=\"URL\",\"mqttserver.tk\",\"1883\"");
  sendATCommand("AT+SMCONF=\"USERNAME\",\"innovation\"");
  sendATCommand("AT+SMCONF=\"PASSWORD\",\"Innovation_RgPQAZoA5N\"");
}

void CATM_SubTopic(const String& topic)
{
  String SubString = "AT+SMSUB=" + topic + "\",35,1";
  sendATCommand(SubString.c_str());
}

void CATM_publishData(const String& topic, const String& str)
{
  getResponse = false;
  String newStr = "";
  for (char c : str) 
  {
    newStr += (c == '"') ? '\'' : c;
  }
  //Create MQTT string
  String mqttString = "AT+SMPUB=\"" + topic + "\"," + String(newStr.length()) + ",1,1";
 
  sendATCommand(mqttString.c_str());

  String mqttPub = "\"" + newStr + "\"";

  sendATCommand(mqttPub.c_str());

}

void CATM_clearBuffer(void)
{
  String receivedData = "";
  while (SerialCATM.available()) 
  {
    char c = SerialCATM.read();
    receivedData += c;
  }
  if(!receivedData.isEmpty()) SerialMon.println(receivedData);

}
