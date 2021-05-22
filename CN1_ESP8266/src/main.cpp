/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  You’ll need:
   - Blynk App (download from AppStore or Google Play)
   - ESP8266 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define CAR 1

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_INA219.h>
#include <QMC5883LCompass.h>
#include <WiFiUdp.h>

#define UDP_PORT (1122U)

QMC5883LCompass compass;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
// char auth[] = "iG9_ld97q9Z8HnMpbMGyQwvJGNkDPZS8";
// char auth[] = "ytEyT-R_rKVru-XyFbMvm37QbbPgH-wv";
// char auth[] = "yR1LwsaIqsSkaopEvOjwMDqpp5IEJvAr";
char auth[] = "SWhqiIs7p9Y7ZLl4jORzUioT9oM2UQWs"; // car

#if CAR
// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V0);

BLYNK_WRITE(V0)
{
  Serial.print( param.asStr() );
}
#endif



// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "EmbeddedSystemLAB-2G";
char pass[] = "nidhipagal";

/* Current sensor */
Adafruit_INA219 ina219;
WiFiUDP UDP;
uint8_t packet[17];
char temp[5];
uint8_t i=0;
void setup()
{
  // Debug console
  Serial.begin(115200);
  compass.init();
  Blynk.begin(auth, ssid, pass);
  // Clear the terminal content
  terminal.clear();

  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("Local IP addr:"));
  terminal.println(WiFi.localIP());

   // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  terminal.println("Listening on UDP port ");
  terminal.println(UDP_PORT);
  terminal.flush();
  
}

void loop()
{
  Blynk.run();
  compass.read();
  int a = compass.getAzimuth();
  Blynk.virtualWrite(V1,a);

  /* Udp part */
  // If packet received...
  int packetSize = UDP.parsePacket();
  if (packetSize) 
  {
    terminal.print("Received packet! Size: ");
    terminal.print(packetSize);
    terminal.println(); 
    terminal.flush();
    int len = UDP.readBytesUntil('@',packet,16);
    if (len > 0)
    {
      /* Check if EOC: End of command is received to us */
      if(!strncmp("EOC",(char*)packet,3))
      {
        terminal.println("End of Command received");
        terminal.flush();
      }
      else
      {
        terminal.println("Sending command to CN0");
        terminal.flush();
        Serial.print((char*)packet);

        /* wait for CN0 to respond 
            CN0 will respose with DONE# this indicates that current command is finished*/
        while(!Serial.available());
        Serial.readBytesUntil('#',temp,5);
        if (!strncmp("DONE",temp,4))
        {
          /* ack to CN2 Host machine to send next command */
          terminal.println("Sending ACK to CN2");
          terminal.flush();
          UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
          UDP.write("DONE#");
          UDP.endPacket();
      }

      }
      
    }
    
  }
}