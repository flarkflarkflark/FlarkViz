# FlarkViz Quick Start Guide 🚀

Welcome to FlarkViz! You've chosen Option 2, and you're about to build something amazing. This guide will get you from zero to a running visualizer in **under 1 hour**.

## ✅ What You Have Right Now

A complete, compilable JUCE project with:
- ✅ OpenGL rendering context
- ✅ Audio analysis (FFT + beat detection)
- ✅ Basic waveform visualization
- ✅ MilkDrop3-style keyboard shortcuts
- ✅ flarkAUDIO branding (orange on black)
- ✅ Project structure for all features

## 🎯 Goal: Running Visualizer in 1 Hour

**Timeline:**
- 0-15 min: Setup JUCE + dependencies
- 15-30 min: Build FlarkViz
- 30-45 min: Test basic visualization
- 45-60 min: Customize and explore

## 📋 Prerequisites

### 1. Install JUCE (if not already)
```bash
# Option A: Use system JUCE
sudo pacman -S juce

# Option B: Clone JUCE (recommended for latest)
cd ~
git clone https://github.com/juce-framework/JUCE.git
```

### 2. Install Dependencies
```bash
# Arch Linux (your system)
sudo pacman -S base-devel cmake git mesa glm pulseaudio jack2 alsa-lib

# Graphics libraries
sudo pacman -S libgl libglvnd

# Optional but recommended
sudo pacman -S pulseaudio-jack  # For JACK support
```

### 3. Verify OpenGL
```bash
glxinfo | grep "OpenGL version"
# Should show OpenGL 3.3 or higher
```

## 🏗️ Building FlarkViz

### Method 1: Using Projucer (Recommended)

1. **Open Projucer**
```bash
# If installed from pacman
Projucer

# If using cloned JUCE
~/JUCE/Projucer
```

2. **Open FlarkViz.jucer**
   - File → Open → Navigate to FlarkViz.jucer
   - Or: `Projucer FlarkViz.jucer`

3. **Configure Module Paths**
   - Click "Modules" in left sidebar
   - Set JUCE module path:
     - System install: `/usr/share/JUCE/modules`
     - Manual install: `~/JUCE/modules`

4. **Save Project**
   - File → Save Project (Ctrl+S)
   - This generates the Makefile

5. **Build**
```bash
cd Builds/LinuxMakefile
make CONFIG=Release -j$(nproc)
```

### Method 2: Using CMake (Alternative)

Coming soon - Projucer is recommended for now.

## 🎵 First Run

### 1. Create Presets Folder
```bash
mkdir -p ~/Documents/FlarkViz/presets
```

### 2. Run FlarkViz
```bash
cd Builds/LinuxMakefile/build
./FlarkViz
```

### 3. What You'll See

- **Black window** (flarkAUDIO style!)
- **Orange waveform** when audio is playing
- **Beat flashes** when bass hits

## 🎹 Keyboard Shortcuts (Already Implemented!)

| Key | Function |
|-----|----------|
| Space | Random transition |
| F2 | Toggle FPS (60/90/120) |
| F7 | Toggle fullscreen |
| F9 | Double-preset mode |
| a | Random mash-up |
| A (Shift+a) | Previous preset |
| c | Randomize colors |

## 🔧 Testing Without Audio

The visualizer needs audio input to work properly. Here are some quick tests:

### Test 1: Play Music
```bash
# In another terminal
reaper  # Or your favorite player
# Play any track
```

### Test 2: Generate Test Tone
```bash
# Install if needed
sudo pacman -S sox

# Generate test tone
play -n synth 10 sine 440
```

### Test 3: Use Loopback
```bash
# Create PulseAudio loopback
pactl load-module module-loopback

# Play music, it will loop back to FlarkViz
```

## 🐛 Troubleshooting

### Problem: Won't Compile

**Error: "Cannot find JuceHeader.h"**
```bash
# Fix module paths in Projucer
# Modules → Set path to your JUCE installation
```

**Error: OpenGL functions not found**
```bash
# Install mesa
sudo pacman -S mesa libgl

# Update graphics drivers
sudo pacman -S mesa-utils
```

### Problem: Black Screen

1. Check OpenGL context:
   - Press F1 to see debug console
   - Look for "OpenGL initialized" message

2. Verify audio input:
```bash
# List audio devices
arecord -l

# Test audio capture
arecord -d 5 test.wav
aplay test.wav
```

### Problem: No Audio Visualization

The current version uses AudioDeviceManager which requires manual audio setup. Quick fix:

