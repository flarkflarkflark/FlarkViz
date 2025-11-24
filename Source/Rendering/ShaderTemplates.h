#pragma once

#include <string>

namespace MilkDrop {
namespace ShaderTemplates {

// Simple vertex shader for fullscreen quad
inline const char* VERTEX_SHADER = R"(
#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 uv;
out vec2 uv_orig;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    uv = aTexCoord;
    uv_orig = aTexCoord;
}
)";

// Base warp shader template
inline const char* WARP_FRAGMENT_BASE = R"(
#version 330 core

in vec2 uv;
in vec2 uv_orig;
out vec4 FragColor;

// Texture samplers
uniform sampler2D mainTexture;

// Time variables
uniform float time;
uniform float frame;
uniform float fps;

// Audio variables
uniform float bass;
uniform float mid;
uniform float treb;
uniform float bass_att;
uniform float mid_att;
uniform float treb_att;

// Preset state
uniform float zoom;
uniform float rot;
uniform float cx;
uniform float cy;
uniform float dx;
uniform float dy;
uniform float warp;
uniform float sx;
uniform float sy;

// Resolution
uniform vec2 resolution;

// Custom variables (q1-q32)
uniform float q1, q2, q3, q4, q5, q6, q7, q8;
uniform float q9, q10, q11, q12, q13, q14, q15, q16;
uniform float q17, q18, q19, q20, q21, q22, q23, q24;
uniform float q25, q26, q27, q28, q29, q30, q31, q32;

// Helper variables
vec2 uv_center = uv - vec2(0.5, 0.5);
float rad = length(uv_center);
float ang = atan(uv_center.y, uv_center.x);

// User shader code will be injected here
// USER_SHADER_CODE

void main()
{
    vec2 uv_warped = uv;

    // USER_MAIN_CODE

    FragColor = texture(mainTexture, uv_warped);
}
)";

// Base composite shader template
inline const char* COMPOSITE_FRAGMENT_BASE = R"(
#version 330 core

in vec2 uv;
in vec2 uv_orig;
out vec4 FragColor;

// Texture samplers
uniform sampler2D mainTexture;

// Time variables
uniform float time;
uniform float frame;
uniform float fps;

// Audio variables
uniform float bass;
uniform float mid;
uniform float treb;
uniform float bass_att;
uniform float mid_att;
uniform float treb_att;

// Preset state
uniform float zoom;
uniform float rot;
uniform float cx;
uniform float cy;
uniform float dx;
uniform float dy;
uniform float warp;
uniform float sx;
uniform float sy;

// Resolution
uniform vec2 resolution;

// Custom variables (q1-q32)
uniform float q1, q2, q3, q4, q5, q6, q7, q8;
uniform float q9, q10, q11, q12, q13, q14, q15, q16;
uniform float q17, q18, q19, q20, q21, q22, q23, q24;
uniform float q25, q26, q27, q28, q29, q30, q31, q32;

// Helper variables
vec2 uv_center = uv - vec2(0.5, 0.5);
float rad = length(uv_center);
float ang = atan(uv_center.y, uv_center.x);

// User shader code will be injected here
// USER_SHADER_CODE

void main()
{
    vec4 color = texture(mainTexture, uv);

    // USER_MAIN_CODE

    FragColor = color;
}
)";

// Default passthrough warp shader
inline const char* DEFAULT_WARP_FRAGMENT = R"(
#version 330 core

in vec2 uv;
out vec4 FragColor;

uniform sampler2D mainTexture;

void main()
{
    FragColor = texture(mainTexture, uv);
}
)";

// Default passthrough composite shader
inline const char* DEFAULT_COMPOSITE_FRAGMENT = R"(
#version 330 core

in vec2 uv;
out vec4 FragColor;

uniform sampler2D mainTexture;

void main()
{
    FragColor = texture(mainTexture, uv);
}
)";

} // namespace ShaderTemplates
} // namespace MilkDrop
