# FlarkViz Audio Plugins v1.0.0

FlarkViz audio visualization plugin for DAWs and audio hosts.

## Included Plugin Formats

### VST3 (Most DAWs)
- **Location**: `FlarkViz.vst3/`
- **Install to**: `~/.vst3/` or `/usr/lib/vst3/`
- **Compatible with**: Reaper, Bitwig, Ardour, Tracktion, etc.

### LV2 (Linux Audio)
- **Location**: `FlarkViz.lv2/`
- **Install to**: `~/.lv2/` or `/usr/lib/lv2/`
- **Compatible with**: Ardour, Qtractor, Carla, jalv, etc.

### Standalone Application
- **File**: `FlarkViz-Standalone`
- **Run directly** without a DAW
- **Executable**: `chmod +x FlarkViz-Standalone && ./FlarkViz-Standalone`

## Installation

### Automatic (Recommended)
```bash
# VST3
cp -r FlarkViz.vst3 ~/.vst3/

# LV2
cp -r FlarkViz.lv2 ~/.lv2/

# Standalone
chmod +x FlarkViz-Standalone
./FlarkViz-Standalone
```

### System-wide (Requires root)
```bash
# VST3
sudo cp -r FlarkViz.vst3 /usr/lib/vst3/

# LV2
sudo cp -r FlarkViz.lv2 /usr/lib/lv2/
```

## Features

- Real-time audio visualization
- MilkDrop-compatible preset support
- FFT-based audio analysis (Bass/Mid/Treble)
- OpenGL hardware-accelerated rendering
- Expression evaluation engine
- HLSL→GLSL shader compiler

## Usage in DAW

1. **Install plugin** using instructions above
2. **Rescan plugins** in your DAW
3. **Load FlarkViz** on any track or master bus
4. **Open plugin editor** to see visualizations
5. **Play audio** and watch the visuals respond

## Standalone Usage

```bash
./FlarkViz-Standalone
```

The standalone version runs independently and captures system audio.

## System Requirements

- **OS**: Linux x86_64 (Ubuntu 20.04+, Debian 10+)
- **OpenGL**: 3.3+ compatible GPU
- **RAM**: 512 MB minimum
- **Audio**: ALSA or PulseAudio

## Plugin Specifications

- **Type**: Audio Effect (Analyzer/Visualizer)
- **Audio I/O**: Stereo In/Out (pass-through)
- **MIDI**: None required
- **Latency**: Zero (pass-through)
- **Categories**: Analyzer, Visualizer, Effect

## Troubleshooting

### Plugin not found in DAW
- Verify installation path
- Rescan plugins in DAW preferences
- Check DAW's VST3/LV2 search paths

### Black screen or no visuals
- Ensure OpenGL 3.3+ support: `glxinfo | grep "OpenGL version"`
- Update graphics drivers
- Check audio is playing through plugin

### Standalone won't run
- Make executable: `chmod +x FlarkViz-Standalone`
- Check dependencies: `ldd FlarkViz-Standalone`

## File Sizes

- **VST3**: ~12 MB
- **LV2**: ~11 MB  
- **Standalone**: ~12 MB

## License

MIT License - See main repository for details

## Links

- **Repository**: https://github.com/flarkflarkflark/FlarkViz
- **Issues**: https://github.com/flarkflarkflark/FlarkViz/issues
- **Documentation**: See main repository README

---

**Built by flarkAUDIO**  
**Release Date**: November 24, 2025
