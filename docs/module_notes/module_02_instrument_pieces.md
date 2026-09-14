# Module 2 — First Real Instrument Pieces

**Phys 39 — Instrumentation and Thermal Physics · Team TEC 8**

| | |
|---|---|
| **Evidence for** | C2, Measurement And Actuator Electronics — demonstrated S6, Wed 16 September |
| **Team members** | `FILL IN: both members' full names` |
| **Date of measurements** | `FILL IN` |
| **Repository URL** | `FILL IN` |
| **Git checkpoint (GC) commit** | `FILL IN: full 40-character hash` |

> **Status.** Parts 1 and 2 were completed in class. Part 3 was not started — the sketch is
> written but has not been uploaded, and no oscilloscope or motor work was done.
>
> **Safety.** The TEC was disconnected throughout, and actuator power was never applied. The
> thermistor circuit ran on Arduino USB power only. For the Part 3 session: every oscilloscope
> probe ground clip goes to Arduino `GND` — never to `M+` or `M-`, which are both driven
> H-bridge outputs.

---

## 1. Thermistor divider

![Thermistor divider](../../figures/module_02/m02_fig01_thermistor_divider.jpg)

**Figure 1.** The thermistor divider on the breadboard.

**Circuit.** `5V` → 100 kΩ fixed resistor → `A0` → thermistor → `GND`. The thermistor is the
lower leg, so `A0` sits across it and the measured voltage *falls* as the thermistor warms and
its resistance drops.

    V_A0 = V_ref · R_th / (R_fixed + R_th)
    R_th = R_fixed · V_A0 / (V_ref − V_A0)

### Constants used

| Constant | Value | Where from |
|---|---|---|
| `V_REF` | 5.00 V | nominal Arduino supply, not measured |
| `FIXED_RESISTOR` | 100 000 Ω | measured value: `FILL IN` Ω |
| `R0` | 100 000 Ω at 25 °C | thermistor datasheet |
| `T0` | 298.15 K | 25 °C |
| `BETA` | 4540 K | datasheet, B57861S0104F040V24 — **confirm which β this is** (β₂₅/₈₅ and β₂₅/₁₀₀ differ) |
| `NUM_SAMPLES` | 100 | within the required 100–1000 |

### Predicted divider voltages

From R(T) = R₀·exp[β(1/T − 1/T₀)]:

| T (°C) | R_th (kΩ) | V_A0 (V) | Expected ADC |
|---|---|---|---|
| 15 | 169.63 | 3.146 | 644 |
| 25 | 100.00 | 2.500 | 512 |
| 35 | 61.01 | 1.895 | 388 |

**Sensitivity.** Near 25 °C, dV/dT ≈ **−63.8 mV/°C**. One ADC count is 4.88 mV, so the
quantisation floor corresponds to about **0.076 °C** — the divider is comfortably matched to the
converter, and temperature resolution is limited by noise and by the β model rather than by the
ADC.

### Three human-readable serial lines

Sketch: [`m02_thermistor_serial`](../../arduino/m02_thermistor_serial/m02_thermistor_serial.ino)

```
FILL IN: paste three consecutive lines copied from Serial Monitor, e.g.
time = 1.50 s    average ADC = 511.8    voltage = 2.501 V    resistance = 100.23 kOhm    temperature = 24.9 C    samples = 100
```

### The conversion chain

Every reported temperature follows the required order — **average the raw ADC counts first,
then convert once**:

    100 raw analogRead(A0) samples
      → average ADC count            averageAdcSamples()
      → one average voltage          adcToVoltage()      V = ADC/1023 × V_ref
      → thermistor resistance        voltageToResistance()  R = R_f·V/(V_ref−V)
      → temperature                  resistanceToCelsius()  1/T = 1/T₀ + (1/β)·ln(R/R₀)

Converting each raw sample to a temperature and then averaging the temperatures would be
wrong here, and not only stylistically: the ADC → temperature map is **non-linear** (a
logarithm and a reciprocal), and the average of a non-linear function is not the function of
the average. Averaging first also lets the √N noise reduction from Module 1 act on the
quantity that actually carries the noise.

**On the divisor.** These sketches use `/1023`; Module 1 standardised on `/1024`. At mid-scale
the two differ by 2.4 mV, about 0.04 °C — negligible against the β-model uncertainty, but the
note should say which convention produced the numbers. These used `/1023`.

---

## 2. Serial Plotter output

![Serial Plotter warming and cooling](../../figures/module_02/m02_fig02_plotter_warm_cool.png)

**Figure 2.** Temperature versus serial read order, 9600 baud on `/dev/cu.usbmodem1101`.

