#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <math.h>
#include <U8g2lib.h>
#include <ui.h>
// #include <IRsend.h>
// #include <irCode.h>

#include <SamsungIRSender.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/*
Add to mqtt API
Topic : homebridge/from/set
Payload: {"name" : "Blind 1" ,  "service_name" : "blind_1" , "characteristic" : "TargetPosition" , "value" : 50}
*/

// Constants
const char *autoconf_ssid = "ESP8266 Smart AC"; //AP name for WiFi setup AP which your ESP will open when not able to connect to other WiFi
const char *autoconf_pwd = "12345678";          //AP password so noone else can connect to the ESP in case your router fails
const char *mqtt_server = "192.168.1.15";       //MQTT Server IP, your home MQTT server eg Mosquitto on RPi, or some public MQTT
const int mqtt_port = 1883;                     //MQTT Server PORT, default is 1883 but can be anything.
const int btnPower = D5;
const int btnTempUp = D6;
const int btnTempDown = D7;

const int OLED_ckl = D3;
const int OLED_sdin = D2;
const int OLED_rst = D1;
const int OLED_cs = D0;
const int IR_pin = D8;

// MQTT Constants
const char *mqtt_device_value_from_set_topic = "homebridge/from/set";
const char *mqtt_device_value_to_set_topic = "homebridge/to/set";
String device_name = "Smart AC";
String service_name = "smart_ac";

// Global Variable
WiFiClient espClient;
PubSubClient client(espClient);
SamsungIRSender irsender;
U8G2_SSD1322_NHD_256X64_1_3W_SW_SPI u8g2(U8G2_R0, /* clock=*/OLED_ckl, /* data=*/OLED_sdin, /* cs=*/OLED_cs, /* reset=*/OLED_rst); // OLED Display
unsigned int isOn = false;
unsigned int setTemp = 25;
unsigned int isSwing = true;
unsigned int isCool = true; //false = fan mode
unsigned int speed = 0;     //0: Auto, 1:Min, 2:Medium, 3:High
// IRsend irsend(IR_pin);

//Declare prototype functions

void updateDisplay()
{
u8g2.firstPage();
  if (isOn){
    char currentTemp[3];
    String(setTemp).toCharArray(currentTemp,3);

    do {
    u8g2.setFont(u8g2_font_logisoso50_tn);
    u8g2.drawStr(14, 57, currentTemp);
    u8g2.setFont(u8g2_font_helvR24_tf);
    u8g2.drawUTF8(89, 57,"°c");


    u8g2.drawXBMP( 200, 37, 58, 27, test_page);
        // u8g2.drawXBMP(0,0, u8g2_logo_97x51_width, u8g2_logo_97x51_height, u8g2_logo_97x51_bits);

    // u8g2.setFont(u8g2_font_ncenB10_tr);
    // u8g2.drawStr(0,12,"On");
    } while ( u8g2.nextPage() );}
  else
  {
    do
    {
      u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.drawStr(0, 12, "Off");
    } while (u8g2.nextPage());

  }
}

void blink()
{

  //Blink on received MQTT message
  digitalWrite(2, LOW);
  delay(20);
  digitalWrite(2, HIGH);
}

void setup_ota()
{

  // Set OTA Password, and change it in platformio.ini
  ArduinoOTA.setPassword("ESP8266_PASSWORD");
  ArduinoOTA.onStart([]() {});
  ArduinoOTA.onEnd([]() {});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {});
  ArduinoOTA.onError([](ota_error_t error) {
    if (error == OTA_AUTH_ERROR)
      ; // Auth failed
    else if (error == OTA_BEGIN_ERROR)
      ; // Begin failed
    else if (error == OTA_CONNECT_ERROR)
      ; // Connect failed
    else if (error == OTA_RECEIVE_ERROR)
      ; // Receive failed
    else if (error == OTA_END_ERROR)
      ; // End failed
  });
  ArduinoOTA.begin();
}

void ACOnOff()
{
  if(!isOn){
  Serial.println("AC On");
   irsender.sendOn();
    isOn = true;
  }
  else{
    Serial.println("AC Off");

    irsender.sendOff();
    isOn = false;
  }
  updateDisplay();
}

