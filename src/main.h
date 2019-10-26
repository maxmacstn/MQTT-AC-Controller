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
const int btnUp = D6;
const int btnDn = D7;
const int btnSwing = 3;
const int btnSpeed = 1;
const int ldrPin = A0;
const int IR_pin = D8;

const int screenBrightnessUpdateInt = 5000;     //Interval time for adjusting screen's brightness according to environment light.

// MQTT Constants
const char *mqtt_device_value_from_set_topic = "homebridge/from/set";
const char *mqtt_device_value_to_set_topic = "homebridge/to/set";
String device_name = "Smart AC";
String service_name = "smart_ac";
String device_name_secondary = "Smart AC Basic";
String service_name_secondary = "smart_ac_basic";

// Global variables
unsigned int isOn = false;
unsigned int setTemp = 23;
unsigned int isCool = true; //false = fan mode
unsigned int fanSpeed = 0;     //0: Auto, 1:Min, 2:Medium, 3:High
unsigned int maxTemp = 28;
unsigned int minTemp = 18;
unsigned int currentAction = 0;
bool isSwing = false;
bool rotary_1 = false;
bool rotary_2 = false;
unsigned long previousMillis = 0;               //For auto screen brightness
unsigned long lastConnectingTime = 0;               //For auto screen brightness
int currentContrast = 255;

// Define NTP properties
#define NTP_OFFSET  7 * 60 * 60      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "ntp.ku.ac.th"  // change this to whatever pool is closest (see ntp.org)

#define ACTION_POWER  1
#define ACTION_UP  2
#define ACTION_DOWN  3


//AC off raw data, because I can't turn off my AC with IRSamsungAc library
uint16_t ac_off[349] = {640, 17770,  3056, 8898,  522, 472,  550, 1436,  554, 438,  560, 434,  560, 434,  560, 432,  556, 438,  554, 438,  554, 438,  554, 1432,  556, 442,  550, 442,  550, 1436,  550, 1458,  474, 520,  516, 1470,  552, 1436,  556, 1428,  560, 1428,  584, 1402,  560, 438,  556, 438,  554, 438,  556, 438,  556, 438,  554, 438,  554, 442,  552, 464,  528, 464,  470, 524,  542, 450,  546, 446,  552, 442,  554, 438,  556, 438,  556, 434,  558, 434,  560, 434,  560, 434,  556, 438,  556, 442,  552, 442,  550, 444,  550, 442,  550, 448,  546, 464,  528, 464,  474, 518,  546, 446,  550, 442,  554, 438,  554, 438,  560, 434,  560, 434,  554, 1432,  556, 1432,  556, 2934,  3030, 8898,  556, 1436,  550, 442,  550, 468,  526, 464,  530, 464,  474, 520,  546, 446,  550, 442,  556, 438,  554, 1432,  554, 438,  554, 440,  554, 1432,  556, 438,  556, 1432,  554, 1436,  552, 1436,  550, 1458,  554, 1432,  474, 1514,  550, 442,  550, 444,  554, 438,  554, 438,  554, 438,  554, 438,  554, 438,  556, 438,  556, 438,  556, 438,  554, 442,  550, 442,  550, 442,  552, 464,  530, 464,  530, 464,  470, 522,  546, 448,  550, 442,  550, 444,  554, 438,  554, 438,  556, 438,  556, 438,  554, 438,  550, 442,  550, 442,  552, 442,  550, 442,  550, 442,  550, 446,  546, 468,  524, 470,  524, 468,  516, 476,  548, 446,  550, 2912,  3052, 8900,  554, 1434,  554, 438,  554, 438,  556, 438,  556, 438,  556, 438,  554, 438,  554, 442,  550, 442,  550, 1438,  550, 464,  530, 464,  474, 1514,  546, 446,  550, 442,  550, 442,  552, 1432,  554, 1432,  554, 1432,  554, 1432,  556, 442,  550, 1436,  552, 442,  550, 1458,  530, 1458,  520, 478,  546, 448,  546, 446,  550, 1434,  554, 1432,  556, 1432,  556, 438,  530, 464,  528, 470,  524, 468,  520, 474,  520, 1466,  522, 472,  522, 494,  500, 1488,  520, 1468,  550, 444,  550, 442,  526, 468,  524, 1462,  524, 468,  526, 468,  526, 468,  526, 468,  526, 468,  524, 474,  520, 472,  520, 472,  522, 498,  494, 1488,  500, 1488,  526};  // SAMSUNG_AC


#endif