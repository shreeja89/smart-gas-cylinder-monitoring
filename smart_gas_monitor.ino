#include <WiFi.h>
#include "HX711.h"
#include "ThingSpeak.h"
#include <Preferences.h>

Preferences prefs;

// ------------------- WiFi Credentials -------------------
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// ------------------- ThingSpeak -------------------
unsigned long channelID = 3270644;
const char* writeAPIKey = "YOUR_API_KEY";
WiFiClient client;

// ------------------- HX711 Pins -------------------
#define DT 13
#define SCK 25
HX711 scale;

// ------------------- Buzzer -------------------
#define BUZZER 21

// ------------------- Calibration -------------------
float calibration_factor = 100.0;

const int readInterval = 1000;
const float leak_threshold = 5.0;

// ------------------- Cylinder Weights -------------------
const float totalWeight = 6000.0;  
float tareWeight = 1000.0;        
const float gasCapacity = 5000.0;

// ------------------- ThingSpeak -------------------
const unsigned long tsInterval = 15000;
unsigned long lastThingSpeak = 0;

// ------------------- Variables -------------------
float previousWeight = 0;
float currentWeight = 0;

void setup() {

Serial.begin(115200);

// Load stored tare weight
prefs.begin("gas", false);
tareWeight = prefs.getFloat("tare", 1000.0);

// Buzzer PWM setup
ledcAttach(BUZZER, 3000, 8);
ledcWrite(BUZZER, 0);

Serial.println("Smart Gas Monitor Started");

Serial.print("Stored Tare Weight: ");
Serial.println(tareWeight);

// HX711 Setup
scale.begin(DT, SCK);
scale.set_scale(calibration_factor);
scale.tare();
delay(1000);

previousWeight = scale.get_units(20);

// WiFi Connect
WiFi.begin(ssid, password);
Serial.print("Connecting WiFi");

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("\nWiFi Connected");

ThingSpeak.begin(client);
}

void loop() {

// ------------------- Read tare weight from ThingSpeak -------------------

// ThingSpeak API URL to send Tare Weight value to Field 4
// https://api.thingspeak.com/update?api_key=A00ZKI0QU2R4K1A9&field4=1000
float newTare = ThingSpeak.readFloatField(channelID, 4);

if (newTare > 0 && newTare != tareWeight) {

tareWeight = newTare;
prefs.putFloat("tare", tareWeight);

Serial.print("Updated Tare Weight from ThingSpeak: ");
Serial.println(tareWeight);
}

// ------------------- Read Weight -------------------
currentWeight = scale.get_units(20);

if (currentWeight < 0) currentWeight = 0;

// ------------------- Gas Weight -------------------
float gasWeight = currentWeight - tareWeight;
if (gasWeight < 0) gasWeight = 0;

// ------------------- Gas Percentage -------------------
float gasPercent = (gasWeight / gasCapacity) * 100;

if (gasPercent > 100) gasPercent = 100;
if (gasPercent < 0) gasPercent = 0;

// ------------------- Detect Gas Leak -------------------
float weightDrop = previousWeight - currentWeight;
bool leakDetected = false;

if (weightDrop > leak_threshold) {

leakDetected = true;
Serial.println("ALERT: GAS LEAK DETECTED!");
}

// ------------------- Detect Low Gas -------------------
bool lowGas = false;

if (gasPercent < 20) {

lowGas = true;

Serial.println("ALERT: GAS BELOW 20%");

// Buzzer Beep
ledcWrite(BUZZER, 200);
delay(200);
ledcWrite(BUZZER, 0);
delay(200);

} else {

ledcWrite(BUZZER, 0);
}

// ------------------- Alert Code -------------------
int alertCode = 0;

if (lowGas && leakDetected) alertCode = 3;
else if (lowGas) alertCode = 1;
else if (leakDetected) alertCode = 2;

// ------------------- Serial Monitor -------------------
Serial.print("Total Weight (g): ");
Serial.println(currentWeight);

Serial.print("Gas Weight (g): ");
Serial.println(gasWeight);

Serial.print("Gas Percentage: ");
Serial.print(gasPercent);
Serial.println("%");

Serial.print("Tare Weight: ");
Serial.println(tareWeight);

Serial.print("Alert Code: ");
Serial.println(alertCode);

Serial.println("----------------------");

// ------------------- ThingSpeak Upload -------------------
if (millis() - lastThingSpeak >= tsInterval || leakDetected) {

ThingSpeak.setField(1, currentWeight);
ThingSpeak.setField(2, gasPercent);
ThingSpeak.setField(3, alertCode);
ThingSpeak.setField(4, tareWeight);

int response = ThingSpeak.writeFields(channelID, writeAPIKey);

if (response == 200)
Serial.println("ThingSpeak Upload Success");
else {

Serial.print("ThingSpeak Error: ");
Serial.println(response);
}

lastThingSpeak = millis();
}

previousWeight = currentWeight;

delay(readInterval);
}