void reconnect()
{

  // Loop until we're reconnected
  while (!client.connected())
  {

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect

    if (client.connect(clientId.c_str()))
    {
      // Once connected, resubscribe.
      client.subscribe(mqtt_device_value_from_set_topic);
    }
    else
    {

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void btnPowerPressed()
{
  while (digitalRead(btnTempUp) == LOW)
  {
    delay(0);
  };
  ACOnOff();

}

void btnUpPressed()
{

  while (digitalRead(btnTempUp) == LOW)
  {
    delay(0);
  };
  // ACOn();
  setTemp++;
  updateDisplay();
}

void btnDownPressed()
{
  while (digitalRead(btnTempUp) == LOW)
  {
    delay(0);
  };
  setTemp --;
  updateDisplay();
  // ACOn();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char c_payload[length];
  memcpy(c_payload, payload, length);
  c_payload[length] = '\0';

  String s_topic = String(topic);
  String s_payload = String(c_payload);

  Serial.println(s_payload + "\0");

  StaticJsonBuffer<200> jsonBuffer;

  JsonObject &root = jsonBuffer.parseObject(s_payload);

  const char *name = root["name"];

  Serial.println(name);
  if (strcmp(name, device_name.c_str()) != 0)
  {
    return;
  }

  blink();
  const char *characteristic = root["characteristic"];

  if (strcmp(characteristic, "TargetPosition") == 0)
  {
    // int value = root["value"];
    // Serial.print("Brightness = ");
    // Serial.println(value, DEC);
    // setPosition(value);
  }
}

void setup()
{
  Serial.begin(9600);

  // Setup buttons
  pinMode(btnPower, INPUT_PULLUP);
  pinMode(btnTempDown, INPUT_PULLUP);
  pinMode(btnTempUp, INPUT_PULLUP);
  pinMode(2, OUTPUT);

  // Setup networking
  WiFiManager wifiManager;
  wifiManager.autoConnect(autoconf_ssid, autoconf_pwd);
  setup_ota();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  //Attach interrupt for manual button controls
  attachInterrupt(digitalPinToInterrupt(btnPower), btnPowerPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(btnTempDown), btnUpPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(btnTempUp), btnDownPressed, FALLING);

  u8g2.begin();
  // irsend.begin();
  irsender.begin();
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  ArduinoOTA.handle();
}

/* IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend.
 *
 * Version 1.0 April, 2017
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 *
 * An IR LED circuit *MUST* be connected to the ESP8266 on a pin
 * as specified by IR_LED below.
 *
 * TL;DR: The IR LED needs to be driven by a transistor for a good result.
 *
 * Suggested circuit:
 *     https://github.com/markszabo/IRremoteESP8266/wiki#ir-sending
 *
 * Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
 *     have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
 *     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Typical digital camera/phones can be used to see if the IR LED is flashed.
 *     Replace the IR LED with a normal LED if you don't have a digital camera
 *     when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 */

// #ifndef UNIT_TEST
// #include <Arduino.h>
// #endif
// #include <IRremoteESP8266.h>
// #include <IRsend.h>

// #define IR_LED 15  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

// IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

// // Example of data captured by IRrecvDumpV2.ino
// uint16_t rawData[67] = {9000, 4500, 650, 550, 650, 1650, 600, 550, 650, 550,
//                         600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650,
//                         600, 550, 650, 1650, 650, 1650, 650, 550, 600, 1650,
//                         650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550,
//                         650, 550, 650, 550, 600, 550, 650, 550, 650, 550,
//                         650, 1650, 600, 550, 650, 1650, 650, 1650, 650, 1650,
//                         650, 1650, 650, 1650, 650, 1650, 600};

// void setup() {
//   irsend.begin();
//   Serial.begin(9600, SERIAL_8N1, SERIAL_TX_ONLY);
// }

// void loop() {
//   Serial.println("a rawData capture from IRrecvDumpV2");
//   irsend.sendRaw(rawData, 67, 36);  // Send a raw data capture at 38kHz.
//   delay(2000);
// }
