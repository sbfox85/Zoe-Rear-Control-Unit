/*

SETTINGS for Rear Control Unit


*/

void readsettings() {
  DynamicJsonDocument settings(6000); // needs calculating.

  File file = LittleFS.open("/settings.json");
  DeserializationError err = deserializeJson(settings, file);
  if (err) { 
      sendserial("*** ERROR Reading Controller Settings File ***"); 
      sendserial("Saving Current Settings Again (due to error)...");
      writesettings();
    } else {
      sendserial("SYSTEM: Loaded Controller Settings from file");
    }
  file.close();


  JsonVariant error;

/*  
 */
 
 
 error = settings["CHAD_MaxkW"];  // CHAD_MaxkW
  if (!error.isNull()) { settings.CHAD_MaxkW = settings["CHAD_MaxkW"]; } 

 error = settings["CHAD_MinkW"];  // CHAD_MinkW
  if (!error.isNull()) { settings.CHAD_MinkW = settings["CHAD_MinkW"]; } 

  error = settings["CHAD_MinAmps"];  // CHAD_MinAmps
  if (!error.isNull()) { settings.CHAD_MinAmps = settings["CHAD_MinAmps"]; } 

  error = settings["CHAD_MaxAmps"];  // CHAD_MaxAmps
  if (!error.isNull()) { settings.CHAD_MaxAmps = settings["CHAD_MaxAmps"]; } 
    
  error = settings["CHAD_MaxPercent"];  // CHAD_MaxPercent
  if (!error.isNull()) { settings.CHAD_MaxPercent = settings["CHAD_MaxPercent"]; } 

  error = settings["CHAD_MinPercent"];  // CHAD_MinPercent
  if (!error.isNull()) { settings.CHAD_MinPercent = settings["CHAD_MinPercent"]; } 

  error = settings["CHAD_MaxVoltage"];  // CHAD_MaxVoltage
  if (!error.isNull()) { settings.CHAD_MaxVoltage = settings["CHAD_MaxVoltage"]; } 

  error = settings["CHAD_MinVoltage"];  // CHAD_MinVoltage
  if (!error.isNull()) { settings.CHAD_MinVoltage = settings["CHAD_MinVoltage"]; } 

  error = settings["CHAD_MaxDelta"];  // CHAD_MaxDelta
  if (!error.isNull()) { settings.CHAD_MaxDelta = settings["CHAD_MaxDelta"]; } 

  error = settings["CHAD_MaxDuration"];  // CHAD_MaxDuration
  if (!error.isNull()) { settings.CHAD_MaxDuration = settings["CHAD_MaxDuration"]; } 

  error = settings["CHAD_MaxCellTempEnd"];  // CHAD_MaxCellTempEnd
  if (!error.isNull()) { settings.CHAD_MaxCellTempEnd = settings["CHAD_MaxCellTempEnd"]; } 

  error = settings["CHAD_MaxCellTempBegin	"];  // CHADMaxCellTempBegin	_
  if (!error.isNull()) { settings.CHAD_MaxCellTempBegin	 = settings["CHAD_MaxCellTempBegin	"]; } 

  error = settings["CHAD_BatteryHeatThreshold"];  // CHAD_BatteryHeatThreshold below this temp, battery heating requested
  if (!error.isNull()) { settings.CHAD_BatteryHeatThreshold = settings["CHAD_BatteryHeatThreshold"]; } 

  error = settings["CHAD_BatteryBlowerThreshold"];  // CHAD_BatteryBlowerThreshold above this temp, battery fan requested
  if (!error.isNull()) { settings.CHAD_BatteryBlowerThreshold = settings["CHAD_BatteryBlowerThreshold"]; } 

  error = settings["CHAD_BatteryCoolThreshold"];  // CHAD_BatteryCoolThreshold above this temp, refrigerant cooling requested.
  if (!error.isNull()) { settings.CHAD_BatteryCoolThreshold = settings["CHAD_BatteryCoolThreshold"]; } 

  error = settings["AC_MaxTotalkW"];  // AC_MaxTotalkW
  if (!error.isNull()) { settings.AC_MaxTotalkW = settings["AC_MaxTotalkW"]; } 

  error = settings["AC_MinTotalkW"];  // AC_MinTotalkW
  if (!error.isNull()) { settings.AC_MinTotalkW = settings["AC_MinTotalkW"]; } 

  error = settings["AC_MaxDeltaStart"];  // AC_MaxDeltaStart
  if (!error.isNull()) { settings.AC_MaxDeltaStart = settings["AC_MaxDeltaStart"]; } 

  error = settings["AC_MaxDeltaEnd"];  // AC_MaxDeltaEnd
  if (!error.isNull()) { settings.AC_MaxDeltaEnd = settings["AC_MaxDeltaEnd"]; } 

  error = settings["AC_MaxCellVoltageEnd"];  // AC_MaxCellVoltageEnd
  if (!error.isNull()) { settings.AC_MaxCellVoltageEnd = settings["AC_MaxCellVoltageEnd"]; } 

  error = settings["AC_MaxCellVoltageStart"];  // AC_MaxCellVoltageStart
  if (!error.isNull()) { settings.AC_MaxCellVoltageStart = settings["AC_MaxCellVoltageStart"]; } 

  error = settings["AC_MaxSOCStandardEnd"];  // AC_MaxSOCStandardEnd
  if (!error.isNull()) { settings.AC_MaxSOCStandardEnd = settings["AC_MaxSOCStandardEnd"]; } 

  error = settings["AC_MaxSOCOverideEnd"];  // AC_MaxSOCOverideEnd
  if (!error.isNull()) { settings.AC_MaxSOCOverideEnd = settings["AC_MaxSOCOverideEnd"]; } 

  error = settings["AC_MaxPackVoltageStart"];  // AC_MaxPackVoltageStart
  if (!error.isNull()) { settings.AC_MaxPackVoltageStart = settings["AC_MaxPackVoltageStart"]; } 

  error = settings["AC_MaxPackVoltageEnd"];  // AC_MaxPackVoltageEnd
  if (!error.isNull()) { settings.AC_MaxPackVoltageEnd = settings["AC_MaxPackVoltageEnd"]; } 

  error = settings["DCDC_AutoStartVoltage"];  // DCDC_AutoStartVoltage
  if (!error.isNull()) { settings.DCDC_AutoStartVoltage = settings["DCDC_AutoStartVoltage"]; } 

  error = settings["DCDC_AutoStartTimout"];  // DCDC_AutoStartTimout
  if (!error.isNull()) { settings.DCDC_AutoStartTimout = settings["DCDC_AutoStartTimout"]; } 

  error = settings["DCDC_IgnitionTimeout"];  // DCDC_IgnitionTimeout
  if (!error.isNull()) { settings.DCDC_IgnitionTimeout = settings["DCDC_IgnitionTimeout"]; } 

  error = settings["DCDC_ChargeTimeout"];  // DCDC_ChargeTimeout
  if (!error.isNull()) { settings.DCDC_ChargeTimeout = settings["DCDC_ChargeTimeout"]; } 

  error = settings["DCDC_WakeTimeout"];  // DCDC_WakeTimeout
  if (!error.isNull()) { settings.DCDC_WakeTimeout = settings["DCDC_WakeTimeout"]; } 

  error = settings["DCDC_MaxVoltage"];  // DCDC_MaxVoltage
  if (!error.isNull()) { settings.DCDC_MaxVoltage = settings["DCDC_MaxVoltage"]; } 

  error = settings["DCDC_MinVoltage"];  // DCDC_MinVoltage
  if (!error.isNull()) { settings.DCDC_MinVoltage = settings["DCDC_MinVoltage"]; } 

  error = settings["DCDC_MaxTemp"];  // DCDC_MaxTemp
  if (!error.isNull()) { settings.DCDC_MaxTemp = settings["DCDC_MaxTemp"]; } 

  error = settings["Pump_IdleSpeed"];  // Pump_IdleSpeed
  if (!error.isNull()) { settings.Pump_IdleSpeed = settings["Pump_IdleSpeed"]; } 

  error = settings["Pump_ChargeSpeed"];  // Pump_ChargeSpeed
  if (!error.isNull()) { settings.Pump_ChargeSpeed = settings["Pump_ChargeSpeed"]; } 

  error = settings["Pump_DCDCSpeed"];  // Pump_DCDCSpeed
  if (!error.isNull()) { settings.Pump_DCDCSpeed = settings["Pump_DCDCSpeed"]; } 

  error = settings["Pump_MaxSpeed"];  // Pump_MaxSpeed
  if (!error.isNull()) { settings.Pump_MaxSpeed = settings["Pump_MaxSpeed"]; } 

  error = settings["Pump_BleedSpeed"];  // Pump_BleedSpeed
  if (!error.isNull()) { settings.Pump_BleedSpeed = settings["Pump_BleedSpeed"]; } 

  error = settings["Pump_Timeout"];  // Pump_Timeout
  if (!error.isNull()) { settings.Pump_Timeout = settings["Pump_Timeout"]; } 

  error = settings["Pump_MaxThreshold"];  // Pump_MaxThreshold
  if (!error.isNull()) { settings.Pump_MaxThreshold = settings["Pump_MaxThreshold"]; } 

  error = settings["Fan_Stage1Speed"];  // Fan_Stage1Speed
  if (!error.isNull()) { settings.Fan_Stage1Speed = settings["Fan_Stage1Speed"]; } 

  error = settings["Fan_Stage1Temp"];  // Fan_Stage1Temp
  if (!error.isNull()) { settings.Fan_Stage1Temp = settings["Fan_Stage1Temp"]; } 

  error = settings["Fan_Stage2Speed"];  // Fan_Stage2Speed
  if (!error.isNull()) { settings.Fan_Stage2Speed = settings["Fan_Stage2Speed"]; } 

  error = settings["Fan_Stage2Temp"];  // Fan_Stage2Temp
  if (!error.isNull()) { settings.Fan_Stage2Temp = settings["Fan_Stage2Temp"]; } 

  error = settings["Fan_Stage3Speed"];  // Fan_Stage3Speed
  if (!error.isNull()) { settings.Fan_Stage3Speed = settings["Fan_Stage3Speed"]; } 

  error = settings["Fan_Stage3Temp"];  // Fan_Stage3Temp
  if (!error.isNull()) { settings.Fan_Stage3Temp = settings["Fan_Stage3Temp"]; } 

  error = settings["Fan_Hysteresis"];  // Fan_Hysteresis
  if (!error.isNull()) { settings.Fan_Hysteresis = settings["Fan_Hysteresis"]; } 

  error = settings["Fan_Timeout"];  // Fan_Timeout
  if (!error.isNull()) { settings.Fan_Timeout = settings["Fan_Timeout"]; } 



}

