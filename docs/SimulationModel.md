# Simulation Model

## Loop
`SpacecraftSimulation` ticks every **16 ms** (~60 Hz) using real elapsed time.
- If a frame takes more than 0.1 s, delta is replaced with 0.016 s (prevents jumps after lag or pausing).
- `effectiveDelta = delta × timeMultiplier` (default 1.0, `setTimeMultiplier()` ignores values ≤ 0).
- Signals only emit when a value actually changes.

## Update order (`SpacecraftState::updateAllSystems`)
1. Power
2. Push bus allocations into Propulsion, Life Support, Navigation, Communications (`powerConsumption`)
3. Propulsion
4. Life Support
5. Navigation (receives thrust, propellant %, engine status)
6. Communications (receives distance)
7. Diagnostics (receives ~35 telemetry values)
8. Missions (receives distance, propellant, velocity)
9. Zeta (updates authorization timeout expiration)

## Power
- Orbit period 180 s. Sunlit 0-120 s, **eclipse 120-180 s** (solar = 0).
- Generation = solar (15 kW) + reactor (25 kW).
- Consumption = sum of four bus allocations (each 0-30 kW; Zeta/Power scene change them in ±1 kW steps via `requestPowerReroute`).
- Battery: `battery += net × 0.05 × dt`, clamped 0-100 %. Starts at 85 %.
- If battery = 0 and net < 0: Propulsion, Navigation and Comms buses are set to 0. Life Support is kept.
- A system with 0 kW is frozen and shows the SYSTEM OFFLINE overlay.

## Propulsion
- Heat in: `throttle² × 25` (only when engine on and available). Heat out: `0.15 × (T − 20)`. Temperature clamped 20-150 °C.
- At 150 °C: engine forced off, availability false. Availability returns when temp < 80 °C.
- Cannot ignite while unavailable or out of propellant.
- Power factor = `bus kW / 8` (max 1). Target thrust = `throttle × 500 × powerFactor`.
- Thrust approaches target at spool speed 5 (first-order lag).
- Propellant burn: `thrust/500 × 0.15 % per s`.
- Delta-V (recalculated only while thrusting): `Isp × g0 × ln((dry + fuel) / dry)`.

## Navigation
- Start: 225,000,000 km from Mars, 24.5 km/s.
- Acceleration: `thrust / totalMass × 50 × (navPower / 4)` while thrusting with fuel. Velocity clamped 10-75 km/s.
- Distance: `distance −= velocity × dt × 150` (150× time compression). At nominal speed the trip takes ~17 real hours at 1×.
- ETA (days) = `distance / velocity / 86400`.

| Distance remaining | Position | Destination |
|---|---|---|
| > 180,000,000 km | Earth Departure Orbit | Deep Space Transit |
| > 50,000,000 | Deep Space Cruise | Mars Approach Corridor |
| > 5,000,000 | Mars Approach | Mars Orbit Insertion (MOI) |
| > 0 | Mars Terminal Insertion | Low Mars Orbit (LMO) |
| 0 | Mars Surface / Orbit | Mission Accomplished |

Trajectory text: "Powered Burn Vector" (thrusting), "Heliocentric Coast", "Fuel Depleted - Ballistic Coast", "Stable Martian Capture" (arrived).

## Communications
Signal needs: power > 0, downlink ON, correct link for the current distance, and antenna alignment inside the link's window.

| Link | Alignment window | Centre | Usable at distance |
|---|---|---|---|
| Earth Relay / DSN | 86-100 | 93 | > 180M km |
| Deep Space Gateway | 53-58 | 55.5 | 5M-180M km |
| Mars Orbiter Relay | 36-43 | 39.5 | < 50M km |

- Signal % = `(1 − offsetFromCentre / halfWidth) × 100`, clamped 10-100, × `(commPower / 3)`.
- Data rate = `signal/100 × 256 × powerFactor` (kbps).
- Telemetry Burst +15 packets, Distress Beacon +20 (need downlink on, LOCKED, power > 0). Each sends an HTTP POST to ntfy.sh.

