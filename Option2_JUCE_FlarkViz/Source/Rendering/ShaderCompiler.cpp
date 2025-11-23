#include "ShaderCompiler.h"
#include <algorithm>
#include <regex>
#include <sstream>

// OpenGL will be provided by JUCE's OpenGL module when built with the full project
// For now, we'll define the necessary constants
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
#endif

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
    // NOTE: This function requires OpenGL context to be active
    // When built with JUCE, OpenGL functions will be available
    // For standalone compilation, this will be a no-op

    auto shader = std::make_unique<MilkDrop::CompiledShader>();

    // In a full implementation with OpenGL:
    // 1. Compile vertex shader
    // 2. Compile fragment shader
    // 3. Link program
    // 4. Extract uniform locations

    // For now, return a placeholder
    // This will be fully implemented when integrated with JUCE OpenGL

    lastError = "Shader compilation requires OpenGL context (provided by JUCE)";

    return shader;
}

unsigned int ShaderCompiler::compileShaderStage(const char* source, unsigned int type)
{
    // OpenGL shader compilation
    // This will be implemented when built with JUCE OpenGL module
    return 0;
}

unsigned int ShaderCompiler::linkShaderProgram(unsigned int vertexShader,
                                               unsigned int fragmentShader)
{
    // OpenGL program linking
    // This will be implemented when built with JUCE OpenGL module
    return 0;
}

void ShaderCompiler::extractUniformLocations(unsigned int programId,
                                             MilkDrop::CompiledShader& shader)
{
    // Extract all uniform locations using glGetUniformLocation
    // This will be implemented when built with JUCE OpenGL module

    // Example:
    // shader.loc_time = glGetUniformLocation(programId, "time");
    // shader.loc_bass = glGetUniformLocation(programId, "bass");
    // etc.
}

std::string ShaderCompiler::getShaderInfoLog(unsigned int shaderId)
{
    // Get shader compilation log
    // This will be implemented when built with JUCE OpenGL module
    return "";
}

std::string ShaderCompiler::getProgramInfoLog(unsigned int programId)
{
    // Get program linking log
    // This will be implemented when built with JUCE OpenGL module
    return "";
}
