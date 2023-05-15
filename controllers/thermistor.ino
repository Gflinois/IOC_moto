#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// https://www.uuidgenerator.net/
#define TR_SERVICE_UUID         "0e4be5c8-b76c-4590-8eeb-35b93104c63e"
#define TR_CHARACTERISTIC_UUID  "177d03bb-a953-4f16-aabf-5246fbb3f72e"

#define THERMISTOR_PIN   36

char glbPrValue[32];
BLECharacteristic *glbPrCharacteristic;

void setup() {
  Serial.begin(9600);

  Serial.println("starting ble services ...");

  // Setup BLE peripheral (GATT server)
  BLEDevice::init("MSN Thermistor");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(TR_SERVICE_UUID);
  
  // Initialize main characteristic
  glbPrCharacteristic = pService->createCharacteristic(
    TR_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
  strncpy(glbPrValue, "0.0", 3);
  glbPrCharacteristic->setValue(glbPrValue);

  // Start the service
  pService->start();

  // Setup & start ble ardvertising (pinging centrals)
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(TR_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("ble services started");

  // Setup the photoresistor
  pinMode(THERMISTOR_PIN, INPUT);
}

void loop() {
  int Vo;
  float R1 = 100000; // 10 kOhm
  float logR2, R2, T;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  Vo = analogRead(THERMISTOR_PIN);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0; 

  // Update thermistor value (convert to string in order to debug)
  snprintf(glbPrValue, sizeof(glbPrValue), "%f", T);
  glbPrCharacteristic->setValue(glbPrValue);
}
