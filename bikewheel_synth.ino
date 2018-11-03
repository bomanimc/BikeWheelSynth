const int REED_PIN = 2; // Pin connected to reed switch
const int LED_PIN = 13; // LED pin - active-high
const int ANALOG_PIN = 11;
const int WINDOW_SIZE_MS = 500;
const int ROUGH_MAX_RPM = 800;

unsigned long stepCounter = 0;
float reedCounter = 0;
float rpm = 0;
int normalizedRPM = 0;

void setup() {
  Serial.begin(9600);
  
  // Since the other end of the reed switch is connected to ground, we need
  // to pull-up the reed switch pin internally.
  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ANALOG_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(REED_PIN), handleReading, RISING);
}

void loop() {
  int reedValue = digitalRead(REED_PIN);

  if (stepCounter % WINDOW_SIZE_MS == 0) {
     printReading("Count", reedCounter);

     // Calculate the actual RPM value    
     rpm = (reedCounter * 60000) / WINDOW_SIZE_MS;
     reedCounter = 0;
     printReading("RPM", rpm);

     // Calculate a values mapped to analog range   
     normalizedRPM = (int)((rpm / ROUGH_MAX_RPM) * 255);
     analogWrite(ANALOG_PIN, normalizedRPM);
     printReading("Normalized RPM", normalizedRPM);

     Serial.println();
  }

  blinkOnRead(reedValue);
  stepCounter++;

  // Force the program to run in 1ms increments to make stepCounting easier.
  // This probably can and should be replaced with an approach that uses millis().
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

void printReading(String text, int value) {
  Serial.print(text + ": ");
  Serial.println(value);
}


