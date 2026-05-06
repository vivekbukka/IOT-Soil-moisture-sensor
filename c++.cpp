// IoT-based Irrigation System for ThingSpeak
// Based on ESP32 WOKWI Simulator by ThinkIOT
// ThingSpeak channel can be found here: https://thingspeak.com/channels/2383114

#include <WiFi.h>
#include "ThingSpeak.h"

const int SOIL_MOISTURE_PIN = 34;
const int SPRINKLER_CONTROL_PIN = 19;

int MOISTURE_THRESHOLD_LOW = 15;            // Set Activation threshold in percentage
int MOISTURE_THRESHOLD_HIGH = 85;           // Set Deactivation threshold in percentage
bool SPRINKLER_ACTIVATION_STATUS = false;

char* WIFI_NAME = "Wokwi-GUEST";
char* WIFI_PASSWORD = "";
int myChannelNumber = 2564339;              // ThingSpeak channel ID
char* myApiKey = "3PDPDDF1RA32WIER";        // ThingSpeak channel write API key
WiFiClient client;

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  Serial.println("Connecting...");
  
  Serial.println("Wi-Fi connected");
  Serial.println("Local IP: " + String(WiFi.localIP()));
  Serial.println("-------------");
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

  pinMode(SPRINKLER_CONTROL_PIN, OUTPUT);
}

void loop()
{
  int soilMoisturePercentage = map(analogRead(SOIL_MOISTURE_PIN), 0, 4095, 0, 100);

  // Sprinkler activation threshold check
  if (soilMoisturePercentage < MOISTURE_THRESHOLD_LOW)
  {
    SPRINKLER_ACTIVATION_STATUS = true;
    digitalWrite(SPRINKLER_CONTROL_PIN, HIGH); // Turn on sprinkler and LED
  }
  else if (soilMoisturePercentage > MOISTURE_THRESHOLD_HIGH)
  {
    SPRINKLER_ACTIVATION_STATUS = false;
    digitalWrite(SPRINKLER_CONTROL_PIN, LOW); // Turn off sprinkler and LED
  }

  // Print status
  Serial.print("Soil Moisture Percentage: ");
  Serial.print(soilMoisturePercentage);
  Serial.println("%");

  Serial.print("Sprinkler: ");
  Serial.println(SPRINKLER_ACTIVATION_STATUS ? "on" : "off");

  // Send data to ThingSpeak
  ThingSpeak.setField(1, soilMoisturePercentage);
  ThingSpeak.setField(2, SPRINKLER_ACTIVATION_STATUS);

  int x = ThingSpeak.writeFields(myChannelNumber, myApiKey);

  Serial.println("-------------");

  delay(15000); // Thingspeak allows for an update every 15 seconds
}
