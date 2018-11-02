const int REED_PIN = 2; // Pin connected to reed switch
const int LED_PIN = 13; // LED pin - active-high
const int WINDOW_SIZE_MS = 1000;

unsigned long stepCounter = 0;
float reedCounter = 0;
float rpm = 0;

void setup() {
  Serial.begin(9600);
  
  // Since the other end of the reed switch is connected to ground, we need
  // to pull-up the reed switch pin internally.
  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(REED_PIN), handleReading, RISING);
}

void loop() {
  int reedValue = digitalRead(REED_PIN); // Read the state of the switch

  if (stepCounter % WINDOW_SIZE_MS == 0) {
     Serial.print("Count: ");
     Serial.println(reedCounter);

     rpm = (reedCounter * 60000) / WINDOW_SIZE_MS;
     reedCounter = 0;

     Serial.print("RPM: ");
     Serial.println(rpm);

     Serial.println();
  }

  blinkOnRead(reedValue);
  stepCounter++;
  delay(1);
}

void blinkOnRead(int reedValue) {
  // If the pin reads low, the switch is closed.
  if (reedValue == LOW) {
    digitalWrite(LED_PIN, HIGH); // Turn the LED on
  }
  else {
    digitalWrite(LED_PIN, LOW); // Turn the LED off
  }
}

void handleReading() {
  reedCounter++;
}


