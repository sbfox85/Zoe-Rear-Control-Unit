// https://github.com/sbfox85/Zoe-Rear-Control-Unit/
//
// Control Unit to manage Outlander Chargers in Parallel
// With Cooling Pump and Small DC Fan Control and basic CHAdeMO.
#include <ArduinoJson.h>
#include <time.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <string.h>
#include <stdio.h>
#include "Wire.h"
#include <ACAN2515.h>
#include <ACAN_ESP32.h>
#include <NetworkUdp.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>  // https://github.com/ESP32Async/AsyncTCP 3.3.8
#include <ArduinoOTA.h>
#include <esp_wifi.h>
// https://github.com/ESP32Async/ESPAsyncWebServer
#include <Arduino.h>
#include <StreamString.h>
#include "FS.h"
#include <LittleFS.h>
//#include "RTClib.h"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "driver/twai.h"
#include "CRC8.h"
#include "CRC.h"       //
#include <ESPFMfGK.h>  // modified to use "FMCRC32.h" and "FMCRC32.cpp" included in the library (they conflicted with our crc)
#include <ESPAsyncWebServer.h>

static const byte MCP2515_SCK = GPIO_NUM_12 ; // SCK input of MCP2515
static const byte MCP2515_SDI = GPIO_NUM_11 ; // SI input of MCP2515
static const byte MCP2515_SDO = GPIO_NUM_13 ; // SO output of MCP2515
static const byte MCP2515_CS1  = GPIO_NUM_17 ; // CS input of MCP2515
static const byte MCP2515_CS2  = GPIO_NUM_7 ; // CS input of MCP2515
static const byte MCP2515_CS3  = GPIO_NUM_15 ; // CS input of MCP2515
static const byte MCP2515_CS4  = GPIO_NUM_6 ; // CS input of MCP2515
static const byte MCP2515_RESET = GPIO_NUM_45 ; // RESET input of MCP2515 (doesn't actually work)
byte ch1info1,ch1info2,ch1info3,ch2info1,ch2info2,ch2info3,ch3info1,ch3info2,ch3info3,summary1,summary2,summary3,summary4 = 0x300;
int CANCycle = 1;
int send0x285 = 0;
int send0x286 = 0;
int PilotStarted=0;
int EVSEACCurrentLimit = 0;
int CableCurrentLimit = 0;
int LockState = 0;
int Bank_Current_Request = 0;
int ChargeState = 0;
unsigned long lastCANSend = 0;
unsigned long LastChargeStateChange = 0;
unsigned long last285 = 0;
unsigned long last286 = 0;
uint16_t TotalHVDCCurrent = 0;
byte Charger1_LVBatVoltMSB = 0;
byte Charger1_LVBatVoltLSB = 0;
int Charger1_LVBatVoltDecimal = 0;
byte Charger1_LVSupplyCurrentMSB = 0;
byte Charger1_LVSupplyCurrentLSB = 0;
int Charger1_LVSupplyCurrentDecimal = 0;
byte HVDC_Charger_Request_Current = 0;
byte Charger1_Status_PilotPresent = 0;
byte Charger1_Status_DCDCRequest = 0;
byte Charger1_Status_Error = 0;
byte Charger1_Status_Charging = 0;
byte Charger1_Temp1 = 0;
byte Charger1_Temp2 = 0;
byte Charger1_Temp3 = 0;
byte Charger1_Charger_Temp1 = 0;
byte Charger1_Charger_Temp2 = 0;
byte Charger1_ChargerUnit_Temp1 = 0;
byte Charger1_ChargerUnit_Temp2 = 0;
byte Charger1_Status = 0;
byte Charger1_HVBatVolt = 0;
byte Charger1_ACVolt = 0;
byte Charger1_ACCurrent = 0;
byte Charger1_DCCurrent = 0;
byte Charger1_DCBusVoltage = 0;
byte Charger1_DCDC_Temp1 = 0;
byte Charger1_DCDC_Temp2 = 0;
byte Charger1_DCDC_Temp3 = 0;
byte Charger1_DCDC_Status = 0;
byte Charger1_EVSE_Pilotduty = 0;
unsigned long Charger1_Last_0x377 = 0;
unsigned long Charger1_Last_0x389 = 0;
unsigned long Charger1_Last_0x38A = 0;

