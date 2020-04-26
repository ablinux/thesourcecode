#include "webServerHandler.h"
#include <nRF24L01.h>
#include "AirConnectionInterface.h"
#include "serialization.h"
#include <RF24.h>
#include <remoreNode1.h>

const byte ExpNode1[5] = {'N', 'O', 'D', 'E', '1'};
const byte slaveAddress_A[5] = {'R', 'x', 'A', 'A', 'A'};
const byte slaveAddress_B[5] = {'R', 'x', 'A', 'A', 'B'};
char NodeAddress[7];

char TempInC[5];
extern RF24 radio;
extern int x,y;

/* ESP8266WebServer Callbacks ********************************/
static int readRequest = 0;

// char value[5] = "11";
airMail_t airMail_a;
uint8_t buffer[10];
void TVLEDState()
{
  Serial.println("TV LED CH Req");
  display.setCursor(x,y);
  display.setTextColor(WHITE,BLACK);display.print("TV");
  display.display();
  static char value[5] = "11";
  static int toggle = 0;
  DeviceOne_t dev1;
  if (readRequest == 0)
  {
    if (toggle % 2)
    {
      strcpy(value, "10");
      dev1.dataByte = 0;
    }
    else
    {
      strcpy(value, "20");
      dev1.dataByte = 1;
    }
    toggle++;
    radio.stopListening();
    radio.openWritingPipe(slaveAddress_A);
    // radio.write(&value, 3);
    airMail_a.airCommand = COMMAND_SET;
    airMail_a.mailHeader.pktType = PKT_DEVICE_ACTUATOR;
    airMail_a.mailHeader.dataLength = 2;
    dev1.deviceID = FISHTANK;

    memcpy(airMail_a.data,&dev1,sizeof(dev1));
    serialize_airmail(buffer,&airMail_a,3);
    radio.write(buffer, 10);
    radio.startListening();
  }
  else
  {
    readRequest ++; if(readRequest == 3 ){readRequest = 0;}
  }
  server.send(200, "text/plain", value);
}
void TOPLEDState()
{
  Serial.println("Top LED CH Req");
  display.setCursor(x,y);
  display.setTextColor(WHITE,BLACK);display.print("LED");
  static char value[5] = "11";
  static int toggle = 0;
   DeviceOne_t dev1;
  if (readRequest == 0)
  {
    if (toggle % 2)
    {
      strcpy(value, "50");
      dev1.dataByte = 0;
    }
    else
    {
      strcpy(value, "60");
      dev1.dataByte = 1;
    }
    toggle++;
    radio.stopListening();
    radio.openWritingPipe(slaveAddress_A);
    airMail_a.airCommand = COMMAND_SET;
    airMail_a.mailHeader.pktType = PKT_DEVICE_ACTUATOR;
    airMail_a.mailHeader.dataLength = 2;
    dev1.deviceID = TV;

    memcpy(airMail_a.data,&dev1,sizeof(dev1));
    serialize_airmail(buffer,&airMail_a,3);
    radio.write(buffer, 10);
    // radio.write(&value, 3);
    radio.startListening();
  }
  else
  {
    readRequest ++; if(readRequest == 3 ){readRequest = 0;}
  }
  server.send(200, "text/plain", value);
}
void BEDLEDState()
{
  static char value[5] = {0x1,0x2,0x3,0x4,0x5};
  if(readRequest == 0)
  {
    server.send(200, "text/plain", "UNDER CONSTRUCTION!!");
    radio.stopListening();
    radio.openWritingPipe(ExpNode1);
    radio.write(&value,5);
    radio.startListening();
  }
  else
  {
    // refresh ++; if(refresh == 4 ){refresh = 0;}
  }
}

void handleSlider()
{
  DeviceOne_t dev1;
  Serial.print("Slider Request:");
  String sliderValue = server.arg("SliderVal");
  Serial.println(sliderValue);
  radio.stopListening();
  radio.openWritingPipe(slaveAddress_A);
  airMail_a.airCommand = COMMAND_SET;
  airMail_a.mailHeader.pktType = PKT_DEVICE_ACTUATOR;
  airMail_a.mailHeader.dataLength = 2;
  dev1.deviceID = TV;
  dev1.dataByte = sliderValue.toInt();
  memcpy(airMail_a.data,&dev1,sizeof(dev1));
  serialize_airmail(buffer,&airMail_a,4);
  radio.write(buffer, 10);
  radio.startListening();
}

#if TEMP
#include <DallasTemperature.h>
float Celcius;
extern DallasTemperature sensors;
#endif

void handleTemp()
{
  #if TEMP
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  String Temp = String(Celcius);
  server.send(200, "text/plane", Temp);
  #else if
  server.send(200, "text/plane", TempInC);
  #endif
}

void handleTime()
{
  char time[10];
  sprintf(time,"%d:%d:%d",timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds());
  server.send(200,"text/plane",time);
}

void sendNodeStatus()
{
  char tempBuff[50];
  sprintf(tempBuff,NODE_DATA,NodeAddress,"ONLINE");
  server.send(200,"text/plane",tempBuff);
}
/**************************************************************/
