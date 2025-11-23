# Contributing to FlarkViz

First off, thank you for considering contributing to FlarkViz! 🎵

FlarkViz is a community-driven project, and we welcome contributions of all kinds.

## Code of Conduct

Be respectful, constructive, and professional. We're all here to build something awesome.

## How Can I Contribute?

### 🐛 Reporting Bugs

Before creating bug reports, please check existing issues. When creating a bug report, include:

- **Description**: Clear description of the bug
- **Steps to reproduce**: Detailed steps to trigger the bug
- **Expected behavior**: What should happen
- **Actual behavior**: What actually happens
- **Environment**: OS, JUCE version, compiler version
- **Logs**: Any error messages or console output

**Example:**
```markdown
**Bug**: Waveform not displaying on Ubuntu 22.04

**Steps to reproduce:**
1. Build FlarkViz on Ubuntu 22.04
2. Run ./FlarkViz
3. Play audio

**Expected**: Waveform should display
**Actual**: Black screen only
**Environment**: Ubuntu 22.04, JUCE 7.0.5, gcc 11.4
**Logs**: [paste relevant logs]
```

### 💡 Suggesting Enhancements

Enhancement suggestions are welcome! Please include:

- **Use case**: Why is this useful?
- **Proposed solution**: How should it work?
- **Alternatives**: Other approaches considered?
- **Examples**: Similar features in other software?

### 📝 Documentation

Documentation improvements are always welcome:
- Fix typos
- Clarify confusing sections
- Add examples
- Improve build instructions
- Translate documentation

### 💻 Code Contributions

#### Getting Started

1. **Fork** the repository
2. **Clone** your fork:
   ```bash
   git clone https://github.com/YOUR_USERNAME/FlarkViz.git
   ```
3. **Create a branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```

#### Development Setup

See [BUILD.md](BUILD.md) for complete build instructions.

**Quick setup:**
```bash
# Install dependencies
sudo pacman -S juce mesa alsa-lib pulseaudio jack2

# Build
cd FlarkViz
mkdir build && cd build
cmake ..
make -j$(nproc)
```

#### Code Style

We follow these conventions:

**C++ Style:**
- C++20 features encouraged
- Use `camelCase` for variables and functions
- Use `PascalCase` for classes
- 4-space indentation
- Opening braces on same line
- Use modern C++ features (auto, range-for, etc.)

**Example:**
```cpp
class AudioAnalyzer {
public:
    void processAudioBlock(const float** inputChannelData, 
                          int numChannels, 
                          int numSamples)
    {
        for (int i = 0; i < numSamples; ++i) {
            auto sample = inputChannelData[0][i];
            // Process sample...
        }
    }
};
```

**JUCE Style:**
- Follow JUCE naming conventions for JUCE classes
- Use JUCE types when appropriate (juce::String, juce::Array, etc.)

#### What to Contribute

**High Priority:**
- [ ] Preset parser (.milk file format)
- [ ] Expression evaluator integration
- [ ] Shader compiler (HLSL → GLSL)
- [ ] .milk2 double-preset support
- [ ] Transition effects
- [ ] Performance optimizations

**Medium Priority:**
- [ ] Preset browser UI
- [ ] Live editor panel
- [ ] Audio capture improvements
- [ ] Additional waveform types
- [ ] Shape rendering

**Low Priority:**
- [ ] Preset management features
- [ ] Keyboard shortcut customization
- [ ] Theme support
- [ ] Export to video

#### Pull Request Process

1. **Update documentation** if needed
2. **Add tests** if applicable (future)
3. **Follow code style** guidelines
4. **Keep commits focused** - one feature/fix per PR
5. **Write good commit messages**:
   ```
   Add preset parser for .milk files
   
   - Implement MilkDrop 1.0 format parsing
   - Extract per-frame equations
   - Parse shader sections
   - Add unit tests
   ```

6. **Update CHANGELOG** (future)
7. **Ensure it builds** on Linux
8. **Submit PR** with clear description

**PR Template:**
```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Performance improvement
- [ ] Documentation update

## Testing
How was this tested?

## Checklist
- [ ] Code follows style guidelines
- [ ] Documentation updated
- [ ] Builds successfully
- [ ] No new warnings
```

## Project Structure

```
FlarkViz/
├── Source/              - Main source code
│   ├── Audio/          - Audio analysis (FFT, beat detection)
│   ├── Rendering/      - OpenGL rendering, shaders
│   ├── Presets/        - Preset loading and management
│   └── Expression/     - Expression evaluator
├── demo/               - Standalone demos
└── docs/               - Documentation
```

## Areas to Contribute

### Audio Processing
**Files**: `Source/Audio/`  
**Skills**: DSP, FFT, audio processing  
**Tasks**: Beat detection improvements, frequency analysis

### Rendering
**Files**: `Source/Rendering/`  
**Skills**: OpenGL, GLSL, graphics programming  
**Tasks**: Shader support, visual effects, optimization

### Preset System
**Files**: `Source/Presets/`  
**Skills**: Parsing, file I/O  
**Tasks**: .milk parser, .milk2 support, preset management

### Expression Evaluator
**Files**: `Source/Expression/`  
**Skills**: Compilers, interpreters, math  
**Tasks**: Integrate projectM-eval or build custom evaluator

## Testing

Currently manual testing only. Future:
- Unit tests (Catch2)
- Integration tests
- Performance benchmarks
- Visual regression tests

## Communication

- **GitHub Issues**: Bug reports, feature requests
- **GitHub Discussions**: Questions, ideas (coming soon)
- **JUCE Forum**: Technical JUCE questions
- **Discord**: Community chat (coming soon)

## Recognition

Contributors will be:
- Listed in CONTRIBUTORS.md (coming soon)
- Credited in release notes
- Thanked profusely! 🙏

## Questions?

Not sure where to start? Open an issue with the "question" label!

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for helping make FlarkViz awesome!** 🎵✨

*FlarkViz by flarkAUDIO*
