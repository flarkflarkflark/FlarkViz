/**
 * FlarkViz Usage Examples
 *
 * This file demonstrates how to use the FlarkViz rendering engine
 * with various presets and configurations.
 */

#include "../Source/Rendering/PresetRenderer.h"
#include "../Source/Presets/PresetLoader.h"
#include "../Source/Rendering/RenderState.h"
#include <iostream>

// ============================================================================
// Example 1: Load and Execute a Simple Preset
// ============================================================================

void example1_SimplePreset()
{
    std::cout << "\n=== Example 1: Simple Preset ===\n";

    // Create a simple preset programmatically
    MilkDropPreset preset;
    preset.name = "Simple Pulse";
    preset.author = "Example";

    // Per-frame code: zoom pulses with bass
    preset.perFrameCode = R"(
        zoom = 1.0 + 0.2 * bass;
        rot = rot + 0.01;
        wave_r = 0.5 + 0.5 * sin(time);
        wave_g = 0.5 + 0.5 * cos(time);
        wave_b = 0.5;
    )";

    // Simple warp shader
    preset.warpShaderCode = R"(
        float2 offset = float2(0.01, 0.01) * bass;
        uv_warped = uv + offset;
    )";

    // Create render state
    RenderState renderState;

    if (renderState.loadPreset(preset))
    {
        std::cout << "✅ Preset loaded successfully\n";

        // Simulate 10 frames
        for (int frame = 0; frame < 10; ++frame)
        {
            // Simulate audio data
            float bass = 0.5f + 0.5f * std::sin(frame * 0.1f);
            float mid = 0.3f;
            float treb = 0.2f;

            // Update audio
            renderState.updateAudioData(bass, mid, treb,
                                       bass * 0.8f, mid * 0.8f, treb * 0.8f);

            // Execute frame
            auto& ctx = renderState.executeFrame(1.0f / 60.0f);

            std::cout << "Frame " << frame
                      << " | zoom=" << ctx.zoom
                      << " | rot=" << ctx.rot << "\n";
        }
    }
}

// ============================================================================
// Example 2: Load Preset from File
// ============================================================================

void example2_LoadFromFile()
{
    std::cout << "\n=== Example 2: Load from File ===\n";

    PresetLoader loader;

    // Load preset from .milk file
    auto preset = loader.loadPresetFromFile("examples/example_bass_pulse.milk");

    if (preset)
    {
        std::cout << "✅ Loaded preset: " << preset->name << "\n";
        std::cout << "   Author: " << preset->author << "\n";
        std::cout << "   Per-frame code: " << preset->perFrameCode.length() << " bytes\n";
        std::cout << "   Warp shader: " << preset->warpShaderCode.length() << " bytes\n";

        // Use the preset
        RenderState renderState;
        if (renderState.loadPreset(*preset))
        {
            std::cout << "✅ Preset compiled and ready\n";
        }
    }
    else
    {
        std::cout << "❌ Failed to load preset: " << loader.getLastError() << "\n";
    }
}

// ============================================================================
// Example 3: Custom Variables (q1-q32)
// ============================================================================

void example3_CustomVariables()
{
    std::cout << "\n=== Example 3: Custom Variables ===\n";

    MilkDropPreset preset;
    preset.name = "Custom Variables Demo";

    // Use q variables to accumulate values
    preset.perFrameInitCode = R"(
        q1 = 0;
        q2 = 0;
        q3 = 1;
    )";

    preset.perFrameCode = R"(
        // Accumulator
        q1 = q1 + bass * 0.1;

        // Smoothed mid
        q2 = q2 * 0.9 + mid * 0.1;

        // Multiplier
        q3 = q3 * 1.01;

        // Use q variables in zoom
        zoom = 1.0 + q1 * 0.1;
        wave_r = q2;
    )";

    RenderState renderState;
    renderState.loadPreset(preset);

    // Simulate frames
    for (int i = 0; i < 5; ++i)
    {
        renderState.updateAudioData(0.5f, 0.3f, 0.2f, 0.4f, 0.2f, 0.1f);
        auto& ctx = renderState.executeFrame(1.0f / 60.0f);

        std::cout << "Frame " << i
                  << " | q1=" << ctx.q[0]
                  << " | q2=" << ctx.q[1]
                  << " | q3=" << ctx.q[2] << "\n";
    }
}

// ============================================================================
// Example 4: Complete Rendering Setup
// ============================================================================

