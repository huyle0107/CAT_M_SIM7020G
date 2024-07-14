#include "Config.h"
#include <M5Atom.h>
#include "Timer_Interrupt.h"

extern bool isListen;
extern bool getResponse;


void sendATCommand(const char* command);
String waitAndReadResponse();
void CATM_Init();
void CATM_CheckConnection();
void CATM_ConnectMQTT();
void CATM_SubTopic(const String& topic);
void CATM_publishData(const String& topic,const String& str);
void CATM_ListenCallback();
void CATM_clearBuffer(void);