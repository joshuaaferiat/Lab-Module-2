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
  <-- list this third one only once it has actually been uploaded and demonstrated

Brief description of what was demonstrated:
<FILL IN after the demonstration — describe only what was actually shown.>
```

## Likely oral ground

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

**Which pin carries PWM in each direction?**
Heat/clockwise: pin 9 → RPWM carries PWM, pin 10 → LPWM held at 0 V. Cool: the reverse. The
inactive pin must be a solid 0 V, not floating.

**How do you know the board is running the sketch you think it is?**
Each sketch prints its `VERSION` string on reset at 9600 baud.