void writesettings() { // Write the settings file back to the controller

  DynamicJsonDocument settingsw(6000);

  settingsw["CHAD_MaxkW"] = settings.CHAD_MaxkW;
  settingsw["CHAD_MinkW"] = settings.CHAD_MinkW;
  settingsw["CHAD_MinAmps"] = settings.CHAD_MinAmps;
  settingsw["CHAD_MaxAmps"] = settings.CHAD_MaxAmps;
  settingsw["CHAD_MaxPercent"] = settings.CHAD_MaxPercent;
  settingsw["CHAD_MinPercent"] = settings.CHAD_MinPercent;
  settingsw["CHAD_MaxVoltage"] = settings.CHAD_MaxVoltage;
  settingsw["CHAD_MinVoltage"] = settings.CHAD_MinVoltage;
  settingsw["CHAD_MaxDelta"] = settings.CHAD_MaxDelta;
  settingsw["CHAD_MaxDuration"] = settings.CHAD_MaxDuration;
  settingsw["CHAD_MaxCellTempEnd"] = settings.CHAD_MaxCellTempEnd;
  settingsw["CHAD_MaxCellTempBegin"] = settings.CHAD_MaxCellTempBegin;
  settingsw["CHAD_BatteryHeatThreshold""] = settings.CHAD_BatteryHeatThreshold";
  settingsw["CHAD_BatteryBlowerThreshold"] = settings.CHAD_BatteryBlowerThreshold;
  settingsw["CHAD_BatteryCoolThreshold"] = settings.CHAD_BatteryCoolThreshold;
  settingsw["AC_MaxTotalkW"] = settings.AC_MaxTotalkW;
  settingsw["AC_MinTotalkW"] = settings.AC_MinTotalkW;
  settingsw["AC_MaxDeltaStart"] = settings.AC_MaxDeltaStart;
  settingsw["AC_MaxDeltaEnd"] = settings.AC_MaxDeltaEnd;
  settingsw["AC_MaxCellVoltageEnd"] = settings.AC_MaxCellVoltageEnd;
  settingsw["AC_MaxCellVoltageStart"] = settings.AC_MaxCellVoltageStart;
  settingsw["AC_MaxSOCStandardEnd"] = settings.AC_MaxSOCStandardEnd;
  settingsw["AC_MaxSOCOverideEnd"] = settings.AC_MaxSOCOverideEnd;
  settingsw["AC_MaxPackVoltageStart"] = settings.AC_MaxPackVoltageStart;
  settingsw["AC_MaxPackVoltageEnd"] = settings.AC_MaxPackVoltageEnd;
  settingsw["DCDC_AutoStartVoltage"] = settings.DCDC_AutoStartVoltage;
  settingsw["DCDC_AutoStartTimout"] = settings.DCDC_AutoStartTimout;
  settingsw["DCDC_IgnitionTimeout"] = settings.DCDC_IgnitionTimeout;
  settingsw["DCDC_ChargeTimeout"] = settings.DCDC_ChargeTimeout;
  settingsw["DCDC_WakeTimeout"] = settings.DCDC_WakeTimeout;
  settingsw["DCDC_MaxVoltage"] = settings.DCDC_MaxVoltage;
  settingsw["DCDC_MinVoltage"] = settings.DCDC_MinVoltage;
  settingsw["DCDC_MaxTemp"] = settings.DCDC_MaxTemp;
  settingsw["Pump_IdleSpeed"] = settings.Pump_IdleSpeed;
  settingsw["Pump_ChargeSpeed"] = settings.Pump_ChargeSpeed;
  settingsw["Pump_DCDCSpeed"] = settings.Pump_DCDCSpeed;
  settingsw["Pump_MaxSpeed"] = settings.Pump_MaxSpeed;
  settingsw["Pump_BleedSpeed"] = settings.Pump_BleedSpeed;
  settingsw["Pump_Timeout"] = settings.Pump_Timeout;
  settingsw["Pump_MaxThreshold"] = settings.Pump_MaxThreshold;
  settingsw["Fan_Stage1Speed"] = settings.Fan_Stage1Speed;
  settingsw["Fan_Stage1Temp"] = settings.Fan_Stage1Temp;
  settingsw["Fan_Stage2Speed"] = settings.Fan_Stage2Speed;
  settingsw["Fan_Stage2Temp"] = settings.Fan_Stage2Temp;
  settingsw["Fan_Stage3Speed"] = settings.Fan_Stage3Speed;
  settingsw["Fan_Stage3Temp"] = settings.Fan_Stage3Temp;
  settingsw["Fan_Hysteresis"] = settings.Fan_Hysteresis;
  settingsw["Fan_Timeout"] = settings.Fan_Timeout;

  File file = LittleFS.open("/settings.json", FILE_WRITE);
    if(!file){
      sendserial("SYSTEM: Faied to open settings.json to write");  
    } else {
      sendserial("SYSTEM: Saved Settings File");
    }
  // Write a prettified JSON document to the file
  serializeJsonPretty(settingsw, file);
  file.close();
  //serializeJsonPretty(settingsw, Serial);
  //readsettings(); - we do this at reboot.
  Flag.writecontrollersettings=0;
}




/*

Last Values to Save at poweroff, and load at power on:

- Pack Voltage
- Max Charge will be set at 7kW temporarily and not loaded (as this value varies with temp etc)
- USOC
- CUV, BUV, EOCR, HVBOC, HVBOT, HVBOV, COV, 
- Max Power Out
- Max Regen should be set to 7kW until updated.
- SOH
- Min Batt Temp
- Max Batt Temp
- LBC RS
- Avail kWh
- Highest Cell Volt
- Lowest Cell Volt

*/
/*
void readlastvals() {
  DynamicJsonDocument lastvals(6000); // needs calculating.

  File file = LittleFS.open("/lastvals.json");
  DeserializationError err = deserializeJson(lastvals, file);
  if (err) { 
      sendserial("*** ERROR Reading Last Values File ***"); 
   
    
    } else {
      sendserial("SYSTEM: Loaded Last Values from file");
    }
  file.close();


  JsonVariant error;


  error = lastvals["PackVoltage"];  
  if (!error.isNull()) { lastknownvalues.PackVoltage = lastvals["PackVoltage"]; }

  error = lastvals["USOC"]; 
  if (!error.isNull()) { lastknownvalues.USOC = lastvals["USOC"]; }

  error = lastvals["CUV"]; 
  if (!error.isNull()) { lastknownvalues.CUV = lastvals["CUV"]; }

  error = lastvals["BUV"];  
  if (!error.isNull()) { lastknownvalues.BUV = lastvals["BUV"]; }

  error = lastvals["EOCR"];
  if (!error.isNull()) { lastknownvalues.EOCR = lastvals["EOCR"]; }

  error = lastvals["HVBOC"];  // 
  if (!error.isNull()) { lastknownvalues.HVBOC = lastvals["HVBOC"]; }

  error = lastvals["HVBOT"];  // 
  if (!error.isNull()) { lastknownvalues.HVBOT = lastvals["HVBOT"]; }

  error = lastvals["HVBOV"];  // 
  if (!error.isNull()) { lastknownvalues.HVBOV = lastvals["HVBOV"]; }

  error = lastvals["COV"];  // 
  if (!error.isNull()) { lastknownvalues.COV = lastvals["COV"]; }

  error = lastvals["MaxAvail"]; 
  if (!error.isNull()) { lastknownvalues.MaxAvail = lastvals["MaxAvail"]; }

  error = lastvals["MaxGen"];  // 
  if (!error.isNull()) { lastknownvalues.USOC = lastvals["MaxGen"]; }

  error = lastvals["SOH"];  // 
  if (!error.isNull()) { lastknownvalues.SOH = lastvals["SOH"]; }

  error = lastvals["MinCellTemp"];  // 
  if (!error.isNull()) { lastknownvalues.MinCellTemp = lastvals["MinCellTemp"]; }

  error = lastvals["MaxCellTemp"];  // 
  if (!error.isNull()) { lastknownvalues.MaxCellTemp = lastvals["MaxCellTemp"]; }

  error = lastvals["LBCRS"];  // 
  if (!error.isNull()) { lastknownvalues.LBCRS = lastvals["LBCRS"]; }

  error = lastvals["HVBS"];  // 
  if (!error.isNull()) { lastknownvalues.HVBS = lastvals["HVBS"]; }

  error = lastvals["kWhRemain"];  // 
  if (!error.isNull()) { lastknownvalues.kWhRemain = lastvals["kWhRemain"]; }

  error = lastvals["MaxCellVolt"];  // 
  if (!error.isNull()) { lastknownvalues.MaxCellVolt = lastvals["MaxCellVolt"]; }

 error = lastvals["MinCellVolt"];  // 
  if (!error.isNull()) { lastknownvalues.MinCellVolt = lastvals["MinCellVolt"]; }

}

void writelastvals() { // Write the last values before oweroff.

  DynamicJsonDocument lastvals(6000);

  lastvals["CHAD_MaxkW"] = batterydata.CHAD_MaxkW;
  lastvals["PackVoltage"] = batterydata.PackVoltage;
  lastvals["USOC"] = batterydata.USOC;
  lastvals["CUV"] = batterydata.CUV;
  lastvals["BUV"] = batterydata.BUV;
  lastvals["EOCR"] = batterydata.EOCR;
  lastvals["HVBOC"] = batterydata.HVBOC;
  lastvals["HVBOT"] = batterydata.HVBOT;
  lastvals["HVBOV"] = batterydata.HVBOV;
  lastvals["COV"] = batterydata.COV;
  lastvals["MaxAvail"] = batterydata.MaxAvail;
  lastvals["MaxGen"] = batterydata.MaxGen;
  lastvals["SOH"] = batterydata.SOH;
  lastvals["MinCellTemp"] = batterydata.MinCellTemp;
  lastvals["MaxCellTemp"] = batterydata.MaxCellTemp;
  lastvals["LBCRS"] = batterydata.LBCRS;
  lastvals["HVBS"] = batterydata.HVBS;
  lastvals["kWhRemain"] = batterydata.kWhRemain;
  lastvals["MaxCellVolt"] = batterydata.MaxCellVolt;
  lastvals["MinCellVolt"] = batterydata.MinCellVolt;
     
  File file = LittleFS.open("/lastvals.json", FILE_WRITE);
    if(!file){
      sendserial("SYSTEM: Faied to open lastvals.json to write");  
    } else {
      sendserial("SYSTEM: Saved Lastvals File");
    }
  // Write a prettified JSON document to the file
  serializeJsonPretty(lastvals, file);
  file.close();
}
*/