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

// DHT sensor setup
#define DHTPIN 5       // GPIO where the DHT11 data pin is connected
const int reedPin = 4; // GPIO pin where the reed switch is connected
const int ledPin = 2; // Built-in LED pin on most ESP32 boards
#define DHTTYPE DHT11  // DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);

void setup() {

   pinMode(ledPin, OUTPUT);
  // Start Serial Monitor
  Serial.begin(115200);
  Serial.println("ESP32 Built-in LED Test");

  while (!Serial) {
    ; // Wait for Serial Monitor to connect
  }

  // Set reedPin as an input with an internal pull-up resistor
  pinMode(reedPin, INPUT_PULLUP);

  Serial.println("Reed switch test initialized.");

  // Initialize the DHT sensor
  dht.begin();

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("eimaste edw");
  digitalWrite(ledPin, HIGH);
  delay(5000);
  digitalWrite(ledPin, LOW);
}

void loop() {
  // Wait a bit between readings
  delay(5000);

  // Read temperature and humidity
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read the state of the reed switch
  int reedState = digitalRead(reedPin);

  // Check if reads failed
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Check if the reed switch is closed (magnet near)
  if (reedState == LOW) {
    Serial.println("Reed switch is CLOSED (Magnet detected)");
  } else {
    Serial.println("Reed switch is OPEN (No magnet)");
  }

  // Print data to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Send data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Build URL with API key and data
    String url = String(server) + "/update?api_key=" + apiKey + "&field1=" + String(temp) + "&field2=" + String(humidity) + "&field3=" + String(reedState) ;
    http.begin(url);

    // Make the HTTP GET request
    int httpCode = http.GET();

    // Check the response
    if (httpCode > 0) {
      Serial.print("ThingSpeak Response: ");
      Serial.println(http.getString());
    } else {
      Serial.print("Error sending data to ThingSpeak: ");
      Serial.println(http.errorToString(httpCode));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}
