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

/*A NRF Setting */
const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

#define CE_PIN 9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);
uint8_t dataReceived[32]; // this must match dataToSend in the TX
/************************************/

/*B Screen Setting OLED */
#define OLED_RESET 4     // Reset pin # (or -1 if sharing Arduino reset
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int i = 0;
unsigned long time;
extern airCommandHandler_t handlers;

void setup()
{
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  /* Serial setting */
  Serial.begin(9600);

  /* NRF Setting */
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH); radio.setRetries(5, 10);
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();

  /* Display Setting */
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);
  display.println("Build Information :");
  display.println(__DATE__);
  display.println("Time");
  display.println(__TIME__);
  display.display();
  delay(4000);
  RegisterHandlers(&handlers);
}

int val;
airMail_t airMail;
void loop()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("NodeShark V1");

  if (Serial.available())
  {
    val = Serial.parseInt();
  }
  else if (radio.available())
  {
    readNotify(true);
    radio.read(&dataReceived, sizeof(dataReceived));
    de_serialize_airmail(&airMail,dataReceived);
    airNotificationHandler(&airMail);
  }
}