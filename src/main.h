#include <Arduino.h>

#ifndef MAIN_H
#define MAIN_H

/*

{
"name": "Smart AC",
"Active": 1,
"CurrentHeatingCoolingState" : 3,
"TargetHeaterCoolerState": 2,
"CurrentTemperature": 26,

"service_name" : "smart_ac",
"service": "HeaterCooler",
"CoolingThresholdTemperature": { "maxValue": 28, "minValue": 23, "minStep": 1},
"SwingMode" : 1,
"RotationSpeed":  { "maxValue": 3, "minValue": 0, "minStep": 1}
}
*/

// Constants
const char *autoconf_ssid = "ESP8266 Smart AC"; //AP name for WiFi setup AP which your ESP will open when not able to connect to other WiFi
const char *autoconf_pwd = "12345678";          //AP password so noone else can connect to the ESP in case your router fails
const char *mqtt_server = "192.168.1.15";       //MQTT Server IP, your home MQTT server eg Mosquitto on RPi, or some public MQTT
const int mqtt_port = 1883;                     //MQTT Server PORT, default is 1883 but can be anything.
const int btnPower = D5;
const int rotary1 = D6;
const int rotary2 = D7;
const int rotary_btn = 3;



const int IR_pin = D8;

// MQTT Constants
const char *mqtt_device_value_from_set_topic = "homebridge/from/set";
const char *mqtt_device_value_to_set_topic = "homebridge/to/set";
String device_name = "Smart AC";
String service_name = "smart_ac";

// Global variables
unsigned int isOn = false;
unsigned int setTemp = 25;
unsigned int isSwing = true;
unsigned int isCool = true; //false = fan mode
unsigned int fanSpeed = 0;     //0: Auto, 1:Min, 2:Medium, 3:High
unsigned int maxTemp = 28;
unsigned int minTemp = 23;
bool rotary_1 = false;
bool rotary_2 = false;


#endif