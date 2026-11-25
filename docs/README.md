# Zeta Avionics
Spacecraft interface for Z-01, with a voice-controlled, hard-coded assistant named Zeta.

| Technology: Qt 6.10+, QML, C++ (CMake) |
| Platform: Desktop Linux |
| Version: 1.0 |
| Purpose: A simulated onboard computer for a fictional spacecraft (Z-01), operated through a graphical interface and voice commands |

> This is *not* a realistic simulator. Physics is simplified and time-compressed on purpose.

## What it does
- Simulates five spacecraft systems: Propulsion, Power, Life Support, Navigation, Communications.
- Runs an Earth -> Mars transfer (225,000,000 km) with six mission phases.
- Raises faults on a Diagnostics screen from live telemetry.
- Lets you talk to Zeta (push-to-talk or typed) to query and control the ship.
- Exposes a small web panel so a phone/browser can change each astronaut's vitals.

## Requirements
- Qt 6.10 or newer with: Quick, Quick3D, HttpServer, Multimedia
- CMake 3.16+, a C++ compiler
- For voice (optional, see `Directories.md`):
  - `piper` on PATH (or in `~/.local/bin`) + voice model `en_US-libritts_r-medium.onnx`
  - `whisper-cli` + `ggml-tiny.en.bin` in `assets/stt/`
- Internet only for Telemetry Burst / Distress Beacon (see Known Issues)

The 3D models, background video, voice model and Whisper files are *git-ignored*. A fresh clone will not build or run fully until you add them (list in `Directories.md`).

## Using it
- Window is 1280x720 (minimum).
- Startup sequence plays first. Press **Space** or **Esc** to skip.
- Side bar: Propulsion, Power, Life Support, Navigation, Comms.
- Bottom bar: Spacecraft, Missions, Diagnostics, Zeta.
- Zeta screen: hold the mic button to speak, or type a command. Full list in `ZetaCommandList.md`.

## Documentation index
README, Directories, License, ProjectScope, SpacecraftSpecifications, SimulationModel, ZetaCommandList, Research, Credits, Extra.

## License
Copyright (c) 2026 @deona701. All rights reserved. Proprietary, for viewing and evaluation only. See `License.md`.