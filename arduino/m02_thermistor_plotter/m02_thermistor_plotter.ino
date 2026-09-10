/*
  m02_thermistor_plotter.ino
  Module 2 -- First Real Instrument Pieces
  Part 2 -- same measurement chain, but prints ONLY the temperature so Arduino
           Serial Plotter draws temperature versus serial read order.

  Team:      TEC 8
  Course:    Phys 39 -- Instrumentation and Thermal Physics
  Board:     Arduino Uno (ELEGOO UNO R3), USB powered.
  Wiring:    5V -> 100 kOhm fixed resistor -> A0 -> thermistor -> GND.
  Baud:      9600
  Output:    one bare number per line, e.g. 24.91

  SAFETY:    TEC disconnected for all of Module 2. Actuator power stays OFF until
             the H-bridge input signals have been checked on the oscilloscope.
             Every scope probe ground clip goes to Arduino GND -- NEVER to M+ or M-,
             which are both driven H-bridge outputs.

  Version:   v1 (2026-09-10)
*/

const char* VERSION = "m02_thermistor_plotter v1";

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
  // NOTE: Module 1 standardised on /1024 (1024 levels, codes 0-1023). These
  // sketches use /1023, which maps code 1023 to exactly V_REF. At mid-scale the
  // two differ by 2.4 mV, about 0.04 C here -- far below the measurement's real
  // uncertainty, so it does not matter numerically. It is left as /1023 because
  // that is what produced the recorded data; say which convention you used.
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
  Serial.println(VERSION);   // identifies the build on the board

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
