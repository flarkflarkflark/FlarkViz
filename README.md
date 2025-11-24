# FlarkViz

**Cross-platform MilkDrop visualizer built with JUCE**

[![License: MIT](https://img.shields.io/badge/License-MIT-orange.svg)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![JUCE](https://img.shields.io/badge/JUCE-7.0+-green.svg)](https://juce.com)

> 🎵 A modern, cross-platform music visualizer bringing MilkDrop/MilkDrop3 functionality to Linux (and beyond)

![FlarkViz Demo](https://img.shields.io/badge/Status-Alpha-orange)

## About

FlarkViz is a cross-platform music visualizer inspired by MilkDrop3, built from scratch using the JUCE framework. It aims to bring the legendary MilkDrop visualization experience to Linux with full preset compatibility and modern architecture.

**By flarkAUDIO** - Part of the flarkAUDIO plugin ecosystem

## Features

### Currently Implemented ✅
- **Audio Analysis**: FFT processing with 512 bins
- **Beat Detection**: MilkDrop-style bass/mid/treble detection
- **OpenGL Rendering**: Hardware-accelerated graphics (OpenGL 3.3+)
- **Waveform Visualization**: Real-time audio waveform display
- **Keyboard Controls**: MilkDrop3-compatible shortcuts
- **Preset Management**: Library scanning and organization

### In Development 🚧
- **Preset Parser**: .milk file format support
- **Expression Evaluator**: Per-frame and per-pixel equations
- **Shader Support**: Warp and composite shaders
- **Double Presets**: .milk2 format support
- **Transitions**: 30+ blend patterns
- **Plugin Versions**: VST3/LV2/CLAP

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

FlarkViz aims for full compatibility with:
- ✅ MilkDrop 1.0 presets (.milk)
- ✅ MilkDrop 2.0 presets (.milk)
- 🚧 MilkDrop3 presets (.milk)
- 🚧 Double presets (.milk2)

## Development Status

**Version**: 0.1.0-alpha  
**Status**: Active Development

### Roadmap

- **v0.1** (Current): Core audio analysis and rendering ✅
- **v0.2**: Preset parsing and basic visualization
- **v0.3**: Expression evaluator and shader support
- **v0.4**: Full MilkDrop preset compatibility
- **v0.5**: Double presets and transitions
- **v1.0**: Feature complete + plugin versions

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
