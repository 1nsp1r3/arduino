#include <BLEDevice.h>

typedef void (*GapDataCallBack)(String);
typedef std::map<const char*, GapDataCallBack> GapDataCallBackMap;

String             deviceAddressFilter;
BLEScan*           bleScan;
GapDataCallBackMap gapDataCallBackMap;

/**
 *
 */
GapDataCallBackMap::iterator i3_bleFindGapDataCallBack(const char* Service){
  GapDataCallBackMap::iterator it = gapDataCallBackMap.begin();
  while (it != gapDataCallBackMap.end()){
    if (strcmp(it->first, Service) == 0) return it;
    it++;
  }
  return gapDataCallBackMap.begin();
}

/**
 * A noter que les méthodes getPayload() & getPayloadLength() de l'objet BLEAdvertisedDevice
 * me retourne dans 95% des cas une payload dont je n'ai pas la moindre idée à quoi elle correspond
 *
 * Pour avoir la vraie payload des advertising, il faut utiliser getServiceDataUUID(i) & getServiceData(i) comme ci-dessous
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks{
  void onResult(BLEAdvertisedDevice bleAdvertisedDevice){
    String deviceFoundAddress = bleAdvertisedDevice.getAddress().toString();

    if (deviceFoundAddress.equalsIgnoreCase(deviceAddressFilter)){ //Focus only on a specific BLE device
      int count = bleAdvertisedDevice.getServiceDataUUIDCount();
      for (int i=0;i<count;i++){
        BLEUUID serviceDataUUID = bleAdvertisedDevice.getServiceDataUUID(i);
        const char* serviceUuid = serviceDataUUID.toString().c_str();
        //Serial.printf("UUID: %s\n", serviceUuid);

        //For each gap service, call the callback with the data
        GapDataCallBackMap::iterator it = i3_bleFindGapDataCallBack(serviceUuid);
        it->second(
          bleAdvertisedDevice.getServiceData(i) //String
        );
      }
    }
  }
};

/**
 *
 */
void i3_bleInit(const char* DeviceName, String DeviceAddressFilter){
  deviceAddressFilter = DeviceAddressFilter;

  BLEDevice::init(DeviceName);
  bleScan = BLEDevice::getScan();
  bleScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  bleScan->setActiveScan(true);
}

/**
 *
 */
void i3_bleScan(){
  bleScan->start(
    1, //1s
    false
  );
}

/**
 *
 */
void i3_bleAddGapDataCallBack(const char* Service, GapDataCallBack GapDataCallBack){
  gapDataCallBackMap.insert(GapDataCallBackMap::value_type(Service, GapDataCallBack));
}
