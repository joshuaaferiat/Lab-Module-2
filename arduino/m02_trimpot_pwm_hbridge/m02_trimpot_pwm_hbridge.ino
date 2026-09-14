/*
  m02_trimpot_pwm_hbridge.ino
  Module 2 -- First Real Instrument Pieces
  Part 3A -- trim pot sets PWM, a digital input selects heat/cool, and the pair
           drives the BTS7960 H-bridge RPWM / LPWM inputs.

  Team:      TEC 8
  Course:    Phys 39 -- Instrumentation and Thermal Physics
  Board:     Arduino Uno (ELEGOO UNO R3), USB powered.
  Wiring:    Trim pot wiper -> A1 (outer terminals 5V and GND).
             Direction input -> pin 11.  Pin 9 -> RPWM, pin 10 -> LPWM.
             H-bridge R_EN, L_EN, VCC -> Arduino 5V;  GND -> Arduino GND.
             R_IS / L_IS left unconnected.
  Baud:      9600
  Output:    Trim Pot ADC / PWM / Direction, every 100 ms

  SAFETY:    TEC disconnected for all of Module 2. Actuator power stays OFF until
             the H-bridge input signals have been checked on the oscilloscope.
             Every scope probe ground clip goes to Arduino GND -- NEVER to M+ or M-,
             which are both driven H-bridge outputs.

  Version:   v1 (2026-09-10)
*/

const char* VERSION = "m02_trimpot_pwm_hbridge v1";

// Pin definitions
const int TRIM_POT_PIN = A1;          // Analog input for trim pot
const int HEAT_COOL_PIN = 11;         // Digital input for heat/cool direction
const int RPWM_PIN = 9;               // H-bridge RPWM (heat/clockwise)
const int LPWM_PIN = 10;              // H-bridge LPWM (cool/counterclockwise)

// Measurement constants
const int NUM_SAMPLES = 100;           // Number of ADC samples per measurement
const unsigned long REPORT_INTERVAL = 100;  // Report every 100 ms

// Timing
unsigned long lastReportTime = 0;

// ============================================================================
// Function: averageTrimPotSamples()
// Read trim pot (A1) NUM_SAMPLES times and return the average ADC value
// ============================================================================
float averageTrimPotSamples() {
  long sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(TRIM_POT_PIN);
  }
  return sum / (float)NUM_SAMPLES;
}

// ============================================================================
// Function: mapAdcToPwm()
// Convert average ADC count (0-1023) to PWM value (0-255)
// ============================================================================
int mapAdcToPwm(float avgAdc) {
  // Map 0-1023 to 0-255
  return (int)(avgAdc * 255.0 / 1023.0);
}

// ============================================================================
// Function: controlHBridge()
// Set H-bridge direction and PWM based on heat/cool input
// Heat (5V on pin 11): RPWM = PWM command, LPWM = 0
// Cool (0V on pin 11): RPWM = 0, LPWM = PWM command
// ============================================================================
void controlHBridge(int pwmValue, bool isHeat) {
  // FIX: always drive the INACTIVE side to 0 FIRST, then the active side.
  // The original set the active side before clearing the other, so on a
  // direction change both H-bridge inputs were briefly commanded together.
  // On a BTS7960 that is a momentary brake rather than a shoot-through, but it
  // is still an unintended state and it is free to avoid.
  if (isHeat) {
    // Heat / clockwise: PWM on RPWM, 0 V on LPWM
    analogWrite(LPWM_PIN, 0);
    analogWrite(RPWM_PIN, pwmValue);
  } else {
    // Cool / counterclockwise: 0 V on RPWM, PWM on LPWM
    analogWrite(RPWM_PIN, 0);
    analogWrite(LPWM_PIN, pwmValue);
  }
}

// ============================================================================
// Function: printStatus()
// Print current trim pot reading, PWM value, and direction
// ============================================================================
void printStatus(float avgAdc, int pwmValue, bool isHeat) {
  Serial.print("Trim Pot ADC = ");
  Serial.print(avgAdc, 1);
  Serial.print("    PWM = ");
  Serial.print(pwmValue);
  Serial.print("    Direction = ");
  Serial.println(isHeat ? "HEAT (clockwise)" : "COOL (counterclockwise)");
}

// ============================================================================
// setup()
// Initialize pins and Serial communication
// ============================================================================
void setup() {
  Serial.begin(9600);
  Serial.println(VERSION);   // identifies the build on the board
  delay(500);
  
  // Set up digital pins
  // FIX: INPUT_PULLUP, not bare INPUT. With a plain INPUT and nothing wired to
  // pin 11 the pin FLOATS and digitalRead returns noise, so the H-bridge
  // direction would flip unpredictably while a load is connected. The pull-up
  // makes an unwired input read HIGH deterministically.
  //   wire pin 11 to GND  -> LOW  -> cool / counterclockwise
  //   wire pin 11 to 5V   -> HIGH -> heat / clockwise
  //   nothing wired       -> HIGH -> heat  (see the warning below)
  // The direction pin must be wired before actuator power is applied. PWM is
  // still commanded to 0 in setup(), so nothing moves until the trim pot is
  // turned up.
  pinMode(HEAT_COOL_PIN, INPUT_PULLUP);   // Heat/Cool selector
  pinMode(RPWM_PIN, OUTPUT);          // H-bridge RPWM output
  pinMode(LPWM_PIN, OUTPUT);          // H-bridge LPWM output
  
  // Initialize H-bridge outputs to OFF
  analogWrite(RPWM_PIN, 0);
  analogWrite(LPWM_PIN, 0);
  
  // Print heading
  Serial.println("============================================================");
  Serial.println("Module 2: Trim Pot PWM and H-Bridge Control");
  Serial.println("Part 3A: Trim-Pot to PWM mapping and heat/cool direction");
  Serial.println("============================================================");
  Serial.println("Trim Pot: A1 (0-1023) -> PWM (0-255)");
  Serial.println("Direction: Pin 11 with INPUT_PULLUP (5V/open = HEAT, GND = COOL)");
  Serial.println("H-Bridge: Pin 9 (RPWM) and Pin 10 (LPWM)");
  Serial.println("============================================================");
  
  lastReportTime = millis();
}

// ============================================================================
// loop()
// Continuously read trim pot, map to PWM, read direction, control H-bridge
// ============================================================================
void loop() {
  unsigned long currentTime = millis();
  
  // Average the trim pot readings
  float avgAdc = averageTrimPotSamples();
  
  // Map ADC value to PWM (0-255)
  int pwmValue = mapAdcToPwm(avgAdc);
  
  // Read heat/cool direction from digital pin
  // HIGH (5V) = heat/clockwise, LOW (0V) = cool/counterclockwise
  bool isHeat = digitalRead(HEAT_COOL_PIN) == HIGH;
  
  // Control the H-bridge with the PWM value and direction
  controlHBridge(pwmValue, isHeat);
  
  // Print status periodically
  if (currentTime - lastReportTime >= REPORT_INTERVAL) {
    lastReportTime = currentTime;
    printStatus(avgAdc, pwmValue, isHeat);
  }
}