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
  

void setup() 
{
  Serial.begin(115200);
  /* NRF setup */
  radio.begin();  radio.setDataRate( RF24_250KBPS ); radio.setPALevel(RF24_PA_HIGH); radio.setRetries(1, 15);

  radio.openReadingPipe(1,thisSlaveAddress);
  radio.startListening();

  /* Display Setting */
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  display.clearDisplay();
  // Start up the library
  sensors.begin();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  // put your setup code here, to run once:
}

void loop() 
{
  display.clearDisplay();
  sensors.requestTemperatures(); // Send the command to get temperatures

  display.setCursor(0, 0);
  display.printf(" RoomTemp \n\n %f",sensors.getTempCByIndex(0));
  // display.printf("RoomTemp=");
  Serial.println(sensors.getTempCByIndex(0));
  display.display();
  delay(10);
  String StrTemp = String(sensors.getTempCByIndex(0));
  radio.stopListening();
  radio.openWritingPipe(Myaddress);
  airMail_a.mailHeader.pktType = PKT_DEVICE_SENSOR;
  airMail_a.mailHeader.dataLength = StrTemp.length();
  memcpy(airMail_a.data,StrTemp.c_str(),airMail_a.mailHeader.dataLength+1);
  serialize_airmail(tempBuffer,&airMail_a,airMail_a.mailHeader.dataLength+1);
  radio.write(tempBuffer,airMail_a.mailHeader.dataLength+3+1);
  radio.startListening();
  // put your main code here, to run repeatedly:
}