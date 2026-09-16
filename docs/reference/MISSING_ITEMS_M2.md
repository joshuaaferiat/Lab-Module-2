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
| `m02_trimpot_pwm_hbridge` uploaded and bench-tested (3A) | `data/module_02/m02_trimpot_pwm_20260916_run{1,2,3,4}.txt`, note §3 |
| Motor direction and speed test (3C), by direct observation | note §3 — clockwise/counterclockwise and low-vs-high-PWM speed trend confirmed; recorded from recollection, not a contemporaneous note or photo |
| Fixed resistor and β confirmed | note §1 — nominal/rated resistor value and β = 4540 K, both per the team from the datasheet |

## Missing

**Parts 1 and 2 were done in class, 3A is uploaded and bench-verified, and 3C's motor test was
actually run — this list used to say Part 3 hadn't started at all, which is well out of date
now.** What's left is finishing 3B (still nothing usable on the scope) and closing the gaps 3C
left behind, before S6 on 16 September.

### Needs no oscilloscope

1. **Three human-readable serial lines** from `m02_thermistor_serial`. If you did not save them
   during class, reconnect the thermistor divider and re-run — the sketch is unchanged, and the
   note only needs three consecutive lines.
2. ~~Measured value of the fixed resistor, and confirmation of which β pair the datasheet
   quotes.~~ **Done** — the team is using the resistor's own nominal/rated value (not an
   independent multimeter measurement — that distinction is noted in note §1) and has confirmed
   β = 4540 K directly from the datasheet (B57861S0104F040V24, referenced from 25 °C).

### Needs the bench

3. ~~Upload `m02_trimpot_pwm_hbridge` and confirm the trim pot moves the printed PWM value and
   the direction input switches the reported mode.~~ **Done** — see the four captures in
   `data/module_02/` and note §3. The full 0–1023 → 0–255 range and clean HEAT/COOL switching
   are both confirmed in the logs.
4. **Add the pins-9/10 oscilloscope video and review it.** The one photo (`figures/module_02/
   m02_fig05_scope_trimpot_pwm.jpg`) still doesn't count: channels unlabeled, VOLTS/DIV and
   TIME/DIV illegible (TIME/DIV is literally out of frame), and the trace is a row of
   disconnected dashes rather than a continuous square wave. The team reports a video exists that
   should do better, but it isn't in the repository yet — add it under `figures/module_02/` or a
   new `videos/module_02/` folder, then this item can be checked against what the video actually
   shows (labeled channels, legible knobs, a locked trace) rather than against a description of
   it.
5. **Oscilloscope evidence for M+ and M-** in both directions, probe ground on Arduino `GND`.
   Still fully open. Neither the motor test nor the reported pins-9/10 video touches this — M+/M-
   is an electrical reading of the H-bridge's *output* side, and nothing so far has probed it;
   the motor spinning correctly is reassuring but not a substitute (see note §3 for why).
6. ~~Motor direction and PWM speed observations (3C)~~ **Done, informally** — the team directly
   observed clockwise rotation under HEAT and counterclockwise under COOL, matching the logged
   `Direction` column, and a visible speed increase toward PWM 255 versus barely turning near PWM
   15–20. This is recorded in note §3 from recollection right after the session, not a
   contemporaneous note or photo — the team reports this was also captured on video, which would
   upgrade it to documented once that file is added to the repository and reviewed. M+/M- were
   not separately observed during the motor test either way — see item 5.
7. **Instructor check.** The safety boundary says this happens *before* the motor test, but per
   item 6 the motor was already run. Confirm with the instructor how to handle that at checkoff —
   this is a process question worth raising directly rather than deciding unilaterally.

## Carry these habits over from Module 1

- **Record VOLTS/DIV and TIME/DIV with every trace.** The analog 2120B puts nothing on screen to
  recover them from. In Module 1 this cost a frequency reading: at 2.72 divisions per period no
  standard timebase step gave the expected 490 Hz, and the setting could not be reconstructed
  afterwards.
- **Frame scope photos so the knobs are in shot**, and write the settings down anyway.
- **Save the raw serial text, not only screenshots.** A screenshot cannot be re-analysed.
- **Duty cycle is calibration-free** — the ratio of the high run to the rise-to-rise spacing
  survives any uncertainty in the knob settings. Period and voltage do not.
