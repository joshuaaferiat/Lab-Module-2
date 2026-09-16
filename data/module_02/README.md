# data/module_02 — raw captures

## Naming

    m02_<what>_<YYYYMMDD>.txt

| File to capture | Contents | Status |
|---|---|---|
| `m02_thermistor_serial_YYYYMMDD.txt` | Serial Monitor output from `m02_thermistor_serial` — the three human-readable lines the note needs come from here | **Missing** — paste three lines into note §1 and save the source capture here |
| `m02_thermistor_plotter_YYYYMMDD.txt` | The bare-number stream behind Figure 2 | **Missing** — Figure 2 exists but the raw number stream behind it wasn't saved |
| `m02_trimpot_pwm_20260916_run1.txt` | Trim pot ADC / PWM / direction; PWM commanded 5–141 | **Have** — team's `3b.rtf`; first bring-up capture; header repeats and the file cuts off mid-line (normal — Serial Monitor was still writing when the capture stopped) |
| `m02_trimpot_pwm_20260916_run2.txt` | Trim pot ADC / PWM / direction; full 0–255 sweep, both directions | **Have** — team's `3B2.rtf`; cleanest of the four; the HEAT↔COOL transition at unchanged ADC/PWM quoted in note §3 comes from here |
| `m02_trimpot_pwm_20260916_run3.txt` | Trim pot ADC / PWM / direction; 0–255, COOL direction only | **Have** — team's `3C.rtf`; filename tracks Part 3C but content is the same telemetry format, not a motor observation — see note §3 |
| `m02_trimpot_pwm_20260916_run4.txt` | Trim pot ADC / PWM / direction; full 0–255 sweep, longest capture (10174 lines) | **Have** — team's `3C3.rtf`; also filed under 3C |
| `m02_scope_hbridge_YYYYMMDD.md` | Bench sheet: VOLTS/DIV, TIME/DIV, divisions, and the derived voltage / frequency / duty for every trace | **Template only** — see `m02_scope_hbridge_TEMPLATE.md` in this folder. Fill it in at the bench with real readings, rename it to the actual date, and delete the `_TEMPLATE` file. This is the actual gap in 3B; `figures/module_02/m02_fig05_scope_trimpot_pwm.jpg` exists but without this sheet its knob settings can't be recovered |

**Dates on the three trim-pot files are inferred** (session S6, 16 September, when the
oscilloscope and bench photos were also taken) — confirm and correct if the captures were
actually taken on a different day.

Start every hand-made file with a header giving the sketch, date, board, wiring and baud, so it
stands alone. Save the raw serial text as well as any screenshot — a screenshot cannot be
re-analysed.
