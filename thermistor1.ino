// ============================================================================
// Module 2: Thermistor Serial Data and Temperature Conversion
// Part 1: Measure thermistor voltage, resistance, and temperature
// ============================================================================

// Circuit constants
const int ADC_PIN = A0;
const float V_REF = 5.0;              // Arduino reference voltage (V)
const float FIXED_RESISTOR = 100000;  // 100 kOhm fixed resistor (Ohms)

// Thermistor beta model constants
const float R0 = 100000;              // Thermistor resistance at T0 (100 kOhm at 25C)
const float T0 = 298.15;              // Reference temperature (25C in Kelvin)
const float BETA = 4540;              // Beta value (check datasheet: B57861S0104F040V24)

// Measurement constants
const int NUM_SAMPLES = 100;           // Number of ADC samples per measurement
const unsigned long REPORT_INTERVAL = 1000;  // Report every 1000 ms

// Timing
unsigned long lastReportTime = 0;
unsigned long startTime = 0;

// ============================================================================
// Function: averageAdcSamples()
// Read A0 NUM_SAMPLES times and return the average ADC value
// ============================================================================
float averageAdcSamples() {
  long sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(ADC_PIN);
  }
  return sum / (float)NUM_SAMPLES;
}

// ============================================================================
// Function: adcToVoltage()
// Convert average ADC count to voltage
// Arduino ADC: 0-1023 maps to 0-V_REF
// ============================================================================
float adcToVoltage(float avgAdc) {
  return (avgAdc / 1023.0) * V_REF;
}

// ============================================================================
// Function: voltageToResistance()
// Use the voltage divider equation to calculate thermistor resistance
// Voltage divider: V_out = V_ref * R_thermistor / (R_fixed + R_thermistor)
// Solving for R_thermistor: R = R_fixed * V_out / (V_ref - V_out)
// ============================================================================
float voltageToResistance(float voltage) {
  if (voltage >= V_REF || voltage <= 0) {
    return 0;  // Invalid measurement
  }
  float resistance = FIXED_RESISTOR * voltage / (V_REF - voltage);
  return resistance;
}

// ============================================================================
// Function: resistanceToCelsius()
// Use the beta model to convert thermistor resistance to temperature
// Beta equation: 1/T = 1/T0 + (1/beta) * ln(R/R0)
// Where T is in Kelvin, then convert to Celsius
// ============================================================================
float resistanceToCelsius(float resistance) {
  if (resistance <= 0) {
    return 0;
  }
  float invT = (1.0 / T0) + (1.0 / BETA) * log(resistance / R0);
  float tempKelvin = 1.0 / invT;
  float tempCelsius = tempKelvin - 273.15;
  return tempCelsius;
}

// ============================================================================
// Function: printHumanReadable()
// Print one measurement line with all values labeled and units
// ============================================================================
void printHumanReadable(float elapsedTime, float avgAdc, float voltage,
                        float resistance, float temperature, int numSamples) {
  // Format: time = 1.50 s    average ADC = 511.8    voltage = 2.501 V    
  //         resistance = 100.23 kOhm    temperature = 24.9 C    samples = 100
  
  Serial.print("time = ");
  Serial.print(elapsedTime, 2);
  Serial.print(" s    average ADC = ");
  Serial.print(avgAdc, 1);
  Serial.print("    voltage = ");
  Serial.print(voltage, 3);
  Serial.print(" V    resistance = ");
  Serial.print(resistance / 1000.0, 2);  // Convert Ohms to kOhm
  Serial.print(" kOhm    temperature = ");
  Serial.print(temperature, 1);
  Serial.print(" C    samples = ");
  Serial.println(numSamples);
}

// ============================================================================
// setup()
// Initialize Serial communication and print a heading
// ============================================================================
void setup() {
  Serial.begin(9600);
  delay(500);  // Wait for serial to initialize
  
  // Print heading
  Serial.println("============================================================");
  Serial.println("Module 2: Thermistor Temperature Measurement");
  Serial.println("Part 1: Serial Data and Temperature Conversion");
  Serial.println("============================================================");
  Serial.print("Thermistor: 100 kOhm NTC (B57861S0104F040V24)");
  Serial.print(", Beta = ");
  Serial.println(BETA);
  Serial.println("============================================================");
  
  startTime = millis();
  lastReportTime = millis();
}

// ============================================================================
// loop()
// Wait until it is time for a new report, then measure, calculate, and print
// ============================================================================
void loop() {
  unsigned long currentTime = millis();
  
  // Check if it's time to make a new report
  if (currentTime - lastReportTime >= REPORT_INTERVAL) {
    lastReportTime = currentTime;
    
    // Measure: average 100-1000 ADC readings
    float avgAdc = averageAdcSamples();
    
    // Convert in sequence: ADC → voltage → resistance → temperature
    float voltage = adcToVoltage(avgAdc);
    float resistance = voltageToResistance(voltage);
    float temperature = resistanceToCelsius(resistance);
    
    // Calculate elapsed time in seconds
    float elapsedTime = (currentTime - startTime) / 1000.0;
    
    // Print human-readable output
    printHumanReadable(elapsedTime, avgAdc, voltage, resistance, temperature, NUM_SAMPLES);
  }
}
