# FlarkViz v1.0.0 - MilkDrop-Compatible Music Visualization Engine

**Release Date:** November 24, 2025
**Platform:** Linux x86_64
**Author:** flarkAUDIO

## 🎵 What is FlarkViz?

FlarkViz is a high-performance music visualization engine compatible with MilkDrop presets (.milk files). It features real-time audio analysis, expression evaluation, shader compilation, and OpenGL rendering at 60fps.

## ✨ Features

- **MilkDrop Preset Support** - Load and run .milk preset files
- **Expression Evaluation** - Full MilkDrop expression language with 50+ math functions
- **HLSL→GLSL Shader Compiler** - Automatic conversion of warp/composite shaders
- **Real-time Audio Analysis** - Bass, mid, treble frequency detection
- **60 FPS Rendering** - Smooth OpenGL rendering with texture feedback
- **Custom Q Variables** - 32 persistent variables (q1-q32) for inter-frame state

## 📦 Package Contents

```
FlarkViz-v1.0.0-linux-x64/
├── FlarkViz                    # Main executable (24 MB)
├── README.md                   # This file
└── examples/
    ├── README.md               # Detailed examples documentation
    ├── EXAMPLES_SUMMARY.md     # Complete feature summary
    │
    ├── example_bass_pulse.milk      # Bass-reactive zoom preset
    ├── example_spiral_tunnel.milk   # Hypnotic spiral effect
    ├── example_kaleidoscope.milk    # Kaleidoscope mirror effect
    │
    ├── expr_demo               # Expression evaluation demo
    ├── animated_demo           # ASCII animation demos
    │
    ├── demo1_bass_pulse.gif         # Animated demo 1
    ├── demo2_spiral_tunnel.gif      # Animated demo 2
    ├── demo3_q_variables.gif        # Animated demo 3
    │
    └── *.cpp                   # C++ source code examples
```

## 🚀 Quick Start

### 1. Run the Expression Demo

```bash
cd examples
./expr_demo
```

This demonstrates the expression evaluation system with:
- Basic arithmetic and math functions
- Audio-reactive variables (bass, mid, treb)
- Variable assignment and state persistence
- Custom Q variables
- Complete preset simulation

### 2. Run the Animated Demos

```bash
./animated_demo 1    # Bass Pulse Zoom (30 frames)
./animated_demo 2    # Spiral Tunnel (40 frames)
./animated_demo 3    # Q Variable Accumulation (50 frames)
```

Watch ASCII animations showing how presets respond to audio in real-time!

### 3. Run the Full FlarkViz Application

```bash
cd ..
./FlarkViz
```

This launches the full OpenGL application with:
- Real-time audio capture
- Preset loading (File → Open Preset)
- 60 FPS rendering
- Live visualization

## 🎨 Loading Presets

1. Launch FlarkViz: `./FlarkViz`
2. Go to **File → Open Preset**
3. Select a `.milk` file from the `examples/` directory
4. Play music and watch the visualization react!

## 📊 System Requirements

- **OS:** Linux (Ubuntu 20.04+, Debian 10+, or equivalent)
- **CPU:** x86_64 processor, 2+ cores recommended
- **GPU:** OpenGL 3.3+ compatible graphics card
- **RAM:** 512 MB minimum, 1 GB recommended
- **Audio:** ALSA or PulseAudio for audio input

## 🔧 Technical Details

### Architecture

FlarkViz is built with a modular pipeline:

1. **Audio Analysis** (`AudioProcessor`)
   - FFT-based frequency analysis
   - Bass/Mid/Treble band detection
   - Smoothing and attack/decay envelopes

2. **Expression Evaluation** (`MilkdropEval`)
   - Stack-based virtual machine
   - Compiles expression code to bytecode
   - 50+ built-in math functions
   - Variable assignment and state management

3. **Shader Compilation** (`ShaderCompiler`)
   - Parses HLSL warp/composite shaders
   - Converts to GLSL (OpenGL Shading Language)
   - Automatic uniform binding (bass, time, q variables, etc.)

4. **Rendering Pipeline** (`RenderState`)
   - Dual framebuffer ping-pong
   - Warp pass (distortion effects)
   - Composite pass (color grading)
   - Texture feedback for trails/echoes

### Performance

- **Expression VM**: <0.05ms per frame @ 60fps
- **Shader Compilation**: ~10-50ms per shader (one-time)
- **Frame Rendering**: ~8-12ms per frame (depends on shader complexity)
- **Memory Usage**: ~10MB + 7.4MB framebuffers (1280×720)

## 📝 Creating Your Own Presets

See `examples/README.md` for detailed preset creation guide.

### Minimal Preset Template

```milk
[preset00]
name=My First Preset
author=Your Name
fDecay=0.970000
zoom=1.000000
rot=0.000000

[per_frame_1]
zoom = 1.0 + 0.1 * bass;
rot = rot + 0.01;
wave_r = 0.5 + 0.5 * sin(time);

[warp_1]
uv_warped = uv + float2(0.01, 0.01) * bass;
```

### Available Variables

**Audio:**
- `bass`, `mid`, `treb` - Current frequency levels (0-1)
- `bass_att`, `mid_att`, `treb_att` - Smoothed/attenuated levels

**State:**
- `zoom`, `rot`, `cx`, `cy`, `dx`, `dy` - Transform parameters
- `wave_r`, `wave_g`, `wave_b` - Waveform colors
- `time`, `fps`, `frame` - Time and frame counters

**Custom:**
- `q1` through `q32` - Persistent variables

**Math Functions:**
- `sin`, `cos`, `tan`, `asin`, `acos`, `atan`, `atan2`
- `sqrt`, `pow`, `exp`, `log`, `log10`
- `abs`, `floor`, `ceil`, `round`
- `min`, `max`, `clamp`, `saturate`
- `if(cond, true_val, false_val)`
- Many more...

## 🐛 Known Issues

- **Linux only** - Currently no Windows/macOS builds
- **Audio input** - Requires ALSA/PulseAudio, no JACK support yet
- **Preset compatibility** - Some advanced MilkDrop features not implemented
  - Per-vertex shaders
  - Certain composite modes
  - Video echo orientation modes

## 📖 Documentation

- `examples/README.md` - Complete examples guide
- `examples/EXAMPLES_SUMMARY.md` - Feature summary and quick reference
- MilkDrop preset format: [https://github.com/projectM-visualizer/projectm/wiki/MilkDrop-Preset-Authoring](https://github.com/projectM-visualizer/projectm/wiki/MilkDrop-Preset-Authoring)

## 🛠️ Building from Source

```bash
# Install dependencies
sudo apt-get install build-essential cmake libasound2-dev \
     libgl1-mesa-dev libx11-dev libxrandr-dev libfreetype6-dev

# Clone and build
git clone https://github.com/flarkflarkflark/FlarkViz.git
cd FlarkViz/Option2_JUCE_FlarkViz
mkdir build && cd build
cmake ..
make -j4

# Run
./FlarkViz_artefacts/FlarkViz
```

## 📄 License

[Your license here - MIT, GPL, etc.]

## 🙏 Acknowledgments

- **MilkDrop** - Original visualization created by Ryan Geiss
- **JUCE Framework** - Cross-platform C++ framework
- **ProjectM** - MilkDrop preset format reference

## 📧 Contact

Report bugs and issues at: [Your GitHub Issues URL]

---

**FlarkViz v1.0.0** - Built with ❤️ by flarkAUDIO
Released: November 24, 2025
