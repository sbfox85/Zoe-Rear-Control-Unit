# Zoe-Rear-Control-Unit
Rear Control Unit for Tesla-Zoe. Includes AC charger functions and cooling.

It uses an ESP32 S3 Mini as the MCU, and uses the onboard CAN for vehicle communication, with 4 additional CAN channels (3x AC Chargers plus CHAdeMO) added using 4x MCP25625 Combined Controller and Tranceiver. It has CHAdeMO digital IO included. It also has a Current Limited PWM Fan Driver DRC8871DDAR, and MIC4422YM Mosfet for PWM Pump Control.


# Functions
## Thermal Management
This should be active all the time that the controller is awake. If the DC-DC is enabled, or the charger is outputting, we should circulate coolant at a steady rate. Fan should be operated based on temperature. Pump and Fan should run-on after shutdown of charger or dc-dc for a defined time.

## AC Charger Management
This should have a master and slave mode. Slave listens for CAN instructions, master makes the decision and broadcasts to the slave device.

### 3/1 Phase AC Charging with front socket.
The Pilot value is received from FOCCCI. It is divided by 2 in order to give the per-unit value. Ie 32amp pilot means 16amp per charging unit. The 3 Pilots on each controller (master/slave) should be set to 16amp. charging should begin.  

## CHAdeMO.
We provide basic CHAdeMO function over a dedicated CAN channel. For this function to work no other charging method should be plugged in. This function should block other charging methods. It should be limited to 125amps.

## Management Webpage.
The Master/Slave unit should provide a webpage that allows values to be altered, and settings to be configured.

# Configurable Settings

