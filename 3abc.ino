// ============================================================================
// Module 2: Trim Pot PWM, H-Bridge Verification, And Motor Direction
// Part 3A: Trim-Pot PWM and Heat/Cool Direction Control
// ============================================================================

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
  if (isHeat) {
    // Heat / Clockwise: PWM on RPWM, 0V on LPWM
    analogWrite(RPWM_PIN, pwmValue);
    analogWrite(LPWM_PIN, 0);
  } else {
    // Cool / Counterclockwise: 0V on RPWM, PWM on LPWM
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
  delay(500);
  
  // Set up digital pins
  pinMode(HEAT_COOL_PIN, INPUT);      // Heat/Cool selector
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
  Serial.println("Direction: Pin 11 (5V = HEAT, 0V = COOL)");
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