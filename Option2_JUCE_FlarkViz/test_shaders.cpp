#include "Source/Rendering/ShaderCompiler.h"
#include "Source/Rendering/ShaderTypes.h"
#include <iostream>
#include <iomanip>

void testHLSLConversion(const std::string& hlsl, const std::string& description)
{
    ShaderCompiler compiler;
    std::string glsl = compiler.convertHLSLtoGLSL(hlsl, MilkDrop::ShaderType::Warp);

    std::cout << "============================================" << std::endl;
    std::cout << description << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "HLSL Input:" << std::endl;
    std::cout << hlsl << std::endl;
    std::cout << std::endl;
    std::cout << "GLSL Output:" << std::endl;
    std::cout << glsl << std::endl;
    std::cout << std::endl;
}

int main()
{
    std::cout << "============================================" << std::endl;
    std::cout << "  FlarkViz Shader Compiler Test" << std::endl;
    std::cout << "============================================" << std::endl << std::endl;

    // Test 1: Type conversions
    testHLSLConversion(
        "float4 color = float4(1.0, 0.5, 0.0, 1.0);",
        "Test 1: Type Conversions"
    );

    // Test 2: tex2D function
    testHLSLConversion(
        "float4 sample = tex2D(mainSampler, uv);",
        "Test 2: tex2D → texture"
    );

    // Test 3: lerp function
    testHLSLConversion(
        "float3 blended = lerp(colorA, colorB, 0.5);",
        "Test 3: lerp → mix"
    );

    // Test 4: saturate function
    testHLSLConversion(
        "float clamped = saturate(value);",
        "Test 4: saturate → clamp"
    );

    // Test 5: frac function
    testHLSLConversion(
        "float fractional = frac(time);",
        "Test 5: frac → fract"
    );

    // Test 6: Multiple types
    testHLSLConversion(
        R"(
        float2 offset = float2(0.01, 0.02);
        float3 color = float3(1.0, 0.5, 0.2);
        half4 result = half4(color, 1.0);
        )",
        "Test 6: Multiple Type Conversions"
    );

    // Test 7: Realistic MilkDrop warp shader snippet
    testHLSLConversion(
        R"(
        float2 uv2 = uv;
        float2 center = float2(0.5, 0.5);
        float dist = length(uv - center);
        float warpAmount = 0.1 * saturate(bass);
        uv2 += normalize(uv - center) * warpAmount * frac(time);
        )",
        "Test 7: Realistic Warp Shader"
    );

    // Test 8: Complex expression with mul()
    testHLSLConversion(
        R"(
        float4 color = tex2D(mainSampler, uv);
        float brightness = mul(color.rgb, float3(0.299, 0.587, 0.114));
        color.rgb = lerp(color.rgb, float3(brightness), 0.5);
        )",
        "Test 8: Complex Color Processing"
    );

    std::cout << "============================================" << std::endl;
    std::cout << "  Shader Template Examples" << std::endl;
    std::cout << "============================================" << std::endl << std::endl;

    std::cout << "Warp Shader Template:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::string warpTemplate = MilkDrop::ShaderTemplates::WARP_FRAGMENT_BASE;
    std::cout << warpTemplate.substr(0, 500) << "..." << std::endl;
    std::cout << std::endl;

    std::cout << "Composite Shader Template:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::string compTemplate = MilkDrop::ShaderTemplates::COMPOSITE_FRAGMENT_BASE;
    std::cout << compTemplate.substr(0, 500) << "..." << std::endl;
    std::cout << std::endl;

    std::cout << "============================================" << std::endl;
    std::cout << "  All tests completed!" << std::endl;
    std::cout << "============================================" << std::endl;

    return 0;
}
