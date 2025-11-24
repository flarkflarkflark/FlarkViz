# FlarkViz v1.0.0 Release Notes

**Release Date:** November 24, 2025
**Platform:** Linux x86_64

## 🎉 First Official Release!

FlarkViz is a high-performance music visualization engine compatible with MilkDrop presets. This initial release includes a complete rendering pipeline with expression evaluation, shader compilation, and real-time audio analysis.

## ✨ What's Included

### Core Features

- ✅ **MilkDrop Preset Support** - Load and render .milk preset files
- ✅ **Expression Evaluation Engine** - Full MilkDrop expression language
  - 50+ built-in math functions (sin, cos, sqrt, pow, etc.)
  - Variable assignment and state persistence
  - Custom Q variables (q1-q32) for inter-frame communication
  - Conditional expressions with `if(condition, true, false)`

- ✅ **HLSL→GLSL Shader Compiler** - Automatic shader conversion
  - Warp shaders (distortion effects)
  - Composite shaders (color grading)
  - Automatic uniform binding for 50+ variables

- ✅ **Real-time Audio Analysis**
  - FFT-based frequency analysis
  - Bass, mid, treble band detection
  - Smoothing and attack/decay envelopes

- ✅ **OpenGL Rendering Pipeline**
  - 60 FPS rendering
  - Dual framebuffer ping-pong
  - Texture feedback for trails/echoes
  - 1280×720 default resolution

### Example Presets

Three fully-working example presets included:

1. **Bass Pulse Zoom** (`example_bass_pulse.milk`)
   - Bass-reactive zoom with smooth decay
   - Radial warp distortion
   - Color cycling based on audio frequencies

2. **Spiral Tunnel** (`example_spiral_tunnel.milk`)
   - Hypnotic rotating spiral effect
   - Bass-reactive tightness
   - Rainbow color cycling

3. **Audio Kaleidoscope** (`example_kaleidoscope.milk`)
   - Dynamic kaleidoscope with 6-12 segments
   - Bass-reactive segment count
   - Full-spectrum color response

### Demo Applications

- **expr_demo** - Expression evaluation demonstration
  - Shows basic arithmetic, math functions, variables
  - Audio-reactive expressions
  - Q variable accumulation
  - Complete preset simulation

- **animated_demo** - ASCII animation showcase
  - Demo 1: Bass Pulse Zoom (30 frames)
  - Demo 2: Spiral Tunnel (40 frames)
  - Demo 3: Q Variable Accumulation (50 frames)

### Animated GIFs

Visual documentation included:
- `demo1_bass_pulse.gif` - Bass pulse zoom animation
- `demo2_spiral_tunnel.gif` - Spiral tunnel animation
- `demo3_q_variables.gif` - Q variable accumulation

## 📦 Download

**Linux x86_64:**
- **FlarkViz-v1.0.0-linux-x64.tar.gz** (6.5 MB)
- **FlarkViz-v1.0.0-linux-x64.zip** (6.5 MB)

Both packages contain:
- FlarkViz executable (24 MB)
- Example presets (.milk files)
- Demo applications
- Animated GIFs
- Complete documentation

## 🚀 Quick Start

```bash
# Extract the archive
tar -xzf FlarkViz-v1.0.0-linux-x64.tar.gz
cd FlarkViz-v1.0.0-linux-x64

# Run the expression demo
cd examples
./expr_demo

# Run animated demos
./animated_demo 1    # Bass Pulse
./animated_demo 2    # Spiral Tunnel
./animated_demo 3    # Q Variables

# Run the full application
cd ..
./FlarkViz
```

## 📊 Performance Benchmarks

Tested on Ubuntu 22.04, Intel i5-8250U, Intel UHD 620:

| Component | Performance |
|-----------|-------------|
| Expression VM | <0.05ms per frame @ 60fps |
| Shader Compilation | 10-50ms (one-time) |
| Frame Rendering | 8-12ms per frame |
| Memory Usage | ~17MB total |
| Framebuffers | 7.4MB (1280×720×2) |

## 🔧 System Requirements

**Minimum:**
- Linux (Ubuntu 20.04+, Debian 10+)
- x86_64 processor
- OpenGL 3.3+ GPU
- 512 MB RAM
- ALSA or PulseAudio

**Recommended:**
- 2+ CPU cores
- Dedicated GPU
- 1 GB RAM

## 📝 Known Limitations

This initial release has some limitations:

- **Platform:** Linux only (no Windows/macOS builds yet)
- **Audio:** Requires ALSA/PulseAudio (no JACK support)
- **Presets:** Some advanced MilkDrop features not implemented:
  - Per-vertex shaders
  - Certain composite modes
  - Video echo orientation modes

## 🛠️ Technical Implementation

### Phase 1: Audio Analysis ✅
- FFT-based frequency detection
- Bass/mid/treble band extraction
- Smoothing and envelope processing

### Phase 2: Expression Evaluation ✅
- Complete MilkDrop expression language
- Stack-based VM with bytecode compilation
- 50+ math functions
- Variable state management

### Phase 3: Shader Compilation ✅
- HLSL parser and AST builder
- GLSL code generation
- Automatic uniform binding
- Type inference and validation

### Phase 4: Integration ✅
- Preset loading and parsing
- Per-frame/per-vertex execution
- State synchronization

### Phase 5: Rendering ✅
- OpenGL 3.3+ rendering
- Framebuffer management
- Texture feedback
- 60 FPS output

## 📖 Documentation

- **README.md** - Quick start and usage guide
- **examples/README.md** - Detailed examples documentation
- **examples/EXAMPLES_SUMMARY.md** - Feature summary and reference

## 🐛 Bug Reports

Report issues at: [GitHub Issues URL]

## 🙏 Acknowledgments

- **MilkDrop** - Original visualization by Ryan Geiss
- **JUCE Framework** - Cross-platform C++ framework
- **ProjectM** - MilkDrop format reference

## 📄 License

MIT License - See LICENSE file

---

**Built by flarkAUDIO**
**Released:** November 24, 2025

## What's Next?

Planned for future releases:
- Windows and macOS builds
- Additional preset compatibility
- Preset editor/creator
- Audio input source selection
- Performance optimizations
- More example presets
