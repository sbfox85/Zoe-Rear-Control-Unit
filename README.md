# Zoe-Rear-Control-Unit
Rear Control Unit for Tesla-Zoe. Includes AC charger functions and cooling.



# Functions
## Thermal Management
This should be active all the time that the controller is awake. If the DC-DC is enabled, or the charger is outputting, we should circulate coolant at a steady rate. Fan should be operated based on temperature. Pump and Fan should run-on after shutdown of charger or dc-dc for a defined time.

## AC Charger Management
This should have a master and slave mode. Slave listens for CAN instructions, master makes the decision and broadcasts to the slave device.

### 3/1 Phase AC Charging with front socket.
The Pilot value is received from FOCCCI. It is divided by 2 in order to give the per-unit value. Ie 32amp pilot means 16amp per charging unit. The 3 Pilots on each controller (master/slave) should be set to 16amp. charging should begin.  

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
