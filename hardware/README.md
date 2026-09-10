# hardware/ — Module 2

## Identified

| Item | Identification |
|---|---|
| Microcontroller | ELEGOO UNO R3 (ATmega328P) |
| Thermistor | 100 kΩ NTC, marked B57861S0104F040V24 (EPCOS/TDK), β = 4540 K per the sketch — **confirm which β pair the datasheet quotes** |
| Fixed divider resistor | 100 kΩ — measure and record the actual value |
| H-bridge | BTS7960 module (blue PCB, marked BT_2), on heatsink |
| Trim pot | 100 kΩ, breadboard trimmer |
| Bench supply | ALITOVE switching supply, 110/220 V selectable — **off for all of Module 2** |
| Actuator assembly | TEC 8 cold plate with liquid fitting — **disconnected for all of Module 2** |
| Oscilloscope | BK Precision 2120B, 30 MHz dual trace, analog |

## H-bridge connections (BTS7960)

| BTS7960 pin | Goes to | Note |
|---|---|---|
| `RPWM` | Arduino pin 9 | heat / clockwise |
| `LPWM` | Arduino pin 10 | cool / counterclockwise |
| `R_EN`, `L_EN` | Arduino 5V | both enables held high |
| `VCC` | Arduino 5V | logic supply |
| `GND` | Arduino GND | logic ground, common with the scope |
| `R_IS`, `L_IS` | not connected | current sense, unused |
| `B+`, `B-` | bench supply V+ / V- | direct, not through the terminal bus |
| `M+`, `M-` | motor, via two isolated paired terminal-bus positions | **never a scope ground** |

## Oscilloscope grounding

Every probe ground clip connects to Arduino `GND`. `M+` and `M-` are both *driven* outputs, not
ground references — clipping a scope ground to either one shorts that half-bridge through the
instrument's chassis ground and can destroy the module and the supply. If only one channel is
available, look at the outputs one at a time with the ground staying on Arduino `GND`.
