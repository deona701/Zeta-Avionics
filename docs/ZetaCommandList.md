# Zeta Command Reference

Zeta matches commands using whole-word regular expression checks rather than raw substring matching.

## Throttle & Propulsion
* `set throttle <0-100>` / `throttle <0-100>` — Sets throttle to an absolute percentage (also supports spoken numbers like `"seventy five"`, `"half"`, `"max"`, `"idle"`).
* `increase throttle` / `decrease throttle` (with optional number, default `10`) — Adjusts throttle relative to the current setting unless a target (`"to <n>"`) is specified.
* `ignite` / `start engines` / `engines on` — Starts main engines when thermal and propellant conditions allow.
* `shutdown` / `engines off` — Stops main engines and transitions to ballistic coast (requires PIN authorization while running).
* `engage` / `punch it` / `make it so` — Ignites main engines and sets throttle to 95% if below 10% (prompts for PIN authorization if throttle would exceed 85%).
* `abort` — Cuts main engines to zero and places flight systems in safe hold (requires PIN authorization).
* `engine temp` / `fuel` / `delta-v` / `thrust` — Reports propulsion telemetry and thermal limits.

## Power Bus
* `reroute` / `divert` / `allocate power` to `<circuit>` (with optional amount, default `2` kW; supports `reduce`/`cut`/`lower`) — Shifts power on `propulsion`, `life_support`, `navigation`, or `comms` (requires a named circuit; prompts for PIN authorization if leaving life support under 2 kW or dropping another circuit to 0 kW).
* `power` / `battery` / `eclipse` / `reactor` / `solar` — Reports electrical bus balance, generation, battery charge, and eclipse status.

## Navigation & Life Support
* `distance` / `mars` / `eta` / `how far` — Reports distance to destination, velocity, and ETA.
* `velocity` / `speed` / `trajectory` / `position` — Reports current position, trajectory, and speed.
* `scrubbers on` / `scrubbers off` / `co2` — Controls or reports Lithium Hydroxide CO2 scrubbers (`scrubbers off` requires PIN authorization when active).
* `cabin` / `oxygen` / `pressure` / `atmosphere` — Reports cabin pressure, O2 partial pressure, and temperature.
* `water` / `greywater` / `humidity` — Reports water tanks and relative humidity.
* `crew` / `vitals` / `biometrics` / `heart` — Reports crew heart rate and SpO2 readings.

## Communications & Status
* `earth relay` / `mars orbiter` / `deep space gateway` / `switch link` — Switches active communications link and slews dish alignment.
* `align antenna <angle>` / `antenna to <angle>` — Sets antenna alignment angle.
* `telemetry burst` / `ping earth` — Sends a telemetry burst and increments packet count.
* `distress` / `beacon` / `mayday` / `sos` — Broadcasts an emergency distress signal (requires PIN authorization).
* `mission` / `objective` / `phase` — Reports active mission and objective progress.
* `status` / `report` / `diagnostics` — Reports overall flight deck telemetry, system health, and active fault count.

## Authorization
These commands are held until you state the authorization PIN (digits or spoken digits). Say `cancel` to drop the order. Requests expire after 15 seconds, and while one is pending, other commands are refused.

| Command | Held when |
| `abort` | Always |
| `shutdown` / `engines off` | Engine is running |
| `engage` / `punch it` / `throttle` above 85 % | Throttle would exceed 85 % |
| `scrubbers off` | Scrubbers are active |
| `reroute` / `cut power` | Leaves life support under 2 kW, or takes another circuit to 0 kW |
| `distress` / `mayday` / `sos` | Always |