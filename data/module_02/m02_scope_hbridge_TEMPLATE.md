# Bench sheet — oscilloscope verification of the H-bridge signals (Part 3B)

**Fill this in at the bench, while the numbers are in front of you — not from memory afterward.**
Rename this file `m02_scope_hbridge_YYYYMMDD.md` (today's date) once filled in, and link it from
`docs/module_notes/module_02_instrument_pieces.md` §3 in place of the current "not recorded"
table rows.

Sketch: `m02_trimpot_pwm_hbridge` (`arduino/m02_trimpot_pwm_hbridge/`)
Board: ELEGOO UNO R3, USB powered for logic; bench supply for actuator power
Oscilloscope: BK Precision 2120B, 30 MHz dual trace, analog
Baud: 9600 (Serial Monitor open alongside, to read the commanded PWM/direction at capture time)
Date: `FILL IN`
Bench-supply output voltage (measured, multimeter): `FILL IN` — the note currently says "12 V,
per the team, not independently confirmed"; replace this line and the note's hedge once measured

Scope ground clip: Arduino `GND` for every trace below. **Never** on `M+` or `M-`.

---

## Pins 9 (RPWM) and 10 (LPWM) — Arduino side of the H-bridge

For each row: set the trim pot to a fixed, known PWM value (read it off Serial Monitor) before
capturing, so the "commanded" column has something real to compare against.

| Direction | Probe | Commanded PWM (from Serial Monitor) | VOLTS/DIV | TIME/DIV | High level | Low level | Divisions per period | Period | Frequency | Duty cycle (measured) | Duty cycle (commanded/255) | Photo |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| HEAT (clockwise) | Pin 9 (RPWM) | | | | | | | | | | | |
| HEAT (clockwise) | Pin 10 (LPWM) — should read ~0 V, inactive | | | | | | | | | | | |
| COOL (counterclockwise) | Pin 9 (RPWM) — should read ~0 V, inactive | | | | | | | | | | | |
| COOL (counterclockwise) | Pin 10 (LPWM) | | | | | | | | | | | |

Frequency = 1 / Period. Duty cycle (measured) = high-time / period — this ratio is the one
number here that survives any uncertainty in the VOLTS/DIV or TIME/DIV knob reading, so get it
even if a voltage or timebase number feels unreliable.

## M+ and M- — H-bridge output to the motor

Same idea, same knob settings recorded per trace. Expect a higher-voltage square wave here than
on pins 9/10 (bench-supply level, not 5 V logic level) if the H-bridge is doing its job.

| Direction | Probe | VOLTS/DIV | TIME/DIV | High level | Low level | Divisions per period | Period | Frequency | Duty cycle (measured) | Photo |
|---|---|---|---|---|---|---|---|---|---|---|
| HEAT (clockwise) | M+ | | | | | | | | | |
| HEAT (clockwise) | M- — should read ~0 V | | | | | | | | | |
| COOL (counterclockwise) | M+ — should read ~0 V | | | | | | | | | |
| COOL (counterclockwise) | M- | | | | | | | | | |

## Trigger and framing notes

- Lock the trigger before shooting — an unlocked trace shows as two floating bands with no
  visible edges and can't be measured. (This is very likely what went wrong in
  `m02_fig05_scope_trimpot_pwm.jpg` — a row of disconnected dashes rather than a continuous
  square wave.)
- Frame every photo so both VOLTS/DIV and TIME/DIV knobs are visible, not just the screen —
  Figure 5's TIME/DIV knob was cropped out of frame entirely, which is why nothing on that
  photo could be measured after the fact.
- Save each photo as `figures/module_02/m02_fig0N_<probe>_<direction>.jpg` and list the mapping
  here once taken, so the note's figure references stay accurate:

| Figure | File | What it shows |
|---|---|---|
| `FILL IN` | `FILL IN` | `FILL IN` |

---

## Instructor sign-off

Fill in after the instructor actually looks at these signals. Don't write anything here before
that conversation happens.

- Instructor name: `FILL IN`
- Date/time checked: `FILL IN`
- What they checked (signals only? wiring and ground too? current limit?): `FILL IN`
- Their verdict, in their own words if possible: `FILL IN`
- Did they know the motor test (3C) had already run before this check? `FILL IN` — this note's
  README and evidence note both flag that the motor ran ahead of this check, out of the order
  the safety boundary specifies; worth confirming this was discussed, not just noted here.
