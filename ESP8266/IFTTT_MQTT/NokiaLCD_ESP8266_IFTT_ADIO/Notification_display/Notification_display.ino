/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Servo.h>
/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "ESP1"
#define WLAN_PASS       "12345678"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Defaulthandler"
#define AIO_KEY         "0ee7338cc1664e749c505a12cf26fa3c"
#define D2 4
#define D1 5
#define D6 12
#define D7 13
#define D5 14
#define D0 16
#define servo_cam   1
#define servo_base  15
// pins
const int8_t RST_PIN = D2;
const int8_t CE_PIN = D1;
const int8_t DC_PIN = D6;
const int8_t BL_PIN = D0;
/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Servo base_servo;
Servo cam_servo;
/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
//Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");

// Setup a feed called 'onoff' for subscribing to changes.
//Adafruit_MQTT_Subscribe cam = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Camera rotation control");
//Adafruit_MQTT_Subscribe base_cam = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/base camera");
Adafruit_MQTT_Subscribe notification = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/notification_display");
Adafruit_MQTT_Subscribe cam = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Camera rotation control");
Adafruit_MQTT_Subscribe base_cam = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/base camera");
Adafruit_PCD8544 display = Adafruit_PCD8544(DC_PIN, CE_PIN, RST_PIN);
/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() 
{
   base_servo.attach(servo_base);
    cam_servo.attach(servo_cam);
  Serial.begin(115200);
  delay(10);
  // Configure LCD
  display.begin();
  display.setContrast(60);  // Adjust for your display
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.clearDisplay();
  display.println(F("Mad4Electronic"));
  display.println();
  display.println(F(" Notification"));
  display.println();
  display.println(F("   DEMO"));
  display.display();
  delay(5000);
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  display.clearDisplay();
  display.println("Connecting to ");
  Serial.print("Connecting to ");
  display.println(WLAN_SSID);
  display.display();
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  display.clearDisplay();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  display.println("WiFi connected");
  display.println("IP address: "); display.println(WiFi.localIP());
  display.display();
  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&notification);
   mqtt.subscribe(&cam);
  mqtt.subscribe(&base_cam);
}

uint32_t x=0;

void loop() 
{
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) 
  {
    if (subscription == &notification) 
    {
      display.clearDisplay();
      Serial.print(F("Got: "));
      Serial.println((char *)notification.lastread);
      display.println("FYI :-");
      display.println((char *)notification.lastread);
      display.display();
    }
    if (subscription == &cam) 
    {
      Serial.println((char *)cam.lastread);
      cam_servo.write(atoi((char *)cam.lastread));
    }
    if(subscription == &base_cam)
    {
      Serial.println((char *)base_cam.lastread);
      base_servo.write(atoi((char *)base_cam.lastread));
    }
    
  }

  // Now we can publish stuff!
//  Serial.print(F("\nSending photocell val "));
//  Serial.print(x);
//  Serial.print("...");
//  if (! photocell.publish(x++)) {
//    Serial.println(F("Failed"));
//  } else {
//    Serial.println(F("OK!"));
//  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