byte Charger3_LVBatVoltMSB = 0;
byte Charger3_LVBatVoltLSB = 0;
int Charger3_LVBatVoltDecimal = 0;
byte Charger3_LVSupplyCurrentMSB = 0;
byte Charger3_LVSupplyCurrentLSB = 0;
int Charger3_LVSupplyCurrentDecimal = 0;
byte Charger3_Status_PilotPresent = 0;
byte Charger3_Status_DCDCRequest = 0;
byte Charger3_Status_Error = 0;
byte Charger3_Status_Charging = 0;
byte Charger3_Temp1 = 0;
byte Charger3_Temp2 = 0;
byte Charger3_Temp3 = 0;
byte Charger3_Charger_Temp1 = 0;
byte Charger3_Charger_Temp2 = 0;
byte Charger3_ChargerUnit_Temp1 = 0;
byte Charger3_ChargerUnit_Temp2 = 0;
byte Charger3_Status = 0;
byte Charger3_HVBatVolt = 0;
byte Charger3_ACVolt = 0;
byte Charger3_ACCurrent = 0;
byte Charger3_DCCurrent = 0;
byte Charger3_DCBusVoltage = 0;
byte Charger3_DCDC_Temp1 = 0;
byte Charger3_DCDC_Temp2 = 0;
byte Charger3_DCDC_Temp3 = 0;
byte Charger3_DCDC_Status = 0;
byte Charger3_EVSE_Pilotduty = 0;
unsigned long Charger3_Last_0x377 = 0;
unsigned long Charger3_Last_0x389 = 0;
unsigned long Charger3_Last_0x38A = 0;

byte Charger2_LVBatVoltMSB = 0;
byte Charger2_LVBatVoltLSB = 0;
int Charger2_LVBatVoltDecimal = 0;
byte Charger2_LVSupplyCurrentMSB = 0;
byte Charger2_LVSupplyCurrentLSB = 0;
int Charger2_LVSupplyCurrentDecimal = 0;
byte Charger2_Status_PilotPresent = 0;
byte Charger2_Status_DCDCRequest = 0;
byte Charger2_Status_Error = 0;
byte Charger2_Status_Charging = 0;
byte Charger2_Temp1 = 0;
byte Charger2_Temp2 = 0;
byte Charger2_Temp3 = 0;
byte Charger2_Charger_Temp1 = 0;
byte Charger2_Charger_Temp2 = 0;
byte Charger2_ChargerUnit_Temp1 = 0;
byte Charger2_ChargerUnit_Temp2 = 0;
byte Charger2_Status = 0;
byte Charger2_HVBatVolt = 0;
byte Charger2_ACVolt = 0;
byte Charger2_ACCurrent = 0;
byte Charger2_DCCurrent = 0;
byte Charger2_DCBusVoltage = 0;
byte Charger2_DCDC_Temp1 = 0;
byte Charger2_DCDC_Temp2 = 0;
byte Charger2_DCDC_Temp3 = 0;
byte Charger2_DCDC_Status = 0;
byte Charger2_EVSE_Pilotduty = 0;
unsigned long Charger2_Last_0x377 = 0;
unsigned long Charger2_Last_0x389 = 0;
unsigned long Charger2_Last_0x38A = 0;

int Master = 0; // Master = 1 changes behaviour.
int CHAdeMO = 1; // CHAdeMO = 1 Means it has chademo capability.

struct SETTINGS { // All overwritten when settings.json is read, if it exists.
int CHAD_MaxkW = 10; 
int CHAD_MinkW = 5;
int CHAD_MinAmps = 5; 
int CHAD_MaxAmps = 20;
int CHAD_MaxPercent = 90;
int CHAD_MinPercent = 1;
int CHAD_MaxVoltage = 380;
int CHAD_MinVoltage = 300;
int CHAD_MaxDelta = 100;
int CHAD_MaxDuration = 30;
int CHAD_MaxCellTempEnd = 400;
int CHAD_MaxCellTempBegin = 380;
int CHAD_BatteryHeatThreshold = 15
int CHAD_BatteryBlowerThreshold = 0;
int CHAD_BatteryCoolThreshold = 28; 
int AC_MaxTotalkW = 23;
int AC_MinTotalkW = 1;
int AC_MaxDeltaStart = 200;
int AC_MaxDeltaEnd = 300;
int AC_MaxCellVoltageEnd = 4000;
int AC_MaxCellVoltageStart = 3950;
int AC_MaxSOCStandardEnd = 750;
int AC_MaxSOCOverideEnd = 999;
int AC_MaxPackVoltageStart = 380;
int AC_MaxPackVoltageEnd = 390;
int DCDC_AutoStartVoltage = 1200;
int DCDC_AutoStartTimout = 5;
int DCDC_IgnitionTimeout = 1;
int DCDC_ChargeTimeout = 1;
int DCDC_WakeTimeout = 1;
int DCDC_MaxVoltage = 1500;
int DCDC_MinVoltage = 1200;
int DCDC_MaxTemp = 750;
int Pump_IdleSpeed = 20;
int Pump_ChargeSpeed = 35;
int Pump_DCDCSpeed = 35;
int Pump_MaxSpeed = 75;
int Pump_BleedSpeed = 100;
int Pump_Timeout = 10;
int Pump_MaxThreshold = 650;
int Fan_Stage1Speed = 10;
int Fan_Stage1Temp = 400;
int Fan_Stage2Speed = 30;
int Fan_Stage2Temp = 450;
int Fan_Stage3Speed = 60;
int Fan_Stage3Temp = 600;
int Fan_Hysteresis = 2;
int Fan_Timeout = 30;
};
SETTINGS settings;

//----------------------------------------------------------------------------------------
//  MCP2515 Driver object
//----------------------------------------------------------------------------------------

