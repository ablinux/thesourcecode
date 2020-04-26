#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <OneWire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DallasTemperature.h>
#include "AirConnectionInterface.h"
#include "airCommandHandler.h"
#include "serialization.h"
#include <Ticker.h>
#include "OTA.hpp"
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "yourSSID"
#define STAPSK  "password"
#endif

#define MOTOR_ON_TIME   5
#define MOTOR_OFF_TIME   80

const char* ssid     = STASSID;
const char* password = STAPSK;

Ticker WaterPump;
Ticker StopPump;
Ticker EverySecond;
void RunMoter();
void stopPump();
void secoundTask();
int Mode = 0;
#define PUMP_PIN D0

static int motorRunsIn= MOTOR_OFF_TIME;
static int MoreRunCount = MOTOR_ON_TIME;

/*NRF Setting */
#define CE_PIN 0 //D3
#define CSN_PIN 2 //DD4

/*A NRF Setting */
const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'B'};
byte Myaddress[5]={'M', 'A', 'S', 'T', 'R'};
RF24 radio(CE_PIN, CSN_PIN);

/*B Screen Setting OLED */
#define OLED_RESET LED_BUILTIN     // Reset pin # (or -1 if sharing Arduino reset
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* Temp */
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS D8
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

uint8_t tempBuffer[20];

airMail_t airMail_a;
extern airCommandHandler_t handlers; 

void setup() 
{
  pinMode(PUMP_PIN,OUTPUT);
  digitalWrite(PUMP_PIN,HIGH);
  Serial.begin(115200);
  /* NRF setup */
  radio.begin();  radio.setDataRate( RF24_250KBPS ); radio.setPALevel(RF24_PA_HIGH); radio.setRetries(1, 15);

  radio.openReadingPipe(1,thisSlaveAddress);
  radio.startListening();

  /* YourSSID */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* Display Setting */
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Start up the library
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
  display.clearDisplay();
  OtaSetup();
  WaterPump.attach(MOTOR_OFF_TIME,RunMoter);
}
uint8_t dataReceived[32]; // this must match dataToSend in the TX
airMail_t airMail;
void loop() 
{
   ArduinoOTA.handle();
  if (radio.available())
  {
    readNotify(true);
    radio.read(&dataReceived, sizeof(dataReceived));
    radio.flush_rx();
    de_serialize_airmail(&airMail,dataReceived);
    airNotificationHandler(&airMail);
  }
  display.clearDisplay();
}

void RunMoter()
{
  digitalWrite(PUMP_PIN,LOW);
  StopPump.attach(MOTOR_ON_TIME,stopPump);
}

void stopPump()
{
  digitalWrite(PUMP_PIN,HIGH);
  StopPump.detach();
}
void secoundTask()
{
  display.setCursor(0,0); display.setTextColor(WHITE,BLACK); display.printf("Motor On in:%d",motorRunsIn); display.display();
  motorRunsIn--;
  if(motorRunsIn == 0)
  {
    motorRunsIn = MOTOR_OFF_TIME;
  }

}