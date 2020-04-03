#ifndef SYS_CFG_H_
#define SYS_CFG_H_

/* SSID Configuration */
#ifndef STASSID
#define STASSID "yourSSID"
#define STAPSK  "password"
#endif

/* IOT Cloud configuration */
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883 // use 8883 for SSL
#define AIO_USERNAME "Defaulthandler"
#define AIO_KEY "0ee7338cc1664e749c505a12cf26fa3c"

/*B Screen Setting OLED */
#define OLED_RESET LED_BUILTIN // Reset pin # (or -1 if sharing Arduino reset
#define SCREEN_WIDTH 128       // OLED display width, in pixels
#define SCREEN_HEIGHT 64       // OLED display height, in pixels

/*NRF Setting */
#define CE_PIN 0 //D3
#define CSN_PIN 2 //DD4

/* Temp sensor setting */
#define TEMP_ONE_WIRE_BUS D0 //GPIO6

/* SD chip select configuration */
#define SD_CARD_CS_PIN    D8

#endif