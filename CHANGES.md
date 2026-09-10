# What changed from the uploaded sketches

The three sketches written in class were reorganised into the layout the Module 2 assignment
specifies — a descriptively named folder per sketch under `arduino/`. The originals are kept
byte-for-byte in `arduino/archive/`.

**Delete this file once the restructure is committed and both teammates have read it.**

## File moves

| Was | Now |
|---|---|
| `thermistor1.ino` | `arduino/m02_thermistor_serial/m02_thermistor_serial.ino` |
| `thermistor2.ino` | `arduino/m02_thermistor_plotter/m02_thermistor_plotter.ino` |
| `3a.ino` | `arduino/m02_trimpot_pwm_hbridge/m02_trimpot_pwm_hbridge.ino` |

Each `.ino` sits in a folder of the same name because the Arduino IDE refuses to open a sketch
whose folder and file names disagree.

## Changes to all three

1. **Header block** — team, course, board, wiring, baud, output format, and the Module 2 safety
   boundary.
2. **`const char* VERSION`**, printed immediately after `Serial.begin(9600)`. The first line in
   Serial Monitor now names the sketch and version, so the build actually on the board can be
   identified. Bump it whenever you change a sketch.

## Changes to the thermistor sketches

3. **A comment on the ADC divisor.** These use `/1023`; Module 1 standardised on `/1024`. The
   two differ by 2.4 mV at mid-scale, about 0.04 °C — far below the β-model uncertainty, so it
   changes nothing numerically. Left as `/1023` because that is what produced the recorded data.

No calculation was altered. The divider algebra, the β model and the average-then-convert
ordering are exactly as written in class.

## Two fixes to `3a.ino`

Both were found by reading the code. The sketch had not been run when they were made.

4. **`pinMode(HEAT_COOL_PIN, INPUT)` → `INPUT_PULLUP`.** With a bare `INPUT` and nothing wired
   to pin 11 the input floats and `digitalRead` returns noise, so the commanded H-bridge
   direction would flip unpredictably. With a motor or a TEC connected that is a hazard, not a
   cosmetic problem. The pull-up makes an unwired input read HIGH deterministically:
   pin 11 to `GND` = cool, to `5V` = heat, unwired = heat.

5. **`controlHBridge()` clears the inactive side first.** The original set the active side
   before clearing the other, so on a heat↔cool transition both H-bridge inputs were briefly
   commanded together. On a BTS7960 that is a momentary brake rather than a shoot-through, but
   it is an unintended state and costs nothing to avoid.

Verify with `git diff` against the archived originals.
