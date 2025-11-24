# FlarkViz Examples

This directory contains example presets and usage demonstrations for the FlarkViz MilkDrop-compatible music visualization engine.

## Example Presets

### 1. Bass Pulse Zoom (`example_bass_pulse.milk`)

**Effect**: Pulsing zoom effect that reacts to bass frequencies with radial warp distortion.

**Key Features**:
- Bass-reactive zoom with smooth decay
- Slow rotation with sine wave modulation
- Color cycling based on time and audio frequencies
- Radial warp shader that creates ripples based on bass
- Color enhancement in composite shader

**Audio Mapping**:
- **Bass** → Zoom intensity and radial warp
- **Mid** → Center offset (dx/dy)
- **Treble** → Blue color channel

**Technical Highlights**:
```
zoom = zoom + bass * 0.15;        // Reactive zoom
zoom = zoom * 0.95 + 1.0 * 0.05;  // Smooth decay
```

---

### 2. Spiral Tunnel (`example_spiral_tunnel.milk`)

**Effect**: Hypnotic spiral tunnel that rotates and pulses with the music.

**Key Features**:
- Continuous rotation with bass boost
- Pulsing zoom synchronized to time
- Rainbow color cycling
- Spiral transformation in warp shader
- Bass-reactive spiral tightness

**Audio Mapping**:
- **Bass** → Rotation speed, zoom pulse, spiral tightness
- **Mid** → Color phase shift
- **Time** → Spiral rotation and color cycling

**Technical Highlights**:
```glsl
// Spiral transformation
float newAngle = angle + spiralSpeed + sin(tunnelDepth * spiralTightness);
float newDist = dist * (1.0 - bass * 0.1);
```

---

### 3. Audio Kaleidoscope (`example_kaleidoscope.milk`)

**Effect**: Kaleidoscope effect with dynamic segment count based on bass intensity.

**Key Features**:
- Oscillating zoom with bass modulation
- Counter-rotating layers
- Audio-reactive colors for all three channels
- Dynamic kaleidoscope segments (6-12 based on bass)
- Mirror symmetry within each segment

**Audio Mapping**:
- **Bass** → Number of kaleidoscope segments, red channel
- **Mid** → Green channel, time phase
- **Treble** → Blue channel

**Technical Highlights**:
```glsl
// Dynamic segment count
float segments = 6.0 + floor(bass * 6.0);

// Mirror within each segment
float mirror = abs(frac(localAngle / segmentAngle) - 0.5) * 2.0;
```

---

## C++ Usage Examples

### Compiling the Examples

```bash
cd /home/user/FlarkViz/Option2_JUCE_FlarkViz
g++ -std=c++20 -I. examples/example_usage.cpp \
    Source/Rendering/RenderState.cpp \
    Source/Rendering/ShaderCompiler.cpp \
    Source/Presets/Preset.cpp \
    Source/Presets/PresetLoader.cpp \
    Source/Expression/MilkdropEval.cpp \
    -o example_usage

./example_usage
```

### What Each Example Demonstrates

#### Example 1: Simple Preset
- Creating a preset programmatically
- Loading it into RenderState
- Simulating frame execution
- Observing zoom and rotation changes

#### Example 2: Load from File
- Using PresetLoader to read .milk files
- Error handling
- Inspecting preset properties

#### Example 3: Custom Variables (q1-q32)
- Using q variables as accumulators
- Smoothing audio values
- Passing data between per-frame and shader code

#### Example 4: Complete Rendering Setup
- Overview of the full rendering pipeline
- Integration with MainComponent
- OpenGL context requirements

#### Example 5: Expression Evaluation
- Compiling individual expressions
- Using the MilkdropEval VM
- Working with ExecutionContext
- Assignment and variable updates

#### Example 6: Shader Conversion
- HLSL to GLSL conversion
- Type mapping (float2 → vec2)
- Function mapping (saturate → clamp)

---

## Using Presets in FlarkViz

### Method 1: Load in MainComponent

```cpp
void MainComponent::loadCustomPreset()
{
    PresetLoader loader;
    auto preset = loader.loadPresetFromFile("examples/example_bass_pulse.milk");

    if (preset && renderer)
    {
        renderer->loadPreset(*preset);
        DBG("Preset loaded: " + preset->name);
    }
}
```

### Method 2: Create Programmatically

```cpp
MilkDropPreset createCustomPreset()
{
    MilkDropPreset preset;
    preset.name = "My Custom Preset";
    preset.author = "Your Name";

    preset.perFrameCode = R"(
        zoom = 1.0 + 0.1 * bass;
        rot = rot + 0.01;
        wave_r = 0.5 + 0.5 * sin(time);
    )";

    preset.warpShaderCode = R"(
        uv_warped = uv + float2(0.01, 0.01) * bass;
    )";

    preset.fDecay = 0.97f;

    return preset;
}
```

---

## Expression Language Quick Reference

