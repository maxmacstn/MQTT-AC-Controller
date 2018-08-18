#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <math.h>
#include <SamsungIRSender.h>
#include "main.h"
// #include <IRsend.h>
// #include <irCode.h>


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
// Global Variable
WiFiClient espClient;
PubSubClient client(espClient);
SamsungIRSender irsender;

//Declare prototype functions

// function for rotory encoder interrupt
void rotary1Int()
{

  if (rotary_2)
  {
    //when rotary turns ccw
    
  }
  rotary_1 = true;
  rotary_2 = false;
}

void rotary2Int()
{
  if (rotary_1)
  {
    //when rotary turns cw
  }
  rotary_1 = false;
  rotary_2 = true;
}

void updateDisplay()
{
  // u8g2.firstPage();
  // if (isOn){
    
    
  //   char currentTemp[3];
  //   String(setTemp).toCharArray(currentTemp, 3);
  //   do
  //   {
  //     u8g2.setFont(u8g2_font_logisoso50_tn);
  //     u8g2.drawStr(14, 57, currentTemp);
  //     u8g2.setFont(u8g2_font_helvR24_tf);
  //     u8g2.drawUTF8(89, 57, "°c");

  //     u8g2.drawXBMP(200, 37, 58, 27, test_page);
  //     // u8g2.drawXBMP(0,0, u8g2_logo_97x51_width, u8g2_logo_97x51_height, u8g2_logo_97x51_bits);

  //     // u8g2.setFont(u8g2_font_ncenB10_tr);
  //     // u8g2.drawStr(0,12,"On");
  //   } while (u8g2.nextPage());
  // }
  // else
  // {
  //   do
  //   {
  //     u8g2.setFont(u8g2_font_ncenB10_tr);
  //     u8g2.drawStr(0, 12, "Off");
  //   } while (u8g2.nextPage());
  // }
}

void update(){
  if(isOn){
  if (isCool){
    irsender.setCoolMode(setTemp,fanSpeed, isSwing);
  }else{
    irsender.setFanMode();
  }}
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
    irsender.sendOn();
    isOn = true;
  }
  else
  {
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

void increment(){
  if(!isCool)
    return;
  setTemp++;
  update();
  
}

void decrement(){
  if(!isCool)
    return;
  setTemp--;
  update();
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
  setTemp--;
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

  // u8g2.begin();
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