#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// https://www.uuidgenerator.net/
#define PR_SERVICE_UUID         "0e4be5c8-b76c-4590-8eeb-35b93104c63e"
#define PR_CHARACTERISTIC_UUID  "177d03bb-a953-4f16-aabf-5246fbb3f72e"

#define PHOTORESISTOR_PIN   36

char glbPrValue[32];
BLECharacteristic *glbPrCharacteristic;

void setup() {
  Serial.begin(9600);

  Serial.println("starting ble services ...");

  // Setup BLE peripheral (GATT server)
  BLEDevice::init("MSN Photoresistor");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(PR_SERVICE_UUID);
  
  // Initialize main characteristic
  glbPrCharacteristic = pService->createCharacteristic(
    PR_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
  strncpy(glbPrValue, "0.0", 3);
  glbPrCharacteristic->setValue(glbPrValue);

  // Start the service
  pService->start();

  // Setup & start ble ardvertising (pinging centrals)
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(PR_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("ble services started");

  // Setup the photoresistor
  pinMode(PHOTORESISTOR_PIN, INPUT);
}

void loop() {
  // Update photoresistor value (convert to string in order to debug)
  double rawPrValue = 1.0 - (analogRead(PHOTORESISTOR_PIN) / 4095.0);
  snprintf(glbPrValue, sizeof(glbPrValue), "%f", rawPrValue);
  glbPrCharacteristic->setValue(glbPrValue);
}