1. **Enable system audio monitoring:**
```bash
# Method 1: PulseAudio loopback (easiest)
pactl load-module module-loopback

# Method 2: JACK routing
# Start JACK, route system audio to FlarkViz
```

2. **Or: Test with mic input** (works out of the box)
   - Speak/clap near microphone
   - Should see waveform react

## 🎨 Customization Quick Wins

### Change Colors
Edit `MainComponent.h`:
```cpp
const juce::Colour flarkOrange {0xFFYOURHEX};  // Your color
const juce::Colour flarkBlack {0xFF000000};
```

### Adjust Waveform
Edit `PresetRenderer.cpp` → `renderWaveform()`:
```cpp
float y = waveformData[i] * 0.8f;  // Make bigger/smaller
```

### Change Window Size
Edit `MainComponent.cpp` constructor:
```cpp
setSize (1920, 1080);  // Your resolution
```

## 📈 Next Steps (Week 1-4)

Now that it's running, here's your roadmap:

### Week 1: Enhanced Visualization
- [ ] Add FFT spectrum display
- [ ] Implement basic preset parser (.milk files)
- [ ] Create test presets

### Week 2: Preset System
- [ ] Parse .milk file format
- [ ] Load presets from files
- [ ] Implement preset browser

### Week 3: Advanced Rendering
- [ ] Add warp shader support
- [ ] Implement composite shader
- [ ] Basic transition effects

### Week 4: Polish
- [ ] Improve beat detection
- [ ] Add more keyboard shortcuts
- [ ] Performance optimization

## 📚 Learning Resources

### Understanding the Code

**MainComponent.cpp** - Your entry point
- `newOpenGLContextCreated()` - OpenGL setup
- `renderOpenGL()` - Called every frame (60 FPS)
- `keyPressed()` - Keyboard input

**AudioAnalyzer.cpp** - Audio processing
- `processAudioBlock()` - Incoming audio
- `detectBeat()` - Beat detection algorithm
- `getFFTData()` - Frequency spectrum

**PresetRenderer.cpp** - Visualization
- `renderWaveform()` - Draws audio waveform
- `renderPreset()` - Main rendering loop

### JUCE Resources
- **JUCE Tutorials:** https://docs.juce.com/master/tutorial_getting_started_linux.html
- **OpenGL in JUCE:** https://docs.juce.com/master/tutorial_open_gl_application.html
- **Audio Basics:** https://docs.juce.com/master/tutorial_processing_audio_input.html

### MilkDrop Preset Format
- Check out `/mnt/skills/public/` for preset documentation
- Look at existing .milk files for examples
- The format is text-based and human-readable

## 🎯 Your First Milestone: Week 1 Goals

By end of Week 1, you should have:
1. ✅ FlarkViz compiling and running
2. ✅ Waveform visualization working
3. ✅ Beat detection flashing
4. ✅ Basic preset loader (even if it just loads test data)
5. ✅ Comfortable with the codebase

## 💡 Quick Wins to Try Right Now

### 1. Make it Fullscreen
Press F7 when running!

### 2. Change Beat Flash Color
Edit `PresetRenderer.cpp` → `renderPreset()`:
```cpp
// Change this line:
auto orange = flarkOrange.withAlpha (beat.intensity * 0.2f);

// To your color:
auto myColor = juce::Colours::cyan.withAlpha (beat.intensity * 0.3f);
```

### 3. Add FPS Counter
Edit `MainComponent.cpp` → `renderOpenGL()`:
```cpp
void MainComponent::renderOpenGL()
{
    static int frameCount = 0;
    static auto lastTime = juce::Time::getMillisecondCounter();
    
    frameCount++;
    auto now = juce::Time::getMillisecondCounter();
    if (now - lastTime > 1000)
    {
        DBG("FPS: " << frameCount);
        frameCount = 0;
        lastTime = now;
    }
    
    // ... rest of rendering
}
```

## 🚀 You're Ready!

You now have:
- ✅ Complete, compilable project
- ✅ Understanding of structure
- ✅ Clear next steps
- ✅ Resources to learn more

**Time to build!**

Open the project in Projucer, hit build, and start creating the most amazing Linux music visualizer. You've got this! 💪

---

## 📞 Need Help?

- **JUCE Forum:** https://forum.juce.com (very active!)
- **Debug Console:** Check terminal output for "FlarkViz:" messages
- **OpenGL Errors:** Look for GL errors in console

## 🎉 Show Your Progress!

As you build FlarkViz:
1. Screenshot your first waveform
2. Record beat detection working
3. Share your first preset loading
4. Tweet progress with #FlarkViz #flarkAUDIO

Let's build something legendary! 🎵✨
