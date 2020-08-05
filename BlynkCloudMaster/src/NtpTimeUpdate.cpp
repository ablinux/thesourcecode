#include "common.h"
#include "sysCfg.h"
void DisplayTimeOnLCD();
void DisplayTimeOnLCD()
{
#if EN_NTP_TIME
    ESP.wdtDisable();
    timeClient.update();
    // display.setCursor(0,30);display.setTextColor(WHITE,BLACK);display.printf("%d:%d:%d",timeClient.getHours()
    //                                                                                    ,timeClient.getMinutes(),
    //                                                                                    timeClient.getSeconds());display.display();
    // display.setCursor(0,30);display.setTextColor(WHITE,BLACK);display.printf("%d:%d:%d",10
    //                                                                                    ,20,
    //                                                                                    30);display.display();
    Serial.printf("%d:%d:%d\n",timeClient.getHours()
                                                                                       ,timeClient.getMinutes(),
                                                                                       timeClient.getSeconds());display.display();
    Serial.print("EverySecoundCall-----------");
    ESP.wdtEnable(10000);
#endif
}