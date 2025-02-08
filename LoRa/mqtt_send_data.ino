#include <DHT.h>
#include <DHT_U.h>
#include <MKRWAN.h> // LoRa communication

#define DHTPIN 5         // Pin connected to DHT11
#define REED_PIN 4       // Pin connected to reed switch
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LoRaModem modem;

const char *appEui = "0000000000000001";
const char *appKey = "9030BE85C739E6BD344E9E3680EBC096"; // Replace with your TTN App Key

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(REED_PIN, INPUT_PULLUP);

  // Connect to LoRaWAN
  Serial.println("Connecting to TTN...");
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1);
  }
  Serial.println("LoRa module initialized");

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Failed to connect to TTN!");
    while (1);
  }
  Serial.println("Connected to TTN");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int reedState = digitalRead(REED_PIN);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT11");
    return;
  }

  // Format payload
  String payload = "{";
  payload += "\"temperature\": " + String(temperature) + ",";
  payload += "\"humidity\": " + String(humidity) + ",";
  payload += "\"door\": " + String(reedState);
  payload += "}";

  Serial.println("Sending: " + payload);

  // Send data via LoRaWAN
  modem.beginPacket();
  modem.print(payload);
  modem.endPacket();

  delay(15000); // Send data every 60 seconds
}
