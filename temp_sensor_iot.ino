#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// WiFi credentials
const char* ssid = "COSMOTE-641940";      // Replace with your WiFi SSID
const char* password = "7hektkut7t8ndsn";  // Replace with your WiFi Password

// ThingSpeak API information
const char* server = "http://api.thingspeak.com";
const char* apiKey = "6SYHHC5DN43PG2EB"; // Replace with your Write API Key

// Define the DHT sensor type and pin
#define DHTPIN 5    // GPIO pin where the DHT11 data pin is connected
#define DHTTYPE DHT11  // Define DHT11 sensor type

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for Serial Monitor to connect (optional)
  }
  
  // Initialize the DHT sensor
  dht.begin();
  Serial.println("DHT11 sensor test!");
}

void loop() {
  // Wait a bit between sensor readings
  delay(2000);

  // Read temperature as Celsius
  float temp = dht.readTemperature();
  // Read humidity
  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the results to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%\t");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("°C");
}
