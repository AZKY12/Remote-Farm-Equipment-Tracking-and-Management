// Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL6uRwNNiaI"
#define BLYNK_TEMPLATE_NAME "Remote Farm Equipment Tracking and Management"
#define BLYNK_AUTH_TOKEN "XP8kv80-KNjmQOMokp2QzoaQRfOW2q5d"

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ArduinoJson.h>     // Include ArduinoJson library for data formatting



// WiFi credentials
const char* ssid = "Wokwi-GUEST";
const char* pass = "";

// MPU6050 setup
Adafruit_MPU6050 mpu;

// Thresholds for activity detection
const float ACC_THRESHOLD = 0.5;   // Adjust based on required sensitivity
const float GYRO_THRESHOLD = 10.0;

// RFID setup
#define SS_PIN 19   // Slave Select (SS) pin
#define IRQ_PIN 4   // Interrupt Request (IRQ) pin
#define SCK_PIN 2   // Clock (SCK) pin
#define MOSI_PIN 15 // Master Out Slave In (MOSI) pin
#define MISO_PIN 12 // Master In Slave Out (MISO) pin

// Battery and Fuel Sensor setup
#define BATTERY_PIN 34       // GPIO34 for battery sensor
#define FUEL_PIN 35          // GPIO35 for fuel sensor

// LED setup
#define BATTERY_LED_PIN 5    // GPIO5 for Battery LED
#define FUEL_LED_PIN 18      // GPIO18 for Fuel LED

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(20);

  WiFi.begin(ssid, pass);
  Serial.println("Connecting to WiFi...");

  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Connected to Blynk.");

  // Initialize the MPU6050 sensor
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip. Check your connections.");
    while (1) {
      delay(20);
    }
  }
  Serial.println("MPU6050 initialized.");

  // Set up battery and fuel sensor pins
  pinMode(BATTERY_PIN, INPUT);
  pinMode(FUEL_PIN, INPUT);

  // Set up LED pins
  pinMode(BATTERY_LED_PIN, OUTPUT);
  pinMode(FUEL_LED_PIN, OUTPUT);

  // Set up SPI and IRQ pin
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH); // Start with SS high (chip deselected)
  pinMode(IRQ_PIN, INPUT);
  SPI.begin(); // Initialize SPI

   // LoRa initialization (commented for Tinkercad)
  // if (!LoRa.begin(868E6)) {  // Frequency setting for LoRa
  //   Serial.println("LoRa initialization failed!");
  //   while (1);
  // 

}

void loop() {
  Blynk.run();

  // Simulated sensor data from MPU6050
  sensors_event_t acc, gyro, temp;
  mpu.getEvent(&acc, &gyro, &temp);

  float gps_lat = 7.8731, gps_lon = 80.7718; //assum that gps data
 



  // Read acceleration and gyroscope values
  float acc_x = acc.acceleration.x;
  float acc_y = acc.acceleration.y;
  float acc_z = acc.acceleration.z;
  float gyro_x = gyro.gyro.x * 180 / 3.14; // Convert rad/s to degrees/s
  float gyro_y = gyro.gyro.y * 180 / 3.14;
  float gyro_z = gyro.gyro.z * 180 / 3.14;

  // Check for activity based on thresholds
  bool isActive = (abs(acc_x) > ACC_THRESHOLD || abs(acc_y) > ACC_THRESHOLD || abs(acc_z) > ACC_THRESHOLD ||
                   abs(gyro_x) > GYRO_THRESHOLD || abs(gyro_y) > GYRO_THRESHOLD || abs(gyro_z) > GYRO_THRESHOLD);

  // Read battery and fuel levels
  int batteryValue = analogRead(BATTERY_PIN);
  int fuelValue = analogRead(FUEL_PIN);

  // Convert analog values to percentages (0-100%)
  int batteryPercentage = map(batteryValue, 0, 4095, 0, 100);
  int fuelPercentage = map(fuelValue, 0, 4095, 0, 100);

  // Print results to serial monitor
  Serial.println("----- Sensor Readings -----");

  // MPU6050 data
  Serial.print("Acceleration - X: "); Serial.println(acc_x);
  Serial.print("Acceleration - Y: "); Serial.println(acc_y);
  Serial.print("Acceleration - Z: "); Serial.println(acc_z);

  Serial.print("Gyroscope - X: "); Serial.println(gyro_x);
  Serial.print("Gyroscope - Y: "); Serial.println(gyro_y);
  Serial.print("Gyroscope - Z: "); Serial.println(gyro_z);

  if (isActive) {
    Serial.println("Status: Active");
  } else {
    Serial.println("Status: Idle");
  }

  // Battery and Fuel data
  Serial.print("Battery Level: ");
  Serial.print(batteryPercentage);
  Serial.println("%");

  Serial.print("Fuel Level: ");
  Serial.print(fuelPercentage);
  Serial.println("%");

  // LED Indicators
  if (batteryPercentage < 20) {
    digitalWrite(BATTERY_LED_PIN, HIGH); // Turn on battery LED
    Serial.println("Warning: Low Battery!");
  } else {
    digitalWrite(BATTERY_LED_PIN, LOW);  // Turn off battery LED
  }

  if (fuelPercentage < 20) {
    digitalWrite(FUEL_LED_PIN, HIGH);    // Turn on fuel LED
    Serial.println("Warning: Low Fuel!");
  } else {
    digitalWrite(FUEL_LED_PIN, LOW);     // Turn off fuel LED
  }

  // Send data to Blynk virtual pins
  Blynk.virtualWrite(V0, batteryPercentage);  // Battery level
  Blynk.virtualWrite(V1, fuelPercentage);     // Fuel level
  Blynk.virtualWrite(V2, temp.temperature);   // Temperature from MPU6050
  Blynk.virtualWrite(V3, isActive ? "Active" : "Idle");  // Activity status
  Blynk.virtualWrite(V6, gps_lat);       // Virtual pin V6 for GPS latitude
  Blynk.virtualWrite(V7, gps_lon);       // Virtual pin V7 for GPS longitude

  // Create a JSON document
  StaticJsonDocument<512> doc;
  doc["GPS"]["lat"] = gps_lat;
  doc["GPS"]["lon"] = gps_lon;


 

  doc["Motion"]["accel"]["x"] = acc_x;
  doc["Motion"]["accel"]["y"] = acc_y;
  doc["Motion"]["accel"]["z"] = acc_z;
  doc["Motion"]["status"] = isActive ? "Active" : "Idle";

  doc["Battery"]["level"] = batteryPercentage;
  doc["Fuel"]["level"] = fuelPercentage;


  //////////////////////////////// Serialize and send data to Master (via SoftwareSerial)
  String output;
  serializeJson(doc, output);
///////////////////// LoRa transmission (commented for Tinkercad)
  // LoRa.beginPacket();
  // LoRa.print(output);      // Send the data packet to LoRa
  // LoRa.endPacket();



  delay(1000); // Adjust delay for simulation speed
}

///////////////////////////////lora receiving part

// #include <LoRa.h> 

//void setup() {
  

  //////////////////////////////////////// LoRa initialization 
  // if (!LoRa.begin(868E6)) {  // Frequency setting for LoRa
  //   Serial.println("LoRa initialization failed!");
  //   while (1);
  // }
  
 
//}

//void loop() {
  

  ////////////////////////////////////////////// LoRa receiving part (commented for Tinkercad)
  // if (LoRa.parsePacket()) {
  //   String receivedMessage = LoRa.readString();
  //   Serial.println("Received from LoRa: " + receivedMessage);
  // }

  

 // delay(1000);
//}