ACAN2515 CAN1(MCP2515_CS1, SPI, 255);
ACAN2515 CAN2(MCP2515_CS2, SPI, 255);
ACAN2515 CAN3(MCP2515_CS3, SPI, 255);
ACAN2515 CAN4(MCP2515_CS4, SPI, 255);
//----------------------------------------------------------------------------------------
//  MCP2515 Quartz: adapt to your design
//----------------------------------------------------------------------------------------

static const uint32_t MCP2515_QUARTZ_FREQUENCY = 16UL * 1000UL * 1000UL ; // 16 MHz

//----------------------------------------------------------------------------------------
// Desired Bit Rate
//----------------------------------------------------------------------------------------

static const uint32_t DESIRED_BIT_RATE = 500UL * 1000UL ; // 0.5 Mb/s


void setup() {
  //--- RESET MCP2515
  pinMode (MCP2515_RESET, OUTPUT) ;
  digitalWrite (MCP2515_RESET, LOW) ;
  delay (10) ;
  digitalWrite (MCP2515_RESET, HIGH) ;

//--- Start serial
  Serial.begin (115200) ;
  delay (100) ;
//--- Configure ESP32 CAN
  Serial.println ("Configure ESP32 CAN") ;
  ACAN_ESP32_Settings settings (DESIRED_BIT_RATE);           // CAN bit rate
  settings.mRxPin = GPIO_NUM_5 ; // 
  settings.mTxPin = GPIO_NUM_4 ; // 
  const uint32_t errorCode = ACAN_ESP32::can.begin (settings) ;
  if (errorCode == 0) {
    Serial.println ("Configuration ESP32 OK!");
  }else{
    Serial.print ("Configuration error 0x") ;
    Serial.println (errorCode, HEX) ;
  }

  //--- Begin SPI1
  SPI.begin (MCP2515_SCK, MCP2515_SDO, MCP2515_SDI) ;
  Serial.println ("Configure ACAN2515") ;
  ACAN2515Settings settingsCAN1 (MCP2515_QUARTZ_FREQUENCY, DESIRED_BIT_RATE) ;
  ACAN2515Settings settingsCAN2 (MCP2515_QUARTZ_FREQUENCY, DESIRED_BIT_RATE) ;
  ACAN2515Settings settingsCAN3 (MCP2515_QUARTZ_FREQUENCY, DESIRED_BIT_RATE) ;
  ACAN2515Settings settingsCAN4 (MCP2515_QUARTZ_FREQUENCY, DESIRED_BIT_RATE) ;
  const uint32_t errorCode1 =  CAN1.begin (settingsCAN1, [] { CAN1.isr () ; }) ;
  if (errorCode1 == 0) {
    Serial.println ("CAN 1 configuration: ok") ;
  }else{
    Serial.print ("CAN 1 Configuration error 0x") ;
    Serial.println (errorCode1, HEX) ;
  }
  const uint32_t errorCode2 =  CAN2.begin (settingsCAN2, [] { CAN2.isr () ; }) ;
  if (errorCode2 == 0) {
    Serial.println ("CAN 2 configuration: ok") ;
  }else{
    Serial.print ("CAN 2 Configuration error 0x") ;
    Serial.println (errorCode2, HEX) ;
  }
  const uint32_t errorCode3 =  CAN3.begin (settingsCAN3, [] { CAN3.isr () ; }) ;
  if (errorCode3 == 0) {
    Serial.println ("CAN 3 configuration: ok") ;
  }else{
    Serial.print ("CAN 3 Configuration error 0x") ;
    Serial.println (errorCode3, HEX) ;
  }
  const uint32_t errorCode4 =  CAN4.begin (settingsCAN2, [] { CAN4.isr () ; }) ;
  if (errorCode4 == 0) {
    Serial.println ("CAN 4 configuration: ok") ;
  }else{
    Serial.print ("CAN 4 Configuration error 0x") ;
    Serial.println (errorCode4, HEX) ;
  }
  if(Master==0) {
      ch1info1 = 0x310;
      ch1info2 = 0x311;
      ch1info3 = 0x312;
      ch2info1 = 0x320;
      ch2info2 = 0x321;
      ch2info3 = 0x322;
      ch3info1 = 0x330;
      ch3info2 = 0x331;
      ch3info3 = 0x332;
      summary1 = 0x340;
      summary2 = 0x341;
      summary3 = 0x342;
      summary4 = 0x343;
    } else if (Master==1) {
      ch1info1 = 0x350;
      ch1info2 = 0x351;
      ch1info3 = 0x352;
      ch2info1 = 0x360;
      ch2info2 = 0x361;
      ch2info3 = 0x362;
      ch3info1 = 0x370;
      ch3info2 = 0x371;
      ch3info3 = 0x372;
      summary1 = 0x380;
      summary2 = 0x381;
      summary3 = 0x382;
      summary4 = 0x383;
    }
    // Configure PWM Pilot Pins, But turn output Off.
    pinMode(GPIO_NUM_37, OUTPUT);
    digitalWrite(GPIO_NUM_37, LOW);
    pinMode(GPIO_NUM_38, OUTPUT);
    digitalWrite(GPIO_NUM_38, LOW);
    pinMode(GPIO_NUM_39, OUTPUT);
    digitalWrite(GPIO_NUM_39, LOW);


}

