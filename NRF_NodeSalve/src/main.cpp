#include <Arduino.h>
#include <HCSR04.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Ultrasonic.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <airCommandHandler.h>
#include <serialization.h>

/* Private Function */
void sendDeviceConnectStatus();
void SendTempData();

/*A NRF Setting */
const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

/* Master address */
const byte MasterAddress[5] = {'M', 'A', 'S', 'T', 'R'};

#define CE_PIN 9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);
uint8_t dataReceived[32]; // this must match dataToSend in the TX
/************************************/

/* LM61 Temp sensor settings */
#define LM61_ANALOG_PIN A0
float tempINc = 0;

/*B Screen Setting OLED */
#define OLED_RESET 4     // Reset pin # (or -1 if sharing Arduino reset
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* Global var */
int val;
airMail_t airMail;
int32_t tempReadCount = 99999;
airMail_t airMail_a;
int i = 0;
unsigned long time;
extern airCommandHandler_t handlers;
uint8_t isDispOn = 1;

void setup()
{
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  Serial.begin(115200);

  /* NRF Setting */
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN); radio.setRetries(5, 15);
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();

  /* Display Setting */
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    // for (;;)
      isDispOn = 0; // Don't proceed, loop forever
  }

  if(isDispOn)
  {
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0, 0);
    display.println("Build Information :");
    display.println(__DATE__);
    display.println("Time");
    display.println(__TIME__);
    display.display();
  }
  
  RegisterHandlers(&handlers);

  if(isDispOn)
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("NodeShark V1");
    display.setCursor(0,14);display.print("FISH TANK :");
    display.setCursor(0,22);display.print("TV :");
    display.setCursor(0,30);display.print("Room Temp: ");
  }
  
}

void loop()
{
  if (Serial.available())
  {
    val = Serial.parseInt();
  }
  else if (radio.available())
  {
    readNotify(true);
    radio.read(&dataReceived, sizeof(dataReceived));
    radio.flush_rx();
    de_serialize_airmail(&airMail,dataReceived);
    airNotificationHandler(&airMail);
  }

  //https://forum.arduino.cc/index.php?topic=474630.0
  int analogVal;
  tempReadCount--;
  if(tempReadCount == 0)
  {
    analogVal = analogRead(LM61_ANALOG_PIN);
    tempINc =  ((analogVal*500.0)/1024)-(60+16.55);
    Serial.print("Room Temp");Serial.println(tempINc);
    if(isDispOn)
    {
      display.setCursor(0,30);display.print("Room Temp: ");display.setCursor(display.getCursorX(),29);display.setTextColor(WHITE,BLACK);display.print(tempINc); 
      display.display();
    }
    tempReadCount = 10000;
    sendDeviceConnectStatus();
  }
  if(tempReadCount == 5000)
    SendTempData();
}

void sendDeviceConnectStatus()
{
  uint8_t tempBuffer[15];
  radio.stopListening();
  radio.openWritingPipe(MasterAddress);
  /* prepair data here */
  airMail_a.mailHeader.pktType = PKT_ACK;
  airMail_a.mailHeader.dataLength = sizeof(thisSlaveAddress);
  memcpy(airMail_a.data,thisSlaveAddress,sizeof(thisSlaveAddress));
  serialize_airmail(tempBuffer,&airMail_a,sizeof(thisSlaveAddress));
  radio.write(tempBuffer,sizeof(thisSlaveAddress)+3+1);
  radio.startListening();
}
void SendTempData()
{
  uint8_t tempBuffer[15];
  String StrTemp = String(tempINc);
  radio.stopListening();
  radio.openWritingPipe(MasterAddress);
  /* prepair data here */
  // sprintf((char*)TempData,"%f", tempINc);
  Serial.println(StrTemp);
  airMail_a.mailHeader.pktType = PKT_DEVICE_SENSOR;
  airMail_a.mailHeader.dataLength = StrTemp.length();
  memcpy(airMail_a.data,StrTemp.c_str(),airMail_a.mailHeader.dataLength+1);
  serialize_airmail(tempBuffer,&airMail_a,airMail_a.mailHeader.dataLength+1);
  radio.write(tempBuffer,airMail_a.mailHeader.dataLength+3+1);
  radio.startListening();
}