Sketch: [`m02_thermistor_plotter`](../../arduino/m02_thermistor_plotter/m02_thermistor_plotter.ino)

**The code change.** `printHumanReadable()` was dropped and `loop()` now ends with a single
`Serial.println(temperature, 2);`. Everything before that line — the averaging and the
conversion chain — is unchanged. Serial Plotter needs exactly one bare number per line; the
labels and units that make Serial Monitor readable are precisely what stop the plotter working.

**What the trace shows.** Reading order 6 → 55:

| Phase | Reading order | Temperature |
|---|---|---|
| Baseline, untouched | 6 → 17 | 23.1 → 22.0 °C, drifting slowly down |
| Thermistor gripped between finger and thumb | 18 → 27 | 22.0 → **30.5 °C**, a sharp rise |
| Released, cooling in still air | 28 → 55 | 30.5 → 22.6 °C, a slow decay |

**Direction check — as expected.** Warming the thermistor raises the reported temperature and
releasing it lowers the reading back toward ambient, so the sign of the whole chain is right.
Getting this backwards is the classic symptom of a swapped divider (thermistor on the top leg),
because an NTC's resistance *falls* as it warms.

**Warming is fast, cooling is slow.** The rise takes about 9 reading intervals and the decay
about 28. That asymmetry is physical, not an artefact: a fingertip is a low-thermal-resistance
path that actively drives heat in, while cooling relies on free convection to still air through
a much higher thermal resistance. The same asymmetry will appear in the TEC loop — driven
transitions are fast, passive ones are slow — and it is one reason a controller needs an
actuator that can push in *both* directions rather than heating and waiting.

**Reading order is not time.** The plotter's x-axis counts serial lines, not seconds. With
`REPORT_INTERVAL = 1000 ms` one reading ≈ 1 s, so the axis is *approximately* seconds — but
that is a property of the sketch's timing, not of the plotter, and it would silently break if
the interval changed. Nothing in Figure 2 measures time.

---

## 3. Trim pot → PWM → H-bridge — not performed

**Parts 1 and 2 were completed in class; Part 3 was not.** The sketch below is written and
compiles, but it has never been uploaded to the board, no oscilloscope evidence was taken, and
the motor was never run. Everything in this section is therefore a **prediction from the code
and the datasheet**, not a measurement. The section is kept so the plan and the reasoning are
on record before the bench work happens.

Sketch: [`m02_trimpot_pwm_hbridge`](../../arduino/m02_trimpot_pwm_hbridge/m02_trimpot_pwm_hbridge.ino)
— written, compiles, **not yet uploaded**.

### Signal path

    trim pot wiper (A1)
      → averageTrimPotSamples()   100 reads, averaged     0 … 1023
      → mapAdcToPwm()             × 255/1023              0 … 255
      → analogWrite()             on pin 9 or pin 10
      → BTS7960 RPWM / LPWM input

The direction input on pin 11 chooses *which* pin carries the PWM; the trim pot sets *how much*.
The two commands are independent, which is exactly the structure the temperature controller
needs later — a signed control effort split into magnitude and direction.

### H-bridge signal table — predicted

From the code and the BTS7960 datasheet. Nothing in this table has been observed.

| Direction input (pin 11) | Mode | Pin 9 → RPWM | Pin 10 → LPWM |
|---|---|---|---|
| 5 V (HIGH) | heat / clockwise | PWM | 0 V |
| 0 V (LOW) | cool / counterclockwise | 0 V | PWM |

Predicted duty at the pin = commanded value / 255. At ≈490 Hz on pins 9 and 10, the period
should read ≈2.04 ms on both.

### Two fixes made to the Part 3A sketch before it is ever run

Both were found by reading the code, not by running it. Fixing them now is the point of having
reviewed the sketch before the bench session.

**The direction pin was floating.** It was `pinMode(HEAT_COOL_PIN, INPUT)`, so with nothing
wired to pin 11 the input reads noise and the commanded direction flips unpredictably — with a
motor or a TEC attached, that is a real hazard rather than a cosmetic bug. Changed to
`INPUT_PULLUP`, which makes an unwired input read HIGH deterministically. Wire pin 11 to `GND`
for cool and to `5V` for heat. PWM is still commanded to 0 in `setup()`, so nothing moves until
the trim pot is turned up.

**Direction changes briefly commanded both inputs.** `controlHBridge()` set the active side
before clearing the inactive one, so on a heat↔cool transition both H-bridge inputs were driven
together for one instruction. On a BTS7960 that is a momentary brake rather than a
shoot-through, but it is an unintended state and costs nothing to avoid: the inactive side is
now written to 0 first.

### Oscilloscope verification — 3B

![H-bridge wiring, overhead](../../figures/module_02/m02_fig03_hbridge_wiring_overhead.jpg)

