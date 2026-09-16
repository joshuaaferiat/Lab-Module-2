# C2 Team Checkoff — Moodle receipt

**One team member submits, by 5:00 PM. C2 is demonstrated during S6, Wednesday 16 September.**

Follow the C2 rubric and oral-question bank. The receipt does not replace the in-class
demonstration or the individual oral questions.

```
Milestone: C2

Student name(s): <both full names>

Repository URL: <https://github.com/ORG/LabModule2>

Branch: main

Full commit hash: <git log -1 --format=%H>

README path: README.md

Arduino sketch path(s):
arduino/m02_thermistor_serial/m02_thermistor_serial.ino
arduino/m02_thermistor_plotter/m02_thermistor_plotter.ino
arduino/m02_trimpot_pwm_hbridge/m02_trimpot_pwm_hbridge.ino
  <-- uploaded and bench-tested by the team (data/module_02/m02_trimpot_pwm_20260916_run*.txt),
      but NOT yet demonstrated to the instructor. Keep this line only if the instructor has
      actually checked the H-bridge signals in person by the time this receipt is submitted —
      otherwise drop it and note 3A as "uploaded, pending instructor check" in the description.

Brief description of what was demonstrated:
<FILL IN after the demonstration — describe only what was actually shown.>
```

## Likely oral ground

**Derive the resistance equation from the divider circuit.**
Circuit: `5V → R_fixed (100 kΩ) → node (A0) → thermistor (R_th) → GND`. Same current flows
through both resistors (series loop, and the ADC pin draws essentially none), so:

    I = (V_ref − V_A0) / R_fixed        (through the fixed resistor, into the node)
    I = V_A0 / R_th                     (through the thermistor, out of the node)

Setting these equal and solving for R_th:

    V_A0 / R_th = (V_ref − V_A0) / R_fixed
    V_A0 · R_fixed = R_th · (V_ref − V_A0)
    R_th = R_fixed · V_A0 / (V_ref − V_A0)

— exactly what `voltageToResistance()` computes. (Equivalently, start from the standard divider
formula V_A0 = V_ref · R_th/(R_fixed + R_th) and cross-multiply to the same result — be ready to
produce either derivation.) Physical sanity check: R_th is the *bottom* leg, so as the thermistor
heats and its resistance drops, V_A0 drops too — that's why Figure 2's warming trace moves the
direction it does, and a swapped resistor/thermistor would flip that sign.

**Why average the ADC counts before converting, rather than averaging temperatures?**
The ADC-to-temperature map is non-linear — a reciprocal and a logarithm — and the average of a
non-linear function is not the function of the average, so averaging temperatures biases the
result. Averaging first also puts the √N noise reduction on the quantity that carries the noise.

**Why is a temperature more model-dependent than a voltage?**
See §4 Q2 of the evidence note: the divider equation, the β approximation and the R₀ tolerance
each add an assumption on top of the one calibration constant a voltage needs.

**Where does the scope ground clip go, and why does it matter?**
Arduino `GND`, always. `M+` and `M-` are both driven H-bridge outputs; grounding either through
the oscilloscope shorts that half-bridge.

**For heating, cooling, and zero output, what should the two H-bridge PWM inputs show?**

| Case | Pin 9 (RPWM) | Pin 10 (LPWM) |
|---|---|---|
| Heat / clockwise | PWM (duty = trim-pot setting) | solid 0 V |
| Cool / counterclockwise | solid 0 V | PWM (duty = trim-pot setting) |
| Zero output | 0 V | 0 V |

Pin 11 is not one of these two — it's a digital *input* the Arduino reads to pick which of 9/10
gets the PWM; it never carries PWM itself. Whichever of pins 9/10 *isn't* the active one for the
current direction must be a solid 0 V, not floating: the original 3A code left pin 11 as plain
`INPUT`, so with nothing wired to it, it floated and read noise, which could flip the commanded
direction unpredictably — a real hazard once a motor or the TEC is attached. `INPUT_PULLUP` fixed
that. Separately, the code also used to set the active side before clearing the inactive one on a
direction change, briefly commanding both pins together for one instruction — a momentary brake
on a BTS7960, not a short, but still fixed so the inactive side clears first.

**Point to an oscilloscope trace — frequency, duty cycle, voltages, and what they mean physically.**
Frequency: count divisions between one rising edge and the next, multiply by TIME/DIV for the
period, invert it. Pins 9/10 should run ≈490 Hz (Arduino's default fast-PWM rate on Timer1), so
≈2.04 ms between rising edges. Duty cycle: (time high)/(period) — a ratio, so it survives
imprecise knob-reading better than an absolute voltage or time does; it should track duty ≈
commanded PWM/255. Voltage levels differ by which node is probed: pins 9/10 are Arduino logic, so
high ≈ 5 V; `M+`/`M-` are H-bridge outputs switching the bench supply, so high ≈ the actuator
supply voltage (team reports 12 V, not independently confirmed) — seeing a bigger swing on M+/M-
than on 9/10 is itself evidence the level-shifting is working. Physical meaning: PWM doesn't
produce an analog voltage directly, it produces a fast on/off signal whose *time-average* — duty
cycle × supply voltage — is what the motor actually responds to, because its mechanical inertia
is far too slow to react to individual ~2 ms pulses; that averaging is the entire mechanism
behind "turning the trim pot changes the speed."

**How do you know the board is running the sketch you think it is?**
Each sketch prints its `VERSION` string on reset at 9600 baud.
