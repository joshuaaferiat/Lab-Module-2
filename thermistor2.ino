// ============================================================================
// Module 2: Serial Plotter Output
// Part 2: Plot thermistor temperature versus serial read order
// ============================================================================

// Circuit constants
const int ADC_PIN = A0;
const float V_REF = 5.0;
const float FIXED_RESISTOR = 100000.0;  // 100 kOhm

// Thermistor beta model constants
const float R0 = 100000.0;   // 100 kOhm at 25 C
const float T0 = 298.15;      // 25 C in Kelvin
const float BETA = 4540.0;    // Datasheet value

// Measurement constants
const int NUM_SAMPLES = 100;
const unsigned long REPORT_INTERVAL = 1000;

// Timing
unsigned long lastReportTime = 0;


// ============================================================================
// averageAdcSamples()
// ============================================================================
float averageAdcSamples() {
  long sum = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(ADC_PIN);
  }

  return sum / (float)NUM_SAMPLES;
}


// ============================================================================
// adcToVoltage()
// ============================================================================
float adcToVoltage(float avgAdc) {
  return (avgAdc / 1023.0) * V_REF;
}


// ============================================================================
// voltageToResistance()
// ============================================================================
float voltageToResistance(float voltage) {
  if (voltage >= V_REF || voltage <= 0) {
    return 0;
  }

  return FIXED_RESISTOR * voltage / (V_REF - voltage);
}


// ============================================================================
// resistanceToCelsius()
// ============================================================================
float resistanceToCelsius(float resistance) {
  if (resistance <= 0) {
    return 0;
  }

  float invT =
      (1.0 / T0)
      + (1.0 / BETA) * log(resistance / R0);

  float tempKelvin = 1.0 / invT;

  return tempKelvin - 273.15;
}


// ============================================================================
// setup()
// ============================================================================
void setup() {
  Serial.begin(9600);

  lastReportTime = millis();
}


// ============================================================================
// loop()
// ============================================================================
void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastReportTime >= REPORT_INTERVAL) {
    lastReportTime = currentTime;

    // Average raw ADC measurements first
    float avgAdc = averageAdcSamples();

    // Convert in required sequence
    float voltage = adcToVoltage(avgAdc);

    float resistance =
        voltageToResistance(voltage);

    float temperature =
        resistanceToCelsius(resistance);

    // IMPORTANT:
    // Serial Plotter should receive only one temperature value per line
    Serial.println(temperature, 2);
  }
}