## Life Support
Let `R` = sum of the four crew respiration rates (default 48). Power factor = `lifeSupportPower / 6`.
| Variable | Start | Rule |
|---|---|---|
| O2 partial pressure (kPa) | 21.1 | `−R × 0.002 × dt + (21.1 − O2) × 0.05 × powerFactor × dt` (when powered) |
| CO2 (%) | 0.31 | `+R × 0.0005 − removal`; removal 0.0001 baseline, or `(cartridge%/100) × 0.04 × powerFactor` with scrubbers on |
| LiOH cartridge (%) | 92 | `−0.02 %/s` while active; starts **on** |
| Humidity (%) | 45 | `+R × 0.001`, dehumidify above 45: `(RH−45) × 0.03 × powerFactor` |
| Cabin pressure (kPa) | 101.3 | slight drain from breathing, relaxes to 101.3 |
| Cabin temp (°C) | 21.5 | `+R × 0.001`; above 21.5 cooled by `coolantLoopA(4.2) × 0.05`; below 10.5 heated 0.02 |
| Potable water (%) | 88.4 | `+processed − R×0.001` |
| Greywater (%) | 32.1 | `+R×0.001 − processed`; processed = `grey × WPA(94.8%) × 0.002` |

## Crew (4 astronauts)
Nominal: 36.8 °C, 72 BPM, 98 % SpO2, 12 breaths/min. Limits: BPM 30-220, SpO2 0-100, resp 0-60, temp 30-45 °C. Changed through the web panel.

## Web panel
- Port **8080**, all interfaces. Pages: `/astronaut/1` to `/astronaut/4`.
- Action endpoints: `/astronaut/N/increaseHeartRate | decreaseHeartRate | increaseSPO2 | decreaseSPO2 | increaseRespRate | decreaseRespRate | increaseTemp | decreaseTemp`.
- Step: ±1 (BPM, SpO2, resp), ±0.1 °C. Returns `OK`, or `SYSTEM OFFLINE` if life support has 0 kW.

## Diagnostics
Health: 0 faults NOMINAL, 1-2 DEGRADED, 3+ CRITICAL FAULT.

| Area | Fault condition |
|---|---|
| Propulsion bus | 0 kW (error), < 2 kW (warn), > 20 kW (warn) |
| Life support bus | 0 kW (error), < 2 kW (warn) |
| Nav / Comms bus | 0 kW |
| Battery | ≤ 15 % |
| Comms (if powered) | not LOCKED; signal 0-30 % |
| Engine | temp ≥ 140 °C (overheat warning; lockout at 150 °C); lockout; propellant ≤ 5 % |
| Cabin | pressure < 80 or > 120 kPa; O2 < 15 kPa; CO2 > 1 %; temp < 15 or > 30 °C; water < 10 % |
| Scrubbers | off while life support powered |
| Crew (each) | SpO2 < 92; BPM > 140 or < 40; temp > 39 or < 35 |

New faults are prepended to the log if that exact text isn't already there. "Scan" adds a sweep entry; "Clear faults" resets log and count (active faults reappear next tick).

## Missions
| Mission | Progress formula | Objective |
|---|---|---|
| Earth Departure (default) | `clamp(((225000000 - dist) / 45000000) × 100, 0, 100)` | TMI burn, clear Earth gravity well |
| Earth Orbit | 20 s timer (resets on `selectMission()`) | Checkout, panel alignment, bus sync |
| Lunar Transfer | `(225M − dist) / 25M` | Lunar flyby, calibrate DSN |
| Deep Space Cruise | `(200M − dist) / 150M` | Life support, shielding, thermal loops |
| Mars Approach | `(50M − dist) / 45M` | Align antenna with Mars Orbiter Relay, prep thrusters |
| Mars Orbit | `(5M − dist) / 5M` | MOI retro-burn, mapping telemetry |

Progress stops at 100 % (COMPLETED). Needs mission power > 0 (this value is not on the power bus).