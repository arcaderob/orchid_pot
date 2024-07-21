// Sensor pins
#define sensorPower A3
#define sensorPin A2
#define pumpPin A1

#define ONE_DAY (1UL * 24 * 60 * 60 * 1000) // 1 day in milliseconds

unsigned long lastRunTime = 0; // Variable to store the last time the sensor was read

void setup() {
    pinMode(sensorPower, OUTPUT);
    pinMode(pumpPin, OUTPUT);

    // Initially keep the sensor OFF
    digitalWrite(sensorPower, LOW);

    // Initially keep the pump OFF
    digitalWrite(pumpPin, HIGH);

    Serial.begin(9600);
}

void loop() {
    unsigned long currentTime = millis(); // Get the current time

    // Check if it's time to read the sensor (7 days from the last run)
    if (lastRunTime == 0 || currentTime - lastRunTime >= (ONE_DAY * 7)) {
      int currentMoistureReading = readSensor();

      if (currentMoistureReading >= 950) {
        // turn on the pump
        digitalWrite(pumpPin, LOW);

        while (currentMoistureReading >= 500) {
          currentMoistureReading = readSensor();
        }

        // turn off the pump
        digitalWrite(pumpPin, HIGH);

        // set the last time it was run to now
        lastRunTime = currentTime;
      } else {
        // if seven days have elapsed but the soil is still moist, we will check again in one day
        lastRunTime = lastRunTime + ONE_DAY;
      }
    }

    delay(1000); // Delay for stability
}

//  This function returns the analog soil moisture measurement
int readSensor() {
    digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
    delay(10);                        // Allow power to settle
    int val = analogRead(sensorPin);  // Read the analog value form sensor
    digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
    return val;                       // Return analog moisture value
}