### Audio Variables
- `bass`, `mid`, `treb` - Current frequency levels (0-1)
- `bass_att`, `mid_att`, `treb_att` - Attenuated (smoothed) levels

### Time Variables
- `time` - Elapsed time in seconds
- `frame` - Frame count
- `fps` - Frames per second

### State Variables
- `zoom` - Zoom level (1.0 = normal)
- `rot` - Rotation in radians
- `cx`, `cy` - Center point (0.5, 0.5 = middle)
- `dx`, `dy` - Delta offset
- `warp` - Warp amount
- `sx`, `sy` - Scale factors

### Wave Colors
- `wave_r`, `wave_g`, `wave_b`, `wave_a` - Wave rendering colors

### Custom Variables
- `q1` through `q32` - User-defined variables
- Persist between frames
- Can pass data to shaders

### Math Functions
- `sin(x)`, `cos(x)`, `tan(x)`
- `asin(x)`, `acos(x)`, `atan(x)`, `atan2(y, x)`
- `sqrt(x)`, `abs(x)`, `sqr(x)`, `pow(x, y)`
- `exp(x)`, `log(x)`, `log10(x)`
- `min(a, b)`, `max(a, b)`, `sign(x)`
- `floor(x)`, `ceil(x)`, `rand(x)`

### Operators
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Logical: `&&`, `||`, `!`
- Assignment: `=`

---

## Shader Language Quick Reference

### HLSL → GLSL Conversions

| HLSL | GLSL | Description |
|------|------|-------------|
| `float2` | `vec2` | 2D vector |
| `float3` | `vec3` | 3D vector |
| `float4` | `vec4` | 4D vector |
| `tex2D(s, uv)` | `texture(s, uv)` | Texture sample |
| `lerp(a, b, t)` | `mix(a, b, t)` | Linear interpolation |
| `saturate(x)` | `clamp(x, 0.0, 1.0)` | Clamp to 0-1 |
| `frac(x)` | `fract(x)` | Fractional part |
| `mul(a, b)` | `a * b` | Multiply |

### Available Uniforms in Shaders

```glsl
// Texture
uniform sampler2D mainTexture;

// Time
uniform float time, frame, fps;

// Audio
uniform float bass, mid, treb;
uniform float bass_att, mid_att, treb_att;

// State
uniform float zoom, rot, cx, cy, dx, dy, warp, sx, sy;

// Wave colors
uniform float wave_r, wave_g, wave_b, wave_a;

// Resolution
uniform vec2 resolution;

// Custom
uniform float q1, q2, ..., q32;
```

### Warp Shader Variables

```glsl
in vec2 uv;           // Input UV coordinates
in vec2 uv_orig;      // Original UV (before warp)
vec2 uv_warped;       // Output UV (modify this!)

// Helper variables (auto-computed)
vec2 uv_center;       // uv - vec2(0.5, 0.5)
float rad;            // length(uv_center)
float ang;            // atan(uv_center.y, uv_center.x)
```

---

## Performance Tips

1. **Per-Frame Code**: Keep it lightweight (<50 operations)
2. **Shader Code**: Avoid expensive operations in tight loops
3. **Decay**: Use fDecay 0.95-0.99 for smooth trails
4. **Custom Variables**: Use q variables to cache expensive calculations
5. **Texture Feedback**: Higher decay = longer trails (more GPU memory)

---

## Creating Your Own Presets

1. Start with a simple preset (copy `example_bass_pulse.milk`)
2. Modify per-frame code first - test zoom/rotation
3. Add color cycling with `wave_r/g/b`
4. Create warp shader effect - start simple
5. Test with different audio (bass/mid/treb emphasis)
6. Fine-tune decay and parameters
7. Add composite shader for color grading (optional)

### Template

```milk
[preset00]
name=My Preset Name
author=Your Name
fDecay=0.970000
zoom=1.000000
rot=0.000000

[per_frame_1]
// Your per-frame code here
zoom = 1.0 + 0.1 * bass;

[warp_1]
// Your warp shader here
uv_warped = uv;
```

---

## Troubleshooting

**Preset won't load:**
- Check syntax in per-frame code
- Ensure shader code uses HLSL syntax
- Verify all variable names are correct

**No visual effect:**
- Check fDecay value (0.95-0.99 recommended)
- Ensure zoom != 1.0 or rot is changing
- Verify warp shader modifies `uv_warped`

**Jerky animation:**
- Smooth variables using: `var = var * 0.9 + target * 0.1`
- Use attenuated audio: `bass_att` instead of `bass`
- Reduce sudden jumps in zoom/rotation

---

## Further Resources

- **MilkDrop Wiki**: Original MilkDrop documentation
- **JUCE OpenGL**: https://docs.juce.com/master/group__juce__opengl.html
- **GLSL Reference**: https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language

---

**Created by flarkAUDIO for the FlarkViz project**
