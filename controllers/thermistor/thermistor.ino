#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// -----------------------------------------------------------------
// OLED CONFIG 
// -----------------------------------------------------------------

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     16    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// -----------------------------------------------------------------
// BLE CONFIG 
// -----------------------------------------------------------------

#define BLE_NAME                "MSN Thermistor"

// https://www.uuidgenerator.net/
#define TR_SERVICE_UUID         "0e4be5c8-b76c-4590-8eeb-35b93104c63e"
#define TR_CHARACTERISTIC_UUID  "177d03bb-a953-4f16-aabf-5246fbb3f72e"

#define PR_SERVICE_UUID         "f9e8c98a-0d57-423c-8ebc-ec69437c1c99"
#define PR_CHARACTERISTIC_UUID  "0dcdd589-bd02-42a5-b9c8-973ac9331c53"

#define THERMISTOR_PIN   36

char glbTrValue[32];
BLECharacteristic *glbTrCharacteristic;

char glbPrValue[32];
BLECharacteristic *glbPrCharacteristic;

bool deviceConnected = false;
bool advertising = false;

class ServerConnectCallback: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("device connected");
    deviceConnected = true;
    advertising = false;
  };
  void onDisconnect(BLEServer* pServer) {
    Serial.println("device disconnected");
    deviceConnected = false;
  }
};

// -----------------------------------------------------------------
// TASK CONFIG 
// -----------------------------------------------------------------
#define MAX_WAIT_FOR_TIMER 2
unsigned int waitFor(int timer, unsigned long period){
  static unsigned long waitForTimer[MAX_WAIT_FOR_TIMER];  
  
  unsigned long newTime = micros() / period;
  int delta = newTime - waitForTimer[timer];
  
  if (delta < 0) 
    delta = 1 + newTime;
  
  if (delta)
    waitForTimer[timer] = newTime;
  
  return delta;
}

struct screen_refresh_s {
  int timer;
  int period;
};
struct screen_refresh_s screen_refresh_ctx;

void setup_screen_refresh(struct screen_refresh_s *ctx, int timer, int period) {
  ctx->timer = timer;
  ctx->period = period;
}

void loop_screen_refresh(struct screen_refresh_s *ctx) {
  if (!waitFor(ctx->timer, ctx->period)) return;
  Serial.println("screen refresh");
  if (deviceConnected) Serial.println("device connected: 1");
  else Serial.println("device connected: 0");
  if (advertising) Serial.println("advertising: 1");
  else Serial.println("advertising: 0");
  
  display.clearDisplay();
  display.setCursor(0, 0);

  display.println(BLE_NAME);
  display.println(F("BLE: on"));
  if (advertising)
    display.println(F("Status: advertising"));
  else
    display.println(F("Status: connected"));

  display.println();
  display.print("Lightning: ");
  display.print(glbPrCharacteristic->getValue().c_str());
  display.display();
}

// -----------------------------------------------------------------
// Main program 
// -----------------------------------------------------------------

void setup() {
  // ------------------------
  // Serial setup 
  // ------------------------
  Serial.begin(9600);
  Serial.println("starting ble services ...");

  // ------------------------
  // OLED setup 
  // ------------------------
  Wire.begin(4, 15);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);                     // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);                     // Start at top-left corner

  setup_screen_refresh(&screen_refresh_ctx, 0, 100000);

  // ------------------------
  // BLE setup 
  // ------------------------

  // Setup BLE peripheral (GATT server)
  BLEDevice::init(BLE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerConnectCallback());
  BLEService *pRService = pServer->createService(TR_SERVICE_UUID);
  BLEService *pWService = pServer->createService(PR_SERVICE_UUID);
  
  // Initialize main characteristic (output)
  glbTrCharacteristic = pRService->createCharacteristic(
    TR_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
  strncpy(glbTrValue, "0.0", 3);
  glbTrCharacteristic->setValue(glbTrValue);

  // Initialize secondary characteristic (input)
  glbPrCharacteristic = pWService->createCharacteristic(
    PR_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  strncpy(glbPrValue, "?", 1);
  glbPrCharacteristic->setValue(glbPrValue);

  // Start the service
  pRService->start();
  pWService->start();
  
  // Setup & start ble ardvertising (pinging centrals)
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(TR_SERVICE_UUID);
  pAdvertising->addServiceUUID(PR_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  advertising = true;

  Serial.println("ble services started");

  // ------------------------
  // Hardware setup setup 
  // ------------------------
  
  // Setup the photoresistor
  pinMode(THERMISTOR_PIN, INPUT);

  display.display();
}

void loop() {
  if (deviceConnected) {
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
    snprintf(glbTrValue, sizeof(glbTrValue), "%f", T);
    glbTrCharacteristic->setValue(glbTrValue);
  }

  if (!advertising && !deviceConnected) {
    BLEDevice::startAdvertising();
    advertising = true;
  }

  loop_screen_refresh(&screen_refresh_ctx);
}