// We need a charge current coordination. 
//  1) BMS must rule supreme.
//  2) If 3 chargers can handle the current, then use those instead of overheads of 6 running. So, perhaps
//      if pilot is 16amp or less, we just command the master to handle it all. We should start all chargers and have
//      a timeout to turn 3 off, because some chargers ramp up current. Maybe after 5mins of under 16amp, turn 3 off?
//      if battery cold is the restriction, then we fire up all chargers as the hope is that it will warm up and speed up
//  

void loop() {
    CANMessage frame ; // creates temporary can frame message.
    CANMessage outframe ; // creates temporary can frame message.

    
    if ( (EVSEACCurrentLimit > 5)&&(EVSEACCurrentLimit < 67)&&(CableCurrentLimit > 5)&&(LockState==1)) {
      // We are good to charge
       
       // Calculate Per Phase Current, so if slave we use Bank_Current_Request ((Current times by 1.667) * 10.23.)
      int desiredpwm = 0;
      if (Bank_Current_Request==6) { desiredpwm=102;}
      else if (Bank_Current_Request==7) { desiredpwm=120;}
      else if (Bank_Current_Request==8) { desiredpwm=137;}
      else if (Bank_Current_Request==9) { desiredpwm=154;}
      else if (Bank_Current_Request==10) { desiredpwm=171;}
      else if (Bank_Current_Request==11) { desiredpwm=188;}
      else if (Bank_Current_Request==12) { desiredpwm=205;}
      else if (Bank_Current_Request==13) { desiredpwm=222;}
      else if (Bank_Current_Request==14) { desiredpwm=239;}
      else if (Bank_Current_Request==15) { desiredpwm=256;}
      else if (Bank_Current_Request==16) { desiredpwm=273;}
      else { desiredpwm=1;} // We should stop charging then.

      if (ChargeState==0) {
        LastChargeStateChange=millis();
        ChargeState=1;
      } 
      if ((ChargeState==1)&&((millis()-LastChargeStateChange) > 1000)) { // If it's been more than 1second since changing to state1
        
        digitalWrite(GPIO_NUM_39, HIGH); // Pilot 1
        digitalWrite(GPIO_NUM_38, HIGH); // Pilot 2
        digitalWrite(GPIO_NUM_37, HIGH); // Pilot 3
        send0x285=1; // 0x285 Needs to start sending ever 100ms
        LastChargeStateChange=millis();
        ChargeState==2;
      }

      if ((ChargeState==2)&&((millis()-LastChargeStateChange) > 1000)) { // If it's been more than 1second since changing to state1
      
        if(PilotStarted==0) {
          ledcAttach(GPIO_NUM_39, 1000, 10); // GPIO, Frequency, and Bit resolution required 10 = 0-1023
          ledcAttach(GPIO_NUM_38, 1000, 10); // GPIO, Frequency, and Bit resolution required 10 = 0-1023
          ledcAttach(GPIO_NUM_37, 1000, 10); // GPIO, Frequency, and Bit resolution required 10 = 0-1023
          ledcWrite(GPIO_NUM_39, 102); // 102 = 6amp. Start Low
          ledcWrite(GPIO_NUM_38, 102); // 102 = 6amp. Start Low
          ledcWrite(GPIO_NUM_37, 102); // 102 = 6amp. Start Low
          send0x286=1;
          PilotStarted=1;
        }

        LastChargeStateChange=millis();
        ChargeState==3;
      }

      if ((ChargeState==3)&&((millis()-LastChargeStateChange) > 5000)) { // If it's been more than 5second since changing then action
        // Now it's time to adjust the pilot to the correct level.
        ledcWrite(GPIO_NUM_39, desiredpwm);
        ledcWrite(GPIO_NUM_38, desiredpwm);
        ledcWrite(GPIO_NUM_37, desiredpwm);
        LastChargeStateChange=millis();
        ChargeState=4;
      }

      if ((ChargeState==4)&&((millis()-LastChargeStateChange) > 100)) {
        // Keep adjusting the pilot to the correct level every 100ms.
        ledcWrite(GPIO_NUM_39, desiredpwm);
        ledcWrite(GPIO_NUM_38, desiredpwm);
        ledcWrite(GPIO_NUM_37, desiredpwm);
        LastChargeStateChange=millis();
      }
        
    } else { // So if not suitable ACCurrentLimit or Cable Limit Or LockIsn't Locked
      if(ChargeState!=0) { // If it's in any state but zero, action....
        if((ChargeState==1)||(ChargeState==2)) { // We just need to set Digital Outputs to Low.
          digitalWrite(GPIO_NUM_39, LOW);
          digitalWrite(GPIO_NUM_38, LOW);
          digitalWrite(GPIO_NUM_37, LOW);
          send0x285=0; // stop sending can messages
          send0x286=0; // stop sending can messages
          PilotStarted=0;
          LastChargeStateChange=millis();

        } else { // We need to detach PWM and set Digital low
          ledcWrite(GPIO_NUM_39, 1); // Set pwm to 1.
          ledcWrite(GPIO_NUM_38, 1);
          ledcWrite(GPIO_NUM_37, 1);
          ledcDetach(GPIO_NUM_39); // Detach PWM from Pin
          ledcDetach(GPIO_NUM_38);
          ledcDetach(GPIO_NUM_37);
          digitalWrite(GPIO_NUM_39, LOW); // Set Pin Low
          digitalWrite(GPIO_NUM_38, LOW);
          digitalWrite(GPIO_NUM_37, LOW);
          send0x285=0; // stop sending can messages
          send0x286=0; // stop sending can messages
          PilotStarted=0;
          LastChargeStateChange=millis();
        }
      }
    }

    
    
  if ((send0x285==1)&&((millis() - last285) >= 30)){ // 30ms send interval?
    CANMessage can285frame;

    can285frame.id = 0x285; // Heartbeat Message
    can285frame.data[0] = 0x00; // 0x00 fine
    can285frame.data[1] = 0x00; // 0x00 fine
    can285frame.data[2] = 0x00; // 0x02, then ges to 0xB6 when we want HV contactors closed
    can285frame.data[3] = 0x00; // 0x00 is probaby fine.
    can285frame.data[4] = 0x00; // 0x00 is probbaly fine.
    can285frame.data[5] = 0x00;
    can285frame.data[6] = 0xB6;
    can285frame.data[7] = 0x39;
    ACAN_ESP32::can.tryToSend (can285frame) ;
    last285=millis();
  }

  if ((send0x285==1)&&((millis() - last286) >= 100)){
    CANMessage can286frame;

    can286frame.id = 0x286; // Heartbeat Message
    can286frame.data[0] = 0x0F; // 0x0F 
    can286frame.data[1] = 0x96; // 0x96 0x0F96 = 3990v = 399.0v
    can286frame.data[2] = HVDC_Charger_Request_Current; // Amps x2. 
    can286frame.data[3] = 0x00; // 0x00 is probaby fine.
    can286frame.data[4] = 0x00; // 0x00 is probbaly fine.
    can286frame.data[5] = 0x00;
    can286frame.data[6] = 0x00;
    can286frame.data[7] = 0x00;
    ACAN_ESP32::can.tryToSend (can286frame) ;
    last286=millis();
  }


       
    
  
  

  if (CANCycle==1) {
    CAN1.poll () ; // No interrupt, call this function as often as possible
    if (CAN1.available ()) {
      CAN1.receive (frame) ;
      //gReceivedFrameCount ++ ;
      Serial.print ("Received on CAN1: ") ;
      //Serial.println (gReceivedFrameCount) ;
      if (frame.id==0x377) { // as per DBC file on OpenInverter.org
        Charger1_Last_0x377=millis();
        Charger1_LVBatVoltMSB = frame.data[0];
        Charger1_LVBatVoltLSB = frame.data[1];
        Charger1_LVBatVoltDecimal = (Charger1_LVBatVoltMSB << 8) + Charger1_LVBatVoltLSB; // divide by 100 for 2 dec place voltage.
        Charger1_LVSupplyCurrentMSB = frame.data[2];
        Charger1_LVSupplyCurrentLSB =  frame.data[3];
        Charger1_LVSupplyCurrentDecimal = (Charger1_LVSupplyCurrentMSB << 8) + Charger1_LVSupplyCurrentLSB; // divide by ten to give current 
        Charger1_DCDC_Temp1 = frame.data[4]; // Temp 1, -40 to give value (range --40 to 215c)
        Charger1_DCDC_Temp2 = frame.data[5]; // Temp 2, -40 to give value (range --40 to 215c)
        Charger1_DCDC_Temp3 = frame.data[6]; // Temp 3, -40 to give value (range --40 to 215c)
        Charger1_DCDC_Status = frame.data[7]; // 0x20 = 00100000 Standby, 0x21 = 00100001 Error, 0x22 = 00100010 = Operation 
      }
      if (frame.id==0x389) {  // As per DBC file on OpenInverter.org
        Charger1_Last_0x389=millis();
        Charger1_HVBatVolt = frame.data[0]; // Byte 0 is HV Battery Voltage (double it)
        Charger1_ACVolt = frame.data[1]; // Byte 1 is AC Voltage (0-255v)
        Charger1_DCCurrent = frame.data[2]; // Byte 2 is DC Charge current (times by 10)
        Charger1_Charger_Temp1 = frame.data[3]; // Byte 3 is the Charger Temp 1 (-40) range -40c - 215c
        Charger1_Charger_Temp2 = frame.data[4]; // Byte 3 is the Charger Temp 1 (-40) range -40c - 215c
        Charger1_Status_PilotPresent = bitRead(frame.data[5],7); // bit 7
        Charger1_Status_DCDCRequest = bitRead(frame.data[5],6); // bit 6 
        Charger1_Status_Error = bitRead(frame.data[5],4); // bit 4
        Charger1_Status_Charging = bitRead(frame.data[5],3); // bit 3
        //Charger1_ACCurrent = frame.data[6];
        //Charger1_DCCurrent = frame.data[7];


      }
      if (frame.id==0x38A) { // As per DBC File on OpenInverter.org
        Charger1_Last_0x38A=millis();
        Charger1_ChargerUnit_Temp1 = frame.data[0]; // Temp 1, -40 to give value (range --40 to 215c)
        Charger1_ChargerUnit_Temp2 = frame.data[1]; // Temp 2, -40 to give value (range --40 to 215c)
        Charger1_DCBusVoltage = frame.data[2]; // DC Bus Voltage, double it.
        Charger1_EVSE_Pilotduty = frame.data[3]; // Percentage
        Charger1_Status = frame.data[4]; // 0x00 Ready, 0x04 Wait for Mains Voltage, 0x08 Ready for Charging, 0x00 No Input
      }
    }

  } else if (CANCycle==2) {
    CAN2.poll () ; // No interrupt, call this function as often as possible
    if (CAN2.available ()) {
    if (frame.id==0x377) { // as per DBC file on OpenInverter.org
        Charger2_Last_0x377=millis();
        Charger2_LVBatVoltMSB = frame.data[0];
        Charger2_LVBatVoltLSB = frame.data[1];
        Charger2_LVBatVoltDecimal = (Charger1_LVBatVoltMSB << 8) + Charger1_LVBatVoltLSB; // divide by 100 for 2 dec place voltage.
        Charger2_LVSupplyCurrentMSB = frame.data[2];
        Charger2_LVSupplyCurrentLSB =  frame.data[3];
        Charger2_LVSupplyCurrentDecimal = (Charger1_LVSupplyCurrentMSB << 8) + Charger1_LVSupplyCurrentLSB; // divide by ten to give current 
        Charger2_DCDC_Temp1 = frame.data[4]; // Temp 1, -40 to give value (range --40 to 215c)
        Charger2_DCDC_Temp2 = frame.data[5]; // Temp 2, -40 to give value (range --40 to 215c)
        Charger2_DCDC_Temp3 = frame.data[6]; // Temp 3, -40 to give value (range --40 to 215c)
        Charger2_DCDC_Status = frame.data[7]; // 0x20 = 00100000 Standby, 0x21 = 00100001 Error, 0x22 = 00100010 = Operation 
      }
      if (frame.id==0x389) {  // As per DBC file on OpenInverter.org
        Charger2_Last_0x389=millis();
        Charger2_HVBatVolt = frame.data[0]; // Byte 0 is HV Battery Voltage (double it)
        Charger2_ACVolt = frame.data[1]; // Byte 1 is AC Voltage (0-255v)
        Charger2_DCCurrent = frame.data[2]; // Byte 2 is DC Charge current (times by 10)
        Charger2_Charger_Temp1 = frame.data[3]; // Byte 3 is the Charger Temp 1 (-40) range -40c - 215c
        Charger2_Charger_Temp2 = frame.data[4]; // Byte 3 is the Charger Temp 1 (-40) range -40c - 215c
        Charger2_Status_PilotPresent = bitRead(frame.data[5],7); // bit 7
        Charger2_Status_DCDCRequest = bitRead(frame.data[5],6); // bit 6 
        Charger2_Status_Error = bitRead(frame.data[5],4); // bit 4
        Charger2_Status_Charging = bitRead(frame.data[5],3); // bit 3
        //Charger1_ACCurrent = frame.data[6];
        //Charger1_DCCurrent = frame.data[7];


      }
      if (frame.id==0x38A) { // As per DBC File on OpenInverter.org
        Charger2_Last_0x38A=millis();
        Charger2_ChargerUnit_Temp1 = frame.data[0]; // Temp 1, -40 to give value (range --40 to 215c)
        Charger2_ChargerUnit_Temp2 = frame.data[1]; // Temp 2, -40 to give value (range --40 to 215c)
        Charger2_DCBusVoltage = frame.data[2]; // DC Bus Voltage, double it.
        Charger2_EVSE_Pilotduty = frame.data[3]; // Percentage
        Charger2_Status = frame.data[4]; // 0x00 Ready, 0x04 Wait for Mains Voltage, 0x08 Ready for Charging, 0x00 No Input
      }
    }
  } else if (CANCycle==3) {
    CAN3.poll () ; // No interrupt, call this function as often as possible
    if (CAN3.available ()) {
      if (frame.id==0x377) { // as per DBC file on OpenInverter.org
        Charger3_Last_0x377=millis();
        Charger3_LVBatVoltMSB = frame.data[0];
        Charger3_LVBatVoltLSB = frame.data[1];
        Charger3_LVBatVoltDecimal = (Charger1_LVBatVoltMSB << 8) + Charger1_LVBatVoltLSB; // divide by 100 for 2 dec place voltage.
        Charger3_LVSupplyCurrentMSB = frame.data[2];
        Charger3_LVSupplyCurrentLSB =  frame.data[3];
        Charger3_LVSupplyCurrentDecimal = (Charger1_LVSupplyCurrentMSB << 8) + Charger1_LVSupplyCurrentLSB; // divide by ten to give current 
        Charger3_DCDC_Temp1 = frame.data[4]; // Temp 1, -40 to give value (range --40 to 215c)
        Charger3_DCDC_Temp2 = frame.data[5]; // Temp 2, -40 to give value (range --40 to 215c)
        Charger3_DCDC_Temp3 = frame.data[6]; // Temp 3, -40 to give value (range --40 to 215c)
        Charger3_DCDC_Status = frame.data[7]; // 0x20 = 00100000 Standby, 0x21 = 00100001 Error, 0x22 = 00100010 = Operation 
      }
      if (frame.id==0x389) {  // As per DBC file on OpenInverter.org
        Charger3_Last_0x389=millis();
        Charger3_HVBatVolt = frame.data[0]; // Byte 0 is HV Battery Voltage (double it)
        Charger3_ACVolt = frame.data[1]; // Byte 1 is AC Voltage (0-255v)
        Charger3_DCCurrent = frame.data[2]; // Byte 2 is DC Charge current (times by 10)
        Charger3_Charger_Temp1 = frame.data[3]; // Byte 3 is the Charger Temp 1 (-40) range -40c - 215c
        Charger3_Charger_Temp2 = frame.data[4]; // Byte 3 is the Charger Temp 1 (-40) range -40c - 215c
        Charger3_Status_PilotPresent = bitRead(frame.data[5],7); // bit 7
        Charger3_Status_DCDCRequest = bitRead(frame.data[5],6); // bit 6 
        Charger3_Status_Error = bitRead(frame.data[5],4); // bit 4
        Charger3_Status_Charging = bitRead(frame.data[5],3); // bit 3
        //Charger1_ACCurrent = frame.data[6];
        //Charger1_DCCurrent = frame.data[7];


      }
      if (frame.id==0x38A) { // As per DBC File on OpenInverter.org
        Charger3_Last_0x38A=millis();
        Charger3_ChargerUnit_Temp1 = frame.data[0]; // Temp 1, -40 to give value (range --40 to 215c)
        Charger3_ChargerUnit_Temp2 = frame.data[1]; // Temp 2, -40 to give value (range --40 to 215c)
        Charger3_DCBusVoltage = frame.data[2]; // DC Bus Voltage, double it.
        Charger3_EVSE_Pilotduty = frame.data[3]; // Percentage
        Charger3_Status = frame.data[4]; // 0x00 Ready, 0x04 Wait for Mains Voltage, 0x08 Ready for Charging, 0x00 No Input
      }
    }
  } else if (CANCycle==4) { // CHAdeMO
    CAN4.poll () ; // No interrupt, call this function as often as possible
    if (CAN4.available ()) {
      CAN4.receive (frame) ;
      if (frame.id==0x001) {
      }
    }
    CANCycle=0; // reset to zero, so next loop it's 1 again
  } 
  CANCycle++;

if ((millis() - lastCANSend) > 100) {
    

    outframe.id = ch1info1; //  This is the 0x377 Equivelant
    outframe.data[0] = Charger1_LVBatVoltMSB;
    outframe.data[1] = Charger1_LVBatVoltLSB;
    outframe.data[2] = Charger1_LVSupplyCurrentMSB;
    outframe.data[3] = Charger1_LVSupplyCurrentLSB;
    outframe.data[4] = Charger1_Temp1;
    outframe.data[5] = Charger1_Temp2;
    outframe.data[6] = Charger1_Temp3;
    outframe.data[7] = Charger1_Status;
    const bool ok1 = ACAN_ESP32::can.tryToSend (outframe) ;
    if (ok1) {
      //gSentFrameCount += 1 ;
    }

    outframe.id = ch2info1; //  This is the 0x377 Equivelant
    outframe.data[0] = Charger2_LVBatVoltMSB;
    outframe.data[1] = Charger2_LVBatVoltLSB;
    outframe.data[2] = Charger2_LVSupplyCurrentMSB;
    outframe.data[3] = Charger2_LVSupplyCurrentLSB;
    outframe.data[4] = Charger2_Temp1;
    outframe.data[5] = Charger2_Temp2;
    outframe.data[6] = Charger2_Temp3;
    outframe.data[7] = Charger2_Status;
    const bool ok2 = ACAN_ESP32::can.tryToSend (outframe) ;
    if (ok2) {
      //gSentFrameCount += 1 ;
    }

    outframe.id = ch3info1; //  This is the 0x377 Equivelant
    outframe.data[0] = Charger3_LVBatVoltMSB;
    outframe.data[1] = Charger3_LVBatVoltLSB;
    outframe.data[2] = Charger3_LVSupplyCurrentMSB;
    outframe.data[3] = Charger3_LVSupplyCurrentLSB;
    outframe.data[4] = Charger3_Temp1;
    outframe.data[5] = Charger3_Temp2;
    outframe.data[6] = Charger3_Temp3;
    outframe.data[7] = Charger3_Status;
    const bool ok3 = ACAN_ESP32::can.tryToSend (outframe) ;
    if (ok3) {
      //gSentFrameCount += 1 ;
    }
    
    outframe.id = ch1info2; //  This is the 0x389 Equivelant
    outframe.data[0] = Charger1_HVBatVolt;
    outframe.data[1] = Charger1_ACVolt;
    outframe.data[2] = 0x00;
    outframe.data[3] = 0x00;
    outframe.data[4] = 0x00;
    outframe.data[5] = 0x00;
    outframe.data[6] = Charger1_ACCurrent;
    outframe.data[7] = Charger1_DCCurrent;
    const bool ok4 = ACAN_ESP32::can.tryToSend (outframe) ;
    if (ok4) {
      //gSentFrameCount += 1 ;
    }

    outframe.id = ch2info2; //  This is the 0x389 Equivelant
    outframe.data[0] = Charger2_HVBatVolt;
    outframe.data[1] = Charger2_ACVolt;
    outframe.data[2] = 0x00;
    outframe.data[3] = 0x00;
    outframe.data[4] = 0x00;
    outframe.data[5] = 0x00;
    outframe.data[6] = Charger2_ACCurrent;
    outframe.data[7] = Charger2_DCCurrent;
    const bool ok5 = ACAN_ESP32::can.tryToSend (outframe) ;
    if (ok5) {
      //gSentFrameCount += 1 ;
    }

    outframe.id = ch3info2; //  This is the 0x389 Equivelant
    outframe.data[0] = Charger3_HVBatVolt;
    outframe.data[1] = Charger3_ACVolt;
    outframe.data[2] = 0x00;
    outframe.data[3] = 0x00;
    outframe.data[4] = 0x00;
    outframe.data[5] = 0x00;
    outframe.data[6] = Charger3_ACCurrent;
    outframe.data[7] = Charger3_DCCurrent;
    const bool ok6 = ACAN_ESP32::can.tryToSend (outframe) ;
    if (ok6) {
      //gSentFrameCount += 1 ;
    }

    outframe.id = ch1info3; //  This is the 0x38A Equivelant
    outframe.data[0] = 0x00;
    outframe.data[1] = 0x00;
    outframe.data[2] = 0x00;
    outframe.data[3] = Charger1_EVSE_Pilotduty;
    outframe.data[4] = 0x00;
    outframe.data[5] = 0x00;
    outframe.data[6] = 0x00;
    outframe.data[7] = 0x00;
    const bool ok7 = ACAN_ESP32::can.tryToSend (outframe) ;
    if (ok7) {
      //gSentFrameCount += 1 ;
    }

    outframe.id = ch2info3; //  This is the 0x38A Equivelant
    outframe.data[0] = 0x00;
    outframe.data[1] = 0x00;
    outframe.data[2] = 0x00;
    outframe.data[3] = Charger1_EVSE_Pilotduty;
    outframe.data[4] = 0x00;
    outframe.data[5] = 0x00;
    outframe.data[6] = 0x00;
    outframe.data[7] = 0x00;
    const bool ok8 = ACAN_ESP32::can.tryToSend (outframe) ;
    if (ok8) {
      //gSentFrameCount += 1 ;
    }

    outframe.id = ch3info3; //  This is the 0x38A Equivelant
    outframe.data[0] = 0x00;
    outframe.data[1] = 0x00;
    outframe.data[2] = 0x00;
    outframe.data[3] = Charger1_EVSE_Pilotduty;
    outframe.data[4] = 0x00;
    outframe.data[5] = 0x00;
    outframe.data[6] = 0x00;
    outframe.data[7] = 0x00;
    const bool ok9 = ACAN_ESP32::can.tryToSend (outframe) ;
    if (ok9) {
      //gSentFrameCount += 1 ;
    }

    TotalHVDCCurrent = Charger1_DCCurrent+Charger2_DCCurrent+Charger3_DCCurrent;
    

    // Total Bank Figures.
    outframe.id = summary1; //  This is the Total Figures
    outframe.data[0] = highByte(TotalHVDCCurrent); // HV DC Current From all three chargers
    outframe.data[1] = lowByte(TotalHVDCCurrent); // HV DC Current From all three chargers
    outframe.data[2] = 0x00; //
    outframe.data[3] = 0x00;
    outframe.data[4] = 0x00;
    outframe.data[5] = 0x00;
    outframe.data[6] = 0x00;
    outframe.data[7] = 0x00;
    const bool ok10 = ACAN_ESP32::can.tryToSend (outframe) ;
    if (ok10) {
      //gSentFrameCount += 1 ;
    }
    lastCANSend = millis();

}


}
