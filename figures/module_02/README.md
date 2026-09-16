# figures/module_02

## Naming

    m02_figNN_<short_description>.<png|jpg>

PNG for screenshots, JPG for photographs. The number is the figure number used in the evidence
note, so captions and links stay stable.

## Status

| File | Figure | State |
|---|---|---|
| `m02_fig01_thermistor_divider.jpg` | 1 | **DONE** — the divider on the breadboard |
| `m02_fig02_plotter_warm_cool.png` | 2 | **DONE** — Serial Plotter, 22.0 → 30.5 → 22.6 °C warming and cooling |
| `m02_fig03_hbridge_wiring_overhead.jpg` | 3 | **DONE** — board with the BTS7960 present, TEC disconnected. Hardware documentation only; the H-bridge was not exercised |
| `m02_fig04_hbridge_wiring_closeup.jpg` | 4 | **DONE** — logic connections between Arduino and H-bridge, as found |
| `m02_fig05_scope_trimpot_pwm.jpg` | 5 | **PARTIAL** — a scope capture exists, but channels aren't labeled and VOLTS/DIV, TIME/DIV aren't legible, so it doesn't fill the 3B table on its own; see note §3 |
| — | — | **MISSING:** a labeled, settings-documented oscilloscope trace for pins 9 and 10 in both directions, and for M+ / M- in both directions |

Figures 3 and 4 were each picked from a burst of near-identical shots at the bench; swap in a
sharper frame from the same burst if one turns up.

## Outstanding, and how to shoot it

Part 3B needs oscilloscope evidence that does not exist yet. Two habits carried over from
Module 1, where not following them cost a frequency reading:

- **Frame the shot so the VOLTS/DIV and TIME/DIV knobs are in it**, and write the settings down
  as well. The 2120B is analog — it puts nothing on screen to recover them from afterwards.
- **Lock the trigger before shooting.** An unlocked trace shows as two floating bands with no
  visible edges and cannot be measured.

Duty cycle is the exception worth knowing: it is the ratio of the high run to the rise-to-rise
spacing, so it survives any uncertainty in the knob settings. Period and voltage do not.

## Rules

- Capture the whole window, including axes and any on-screen settings.
- Every figure must be referenced from the evidence note; unreferenced files are clutter.
- Keep files to a few MB — resize oversized phone photos rather than committing 8 MB JPEGs.
