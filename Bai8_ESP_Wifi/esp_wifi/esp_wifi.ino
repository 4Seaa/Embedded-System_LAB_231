#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Wifi name
const char *WLAN_SSID = "";
// Wifi password
const char *WLAN_PASS = "";

// setup Adafruit
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
// fill your username
#define AIO_USERNAME ""
// fill your key
#define AIO_KEY ""

// setup MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// setup publish
Adafruit_MQTT_Publish light_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/light");
Adafruit_MQTT_Publish temp_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");

// setup subcribe
Adafruit_MQTT_Subscribe light_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light", MQTT_QOS_1);

int led_counter = 0;
int led_status = HIGH;

int ada_counter = 0;

void lightcallback(char *value, uint16_t len)
{
  switch (value[0])
  {
  case '0':
    Serial.println('a');
    break;
  case '1':
    Serial.println('A');
    break;
  }
}

void setup()
{
  // put your setup code here, to run once:
  // set pin 2,5 as OUTPUT
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  // set busy pin HIGH
  digitalWrite(5, HIGH);

  Serial.begin(115200);

  // connect Wifi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  // subscribe light feed
  light_sub.setCallback(lightcallback);
  mqtt.subscribe(&light_sub);

  // connect MQTT
  while (mqtt.connect() != 0)
  {
    mqtt.disconnect();
    delay(500);
  }

  // finish setup, set busy pin LOW
  digitalWrite(5, LOW);
}

typedef union
{
  float f;
  uint8_t b[4];
} float_bytes;

void loop()
{
  // put your main code here, to run repeatedly:

  // receive packet
  mqtt.processPackets(1);

  // read serial
  if (Serial.available())
  {
    String msg_str = Serial.readStringUntil('#'); // read message string
    float temp;
    sscanf(msg_str.c_str(), "!TEMP:%f#", &temp); // parse temp value
    switch (msg_str[0])
    {
    case 'o':
      Serial.println('O');
      break;
    case 'a':
      light_pub.publish(LOW);
      break;
    case 'A':
      light_pub.publish(HIGH);
      break;
    default:
      temp_pub.publish(temp); // send temp value
      delay(500);
      break;
    }
  }

  led_counter++;
  if (led_counter == 100)
  {
    // every 1s
    led_counter = 0;
    // toggle LED
    if (led_status == HIGH)
      led_status = LOW;
    else
      led_status = HIGH;

    digitalWrite(2, led_status);
  }
  // use millis() instead of delay()
  unsigned long currentMillis = millis();
  while (millis() - currentMillis < 10)
  {
    // do other tasks
  }
}