# arduino/

The Module 2 assignment asks for "the authoritative sketch in a descriptively named folder
under `arduino/`" — hence this layout rather than Module 1's `firmware/module_NN/`.

## Folder rule (Arduino IDE requirement)

Each sketch sits in a folder whose name exactly matches the `.ino` filename:

    arduino/m02_thermistor_serial/m02_thermistor_serial.ino   <- opens correctly
    arduino/thermistor1.ino                                    <- IDE will relocate it

The IDE also merges every `.ino` in one folder into a single compilation unit, which is why
each sketch needs its own folder rather than sharing one.

## Sketches

All at 9600 baud. Each prints its `VERSION` string on reset.

| Folder | Part | Purpose | Serial output |
|---|---|---|---|
| `m02_thermistor_serial` | 1 | Averaged ADC → voltage → resistance → temperature | one labeled line, `time = … average ADC = … voltage = … V …` |
| `m02_thermistor_plotter` | 2 | Same chain, temperature only | one bare number per line, e.g. `24.91` |
| `m02_trimpot_pwm_hbridge` | 3A | Trim pot → PWM, direction input selects heat/cool | `Trim Pot ADC / PWM / Direction` every 100 ms |

`m02_trimpot_pwm_hbridge` compiles but has **never been uploaded**. Part 3 was not performed.

## The two output formats, and why they differ

Serial Monitor and Serial Plotter want opposite things. The monitor wants labels and units so a
person can read one line without memorising a column order. The plotter wants exactly one bare
number per line and silently discards anything it cannot parse. The same measurement chain
therefore appears twice, differing only in the final `print` — which is the cleanest possible
demonstration that display formatting and measurement are separate concerns.

## Version strings

`const char* VERSION` is printed straight after `Serial.begin(9600)`, so the first line in
Serial Monitor names the build on the board. This is how you answer "how do you know the board
is running that sketch?" — bump the version when you change a sketch and watch the line change.

## archive/

Holds the three sketches exactly as written in class, before the restructure. Each sits in its
own folder so the IDE can still open it. They produced the recorded data and are kept for
provenance; do not edit them. See `CHANGES.md` for what differs.
