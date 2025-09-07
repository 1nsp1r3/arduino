/**
 * [0,9,12] -> "00090c"
 */
String i3_hexStringFromByteArray(const uint8_t* Data, uint8_t Length){
  String ret = "";
  for (uint8_t i=0;i<Length;i++){
    if (Data[i]<16){
      ret += "0";
    }
    ret += String(Data[i], HEX);
    ret += " ";
  }
  ret.trim();
  return ret;
}

/**
 * [0a,0b] -> 0xba -> 186
 */
int16_t i3_hexExtractInt16Value(String Data){
  const uint8_t* data = (const uint8_t*)Data.c_str();
  //Serial.printf("%s\n", i3_byteArrayToHexString(data, 2).c_str());
  return (data[1] << 8) | data[0]; //Little endian
}
