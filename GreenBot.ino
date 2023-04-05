/*
  Blynk Authorization
  
  Must have necessary Arduino libraries installed.
  
  Libraries:
    SimpleDHT
    Blynk
    
  Code for:
    Soil Moisture Capacitive Sensor
    DHT11 Humidity & Temperature Sensor
    Photoresistor 
    DSD BLE SH-08 Bluetooth Module
  
  Code should work just fine with serial monitor on Arduino IDE
  Displays through setup onto Blynk app, must be using virtual pins 4, 5, 6, 7, & 8(Or desired pins);
  
*/

#define BLYNK_USE_DIRECT_CONNECT

#include <SPI.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleSerialBLE.h>
#include <SimpleDHT.h>

// Miscellaneous Variables
SoftwareSerial DebugSerial(0, 1); // RX, TX
#define BLYNK_PRINT DebugSerial
//char auth[] = "ZdX8rJqp8GbmalRhp84ZzFmnPSSpl7cb";
char auth[] = "YOUR_BLYNK_AUTHORIZATION";

// DHT Variables
int pinDHT11 = 3;
SimpleDHT11 dht11;

// CS Moisture Variables
const int AirValue = 580;   
const int WaterValue = 300;  
int intervals = (AirValue - WaterValue)/3;   
double soilMoistureValue = 0;
double moisturePercentage = 0;

// Photoresistor Variables
const int pResistor = A0;  
double darkness = 250;
double light = 35;
double lightPercentage = 0;
int prValue = 0;

// Blynk Variables
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pins (4, 5, 6, 7, 8).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensors()
{
  // DHT11 START
  Serial.println();
  Serial.println("Sample DHT11...");
  
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  float humidity1 = 0;
  float temperature1 = 0;
  
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) 
  {
    Serial.println("Read DHT11 failed");
    return;
  }
  
  Serial.println("");
  Serial.print("Sample OK: ");
  Serial.print(" Celsius: "); 
  Serial.print((int)temperature); 
  Serial.println("");
  Serial.println("Humidity:");
  Serial.print((int)humidity); 
  Serial.println(" %");
  Serial.println("");

  humidity1 = (static_cast<int>(humidity));
  temperature1 = ((static_cast<int>(temperature)) * 9/5) + 32; // Convert to Fahrenheit
  // DHT11 END
  
  // CSM START
  soilMoistureValue = analogRead(A1);  //put Sensor insert into soil
  moisturePercentage = 100 - (((soilMoistureValue - WaterValue)/(AirValue - WaterValue)) * 100);

  Serial.println("CSMS...");
  Serial.println(String("Soil Moisture Value: ") + soilMoistureValue);
  Serial.println(String("Wetness level: ") + moisturePercentage + "%");
  Serial.println("");

  // CSM END

  // Photosensors START
  Serial.println("PR...");
  prValue = analogRead(pResistor);
  Serial.println(String("Light level: ") + prValue);
  //lightPercentage = ((1 - ((prValue - (darkness - light))/(darkness - light))) * 100);
  lightPercentage = 100 - (((prValue - light) / (darkness - light)) * 100);
  Serial.println(String("Light percentage: ") + static_cast<int>(lightPercentage) + "%");
  Serial.println("");

  // Photosensors END
  
  delay(2000);
  
  // Send to Blynk
  Blynk.virtualWrite(V4, temperature1);
  Blynk.virtualWrite(V5, humidity1);
  Blynk.virtualWrite(V6, static_cast<int>(soilMoistureValue));
  Blynk.virtualWrite(V7, static_cast<int>(moisturePercentage));
  Blynk.virtualWrite(V8, static_cast<int>(lightPercentage));

}

void setup() 
{
  // Debug console + Serial Monitor
  DebugSerial.begin(9600);
  DebugSerial.println("Waiting for connections...");
  Serial.println("Waiting for connections...");
  
  Serial.begin(9600);
  Blynk.begin(Serial, auth); // Send Blynk Authorization
  
  timer.setInterval(1000L, sendSensors);
}

void loop()
{
  Blynk.run();
  timer.run();
}
