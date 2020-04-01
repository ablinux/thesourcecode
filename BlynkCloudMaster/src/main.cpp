#include "common.h"
#include "sysCfg.h"
#include <BlynkSimpleEsp8266.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "config.h"
#include "webServerHandler.h"
#include "airCommandHandler.h"
#include "serialization.h"
#include <Sd.h>
#include "remoreNode1.h"

/* AirMail Settings */
airMail_t airMail = {NULL};

/* Blynk Settings */
#define BLYNK_PRINT Serial
char auth[] = "46bbdefa300c49aa9bf51b40ed5c2357";

/* WIFI Settings for open networks.*/
char ssid[] = "yourSSID";
char pass[] = "password";

/* RF Setting : Slave address configuration */
extern byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};
byte Myaddress[5]={'M', 'A', 'S', 'T', 'R'};
RF24 radio(CE_PIN, CSN_PIN);

/* Display */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#if 0
/*Create an ESP8266 WiFiClient class to connect to the MQTT server.*/
WiFiClient client;
/* Setup the MQTT client class by passing in the WiFi client
  and MQTT server and login details.*/
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/* Cloud feed setting */
Adafruit_MQTT_Subscribe tv_back_led = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay-switch_tv");
Adafruit_MQTT_Subscribe top_led = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay-switch_notv");
int MQTT_connect();
#endif
/* Server */
/* Web Server */
ESP8266WebServer server(80);
NodeDescripter_t NodeDescripter[MAX_NODE_SUPPORTED];
// uint8_t NodeID = 0;

/* Virtual pin callbacks **********************************/
char val[5];
extern airMail_t airMail_a;
extern uint8_t buffer[10];
BLYNK_WRITE(V0)
{
  DeviceOne_t dev1;
  memcpy(val,param.asStr(),4);
  radio.stopListening();
  radio.openWritingPipe(slaveAddress);
  airMail_a.airCommand = COMMAND_SET;
  airMail_a.mailHeader.pktType = PKT_DEVICE_SENSOR;
  airMail_a.mailHeader.dataLength = 2;
  dev1.deviceID = FISHTANK;
  dev1.dataByte = param.asInt();
  memcpy(airMail_a.data,&dev1,sizeof(dev1));
  serialize_airmail(buffer,&airMail_a,3);
  radio.write(buffer, 10);
  //radio.write(val,4);//param.asStr; // Get value as integer
  radio.startListening();
}
BLYNK_WRITE(V1)
{
  radio.stopListening();
  memcpy(val,param.asStr(),4);
  radio.openWritingPipe(slaveAddress);
  radio.write(val,4);//param.asStr; // Get value as integer
  radio.startListening();
}
/*************************************************************/
/* RF Interrupt callback */
/* NRF24 Interrupt */
uint8_t rcvRadioByteStream[32];
void ICACHE_RAM_ATTR RF_ISR()
{
  bool tx, tx_fail, rx_ready;
  radio.whatHappened(tx, tx_fail, rx_ready);
  if (rx_ready)
  {
    /* Read the Message */
    radio.read(rcvRadioByteStream, AIR_MAIL_HEADER_SIZE + MAX_PAYLOAD_SIZE +1);

    /*Now DeSerialize everything to airMail_t */
    de_serialize_airmail(&airMail,rcvRadioByteStream);

    /* Notify the AirmailDelivery services */
    readNotify(true);
  }
  if (tx_fail)
  {
    /* callback to failed tx no ack received so retry sending */
  }
  if (tx)
  {
    /* data transmitted */
  }
  Serial.println("GOt Something");
}
int x,y;
File myFile;
void handleSlider()
{
  display.setCursor(x,y);
  String t_state = server.arg("range");
  display.print("t_state.toInt()");
  display.display();
  Serial.println("Slider is on");
  Serial.println(t_state.toInt());
}
void setup()
{
  Serial.begin(115200);
  /* NRF setup */
  radio.begin();  radio.setDataRate( RF24_250KBPS ); radio.setPALevel(RF24_PA_HIGH); radio.setRetries(5, 10);

  radio.openReadingPipe(1,Myaddress);
  radio.startListening();

  /* Init The SD card */
  #if 0
  if (!SD.begin(D0)) {
    Serial.println("initialization failed!");
    while (1);
  }
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
#endif
  /* Display Setting */
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();

  display.clearDisplay(); display.setTextSize(1);display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);

  display.print("connecting...to WIFI ");display.println();display.display();
  /* Connect to the wifi */
  
  /* Blynk */
  Blynk.begin(auth, ssid, pass);

  /* Set the hostname */
  WiFi.hostname("abhi");
#if 0
   // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&tv_back_led);
  mqtt.subscribe(&top_led);
#endif
  /* Setup the OTA */
  OtaSetup();
  // pinMode(16, INPUT_PULLUP);
  display.println("connected.");
  display.print("IP: "); display.print(WiFi.localIP());display.println();display.display();
  x = display.getCursorX(); y = display.getCursorY();

  server.on("/", handleRoot);
  server.on("/tvled", TVLEDState);
  server.on("/topled", TOPLEDState);
  server.on("/bedroomled", BEDLEDState);
  server.on("/readADC", handleTemp);
  server.on("/Slider", handleSlider);
  server.begin();
  // attachInterrupt(digitalPinToInterrupt(16),RF_ISR,FALLING);
}

void loop()
{
  if(radio.available())
  {
    radio.read(rcvRadioByteStream,32);
    de_serialize_airmail(&airMail,rcvRadioByteStream);
    readNotify(true);
  }
  airNotificationHandler(&airMail);
  server.handleClient();
  ArduinoOTA.handle();
  Blynk.run();
}

#if 0
/* Adafruit_MQTT_Client Connect */
int MQTT_connect()
{
  int8_t ret = 0;
  // Stop if already connected.
  if (mqtt.connected())   { return 1;}

  uint8_t retries = 1;
  while ((ret = mqtt.connect()) != 0)
  { 
    delay(1); // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      return 0;
    }
  }
  if(ret) return 1;
}
#endif