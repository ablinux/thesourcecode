#include "webServerHandler.h"
#include <nRF24L01.h>
#include "AirConnectionInterface.h"
#include <RF24.h>

extern char httmlpage[];

extern ESP8266WebServer server;
const byte ExpNode1[5] = {'N', 'O', 'D', 'E', '1'};
const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

extern RF24 radio;
extern int x,y;
extern Adafruit_SSD1306 display;
/* ESP8266WebServer Callbacks ********************************/
static int readRequest = 0;
void handleRoot()
{
  Serial.println("WebPage request..");
  server.send(200, "text/html", httmlpage);
  readRequest=1;
}

// char value[5] = "11";
void TVLEDState()
{
  Serial.println("TV LED CH Req");
  display.setCursor(x,y);
  display.print("TV");
  display.display();
  static char value[5] = "11";
  static int toggle = 0;
  if (readRequest == 0)
  {
    if (toggle % 2)
    {
      strcpy(value, "10");
    }
    else
    {
      strcpy(value, "20");
    }
    toggle++;
    radio.stopListening();
    radio.openWritingPipe(slaveAddress);
    radio.write(&value, 3);
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
  display.print("LED");
  static char value[5] = "11";
  static int toggle = 0;
  if (readRequest == 0)
  {
    if (toggle % 2)
    {
      strcpy(value, "50");
    }
    else
    {
      strcpy(value, "60");
    }
    toggle++;
    radio.stopListening();
    radio.openWritingPipe(slaveAddress);
    radio.write(&value, 3);
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

void handleTemp()
{
  int reading = analogRead(PIN_A0);
  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 3.3 / 1024;
  float temperatureC = (voltage - 0.5) * 100;
  String temperatureC_string = String(temperatureC);
  server.send(200, "text/plane", temperatureC_string);
}
/**************************************************************/
