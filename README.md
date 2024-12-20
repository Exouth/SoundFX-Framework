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
- 3D positional sound effects to create immersive audio experiences, with support for distance-based volume attenuation and spatial positioning.
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
- [openal-soft](https://github.com/kcat/openal-soft).
- [libsndfile](https://github.com/libsndfile/libsndfile).
- [Vcpkg](https://vcpkg.io/) for dependency management.


## Licensing

This project is licensed under the **GNU General Public License v3.0**.  
You are free to use, modify, and distribute this code under the terms of the license.  
**Attribution is required**, meaning you must retain the original copyright notice in the source code.  
**If you modify and distribute the code, you must also distribute your changes and include the source code.**

This project also includes specific exceptions to facilitate compatibility with proprietary software and libraries commonly used in the modding community. For details about these exceptions, see the [`EXCEPTIONS.md`](EXCEPTIONS.md) file.

For the full license text, see the [`LICENSE`](LICENSE) file.

### Third-Party Licenses

This project also uses third-party libraries that are distributed under their own licenses.  
See the `licenses` folder for more information:
- `libsndfile` is licensed under the LGPL-2.1. See [`licenses/LICENSE-libsndfile`](licenses/LICENSE-libsndfile).
- `OpenAL` is licensed under the LGPL. See [`licenses/LICENSE-openal`](licenses/LICENSE-openal).