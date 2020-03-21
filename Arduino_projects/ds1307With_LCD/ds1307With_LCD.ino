// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
RTC_DS1307 rtc;
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
bool blink_secound = false;
int _H, H_, _M, M_;
//char buff[10];
String str_val;
int hour_val = 0;
int minute_val = 0;
int day_val = 0;
int month_val = 0;
int year_val = 0;
void setup ()
{
  display.begin();
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.setContrast(50);
  display.clearDisplay();
  while (!Serial); // for Leonardo/Micro/Zero

  Serial.begin(9600);
  if (! rtc.begin())
  {
    display.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning())
  {
    display.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    rtc.adjust(DateTime(2018, 1, 17, 8, 47, 2));
  }
  display.println("    WELCOME");
  display.display();
  //  rtc.adjust(DateTime(2018, 1, 17, 8, 47, 2));
  delay(1000);
}

void loop ()
{
  /* set the time from the serial terminal */
  if (Serial.available())
  {
    Serial.println("Set the time");
        while (Serial.available()==0);
    Serial.println("Set hour in HH format :");
        while (Serial.available()==0);
        str_val = Serial.readString();
        hour_val = atoi(str_val.c_str());
//    hour_val = atoi(read_console());
    Serial.println("Set minute in MM format : ");
        while (Serial.available()==0);
        str_val = Serial.readString();
        minute_val = atoi(str_val.c_str());
//    month_val = atoi(read_console());
    Serial.println("Set DAY in  DD format : ");
        while (Serial.available()==0);
        str_val = Serial.readString();
        day_val = atoi(str_val.c_str());
//    day_val = atoi(read_console());
    Serial.println("Set month in  MM format : ");
        while (Serial.available()==0);
        str_val = Serial.readString();
        month_val = atoi(str_val.c_str());
//    month_val = atoi(read_console());
    Serial.println("Set year in  YYYY format : ");
        while (Serial.available()==0);
        str_val = Serial.readString();
        year_val = atoi(str_val.c_str());
//    year_val = atoi(read_console());
    rtc.adjust(DateTime(year_val, month_val, day_val, hour_val, minute_val, 0));
  }
  DateTime now = rtc.now();
  H_ = now.hour() / 10;
  _H = now.hour() % 10;
  M_ = now.minute() / 10;
  _M = now.minute() % 10;

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextSize(2);
  //  display.setCursor(0, 0);
  // HH : MM
  display.print(" ");
  display.print(H_, DEC); display.print(_H, DEC);
  if (blink_secound)
  {
    display.print(':');
  }
  else
  {
    display.print(' ');
  }
  display.print(M_, DEC); display.print(_M, DEC);
  display.setCursor(0, 20);
  display.setTextSize(1);
  // DD / MM
  display.print("  ");
  display.print(now.day(), DEC); display.print('/'); display.print(now.month(), DEC); display.print('/'); display.print(now.year(), DEC);
  display.setCursor(0, 20 + 10);
  display.print("  ");
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);
  display.display();
  delay(1000);
  blink_secound = !blink_secound;
}
//uint8_t* read_console()
//{
//  int i = 0;
//  do
//  {
//    buff[i] = Serial.read();
//    i++;
//  } while (buff[i] != '\n'); // exit on new line char
//   buff[i] = '\0'; 
//   return buff; //  return the string
//}