void example4_CompleteRendering()
{
    std::cout << "\n=== Example 4: Complete Rendering Setup ===\n";

    // This example shows the complete flow used in MainComponent

    // 1. Create preset renderer
    PresetRenderer renderer;

    // 2. Initialize OpenGL (would happen in OpenGL context)
    std::cout << "Note: OpenGL initialization happens in MainComponent::newOpenGLContextCreated()\n";

    // 3. Create and load preset
    MilkDropPreset preset;
    preset.name = "Complete Example";
    preset.perFrameCode = R"(
        zoom = 1.0 + 0.1 * sin(time + bass);
        rot = rot + 0.02;
        wave_r = 0.5 + 0.5 * sin(time);
        wave_g = 0.5 + 0.5 * cos(time);
        wave_b = 0.8;
    )";
    preset.warpShaderCode = R"(
        uv_warped = uv + float2(0.01, 0.01) * bass;
    )";

    // 4. Load preset (in real app, after OpenGL init)
    std::cout << "renderer.loadPreset(preset);\n";

    // 5. Render loop (in MainComponent::renderOpenGL())
    std::cout << "\nRender Loop:\n";
    std::cout << "  1. Get audio levels from AudioAnalyzer\n";
    std::cout << "  2. renderer.beginFrame(deltaTime)\n";
    std::cout << "  3. renderer.renderPreset(bass, mid, treb, bassAtt, midAtt, trebAtt)\n";
    std::cout << "     → RenderState executes per-frame code\n";
    std::cout << "     → Warp pass renders to framebuffer\n";
    std::cout << "     → Composite pass renders to screen\n";
    std::cout << "  4. renderer.endFrame()\n";
    std::cout << "  5. Repeat at 60fps\n";
}

// ============================================================================
// Example 5: Expression Evaluation System
// ============================================================================

void example5_ExpressionSystem()
{
    std::cout << "\n=== Example 5: Expression Evaluation ===\n";

    MilkdropEval evaluator;
    MilkDrop::ExecutionContext context;

    // Set up context
    context.bass = 0.8;
    context.time = 1.5;
    context.zoom = 1.0;

    // Example 1: Simple arithmetic
    std::string expr1 = "2 + 3 * 4";
    if (evaluator.compile(expr1))
    {
        double result = evaluator.execute(context);
        std::cout << expr1 << " = " << result << "\n";
    }

    // Example 2: Using audio variables
    std::string expr2 = "1.0 + 0.2 * bass";
    if (evaluator.compile(expr2))
    {
        double result = evaluator.execute(context);
        std::cout << expr2 << " = " << result << " (bass=" << context.bass << ")\n";
    }

    // Example 3: Using math functions
    std::string expr3 = "sin(time)";
    if (evaluator.compile(expr3))
    {
        double result = evaluator.execute(context);
        std::cout << expr3 << " = " << result << " (time=" << context.time << ")\n";
    }

    // Example 4: Assignment
    std::string expr4 = "zoom = zoom + bass * 0.1";
    if (evaluator.compile(expr4))
    {
        double result = evaluator.execute(context);
        std::cout << expr4 << " → zoom = " << context.zoom << "\n";
    }
}

// ============================================================================
// Example 6: Shader Conversion (HLSL → GLSL)
// ============================================================================

void example6_ShaderConversion()
{
    std::cout << "\n=== Example 6: Shader Conversion ===\n";

    ShaderCompiler compiler;

    // Example HLSL shader
    std::string hlsl = R"(
        float2 center = float2(0.5, 0.5);
        float2 delta = uv - center;
        float dist = length(delta);
        float warpAmount = saturate(bass) * 0.1;
        uv_warped = uv + delta * warpAmount;
    )";

    std::cout << "HLSL Input:\n" << hlsl << "\n\n";

    // Convert to GLSL
    std::string glsl = compiler.convertHLSLtoGLSL(hlsl, MilkDrop::ShaderType::Warp);

    std::cout << "GLSL Output:\n" << glsl << "\n";
    std::cout << "\nConversions made:\n";
    std::cout << "  • float2 → vec2\n";
    std::cout << "  • saturate(x) → clamp(x, 0.0, 1.0)\n";
}

// ============================================================================
// Main
// ============================================================================

int main()
{
    std::cout << "╔════════════════════════════════════════════╗\n";
    std::cout << "║      FlarkViz Usage Examples              ║\n";
    std::cout << "║      Demonstrating the MilkDrop Engine    ║\n";
    std::cout << "╚════════════════════════════════════════════╝\n";

    example1_SimplePreset();
    example2_LoadFromFile();
    example3_CustomVariables();
    example4_CompleteRendering();
    example5_ExpressionSystem();
    example6_ShaderConversion();

    std::cout << "\n✅ All examples complete!\n\n";

    return 0;
}
