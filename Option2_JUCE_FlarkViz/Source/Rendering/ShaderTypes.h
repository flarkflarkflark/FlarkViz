#pragma once

#include <string>
#include <vector>
#include <map>

namespace MilkDrop {

/**
 * @enum ShaderType
 * @brief Types of MilkDrop shaders
 */
enum class ShaderType
{
    Warp,       // Per-pixel distortion shader (runs before composite)
    Composite   // Final color/blur shader (runs after warp)
};

/**
 * @struct ShaderCode
 * @brief Container for shader source code
 */
struct ShaderCode
{
    std::string hlsl;  // Original HLSL from preset
    std::string glsl;  // Converted GLSL
    ShaderType type;
    bool compiled = false;

    ShaderCode() : type(ShaderType::Warp) {}
    ShaderCode(ShaderType t) : type(t) {}
};

/**
 * @struct CompiledShader
 * @brief OpenGL shader program with uniform locations
 */
struct CompiledShader
{
    unsigned int programId = 0;

    // Uniform locations for common variables
    int loc_time = -1;
    int loc_frame = -1;
    int loc_fps = -1;
    int loc_bass = -1;
    int loc_mid = -1;
    int loc_treb = -1;
    int loc_bass_att = -1;
    int loc_mid_att = -1;
    int loc_treb_att = -1;

    // Preset state uniforms
    int loc_zoom = -1;
    int loc_rot = -1;
    int loc_cx = -1;
    int loc_cy = -1;
    int loc_dx = -1;
    int loc_dy = -1;
    int loc_warp = -1;
    int loc_sx = -1;
    int loc_sy = -1;

    // Resolution
    int loc_resolution = -1;

    // Textures
    int loc_texture = -1;

    // Custom q variables (q1-q32)
    int loc_q[32];

    CompiledShader()
    {
        for (int i = 0; i < 32; ++i)
            loc_q[i] = -1;
    }

    bool isValid() const { return programId != 0; }
};

} // namespace MilkDrop
