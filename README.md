# SoundFX-Framework

An SKSE plugin that allows modders to add custom sound effects to various in-game events via an easy-to-use JSON configuration. Supports flexible settings like playback chance, volume, and repeat frequency.

## :warning: Work in Progress :warning:

**This project is currently under development and not ready for production use.**  
Please do not use this in live mod releases until an official stable version is released. Breaking changes and major updates are expected during this phase.

## Features

- Add custom sound effects to a variety of Skyrim events.
- Sounds can be directly loaded into the engine without requiring ESP plugins or additional mods.
- Highly configurable JSON structure to define triggers.
- Support for various playback options like chance, volume, and repetition.
- Ability to specify detailed conditions for sound triggers, such as:
  - Playing a sound only for specific events (e.g., PowerAttack, SneakAttack).
  - Restricting sounds to certain items, weapons, or conditions.
- **Compatible with Skyrim Special Edition (SSE) and Anniversary Edition (AE)** using [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444).

## Installation

> This section will be updated once the project is stable. For now, cloning the repository and building the plugin is required.

## Requirements

### For Users:
- [Skyrim Script Extender (SKSE)](https://skse.silverlock.org/)
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)

### For Developers (Building the Plugin):
- A compiler supporting **C++23** (e.g., Visual Studio 2022).
- [CMake](https://cmake.org/) (3.21 or higher).
- [CommonLibSSE-NG](https://github.com/CharmedBaryon/CommonLibSSE).
- [spdlog](https://github.com/gabime/spdlog).
- [nlohmann_json](https://github.com/nlohmann/json).
- [Vcpkg](https://vcpkg.io/) for dependency management.

## Licensing

This project is licensed under the **Apache 2.0 License**.  
You are free to use, modify, and distribute the code under the terms of the license.  
**However, attribution is required** for any use of this code in other projects.

For details, see the [`LICENSE`](LICENSE) file.
