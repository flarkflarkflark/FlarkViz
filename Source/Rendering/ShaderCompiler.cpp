#include "ShaderCompiler.h"
#include <JuceHeader.h>
#include <algorithm>
#include <regex>
#include <sstream>

using namespace juce::gl;

ShaderCompiler::ShaderCompiler()
{
}

ShaderCompiler::~ShaderCompiler()
{
}

std::string ShaderCompiler::convertHLSLtoGLSL(const std::string& hlsl, MilkDrop::ShaderType type)
{
    std::string glsl = hlsl;

    // Replace HLSL types with GLSL types
    glsl = replaceHLSLTypes(glsl);

    // Replace HLSL functions with GLSL equivalents
    glsl = replaceHLSLFunctions(glsl);

    // Remove HLSL semantics
    glsl = replaceHLSLSemantics(glsl);

    return glsl;
}

std::string ShaderCompiler::replaceHLSLTypes(const std::string& code)
{
    std::string result = code;

    // HLSL → GLSL type mappings
    std::map<std::string, std::string> typeMap = {
        {"float2", "vec2"},
        {"float3", "vec3"},
        {"float4", "vec4"},
        {"half", "float"},
        {"half2", "vec2"},
        {"half3", "vec3"},
        {"half4", "vec4"},
        {"sampler2D", "sampler2D"},  // Same in both
    };

    for (const auto& [hlslType, glslType] : typeMap)
    {
        size_t pos = 0;
        while ((pos = result.find(hlslType, pos)) != std::string::npos)
        {
            // Check if it's a whole word (not part of another identifier)
            bool isWholeWord = true;
            if (pos > 0)
            {
                char prev = result[pos - 1];
                if (std::isalnum(prev) || prev == '_')
                    isWholeWord = false;
            }
            if (pos + hlslType.length() < result.length())
            {
                char next = result[pos + hlslType.length()];
                if (std::isalnum(next) || next == '_')
                    isWholeWord = false;
            }

            if (isWholeWord)
            {
                result.replace(pos, hlslType.length(), glslType);
                pos += glslType.length();
            }
            else
            {
                pos += hlslType.length();
            }
        }
    }

    return result;
}

std::string ShaderCompiler::replaceHLSLFunctions(const std::string& code)
{
    std::string result = code;

    // tex2D(sampler, uv) → texture(sampler, uv)
    std::regex tex2DRegex(R"(tex2D\s*\()");
    result = std::regex_replace(result, tex2DRegex, "texture(");

    // mul(a, b) → (a * b) for simple cases
    // This is simplified; full HLSL mul() is more complex
    std::regex mulRegex(R"(mul\s*\(\s*([^,]+)\s*,\s*([^)]+)\s*\))");
    result = std::regex_replace(result, mulRegex, "($1 * $2)");

    // lerp(a, b, t) → mix(a, b, t)
    std::regex lerpRegex(R"(lerp\s*\()");
    result = std::regex_replace(result, lerpRegex, "mix(");

    // saturate(x) → clamp(x, 0.0, 1.0)
    std::regex saturateRegex(R"(saturate\s*\(\s*([^)]+)\s*\))");
    result = std::regex_replace(result, saturateRegex, "clamp($1, 0.0, 1.0)");

    // frac(x) → fract(x)
    std::regex fracRegex(R"(frac\s*\()");
    result = std::regex_replace(result, fracRegex, "fract(");

    return result;
}

std::string ShaderCompiler::replaceHLSLSemantics(const std::string& code)
{
    std::string result = code;

    // Remove HLSL semantics like : POSITION, : TEXCOORD0, etc.
    std::regex semanticsRegex(R"(:\s*[A-Z_][A-Z0-9_]*)");
    result = std::regex_replace(result, semanticsRegex, "");

    return result;
}

std::string ShaderCompiler::injectCodeIntoTemplate(const std::string& templateCode,
                                                   const std::string& userCode)
{
    std::string result = templateCode;

    // Replace USER_SHADER_CODE marker with user's shader code
    size_t pos = result.find("// USER_SHADER_CODE");
    if (pos != std::string::npos)
    {
        result.replace(pos, std::string("// USER_SHADER_CODE").length(), userCode);
    }

    // Replace USER_MAIN_CODE marker
    pos = result.find("// USER_MAIN_CODE");
    if (pos != std::string::npos)
    {
        // For now, just remove the marker
        // In a full implementation, we'd extract the main() body from userCode
        result.replace(pos, std::string("// USER_MAIN_CODE").length(), "");
    }

    return result;
}

std::unique_ptr<MilkDrop::CompiledShader> ShaderCompiler::compileMilkDropShader(
    const std::string& hlsl,
    MilkDrop::ShaderType type)
{
    // Convert HLSL to GLSL
    std::string glsl = convertHLSLtoGLSL(hlsl, type);

    // Get the appropriate template
    const char* templateCode = (type == MilkDrop::ShaderType::Warp) ?
        MilkDrop::ShaderTemplates::WARP_FRAGMENT_BASE :
        MilkDrop::ShaderTemplates::COMPOSITE_FRAGMENT_BASE;

    // Inject user code into template
    std::string fragmentSource = injectCodeIntoTemplate(templateCode, glsl);

    // Compile the shader
    return compileShader(MilkDrop::ShaderTemplates::VERTEX_SHADER, fragmentSource);
}

