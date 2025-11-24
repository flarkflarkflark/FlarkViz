# FlarkViz

**Cross-platform MilkDrop visualizer built with JUCE**

[![License: MIT](https://img.shields.io/badge/License-MIT-orange.svg)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![JUCE](https://img.shields.io/badge/JUCE-7.0+-green.svg)](https://juce.com)
[![Version](https://img.shields.io/badge/Version-1.0.0-blue.svg)](https://github.com/flarkflarkflark/FlarkViz/releases/tag/v1.0.0)

> 🎵 A modern, cross-platform music visualizer bringing MilkDrop/MilkDrop3 functionality to Linux (and beyond)

![FlarkViz Demo](https://img.shields.io/badge/Status-v1.0.0_Released-green)

## About

FlarkViz is a cross-platform music visualizer inspired by MilkDrop3, built from scratch using the JUCE framework. It aims to bring the legendary MilkDrop visualization experience to Linux with full preset compatibility and modern architecture.

**By flarkAUDIO** - Part of the flarkAUDIO plugin ecosystem

## Features

### v1.0.0 Release ✅
- **Audio Analysis**: FFT processing with 512 bins, bass/mid/treble detection
- **Expression Evaluation**: Complete MilkDrop expression language (50+ functions)
- **Shader Compiler**: HLSL→GLSL automatic conversion (warp & composite shaders)
- **Preset Support**: Full .milk and .milk2 file format support
- **OpenGL Rendering**: Hardware-accelerated 60 FPS rendering (OpenGL 3.3+)
- **Plugin Formats**: VST3, LV2, CLAP, and Standalone builds
- **Transitions**: Advanced preset blending and transitions
- **Real-time Audio**: FFT-based frequency analysis with smoothing

### Future Enhancements 🚧
- **Cross-platform**: Windows and macOS builds
- **Preset Editor**: Built-in preset creation and editing
- **Advanced Features**: Additional MilkDrop3 compatibility
- **Performance**: GPU compute shader optimization
- **Audio Sources**: Multiple input source selection

## Download

### v1.0.0 Release (November 24, 2025)

**Standalone Application:**
- [FlarkViz-v1.0.0-linux-x64.tar.gz](https://github.com/flarkflarkflark/FlarkViz/releases/download/v1.0.0/FlarkViz-v1.0.0-linux-x64.tar.gz) (6.5 MB)
- [FlarkViz-v1.0.0-linux-x64.zip](https://github.com/flarkflarkflark/FlarkViz/releases/download/v1.0.0/FlarkViz-v1.0.0-linux-x64.zip) (6.5 MB)

**Audio Plugins (VST3/LV2/CLAP):**
- [FlarkViz-Plugins-v1.0.0-linux-x64.tar.gz](https://github.com/flarkflarkflark/FlarkViz/releases/download/v1.0.0/FlarkViz-Plugins-v1.0.0-linux-x64.tar.gz) (13 MB)
- [FlarkViz-Plugins-v1.0.0-linux-x64.zip](https://github.com/flarkflarkflark/FlarkViz/releases/download/v1.0.0/FlarkViz-Plugins-v1.0.0-linux-x64.zip) (13 MB)

**Checksums:**
- [SHA256SUMS](https://github.com/flarkflarkflark/FlarkViz/releases/download/v1.0.0/SHA256SUMS)

See the [Release Notes](https://github.com/flarkflarkflark/FlarkViz/releases/tag/v1.0.0) for full details.

## Quick Start

### Prerequisites

**Linux (Arch):**
```bash
sudo pacman -S base-devel juce mesa alsa-lib pulseaudio jack2
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt install build-essential libjuce-dev mesa-common-dev \
                 libasound2-dev pulseaudio jackd2
```

**macOS:**
```bash
brew install juce
```

### Clone and Build

```bash
git clone https://github.com/flarkflarkflark/FlarkViz.git
cd FlarkViz

# Option 1: Using Projucer (Recommended)
Projucer FlarkViz.jucer
# Configure module path, save, then build from Builds/LinuxMakefile

# Option 2: Using CMake
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Quick Demo

Try the standalone demos (no JUCE required):

```bash
cd demo
make
./FlarkVizDemo        # OpenGL demo (needs SDL2)
./verify_build        # Component verification
```

## Documentation

- **[BUILD.md](BUILD.md)** - Complete build instructions
- **[QUICKSTART.md](QUICKSTART.md)** - Get running in 1 hour
- **[README.md](README.md)** - Architecture and development guide

## Architecture

```
FlarkViz/
├── Source/
│   ├── Audio/              # FFT, beat detection
│   ├── Rendering/          # OpenGL, shaders
│   ├── Presets/            # .milk/.milk2 parsing
│   └── Expression/         # Equation evaluator
├── demo/                   # Standalone demos
└── Resources/              # Shaders, presets
```

## Keyboard Shortcuts

| Key | Function |
|-----|----------|
| **Space** | Random preset transition |
| **F2** | Cycle FPS (60/90/120) |
| **F7** | Toggle fullscreen |
| **F9** | Double-preset mode |
| **a** | Random mash-up |
| **A** | Previous preset |
| **c** | Randomize colors |

## Preset Compatibility

FlarkViz v1.0.0 supports:
- ✅ MilkDrop 1.0 presets (.milk)
- ✅ MilkDrop 2.0 presets (.milk)
- ✅ MilkDrop3 presets (.milk) - Core features
- ✅ Double presets (.milk2)
- ✅ Custom warp shaders (HLSL→GLSL)
- ✅ Composite shaders
- ✅ Q variables and per-frame expressions

## Development Status

**Version**: 1.0.0
**Status**: Production Release
**Released**: November 24, 2025

### Completed Features (v1.0.0) ✅

- **Audio Engine**: FFT analysis, beat detection, frequency bands
- **Expression Evaluator**: Complete MilkDrop expression language with 50+ functions
- **Shader Compiler**: HLSL→GLSL conversion with automatic uniform binding
- **Preset System**: Full .milk and .milk2 format support
- **Rendering Pipeline**: OpenGL 3.3+ with dual framebuffers, 60 FPS
- **Plugin Formats**: VST3, LV2, CLAP, and Standalone builds
- **Transitions**: Preset blending and smooth transitions
- **Platform**: Linux x86_64 (Ubuntu 20.04+, Debian 10+, Arch)

### Roadmap (Future Versions)

- **v1.1**: Windows and macOS builds
- **v1.2**: Preset editor and creator
- **v1.3**: Enhanced MilkDrop3 compatibility
- **v1.4**: Audio source selection and routing
- **v2.0**: GPU compute shaders, performance optimizations

## Contributing

FlarkViz is in active development. Contributions welcome!

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Technology Stack

- **Language**: C++20
- **Framework**: JUCE 7.0+
- **Graphics**: OpenGL 3.3+
- **Audio**: JUCE DSP module (FFT, filters)
- **Build**: CMake, Projucer

## Related Projects

- [MilkDrop3](https://github.com/milkdrop2077/MilkDrop3) - Windows-only visualizer (inspiration)
- [projectM](https://github.com/projectM-visualizer/projectm) - Cross-platform MilkDrop implementation
- [flarkAUDIO plugins](https://github.com/flarkflarkflark) - Audio effects ecosystem

## License

MIT License - see [LICENSE](LICENSE) file for details

## Credits

- **Development**: flarkAUDIO
- **Original MilkDrop**: Ryan Geiss
- **MilkDrop3**: milkdrop2077
- **JUCE Framework**: ROLI Ltd.

## Links

- **Website**: Coming soon
- **Forum**: [JUCE Forum thread](https://forum.juce.com)
- **Discord**: Coming soon

---

**FlarkViz** - Because Linux deserves legendary visualizations 🎵✨

*Part of the flarkAUDIO ecosystem*
