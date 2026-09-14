# Module 2 — outstanding for C2

Audited against the "Collect Your C2 Evidence During Class" list.

## Have

| Item | Where |
|---|---|
| Labeled thermistor-divider diagram and constants | note §1, Figure 1 |
| Serial Plotter warming/cooling record | note §2, Figure 2 |
| The heat/cool H-bridge signal table (predicted) | note §3 |
| The two sketches actually run | `arduino/m02_thermistor_serial`, `m02_thermistor_plotter` |
| Trim-pot-to-PWM signal path | note §3 |
| Pre-class questions answered | note §4 |

## Missing

**Parts 1 and 2 were done in class. Part 3 was not started at all** — the sketch has never been
uploaded, no oscilloscope work was done, and the motor was never run. That is the bulk of what
C2 assesses, and it needs a bench session before S6 on 16 September.

### Needs no oscilloscope

1. **Three human-readable serial lines** from `m02_thermistor_serial`. If you did not save them
   during class, reconnect the thermistor divider and re-run — the sketch is unchanged, and the
   note only needs three consecutive lines.
2. **Measured value of the fixed resistor**, and confirmation of which β pair the datasheet
   quotes (β₂₅/₈₅ and β₂₅/₁₀₀ differ by a percent or more).

### Needs the bench — all of Part 3

3. **Upload `m02_trimpot_pwm_hbridge`** and confirm the trim pot moves the printed PWM value and
   the direction input switches the reported mode. Do this with actuator power off.
4. **Oscilloscope evidence for both PWM pins** — voltage, frequency and duty cycle on pins 9 and
   10, in both directions, with the inactive side confirmed at a solid 0 V.
5. **Oscilloscope evidence for M+ and M-** in both directions, probe ground on Arduino `GND`.
6. **Instructor check** of the H-bridge signals, wiring, ground and current limit.
7. **Motor direction and PWM speed observations** (3C), only after that check.

## Carry these habits over from Module 1

- **Record VOLTS/DIV and TIME/DIV with every trace.** The analog 2120B puts nothing on screen to
  recover them from. In Module 1 this cost a frequency reading: at 2.72 divisions per period no
  standard timebase step gave the expected 490 Hz, and the setting could not be reconstructed
  afterwards.
- **Frame scope photos so the knobs are in shot**, and write the settings down anyway.
- **Save the raw serial text, not only screenshots.** A screenshot cannot be re-analysed.
- **Duty cycle is calibration-free** — the ratio of the high run to the rise-to-rise spacing
  survives any uncertainty in the knob settings. Period and voltage do not.
