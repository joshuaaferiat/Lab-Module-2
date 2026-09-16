# Lab Module 2 — First Real Instrument Pieces

**Course:** Phys 39 — Instrumentation and Thermal Physics
**Team:** TEC 8
**Repository:** `FILL IN`

Module 2 turns the Module 1 techniques — analog input, averaging, PWM — into the first real
instrument pieces: thermistor temperature measurement, Serial Plotter output, and trim-pot
controlled PWM driving a BTS7960 H-bridge.

> **Safety.** The TEC is disconnected for all of Module 2. Actuator power was applied for the 3C
> motor test — the safety boundary calls for the instructor to check the H-bridge signals on the
> oscilloscope *before* that happens, and that check isn't established to have occurred here (see
> `docs/reference/MISSING_ITEMS_M2.md`). Confirm with the instructor. **Every scope probe ground
> clip goes to Arduino `GND` — never to `M+` or `M-`.** Both are driven H-bridge outputs;
> grounding either through the oscilloscope shorts that half-bridge.

---

## Repository map

| Path | What lives here |
|---|---|
| `arduino/<sketch_name>/` | Authoritative sketches, one folder per sketch; the folder name must equal the `.ino` name |
| `docs/module_notes/module_02_instrument_pieces.md` | The C2 evidence note |
| `figures/module_02/` | Screenshots and photographs referenced by the note |
| `data/module_02/` | Raw serial captures and bench readings |
| `arduino/archive/` | The three sketches exactly as written in class, before the restructure |
| `docs/reference/` | Checkoff receipt and outstanding-items list |
| `hardware/` | Wiring notes and part identification |
| `CHANGES.md` | What differs from the uploaded sketches — delete once committed and read |

Every folder carries a README describing what belongs in it and what is currently there.

## Sketches

All at 9600 baud. Each prints its `VERSION` string on reset, so Serial Monitor identifies the
build actually on the board.

| Sketch | Part | What it does | Uploaded & verified |
|---|---|---|---|
| [`m02_thermistor_serial`](arduino/m02_thermistor_serial/) | 1 | Averaged ADC → voltage → resistance → temperature, one labeled line per report | ☑ produced the §1 serial output |
| [`m02_thermistor_plotter`](arduino/m02_thermistor_plotter/) | 2 | Same chain, prints only the temperature for Serial Plotter | ☑ produced Figure 2 |
| [`m02_trimpot_pwm_hbridge`](arduino/m02_trimpot_pwm_hbridge/) | 3A | Trim pot → PWM, digital input selects heat/cool, drives RPWM/LPWM | ☑ uploaded and bench-tested — see `data/module_02/m02_trimpot_pwm_*.txt`; oscilloscope (3B) and instructor sign-off still outstanding |
| [`m03_manual_fixed_direction`](arduino/m03_manual_fixed_direction/) | Module 3, Part 2 | Thermistor + trim pot in one sketch: pin 9 fixed LOW, all PWM on pin 10, one combined temperature/time/PWM line | ☑ By words in description, direct video is missing |

## Measurement convention

Every reported temperature uses the same acquisition sequence, and this does not change for the
rest of the course:

    100–1000 raw analogRead(A0) samples  →  average the counts  →  one average voltage
      →  thermistor resistance  →  temperature

Never convert each raw sample to a temperature and then average the temperatures. The
ADC → temperature map is non-linear, so the average of the function is not the function of the
average.

## Circuit

- **Thermistor divider:** `5V` → 100 kΩ fixed resistor → `A0` → thermistor → `GND`
- **Trim pot:** outer terminals to `5V` and `GND`, wiper → `A1`
- **Direction input:** pin `11` — `INPUT_PULLUP`; `GND` = cool, `5V` or open = heat
- **H-bridge:** pin `9` → `RPWM`, pin `10` → `LPWM`; `R_EN`, `L_EN`, `VCC` → Arduino `5V`;
  `GND` → Arduino `GND`; `R_IS` and `L_IS` left unconnected

## Progress

| Part | State |
|---|---|
| 1 — thermistor serial data and conversion | **Done in class** |
| 2 — Serial Plotter output | **Done in class** — Figure 2 |
| 3A — trim pot PWM and direction code | **Uploaded and bench-tested** — four serial captures in `data/module_02/` |
| 3B — oscilloscope verification of the command signals | **Done in class** — one scope capture (Figure 5) still doesn't count as evidence (unlabeled channels, illegible knobs, non-square trace); a video of pins 9/10 reportedly exists — M+/M- attempted and evidence in words |
| 3C — motor direction and speed test | **Run, and confirmed by direct observation** — clockwise/counterclockwise matched the commanded direction, and speed visibly rose toward PWM 255; recorded from recollection, not yet from the video the team reports also covers this test — see `docs/reference/MISSING_ITEMS_M2.md` |

Finishing 3B, and confirming the instructor sign-off question raised above, is what's left for
C2, demonstrated S6 on Wednesday 16 September.

## Results so far

| Quantity | Value |
|---|---|
| Predicted divider voltage at 15 / 25 / 35 °C | 3.146 / 2.500 / 1.895 V |
| Sensitivity near 25 °C | −63.8 mV/°C |
| One ADC count in temperature | ≈0.076 °C |
| Serial Plotter warm/cool excursion | 22.0 → 30.5 → 22.6 °C (Figure 2) |