**Figure 3.** The board with the BTS7960 module present and the TEC disconnected. Hardware
documentation only — the H-bridge was not exercised. _Confirm whether this wiring was made by
the team or was already on the board, and caption it accordingly._

![H-bridge wiring, close](../../figures/module_02/m02_fig04_hbridge_wiring_closeup.jpg)

**Figure 4.** The logic connections between the Arduino and the H-bridge module, as found.

**Not measured — Part 3B was not performed.** No oscilloscope evidence was taken in Module 2 at
all. The table below is what 3B requires; every row is outstanding.

| Check | Heat / clockwise | Cool / counterclockwise |
|---|---|---|
| Which pin is active | | |
| Inactive side stays at 0 V? | | |
| High / low voltage | | |
| Period, frequency | | |
| Measured duty vs commanded | | |
| Scope ground on Arduino GND? | | |

Record the VOLTS/DIV and TIME/DIV for every trace — the analog 2120B puts nothing on screen to
recover them from, which cost us a frequency reading in Module 1.

### H-bridge outputs M+ and M- — 3B

**Not measured — Part 3B was not performed.** Both outputs, in both directions, with the probe ground on Arduino `GND`.

| | Heat / clockwise | Cool / counterclockwise |
|---|---|---|
| M+ waveform | | |
| M- waveform | | |
| Where the probe ground clip was connected | Arduino GND | Arduino GND |

> **Never** clip a scope ground to `M+` or `M-`. Both are driven outputs; grounding either one
> through the oscilloscope shorts that half-bridge and can destroy the module.

### Motor test — 3C

**Not performed.** Actuator power was never applied. This requires the instructor to check the
H-bridge signals first, which has also not happened.

| Observation | Heat command | Cool command |
|---|---|---|
| Direction of the tape flag | | |
| Relative speed at low PWM | | |
| Relative speed at high PWM | | |
| What changes on M+ / M- | | |

Expected: heat → clockwise, cool → counterclockwise.

---

## 4. Pre-class questions

**Q1 — Expected divider voltage at 15, 25 and 35 °C.** 3.146 V, 2.500 V and 1.895 V
respectively, from the table in §1. The voltage falls as temperature rises because the NTC
thermistor is the lower leg of the divider and its resistance drops with heating.

**Q2 — Why is a temperature reading more model-dependent than a voltage reading?**
A voltage reading needs only the reference and the converter: ADC count × V_ref/1023 involves
one calibration constant and no physics. Getting from that voltage to a temperature stacks
three modelled steps on top. The divider equation assumes the fixed resistor is exactly its
marked value and that the ADC draws no current. The β model is itself an approximation — a
two-point fit to a curve that is not exactly exponential, so β quoted between different
temperature pairs (β₂₅/₈₅ versus β₂₅/₁₀₀) differs by a percent or more and the model degrades
away from the interval it was fitted over. And R₀ carries the thermistor's own tolerance, often
±1 %, which maps straight into a temperature offset. A voltage measurement can be wrong by the
reference error alone; a temperature can be wrong because any of those assumptions is off,
while the voltage underneath it is perfectly accurate. This is the accuracy-versus-precision
distinction from Module 1 in a new place: averaging tightens the scatter on the voltage and
does nothing at all about a wrong β.

**Q3 — Expected H-bridge inputs for each case.** See the signal table in §3. At PWM = 0 both
inputs are at 0 V and the bridge is idle. For heat, pin 9 carries the PWM and pin 10 is held at
0 V; for cool, the roles swap. In each case the pin *not* carrying PWM must be held at a solid
0 V, not left floating — that is what selects the direction, and a floating input would leave
the bridge state undefined.

**Q4 — One advantage of Serial Plotter over Serial Monitor.** It shows the *shape* of a signal
over time immediately. Figure 2's warm-then-cool excursion, and the asymmetry between the fast
rise and the slow decay, are obvious at a glance and would be nearly invisible in a scrolling
column of numbers. The tradeoff is the reverse of Serial Monitor's: the plotter gives no exact
value for any single point and silently discards any text it cannot parse as a number.

---

## Before submitting

1. Both members' full names, date, repository URL and full commit hash
2. Measured value of the fixed resistor (§1)
3. Confirm which β the datasheet quotes (§1)
4. Three human-readable serial lines pasted into §1
5. **All of Part 3** — upload `m02_trimpot_pwm_hbridge`, verify the command signals on the
   oscilloscope (3B), have the instructor check the wiring, then run the motor test (3C)

**One team member** submits the C2 Team Checkoff Moodle receipt by 5:00 PM. C2 is demonstrated
during S6 on Wednesday 16 September.
