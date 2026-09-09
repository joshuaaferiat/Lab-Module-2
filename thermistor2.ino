/*
  FULL INTEGRATED SKETCH: Part 3C + Part 3D
  
  - setup() runs once: Measures time for 1000 analogRead() conversions (3D)
  - loop() runs forever: Prints 100 averaged + 100 single readings (3C)
  
  Part 3C format: Ave1000_Point_1 Voltage_V:2.4561
                  Ave1_Point_1 Voltage_V:2.4570
*/

void setup() {
  Serial.begin(9600);
  
  // ================================================================
  // PART 3D: MEASURE THE TIME COST OF AVERAGING (RUNS ONCE)
  // ================================================================
  Serial.println("=== Part 3D: Time Cost of Averaging ===");
  
  unsigned long startTime = micros();   // Start timer
  
  for (int i = 0; i < 1000; i++) {
    analogRead(A0);                     // Just read, ignore the value
  }
  
  unsigned long endTime = micros();     // Stop timer
  unsigned long elapsedMicros = endTime - startTime;
  
  // Print results to Serial Monitor (only once)
  Serial.print("Time for 1000 analogRead() conversions: ");
  Serial.print(elapsedMicros);
  Serial.println(" microseconds");
  
  float elapsedMillis = elapsedMicros / 1000.0;
  Serial.print("That is ");
  Serial.print(elapsedMillis, 2);
  Serial.println(" milliseconds");
  
  float conversionRate = (1000.0 * 1000000.0) / elapsedMicros;
  Serial.print("Conversion rate: ");
  Serial.print(conversionRate, 1);
  Serial.println(" readings/second");
  
  float timePerReading = elapsedMicros / 1000.0;
  Serial.print("Time per reading: ");
  Serial.print(timePerReading, 1);
  Serial.println(" microseconds");
  
  Serial.println("Arduino reference: ~100 microseconds per conversion");
  Serial.println("=== End of Part 3D ===");
  Serial.println();
  Serial.println("=== Part 3C: Data Collection Starting ===");
  Serial.println("(Averaged block = smooth, Single block = noisy)");
  Serial.println();
}

// ================================================================
// PART 3C: COMPARE ONE READING WITH A 1000-READING AVERAGE
// ================================================================

// --- Reads a single ADC conversion and returns voltage ---
float readVoltageSingle() {
  int sensorValue = analogRead(A0);
  // 10-bit ADC: 1024 levels, resolution = 5.00V / 1024 = 4.88mV
  return sensorValue * (5.00 / 1024.0);
}

// --- Averages 'numReadings' ADC conversions and returns voltage ---
float readVoltageAveraged(int numReadings) {
  long sum = 0;  // long prevents overflow (1000 * 1023 = 1,023,000)
  for (int i = 0; i < numReadings; i++) {
    sum += analogRead(A0);
  }
  float avgADC = sum / (float)numReadings;
  return avgADC * (5.00 / 1024.0);
}

void loop() {

  // // ---- BLOCK 1: 100 readings, each averaging 1000 conversions ----
  // for (int i = 1; i <= 100; i++) {
  //   float voltage = readVoltageAveraged(1000);
  //   Serial.print("Ave1000_Point_");
  //   Serial.print(i);
  //   Serial.print(" Voltage_V:");
  //   Serial.println(voltage, 4);
  //   delay(200);  // Slow enough to capture the transition in Serial Plotter
  // }

  // ---- BLOCK 2: 100 readings, each from a single conversion ----
  for (int i = 1; i <= 100; i++) {
    float voltage = readVoltageSingle();
    Serial.print("Ave1_Point_");
    Serial.print(i);
    Serial.print(" Voltage_V:");
    Serial.println(voltage, 4);
    delay(200);
  }

  // Repeats forever: Averaged → Single → Averaged → Single → ...
}
