# Spacecraft Specifications

| Project | Zeta Avionics |
| Spacecraft | Z-01 |
| Onboard computer | Zeta |
| Class | Orbital Research & Transport Spacecraft |
| Purpose | Transport a small crew to orbital destinations for scientific observation and experiments |
| Crew | 4 astronauts |
| Power | Deployable solar arrays, rechargeable lithium-ion battery system (the simulation also has a reactor core, see note) |
| Communications | High-gain directional antenna (long range), navigation and low-gain antennas (local/backup) |
| Life support | Pressure regulation, oxygen management, CO2 removal, temperature control, humidity control, water recycling, waste management |

## Simulated mass and propulsion constants
| Constant | Value |
| Dry mass | 1000 (kg) |
| Max propellant mass | 2000 (kg) |
| Gravity constant (g0) | 9.80665 m/s² |
| Specific impulse (Isp) | 310 s |
| Max thrust | 500 (sim units, "N") |
| Max engine temperature | 150 °C (lockout) |
| Initial delta-V | 3340 m/s |

## Power plant (simulated)
Solar array 15 kW (0 in eclipse), reactor core 25 kW, battery bus 0-100 %. Default loads: Propulsion 8, Life Support 6, Navigation 4, Comms 3 kW.

## Zeta specifications
Zeta is the onboard assistant. Commands are hard-coded and the astronauts have a specific command list (`ZetaCommandList.md`). Zeta does not have unrestricted access to the spacecraft. It is an assistant only, and if the crew asks for something that may endanger them, Zeta will ask for authorization before proceeding.

| Address style | Calls the user "Captain" |
| Input | Push-to-talk microphone (16 kHz mono) or typed text |
| Speech recognition | whisper.cpp (`ggml-tiny.en`), runs locally |
| Voice | Piper TTS, `en_US-libritts_r-medium`, speaker ID 10, runs locally |