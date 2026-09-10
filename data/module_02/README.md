# data/module_02 — raw captures

**Empty so far.** Nothing has been captured to a file for Module 2; the note's evidence is
currently figures and calculation.

## Naming

    m02_<what>_<YYYYMMDD>.txt

| File to capture | Contents |
|---|---|
| `m02_thermistor_serial_YYYYMMDD.txt` | Serial Monitor output from `m02_thermistor_serial` — the three human-readable lines the note needs come from here |
| `m02_thermistor_plotter_YYYYMMDD.txt` | The bare-number stream behind Figure 2 |
| `m02_trimpot_pwm_YYYYMMDD.txt` | Trim pot ADC / PWM / direction, at the settings used for the scope traces |
| `m02_scope_hbridge_YYYYMMDD.md` | Bench sheet: VOLTS/DIV, TIME/DIV, divisions, and the derived voltage / frequency / duty for every trace |

Start every hand-made file with a header giving the sketch, date, board, wiring and baud, so it
stands alone. Save the raw serial text as well as any screenshot — a screenshot cannot be
re-analysed.