| Name | Possible Values | Description |
|------|-------------|--------------|
|CHAD_MaxkW| 1-50| Maximum kW Possible (subject to BMS) |
|CHAD_MinkW| 1-50| Minimum kW Before Ending Charge - don't set too high |
|CHAD_MinAmps| 1-100| Minimum Amps Before Ending Charge |
|CHAD_MaxAmps| 1-200| Max Amps Possible (subject to BMS) Hardware Limit of 125 on Leaf Socket |
|CHAD_MaxPercent| 50-100 | Max Percent Before Ending Charge |
|CHAD_MinPercent| 0-20 | Minimum SOC to allow a standard start (supervised override possible) |
|CHAD_MaxVoltage| 300-450 | Max Pack Voltage Before Ending Charge |
|CHAD_MinVoltage| 200-350 | Min Pack Voltage to allow standard start (supervised override possible) |
|CHAD_MaxDelta| 10-1000 | Maximum Millivolts between lowest cell and highest cell to allow standard start |
|CHAD_MaxDuration| 0-300 | Maximum Minutes a CHAdeMO session can last before termination |
|CHAD_MaxCellTempEnd | 20-50 | Maximum Cell Temp before termination |
|CHAD_MaxCellTempBegin | 20-50 | Maximum Cell Temp allowed to start charge |
|AC_MaxTotalkW| 1-23 | Maximum kW Allowed for AC charging. Default 23kW |
|AC_MinTotalkW| 1-10 | Minimum kW Allowed for AC charging to continue. Default 1kW |
|AC_MaxDeltaStart| 10-1000 | Maximum Millivolts between lowest and highest cell to allow charge to start |
|AC_MaxDeltaEnd| 10-1000 | Maximum Millivolts between lowest and highest cell to terminate charging |
|AC_MaxCellVoltageEnd| 1000-4300 | Maximum Millivolts of one cell to end charging. |
|AC_MaxCellVoltageStart| 1000-4300 | Maximum Millivolts of one cell to allow charging to start |
|AC_MaxSOCStandardEnd | 7500-10000 | SOC (*100) At Which to Terminate charging on a standard charge. 80% Default. |
|AC_MaxSOCOverideEnd | 7500-10000 | SOC (*100) At which to Terminate charging on a FULL Override Charge. 100% Default. |
|AC_MaxPackVoltageStart | 3500-4500 | Max Pack Voltage (*10) to allow Starting of charging |
|AC_MaxPackVoltageEnd | 3500-4500 | Max Pack Voltage (*10) when charging is terminated |
|DCDC_AutoStartVoltage | 1000-1500 | Voltage (*100) when DC-DC should auto start to prevent flat 12v Battery |
|DCDC_AutoStartTimout | 1-60 | Time in minutes to run DCDC when autostarted due to low 12v Battery |
|DCDC_IgnitionTimeout | 0-600 | Time in seconds to run DCDC after ignition state changed to off |
|DCDC_ChargeTimeout | 0-600 | Time in seconds to run DCDC after charging session has ended |
|DCDC_WakeTimeout | 0-600 | Time in seconds to run DCDC after vehicle wakeup (but no ignition)
|DCDC_MaxVoltage | 1200-1600 | Max Voltage (*100) When Operational before Error |
|DCDC_MinVoltage | 1200-1600 | Min Voltage (*100) When Operation before Error |
|DCDC_MaxTemp | 300-950 | Max Temp (*10) before error and shutdown of DCDC |
|Pump_IdleSpeed | | Speed for awake but not doing anything, can be set to 0 |
|Pump_ChargeSpeed | | Speed whilst charger running |
|Pump_DCDCSpeed | | Speed whilst dc-dc only running |
|Pump_MaxSpeed | | Max Speed - when any charger in circuit is overtemp threshold |
|Pump_BleedSpeed | | Speed to bleed |
|Pump_Timeout | 0-300 | Timeout in seconds after charging / dc-dc event is ended |
|Pump_MaxThreshold | 400-800 | Temperature (*10) at which maximum speed is engaged |
|FanStage1Speed | 1-100 | Speed Percent when in stage 1 range |
|FanStage1Temp | 0-1000 | Temp (*10) at which Stage 1 Starts |
|FanStage2Speed | 1-100 | Speed Percent when in stage 2 range |
|FanStage2Temp | 0-1000 | Temp (*10) at which Stage 2 Starts |
|FanStage3Speed | 1-100 | Speed Percent when in stage 3 range |
|FanStage3Temp | 0-1000 | Temp (*10) at which Stage 3 Starts |
|FanHysteresis | 0-100 | Temp (*10) below each stage threshold that it should switch off |
|Fan_Timeout | 0-300 | Seconds fan should run-on after charge/dcdc switched off, can be 0 |
# ESP32 Pin Assignment

| ESP32 Pin | Function Description | Details |
|-----------|----------------------|---------|
| IO0 | Boot |  |
| IO1 | PWM Measure Pin 1| |
| IO2 | PWM Measure Pin 2| |
| IO3 | Not Used | |
| IO4 | CAN0 Tx | |
| IO5 | CAN0 Rx | |
| IO6 | CS4 | |
| IO7 | CS2 | |
| IO8 | CHG_CAN2_Sby | |
| IO9 | DC-DC En1 | |
| IO10 | DC-DC En2 | |
| IO11 | MOSI | |
| IO12 | SCK | |
| IO13 | MISO | |
| IO14 | CHAD CAN4 Sby | |
| IO15 | CS3 | |
| IO16 | CHG CAN3 Sby | |
| IO17 | CS1 | |
| IO18 | CHG CAN1 Sby | |
| IO21 | Heater2 | |
| IO26 | Not Used | |
| IO33 | Out Charge En | |
| IO34 | IN1 | |
| IO35 | IN2 | |
| IO36 | IGCT Pin | |
| IO37 | CP3 Out PWM | |
| IO38 | CP2 Out PWM | |
| IO39 | CP1 Out PWM | |
| IO40 | Fan Control Pin 1| |
| IO41 | Fan Control Pin 2| |
| IO42 | PWM Pump | |
| IO45 | Not Used | |
| IO46 | Not Used | |
| IO47 | Out Contactors| |
| IO48 | Heater 1 Pin | |
