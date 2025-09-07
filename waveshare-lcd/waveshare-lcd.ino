#include <i3_lcd.h> //https://github.com/1nsp1r3/arduino-lib/blob/main/i3_lcd.h
#include <i3_ble.h> //https://github.com/1nsp1r3/arduino-lib/blob/main/i3_ble.h
#include <i3_hex.h> //https://github.com/1nsp1r3/arduino-lib/blob/main/i3_hex.h

String boshSensorAddress = String("3c:84:27:ca:6e:4a"); //Sonde Régis
//String boshSensorAddress = String("?"); //Sonde Julien
//String boshSensorAddress = String("ed:bf:70:51:36:c7"); //PixlJs

char temperatureText[4] = "";
char pressureText[4] = "";

/**
 * 2589 -> 25.89 -> 26
 */
int8_t roundToInt8(int16_t Value){
  float f = Value / (float)100;    //2589  -> 25.89  
  int8_t ret = (int8_t)(f + 0.5f); //25.89 -> 26.39 -> 26
  return ret;  
}

/**
 * 7599 -> 75.99 psi -> 5.2 bars
 */
float roundToFloat1(int16_t Value){
  float f = Value / (float)100;    //7599      -> 75.99 psi
  f /= 14.504;                     //75.99 psi -> 5.239 bars
  f *= 10;                         //5.239     -> 52.39
  int8_t ret = (int8_t)(f + 0.5f); //52.39     -> 52.89 -> 52  
  return ret / (float)10;  
}

void onTemperatureData(String Data){
  int16_t rawValue = i3_hexExtractInt16Value(Data);
  //Serial.printf("Temperature: %d\n", rawValue);
  sprintf(temperatureText, "%d", roundToInt8(rawValue));  
}

void onPressureData(String Data){
  int16_t rawValue = i3_hexExtractInt16Value(Data);
  //Serial.printf("Pressure: %d\n", rawValue);
  sprintf(pressureText, "%.1f", roundToFloat1(rawValue));  

  refreshScreen();
}

void refreshScreen(){
  i3_lcdClear();
  i3_lcdText(0, 0, YELLOW, 17, temperatureText);
  i3_lcdText(100, 145, RED, 12, pressureText);
}

void setup(void){
  //Serial.begin(115200);
  //Serial.println("Bonjour !");

  i3_lcdInit();
  i3_lcdClear();
  i3_lcdText(0, 0, WHITE, 1, "Searching sensor...");

  i3_bleInit("LCD", boshSensorAddress);
  i3_bleAddGapDataCallBack("00001809-0000-1000-8000-00805f9b34fb", onTemperatureData);
  i3_bleAddGapDataCallBack("00002a6d-0000-1000-8000-00805f9b34fb", onPressureData);
}

void loop(){
  i3_bleScan();  
}
