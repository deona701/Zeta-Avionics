# Research

Real-world concepts the simulation is loosely based on. Values are simplified for gameplay, not engineering accuracy.

| Topic | Real-world basis | How it appears in the sim |
|---|---|---|
| Rocket equation (Tsiolkovsky) | Δv = Isp × g0 × ln(m_full / m_dry) | Delta-V is recomputed from propellant mass; 310 s Isp gives ~3340 m/s at full tanks |
| Earth-Mars distance | Average separation is roughly 225 million km | Start distance 225,000,000 km |
| Interplanetary transfer | Hohmann-style transfers to Mars take roughly 6-9 months | Nominal ETA ~106 days at 24.5 km/s; 150× time compression |
| Deep Space Network | Ground antennas used for deep space links | "Earth Relay / DSN" link and dish alignment |
| Low Earth orbit eclipses | Satellites lose sunlight for part of each orbit (~90 min orbits) | 180 s orbit with a 60 s eclipse |
| Cabin atmosphere | Sea-level pressure ~101.3 kPa, O2 ~21 kPa | Start 101.3 / 21.1 kPa |
| CO2 removal | Lithium hydroxide (LiOH) canisters used on crewed spacecraft | LiOH scrubbers with a wearing cartridge |
| Water recovery | ISS Water Processor Assembly recycles crew water | Greywater tank and WPA efficiency (94.8 %) |
| Vital signs | Resting HR ~60-100 BPM, SpO2 ~95-100 %, temp ~36.5-37.5 °C, breathing 12-20/min | Crew 72 / 98 / 36.8 / 12, fault thresholds in Diagnostics |
| Thermal limits | Engine nozzles and chambers have thermal margins | Engine lockout at 150 °C, restart under 80 °C |

## Tools used
- Qt 6 (Quick, Quick3D, HttpServer, Multimedia): UI and backend
- whisper.cpp (`ggml-tiny.en`): offline speech recognition
- Piper TTS (`en_US-libritts_r-medium`): offline speech synthesis
- ntfy.sh: push notifications for telemetry/distress
