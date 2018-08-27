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
#include "SamsungIRSender.h"
#include "main.h"
// #include <IRsend.h>
#include "ui.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Global Variable
const int OLED_ckl = D3;
const int OLED_sdin = D2;
const int OLED_rst = D1;
const int OLED_cs = D0;
WiFiClient espClient;
PubSubClient client(espClient);
SamsungIRSender irsender;
U8G2_SSD1322_NHD_256X64_1_3W_SW_SPI u8g2(U8G2_R0, /* clock=*/OLED_ckl, /* data=*/OLED_sdin, /* cs=*/OLED_cs, /* reset=*/OLED_rst); // OLED Display

//Declare prototype functions
void increment();
void decrement();
void update();

void btnSwingInt()
{
  isSwing = !isSwing;
  update();
}

void btnSpeedInt()
{
  fanSpeed = ++fanSpeed % 4;
  update();
}

// function for rotory encoder interrupt
void rotary1Int()
{

  if (!digitalRead(rotary2))
  {
    increment();
  }
}

void rotary2Int()
{
  if (!digitalRead(rotary1))
  {
    decrement();
  }
}

void changeACmode()
{
  isCool = !isCool;
  update();
}

void updateDisplay()
{
  u8g2.firstPage();

  if (isOn)
  {

    char currentTemp[3];
    String(setTemp).toCharArray(currentTemp, 3);
    do
    {
      u8g2.drawBox(215, 4, 2, 55);

      u8g2.drawXBMP(221, 4, 29, 13, wifi);
      u8g2.drawXBMP(224, 21, 24, 5, mqtt);
      u8g2.drawXBMP(227, 45, 19, 15, bulb);

      if (isCool)
      {
        u8g2.setFont(u8g2_font_glasstown_nbp_tf);
        u8g2.drawStr(3, 17, "SET");
        u8g2.setFont(u8g2_font_logisoso50_tn);
        u8g2.drawStr(23, 57, currentTemp);
        u8g2.setFont(u8g2_font_helvR24_tf);
        u8g2.drawUTF8(90, 57, "°c");
        u8g2.drawXBMP(199, 5, 13, 12, cool);
      }
      else
      {
        u8g2.drawStr(10, 30, "COOL OFF");
      }

      if (isSwing)
        u8g2.drawXBMP(197, 24, 13, 13, swing_on);
      else
        u8g2.drawXBMP(197, 24, 13, 13, swing_off);

      switch (fanSpeed)
      {
      case 0:
        u8g2.drawXBMP(171, 44, 39, 15, fan_auto);
        break;
      case 1:
        u8g2.drawXBMP(171, 44, 39, 15, fan_min);
        break;
      case 2:
        u8g2.drawXBMP(171, 44, 39, 15, fan_mid);
        break;
      case 3:
        u8g2.drawXBMP(171, 44, 39, 15, fan_max);
        break;
      }

      // u8g2.drawXBMP(0,0, u8g2_logo_97x51_width, u8g2_logo_97x51_height, u8g2_logo_97x51_bits);

      // u8g2.setFont(u8g2_font_ncenB10_tr);
      // u8g2.drawStr(0,12,"On");
    } while (u8g2.nextPage());
  }
  else
  {
    do
    {
      u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.drawStr(0, 12, "Off");
    } while (u8g2.nextPage());
  }
}

