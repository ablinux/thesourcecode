/*
  Controlling a servo position using a potentiometer (variable resistor)
  by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

  modified on 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>
#include <NewPing.h>
#define DIST_SENS 0
Servo myservo;  // create servo object to control a servo
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define FAN 2 // pin 2 for fan on - off
#define ON LOW
#define OFF HIGH
int potpin = 0;  // analog pin used to connect the potentiometer
int val = 90;    // variable to read the value from the analog pin
int pos = 0;
uint8_t serial_read = '0';
uint8_t delay_val = 50;

#if DIST_SENS
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
#endif

void setup()
{
  pinMode(FAN, OUTPUT);
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop()
{
#if DIST_SENS
  int dist = sonar.ping_cm();
  if (dist != 0)
  {
    if (dist > 100)
    {
      digitalWrite(2, HIGH);
    }
    else
    {
      digitalWrite(2, LOW);
      delay(50);
    }
  }
#endif
  /* check serial is avaliable or not */
  if (Serial.available())
  {
    serial_read =  Serial.read();

    /*************************Fan On ********************************************/
    if(serial_read == 'o')
    {
      digitalWrite(FAN,ON);
    }
    if(serial_read == 'f')
    {
      digitalWrite(FAN,OFF);
    }
    /****************** Rest the camera to 90* **************************************/
    if (serial_read == '5')
    {
      /* Do the smooth rotation from current position */
      if (val >= 90)
      {
        for (pos = val; pos >= 90; pos -= 1)
        {
          myservo.write(pos);
          delay(delay_val);
        }
        val = pos;
      }
      else if (val <= 90)
      {
        for (pos = val; pos <= 90; pos += 1)
        {
          myservo.write(pos);
          delay(delay_val);
        }
        val = pos;
        val = 90;
      }
      else
      {

      }
    }
    /********************* Move camera to left *************************/
    if (serial_read == '4')
    {
      if (val >= 10)
      {
        for (pos = val; pos >= 10; pos -= 1)
        {
          myservo.write(pos);
          delay(delay_val);
        }
        val = pos;
      }
      else
      {
        val = 10;
      }
    }
    /********************* Move camera to right *************************/
    if (serial_read == '6')
    {
      if (val <= 170)
      {
        for (pos = val; pos <= 170; pos += 1)
        {
          myservo.write(pos);
          delay(delay_val);
        }
        val = pos;
      }
      else
      {
        val = 170;
      }
    }
    if (serial_read == 'a')
    {
      val++;
    }
    if (serial_read == 'd')
    {
      val--;
    }
    if (val < 0)
    {
      val = 1;
    }
    if (val > 180)
    {
      val = 179;
    }
    myservo.write(val);
  }
}
