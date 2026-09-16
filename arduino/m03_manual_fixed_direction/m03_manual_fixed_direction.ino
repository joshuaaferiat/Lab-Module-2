/*
  m03_manual_fixed_direction.ino
  Module 3 -- First Manual Sketch, Fixed Direction

  Two input paths, one fixed-direction output path:
    A0 thermistor  -> average -> temperature
    A1 trim pot     -> ADC value -> PWM command
    fixed direction -> H-bridge -> TEC

  Team:      TEC 8
  Course:    Phys 39 -- Instrumentation and Thermal Physics
  Board:     Arduino Uno (ELEGOO UNO R3), USB powered for logic; bench supply
             for actuator power (applied only after instructor approval).
  Wiring:    5V -> 100 kOhm fixed resistor -> A0 -> thermistor -> GND.
             Trim pot wiper -> A1 (outer terminals 5V and GND).
             Pin 9  -> RPWM, held LOW / 0 the whole time (fixed side, unused
                       this test).
             Pin 10 -> LPWM, receives the trim-pot PWM command.
             Pin 11 -> direction input for the SECOND sketch only; not read
                       here. If wired, tie it to 5V per the assignment.
             H-bridge R_EN, L_EN, VCC -> Arduino 5V; GND -> Arduino GND.
             R_IS / L_IS left unconnected. H-bridge M+/M- -> TEC.
  Baud:      9600
  Output:    Serial Monitor only, one labeled line per report, e.g.
             "Temperature (C): 27.73, Time (s): 645.06, PWM: 120, Active PWM pin: 10"
             No Serial Plotter output in this sketch.

  Procedure: Bring this up and confirm the printed line looks right BEFORE any
             actuator power is applied. After instructor approval, apply low
             power and note whether the fixed command heats or cools the TEC.
             For the direction-reversal test, leave pin 11 at 5V and swap only
             the two Arduino-to-H-bridge control leads on pins 9/10 -- do NOT
             swap the M+/M- leads at the TEC.

  SAFETY:    Do not apply actuator power until the instructor has checked pins
             9/10 on the oscilloscope. Every scope probe ground clip goes to
             Arduino GND -- NEVER to M+ or M-, which are both driven H-bridge
             outputs.

  Version:   v1
*/

const char* VERSION = "m03_manual_fixed_direction v1";

// ---- Thermistor divider (A0) ----
const int THERMISTOR_PIN = A0;
const float V_REF = 5.0;              // Arduino reference voltage (V)
const float FIXED_RESISTOR = 100000;  // 100 kOhm fixed resistor (Ohms)
const float R0 = 100000;              // Thermistor resistance at T0 (100 kOhm at 25C)
const float T0 = 298.15;              // Reference temperature (25C in Kelvin)
const float BETA = 4540;              // Beta value (check datasheet: B57861S0104F040V24)

// ---- Trim pot / PWM (A1) ----
const int TRIM_POT_PIN = A1;

// ---- Fixed-direction H-bridge output ----
// This first sketch does not read pin 11 at all -- direction is fixed by
// which physical pin (9 or 10) carries the PWM command, set here in code.
// Swapping direction for the second half of the exercise means swapping the
// two wires on the H-bridge, not changing this code.
const int RPWM_PIN = 9;               // held LOW / 0 for this whole sketch
const int LPWM_PIN = 10;              // carries the trim-pot PWM command
const int ACTIVE_PWM_PIN = LPWM_PIN;  // used only for the printed line

const int DIRECTION_PIN = 11;         // not read this sketch; wire to 5V if present

// ---- Measurement / report timing ----
const int NUM_SAMPLES = 100;                  // ADC samples averaged per measurement
const unsigned long REPORT_INTERVAL = 1000;   // report every 1000 ms

unsigned long lastReportTime = 0;
unsigned long startTime = 0;

// ============================================================================
// averageAnalogSamples()
// Read the given analog pin NUM_SAMPLES times and return the average count
// ============================================================================
float averageAnalogSamples(int pin) {
  long sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(pin);
  }
  return sum / (float)NUM_SAMPLES;
}

