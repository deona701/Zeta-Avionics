# Known Issues

## Likely bugs
- Waste management is not simulated.

## Power model
- Reroute does not conserve power: raising one bus does not lower another.
- Missions has its own power value (default 4 kW) that is not on the power bus.

## Network and security
- **Telemetry Burst and Distress Beacon POST to a public topic**

## Cleanup
- `Qt6::TextToSpeech` is linked but unused (voice uses Piper).