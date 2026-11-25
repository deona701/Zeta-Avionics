# Directories

Zeta-Avionics/
├── CMakeLists.txt            Build definition (target: appZeta_Avionics, QML module URI: Zeta_Avionics)
├── .gitignore
├── .vscode/                  launch.json (gdb debug), tasks.json (g++ single-file build)
├── docs/                     Documentation
├── src/                      C++ backend (exposed to QML via QML_ELEMENT)
│   ├── main.cpp              Loads Zeta_Avionics/Main, exits on QML load failure
│   ├── spacecraft/
│   │   ├── SpacecraftSimulation   16 ms loop timer, delta-time, start/pause, time multiplier
│   │   └── SpacecraftState        Owns all systems, defines update order and ship constants
│   ├── systems/
│   │   ├── PowerSystem            Generation, bus allocation, battery, eclipse cycle
│   │   ├── PropulsionSystem       Engine, throttle, heat, propellant, delta-V
│   │   ├── NavigationSystem       Distance, velocity, ETA, route phase
│   │   ├── CommunicationsSystem   Antenna, links, signal, telemetry, ntfy notifications
│   │   └── LifeSupportSystem      Cabin atmosphere, water, scrubbers, crew vitals, HTTP panel
│   ├── diagnostics/DiagnosticsSystem   Fault detection, health status, error log
│   ├── missions/MissionsSystem         Mission list, objectives, progress
│   └── zeta/ZetaAssistantSystem        Voice capture, STT, command parser, TTS
├── qml/
│   ├── Main.qml              Window, scene switching, background video, startup gate
│   ├── Theme.qml             Singleton: colours, font sizes, margins
│   ├── scenes/               Startup, Overview, Propulsion, Power, LifeSupport,
│   │                         Navigation, Communications, Missions, Diagnostics, Zeta
│   ├── components/           SpacecraftView, PlanetsView, EngineView (3D), SideNavBar, BottomNavBar
│   └── overlays/UnpoweredOverlay.qml   "SYSTEM OFFLINE - 0 kW ALLOCATED" cover
├── assets/
│   ├── fonts/                IBM Plex Mono (Regular, Medium, SemiBold, Bold) + LICENSE.txt
│   ├── web/astronaut_control.html   Phone/browser panel served on port 8080
│   ├── voices/               Piper voice config (.onnx.json committed, .onnx ignored)
│   ├── background/           Photoman61.mp4        (git-ignored)
│   └── stt/                  whisper-cli, ggml-tiny.en.bin   (git-ignored)
└── models/                   (git-ignored, required by CMake)
    ├── SpacecraftModel/      Futuristic_spacecraft_3dmodel.qml, meshes, maps
    ├── EarthModel/           Timeworx_world_4045.qml, meshes, maps
    ├── MarsModel/            Timeworx_mars_4071.qml, meshes, maps
    └── EngineModel/          Raptorengine.qml, meshes
```

## Files you must add manually after cloning
| Path | Needed for |
| `models/**` | Build (listed in CMake resources) and all 3D views |
| `assets/background/Photoman61.mp4` | Build and background video |
| `assets/voices/en_US-libritts_r-medium.onnx` | Zeta's voice (without it: status "MISSING ONNX MODEL") |
| `assets/stt/whisper-cli`, `assets/stt/ggml-tiny.en.bin` | Speech recognition (without them: "MISSING WHISPER STT ASSETS") |
| `piper` executable | Zeta's voice |

Voice assets are searched relative to the current directory, the executable directory, and one and two levels above it.

## Runtime temp files
`zeta_stt_input.wav` and `zeta_tts_output.wav` in the system temp directory.