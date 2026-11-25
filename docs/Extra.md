# Known Issues

## Likely bugs
1. **Overheat fault never fires.** Diagnostics checks engine temp ≥ 250 °C, but the engine locks out at 150 °C. Only `FAIL_MAIN_ENGINE` appears. Change 250 → ~140 to get an early warning.
2. **O2 never replenishes.** It drains `R × 0.002` kPa/s (0.096/s at defaults), so `CRIT_O2` (< 15 kPa) triggers after ~64 s of runtime. It also drains while life support has 0 kW.
- Waste management is not simulated.

## Power model
- Reroute does not conserve power: raising one bus does not lower another.
- Missions has its own power value (default 4 kW) that is not on the power bus.

## Network and security
- **Telemetry Burst and Distress Beacon POST to a public topic**

## Cleanup
- `Qt6::TextToSpeech` is linked but unused (voice uses Piper).