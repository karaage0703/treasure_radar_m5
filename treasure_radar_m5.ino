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
  M5.Speaker.begin();
  M5.Power.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setBrightness(brightness);

  Serial.println("Scanning...");

  // Bluetooth Setup
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void drawSimple() {
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

void drawLadar() {
    if (rssi > -50){
        M5.Lcd.fillCircle((M5.Lcd.width() - 1)/2, (M5.Lcd.height() - 1)/2, 30, YELLOW);
        ledcSetup(TONE_PIN_CHANNEL, 8800, 13);
        ledcWrite(TONE_PIN_CHANNEL, 0x1FF>>(9-7));
        delay(400);
        M5.Speaker.mute();
    }

    if (rssi > -70 && rssi <= -50){
        M5.Lcd.fillCircle((M5.Lcd.width() - 1)/2, (M5.Lcd.height() - 1)/2, 10, YELLOW);
        ledcSetup(TONE_PIN_CHANNEL, 4400, 13);
        ledcWrite(TONE_PIN_CHANNEL, 0x1FF>>(9-5));
        delay(200);
        M5.Speaker.mute();
    }

    if (rssi > -90 && rssi <= -70){
        M5.Lcd.fillCircle((M5.Lcd.width() - 1)/2, (M5.Lcd.height() - 1)/2, 10, YELLOW);
        ledcSetup(TONE_PIN_CHANNEL, 2200, 13);
        ledcWrite(TONE_PIN_CHANNEL, 0x1FF>>(9-3));
        delay(100);
        M5.Speaker.mute();
    }

    if (rssi <= -90){
        M5.Lcd.fillCircle((M5.Lcd.width() - 1)/2, (M5.Lcd.height() - 1)/2, 5, YELLOW);
        ledcSetup(TONE_PIN_CHANNEL, 1100, 13);
        ledcWrite(TONE_PIN_CHANNEL, 0x1FF>>(9-1));
        delay(50);
        M5.Speaker.mute();
    }
}


void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  M5.Lcd.fillScreen(GREEN);
  delay(2000);
//  drawSimple();
  drawLadar();
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}
