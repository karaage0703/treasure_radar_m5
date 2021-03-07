#include <NimBLEDevice.h>
#include <M5Stack.h>

const char* deviceName = "Mi Smart Band 4";

int scanTime = 5; //In seconds
int brightness = 200;
BLEScan* pBLEScan;
int rssi = 0;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  /*** Only a reference to the advertised device is passed now
    void onResult(BLEAdvertisedDevice advertisedDevice) { **/
    void onResult(BLEAdvertisedDevice* advertisedDevice) {
  /** Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str()); **/
  //    Serial.printf("Advertised Device: %s \n", advertisedDevice->toString().c_str());
  //    rssi = advertisedDevice->getRSSI();
  //    Serial.println(rssi);
      if(strncmp(advertisedDevice->getName().c_str(), deviceName, 15) == 0){
        Serial.print("RSSI: ");
        rssi = advertisedDevice->getRSSI();
        Serial.println(rssi);
      }
    }
};

void setup() {
  M5.begin();
  // Serial.begin(115200); 
  M5.Power.begin();

  M5.Lcd.setRotation(1);
  M5.Lcd.setBrightness(brightness);

  Serial.println("Scanning...");


  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void drawScreen() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.print("Treasure Radar\n");

    M5.Lcd.setCursor(0, 30);
    M5.Lcd.setTextSize(7);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.printf(" %2d",rssi);

    if (M5.Power.canControl()) {
      M5.Lcd.setTextSize(1);
      M5.Lcd.setCursor(0, M5.Lcd.height() - 10);
      M5.Lcd.printf("Battery:%d%% ", M5.Power.getBatteryLevel());
      if (M5.Power.isCharging()) {
        M5.Lcd.printf("(Charging)");
      }
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  drawScreen();
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}
