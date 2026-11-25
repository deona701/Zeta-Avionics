# Project Scope

## Goal
Build a simulated onboard computer for a fictional spacecraft, operated through a graphical interface and a voice-controlled, hard-coded assistant.

## In scope
- Five simulated systems: Power, Propulsion, Life Support, Navigation, Communications
- Diagnostics (fault detection and log) and Missions (six phases with progress)
- Zeta: push-to-talk or typed commands, local speech-to-text, local text-to-speech
- A 3D spacecraft, engine, Earth and Mars view
- A fixed 4-person crew with adjustable vitals (in-app and via web panel)
- A scripted Earth -> Mars transfer

## Out of scope
- Realistic orbital mechanics, n-body physics, accurate transfer windows
- Real hardware, real spacecraft protocols, real mission data
- Natural-language understanding or AI (Zeta is keyword matching only)
- Multiplayer, save/load, persistence
- Waste management simulation (listed in the spec, not simulated)

## Design principles
- Hard-coded and predictable: every Zeta command is explicit.
- Zeta is an assistant, not the pilot. Dangerous requests need authorization by PIN.
- Black/white, monospace, flight-instrument look (IBM Plex Mono).
- Systems can be starved: 0 kW on a bus shows the SYSTEM OFFLINE overlay and freezes that system.