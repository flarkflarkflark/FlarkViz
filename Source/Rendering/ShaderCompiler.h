#pragma once

#include "ShaderTypes.h"
#include "ShaderTemplates.h"
#include <string>
#include <memory>

/**
 * @class ShaderCompiler
 * @brief Converts MilkDrop HLSL shaders to GLSL and compiles them
 *
 * Handles the conversion from DirectX HLSL syntax to OpenGL GLSL,
 * injects preset variables, and compiles shaders for rendering.
 */
class ShaderCompiler
{
public:
    ShaderCompiler();
    ~ShaderCompiler();

    /**
     * @brief Convert HLSL shader code to GLSL
     * @param hlsl Original HLSL code from preset
     * @param type Shader type (warp or composite)
     * @return Converted GLSL code
     */
    std::string convertHLSLtoGLSL(const std::string& hlsl, MilkDrop::ShaderType type);

    /**
     * @brief Compile a complete shader program
     * @param vertexSource Vertex shader GLSL source
     * @param fragmentSource Fragment shader GLSL source
     * @return CompiledShader with program ID and uniform locations
     */
    std::unique_ptr<MilkDrop::CompiledShader> compileShader(
        const std::string& vertexSource,
        const std::string& fragmentSource);

    /**
     * @brief Compile a MilkDrop shader from HLSL
     * @param hlsl HLSL shader code
     * @param type Shader type
     * @return Compiled shader program
     */
    std::unique_ptr<MilkDrop::CompiledShader> compileMilkDropShader(
        const std::string& hlsl,
        MilkDrop::ShaderType type);

    /**
     * @brief Create default passthrough shaders
     * @param type Shader type
     * @return Default compiled shader
     */
    std::unique_ptr<MilkDrop::CompiledShader> createDefaultShader(
        MilkDrop::ShaderType type);

    /**
     * @brief Get the last compilation error
     */
    std::string getLastError() const { return lastError; }

private:
    std::string lastError;

    // HLSL to GLSL conversion helpers
    std::string replaceHLSLFunctions(const std::string& code);
    std::string replaceHLSLTypes(const std::string& code);
    std::string replaceHLSLSemantics(const std::string& code);
    std::string injectCodeIntoTemplate(const std::string& templateCode,
                                      const std::string& userCode);

    // OpenGL shader compilation
    unsigned int compileShaderStage(const char* source, unsigned int type);
    unsigned int linkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
    void extractUniformLocations(unsigned int programId, MilkDrop::CompiledShader& shader);
    std::string getShaderInfoLog(unsigned int shaderId);
    std::string getProgramInfoLog(unsigned int programId);
};
