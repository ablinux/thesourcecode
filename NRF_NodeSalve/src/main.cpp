#include <Arduino.h>
#include <HCSR04.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Ultrasonic.h>
#include <nRF24L01.h>
#include <RF24.h>

/*A NRF Setting */
const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

#define CE_PIN 9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);
char dataReceived[10]; // this must match dataToSend in the TX
/************************************/

/*B Screen Setting OLED */
#define OLED_RESET 4     // Reset pin # (or -1 if sharing Arduino reset
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int i = 0;
unsigned long time;

void setup()
{
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  /* Serial setting */
  Serial.begin(9600);

  /* NRF Setting */
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
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
}

int val;

void loop()
{
  if (Serial.available())
  {
    val = Serial.parseInt();
  }
  else if (radio.available())
  {
    radio.read(&dataReceived, sizeof(dataReceived));
    val = atoi(dataReceived);
    Serial.print("NRF message"); Serial.println(atoi(dataReceived));
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("NodeShark V1");
  display.print("Message:"); display.print(val);
  display.display();
  if(10 == val)
  {
    digitalWrite(8,HIGH);
  }
  else if(20 == val)
  {
    digitalWrite(8,LOW);
  }
  else if (50 == val)
  {
    digitalWrite(7,HIGH);
  }
  else if (60 == val)
  {
    digitalWrite(7,LOW);
  }
}