// ============================================================================
// adcToVoltage()
// Average ADC count (0-1023) -> voltage (0-V_REF)
// ============================================================================
float adcToVoltage(float avgAdc) {
  return (avgAdc / 1023.0) * V_REF;
}

// ============================================================================
// voltageToResistance()
// Voltage divider: V_A0 = V_ref * R_th / (R_fixed + R_th)
// Solved for R_th:  R_th = R_fixed * V_A0 / (V_ref - V_A0)
// ============================================================================
float voltageToResistance(float voltage) {
  if (voltage >= V_REF || voltage <= 0) {
    return 0;  // invalid measurement
  }
  return FIXED_RESISTOR * voltage / (V_REF - voltage);
}

// ============================================================================
// resistanceToCelsius()
// Beta equation: 1/T = 1/T0 + (1/beta) * ln(R/R0), T in Kelvin
// ============================================================================
float resistanceToCelsius(float resistance) {
  if (resistance <= 0) {
    return 0;
  }
  float invT = (1.0 / T0) + (1.0 / BETA) * log(resistance / R0);
  float tempKelvin = 1.0 / invT;
  return tempKelvin - 273.15;
}

// ============================================================================
// mapAdcToPwm()
// Average trim-pot ADC count (0-1023) -> PWM command (0-255)
// ============================================================================
int mapAdcToPwm(float avgAdc) {
  return (int)(avgAdc * 255.0 / 1023.0);
}

// ============================================================================
// printStatus()
// One labeled line: temperature, elapsed time, PWM command, active PWM pin
// ============================================================================
void printStatus(float temperature, float elapsedTime, int pwmValue, int activePin) {
  Serial.print("Temperature (C): ");
  Serial.print(temperature, 2);
  Serial.print(", Time (s): ");
  Serial.print(elapsedTime, 2);
  Serial.print(", PWM: ");
  Serial.print(pwmValue);
  Serial.print(", Active PWM pin: ");
  Serial.println(activePin);
}

// ============================================================================
// setup()
// ============================================================================
void setup() {
  Serial.begin(9600);
  Serial.println(VERSION);   // identifies the build on the board
  delay(500);

  pinMode(RPWM_PIN, OUTPUT);
  pinMode(LPWM_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, INPUT_PULLUP);  // not read this sketch; wired to 5V per assignment

  // Fixed direction: pin 9 stays LOW for the whole sketch, all PWM goes to pin 10.
  analogWrite(RPWM_PIN, 0);
  analogWrite(LPWM_PIN, 0);

  Serial.println("============================================================");
  Serial.println("Module 3: First Manual Sketch -- Fixed Direction");
  Serial.println("Pin 9 (RPWM): fixed LOW. Pin 10 (LPWM): trim-pot PWM command.");
  Serial.println("Serial Monitor only -- no plotting in this sketch.");
  Serial.println("Do not apply actuator power before instructor approval.");
  Serial.println("============================================================");

  startTime = millis();
  lastReportTime = millis();
}

// ============================================================================
// loop()
// ============================================================================
void loop() {
  unsigned long currentTime = millis();

  // Average the trim pot and drive pin 10 continuously, so the H-bridge
  // tracks the knob in real time even between printed reports.
  float avgTrimPot = averageAnalogSamples(TRIM_POT_PIN);
  int pwmValue = mapAdcToPwm(avgTrimPot);
  analogWrite(RPWM_PIN, 0);           // fixed side stays LOW
  analogWrite(LPWM_PIN, pwmValue);    // fixed direction: all PWM on pin 10

  if (currentTime - lastReportTime >= REPORT_INTERVAL) {
    lastReportTime = currentTime;

    // Thermistor chain: average ADC -> voltage -> resistance -> temperature
    float avgThermistor = averageAnalogSamples(THERMISTOR_PIN);
    float voltage = adcToVoltage(avgThermistor);
    float resistance = voltageToResistance(voltage);
    float temperature = resistanceToCelsius(resistance);

    float elapsedTime = (currentTime - startTime) / 1000.0;

    printStatus(temperature, elapsedTime, pwmValue, ACTIVE_PWM_PIN);
  }
}