void update()
{
  static bool isLastOn = false;
  if (isOn)
  {
    if (isCool)
    {
      irsender.setCoolMode(setTemp, fanSpeed, isSwing, !isLastOn);
      Serial.println("Send IR : temp = "+ String(setTemp) + " swing = "+ String(isSwing) + " Fan Speed : " +String(fanSpeed));
      isLastOn = true;
    }
    else
    {
      // irsender.setFanMode();
    }
  }
  else
  {
    Serial.println("Send off");
    irsender.sendOff();
    isLastOn = false;
  }
  updateDisplay();
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
  if (!isOn)
  {
    Serial.println("AC On");
    isOn = true;
  }
  else
  {
    Serial.println("AC Off");
    isOn = false;
  }
  update();
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

void increment()
{
  if (!isCool)
    return;
  setTemp++;
  update();
}

void decrement()
{
  if (!isCool)
    return;
  setTemp--;
  update();
}

void btnPowerPressed()
{
  while (digitalRead(btnPower) == LOW)
  {
    delay(0);
  };
  ACOnOff();
}

// void btnUpPressed()
// {

//   while (digitalRead(btnTempUp) == LOW)
//   {
//     delay(0);
//   };
//   // ACOn();
//   setTemp++;
//   updateDisplay();
// }

// void btnDownPressed()
// {
//   while (digitalRead(btnTempUp) == LOW)
//   {
//     delay(0);
//   };
//   setTemp--;
//   updateDisplay();
//   // ACOn();
// }

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

  // Serial.println(name);
  if (strcmp(name, device_name.c_str()) != 0)
  {
    return;
  }

  // blink();
  const char *characteristic = root["characteristic"];

  if (strcmp(characteristic, "CoolingThresholdTemperature") == 0)
  {
    int value = root["value"];
    if (value < minTemp || value > maxTemp)
    {
      return;
    }
    setTemp = value;
    update();
  }
  if (strcmp(characteristic, "Active") == 0)
  {
    int value = root["value"];
    if (value != 1 && value != 0)
    {
      return;
    }
    isOn = value;
    Serial.println(isOn);
    update();
  }
  if (strcmp(characteristic, "SwingMode") == 0)
  {
    int value = root["value"];
    if (value != 1 && value != 0)
    {
      return;
    }
    isSwing = value;
    update();
  }
  if (strcmp(characteristic, "RotationSpeed") == 0)
  {
    int value = root["value"];
    if (value < 0 || value > 3)
    {
      return;
    }
    fanSpeed = value;
    update();
  }
  /*
    // The value property of TargetHeaterCoolerState must be one of the following:
  Characteristic.TargetHeaterCoolerState.AUTO = 0;
  Characteristic.TargetHeaterCoolerState.HEAT = 1;
  Characteristic.TargetHeaterCoolerState.COOL = 2;
  */
  if (strcmp(characteristic, "TargetHeaterCoolerState") == 0)
  {
    int value = root["value"];
    if (value < 0 || value > 3)
    {
      return;
    }
    if (value == 0 || value == 2)
    {
      isCool = true;
    }
    else
    {
      isCool = false;
    }
    update();
  }
}

void setup()
{
  Serial.begin(9600);

  // Setup buttons
  pinMode(btnPower, INPUT_PULLUP);
  pinMode(rotary1, INPUT_PULLUP);
  pinMode(rotary2, INPUT_PULLUP);
  pinMode(rotary_btn, INPUT_PULLUP);
  // pinMode(btnSpeed, INPUT_PULLUP);
  // pinMode(btnSwing, INPUT_PULLUP);

  pinMode(2, OUTPUT);

  // Setup networking
  WiFiManager wifiManager;
  wifiManager.autoConnect(autoconf_ssid, autoconf_pwd);
  setup_ota();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  //Attach interrupt for manual button controls
  attachInterrupt(digitalPinToInterrupt(btnPower), btnPowerPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(rotary1), rotary1Int, FALLING);
  attachInterrupt(digitalPinToInterrupt(rotary2), rotary2Int, FALLING);
  attachInterrupt(digitalPinToInterrupt(rotary_btn), changeACmode, FALLING);
    // attachInterrupt(digitalPinToInterrupt(btnSpeed), btnSpeedInt, FALLING);
      // attachInterrupt(digitalPinToInterrupt(btnSwing), btnSwingInt, FALLING);

  u8g2.begin();
  irsender.begin();
  digitalWrite(2, HIGH);
  u8g2.setContrast(10);
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