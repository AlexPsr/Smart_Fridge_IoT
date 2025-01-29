const int reedPin = 2; // GPIO pin where the reed switch is connected

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for Serial to connect (optional)
  }

  // Set reedPin as an input with an internal pull-up resistor
  pinMode(reedPin, INPUT_PULLUP);

  Serial.println("Reed switch test initialized.");
}

void loop() {
  // Read the state of the reed switch
  int reedState = digitalRead(reedPin);

  // Check if the reed switch is closed (magnet near)
  if (reedState == LOW) {
    Serial.println("Reed switch is CLOSED (Magnet detected)");
  } else {
    Serial.println("Reed switch is OPEN (No magnet)");
  }

  // Delay to make it readable in the Serial Monitor
  delay(500);
}
