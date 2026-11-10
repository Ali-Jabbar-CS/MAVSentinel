# MAVSentinel

A hardened MAVLink 2 telemetry pipeline for UAV ground control, written in C++23.

MAVSentinel receives telemetry from a PX4 quadcopter over UDP, decodes it, and checks it before anything reaches the operator. The goal is a ground station that can tell the difference between ordinary packet loss and telemetry that was replayed or spoofed. That matters because MAVLink signing is optional, and an unsigned link accepts any well-formed packet.

> **Status: early development.** The build system, dev container, and first capture fixture are done. The decoders and integrity checks are next. The roadmap below separates what exists today from what is planned.

## What's built

| Area | Details |
| --- | --- |
| Project layout | A static library (`mavsentinel_core`) holds all logic, so the app, unit tests, and fuzz harnesses can share one codebase. The app (`mavsentinel`) links against it. |
| Build | CMake 3.25+ with Ninja, C++23 required, warnings as errors (`-Wall -Wextra -Wpedantic -Werror`), and `compile_commands.json` exported for clangd. |
| Dev container | Ubuntu 26.04 image with GCC, CMake, Ninja, GDB, clangd, clang-tidy, clang-format, and Python. It runs as a non-root user with host networking, so it can receive simulator UDP traffic directly. The same environment builds on both of my machines. |
| Test fixture | `tests/data/sih_idle.pcap`: a tcpdump capture of PX4 running in SIH (simulation-in-hardware) mode. It holds 500 unsigned MAVLink 2 frames from system 1, component 1, sent from UDP port 18570 to 14550 over 1.39 seconds, with no sequence gaps. |
| Protocol analysis | I hand-decoded frames from this capture byte by byte (header, sequence counter, system and component IDs, message ID) and verified attitude and GPS payload fields in Python before writing any C++. |

### What the capture contains

The most frequent messages in the fixture, which the first decoders will target:

| ID | Message | Frames |
| --- | --- | --- |
| 30 | `ATTITUDE` | 67 |
| 31 | `ATTITUDE_QUATERNION` | 67 |
| 32 | `LOCAL_POSITION_NED` | 67 |
| 33 | `GLOBAL_POSITION_INT` | 67 |
| 83 | `ATTITUDE_TARGET` | 67 |
| 36 | `SERVO_OUTPUT_RAW` | 66 |
| 85 | `POSITION_TARGET_LOCAL_NED` | 66 |
| 24 | `GPS_RAW_INT` | 7 |
| 74 | `VFR_HUD` | 5 |
| 0 | `HEARTBEAT` | 1 |

## Roadmap

### Next

- [ ] **Frame parser.** Find the `0xFD` start byte, read the 10-byte header, and bound-check the payload length before reading.
- [ ] **CRC validation.** CRC-16/MCRF4XX over the header and payload, seeded with each message's `CRC_EXTRA` byte so mismatched message definitions are rejected.
- [ ] **Endian-portable decoders.** Read fields with `std::memcpy` and `std::bit_cast`, swapping with `std::byteswap` (C++23) only on big-endian hosts, avoiding `reinterpret_cast` and its strict-aliasing undefined behavior.
- [ ] **Unit tests** that replay `sih_idle.pcap` and check decoded values against the Python reference decode.

### After that

- [ ] **Link integrity analysis.** Use sequence numbers, timing, and system and component IDs to classify gaps as packet loss, replayed frames, or injected frames from a spoofed sender.
- [ ] **Fuzzing.** libFuzzer harnesses with AddressSanitizer and UndefinedBehaviorSanitizer on the parser.
- [ ] **Live telemetry display** for attitude, position, and link health.
- [ ] **Big-endian CI.** Run the test suite on s390x under QEMU in GitHub Actions to prove the decoders work on both byte orders.

### Planned

- [ ] **Link-health state machine** driven by `HEARTBEAT`, covering signal loss, reacquisition, and latency, tested under simulated radio and satellite link conditions with Linux `tc netem`.
- [ ] **GPS spoofing detection** that cross-checks GPS position against inertial data.
- [ ] **Multi-vehicle separation monitor.** Track several simulated vehicles at once, predict their closest approach, and warn before they come too close.
- [ ] **Decoder generator.** A Python tool that generates decoders from the MAVLink XML definitions, after the hand-written versions are tested.

## Build and run

Open the folder in VS Code and choose **Reopen in Container**, or build directly on Linux with GCC 13 or newer, CMake 3.25+, and Ninja:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/apps/mavsentinel
```

Today this prints the library version (`mavsentinel 0.1.0`).

## Repository layout

```
include/mavsentinel/   public headers for the core library
src/                   core library (mavsentinel_core)
apps/                  ground station executable
tests/data/            packet capture fixtures
tools/                 Python helpers (planned)
docs/                  design notes (planned)
.devcontainer/         Dockerfile and VS Code dev container config
```

## License

[Apache License 2.0](LICENSE)