std::unique_ptr<MilkDrop::CompiledShader> ShaderCompiler::createDefaultShader(
    MilkDrop::ShaderType type)
{
    const char* fragmentSource = (type == MilkDrop::ShaderType::Warp) ?
        MilkDrop::ShaderTemplates::DEFAULT_WARP_FRAGMENT :
        MilkDrop::ShaderTemplates::DEFAULT_COMPOSITE_FRAGMENT;

    return compileShader(MilkDrop::ShaderTemplates::VERTEX_SHADER, fragmentSource);
}

std::unique_ptr<MilkDrop::CompiledShader> ShaderCompiler::compileShader(
    const std::string& vertexSource,
    const std::string& fragmentSource)
{
    auto shader = std::make_unique<MilkDrop::CompiledShader>();

    // Compile vertex shader
    unsigned int vertexShader = compileShaderStage(vertexSource.c_str(), GL_VERTEX_SHADER);
    if (vertexShader == 0)
    {
        lastError = "Failed to compile vertex shader: " + getShaderInfoLog(0);
        return nullptr;
    }

    // Compile fragment shader
    unsigned int fragmentShader = compileShaderStage(fragmentSource.c_str(), GL_FRAGMENT_SHADER);
    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        lastError = "Failed to compile fragment shader: " + getShaderInfoLog(0);
        return nullptr;
    }

    // Link program
    unsigned int programId = linkShaderProgram(vertexShader, fragmentShader);
    if (programId == 0)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        lastError = "Failed to link shader program: " + getProgramInfoLog(0);
        return nullptr;
    }

    // Clean up shader objects (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Store program ID
    shader->programId = programId;

    // Extract all uniform locations
    extractUniformLocations(programId, *shader);

    lastError.clear();
    return shader;
}

unsigned int ShaderCompiler::compileShaderStage(const char* source, unsigned int type)
{
    unsigned int shader = glCreateShader(type);
    if (shader == 0)
        return 0;

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        lastError = getShaderInfoLog(shader);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

unsigned int ShaderCompiler::linkShaderProgram(unsigned int vertexShader,
                                               unsigned int fragmentShader)
{
    unsigned int program = glCreateProgram();
    if (program == 0)
        return 0;

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        lastError = getProgramInfoLog(program);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

void ShaderCompiler::extractUniformLocations(unsigned int programId,
                                             MilkDrop::CompiledShader& shader)
{
    // Time variables
    shader.loc_time = glGetUniformLocation(programId, "time");
    shader.loc_frame = glGetUniformLocation(programId, "frame");
    shader.loc_fps = glGetUniformLocation(programId, "fps");

    // Audio variables
    shader.loc_bass = glGetUniformLocation(programId, "bass");
    shader.loc_mid = glGetUniformLocation(programId, "mid");
    shader.loc_treb = glGetUniformLocation(programId, "treb");
    shader.loc_bass_att = glGetUniformLocation(programId, "bass_att");
    shader.loc_mid_att = glGetUniformLocation(programId, "mid_att");
    shader.loc_treb_att = glGetUniformLocation(programId, "treb_att");

    // Preset state
    shader.loc_zoom = glGetUniformLocation(programId, "zoom");
    shader.loc_rot = glGetUniformLocation(programId, "rot");
    shader.loc_cx = glGetUniformLocation(programId, "cx");
    shader.loc_cy = glGetUniformLocation(programId, "cy");
    shader.loc_dx = glGetUniformLocation(programId, "dx");
    shader.loc_dy = glGetUniformLocation(programId, "dy");
    shader.loc_warp = glGetUniformLocation(programId, "warp");
    shader.loc_sx = glGetUniformLocation(programId, "sx");
    shader.loc_sy = glGetUniformLocation(programId, "sy");

    // Wave colors
    shader.loc_wave_r = glGetUniformLocation(programId, "wave_r");
    shader.loc_wave_g = glGetUniformLocation(programId, "wave_g");
    shader.loc_wave_b = glGetUniformLocation(programId, "wave_b");
    shader.loc_wave_a = glGetUniformLocation(programId, "wave_a");

    // Resolution
    shader.loc_resolution = glGetUniformLocation(programId, "resolution");

    // Texture sampler
    shader.loc_mainTexture = glGetUniformLocation(programId, "mainTexture");

    // Custom variables (q1-q32)
    for (int i = 0; i < 32; ++i)
    {
        std::string qName = "q" + std::to_string(i + 1);
        shader.loc_q[i] = glGetUniformLocation(programId, qName.c_str());
    }
}

std::string ShaderCompiler::getShaderInfoLog(unsigned int shaderId)
{
    if (shaderId == 0)
        return "Invalid shader ID";

    GLint logLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength <= 0)
        return "";

    std::string log(logLength, '\0');
    glGetShaderInfoLog(shaderId, logLength, nullptr, &log[0]);
    return log;
}

std::string ShaderCompiler::getProgramInfoLog(unsigned int programId)
{
    if (programId == 0)
        return "Invalid program ID";

    GLint logLength;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength <= 0)
        return "";

    std::string log(logLength, '\0');
    glGetProgramInfoLog(programId, logLength, nullptr, &log[0]);
    return log;
}
