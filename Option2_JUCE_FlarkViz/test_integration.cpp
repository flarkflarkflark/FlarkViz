#include "Source/Rendering/RenderState.h"
#include "Source/Presets/PresetLoader.h"
#include "Source/Presets/Preset.h"
#include "Source/Expression/MilkdropEval.h"
#include "Source/Audio/AudioAnalyzer.h"
#include <iostream>
#include <iomanip>
#include <cmath>

/**
 * @brief Simulates a complete FlarkViz rendering session
 *
 * This test demonstrates the full integration of:
 * - Preset loading
 * - Expression evaluation
 * - Shader compilation
 * - Per-frame execution
 * - Audio-reactive visuals
 */

void printSeparator(const std::string& title = "")
{
    std::cout << "\n";
    std::cout << std::string(70, '=') << "\n";
    if (!title.empty())
        std::cout << "  " << title << "\n";
    std::cout << std::string(70, '=') << "\n";
}

void printState(const MilkDrop::ExecutionContext& ctx, int frame)
{
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Frame " << std::setw(3) << frame << " | ";
    std::cout << "time=" << std::setw(6) << ctx.time << " | ";
    std::cout << "bass=" << std::setw(5) << ctx.bass << " | ";
    std::cout << "zoom=" << std::setw(6) << ctx.zoom << " | ";
    std::cout << "rot=" << std::setw(6) << ctx.rot << " | ";
    std::cout << "wave_r=" << std::setw(5) << ctx.wave_r;
    std::cout << "\n";
}

int main()
{
    printSeparator("FlarkViz Integration Test");
    std::cout << "Testing complete preset → rendering pipeline\n";

    // ========================================================================
    // Step 1: Create and configure a test preset
    // ========================================================================
    printSeparator("Step 1: Create Test Preset");

    MilkDropPreset preset;
    preset.name = "FlarkViz Integration Test Preset";
    preset.author = "flarkAUDIO";

    // Per-frame init code
    preset.perFrameInitCode = R"(
        q1 = 0;
        q2 = 0
    )";

    // Per-frame code (audio-reactive)
    preset.perFrameCode = R"(
        zoom = 1.0 + 0.1 * sin(time + bass);
        rot = rot + 0.02 * cos(time * 0.5);
        wave_r = 0.5 + 0.5 * sin(time * 0.7 + bass);
        wave_g = 0.5 + 0.5 * sin(time * 0.9 + mid);
        wave_b = 0.5 + 0.5 * sin(time * 1.1 + treb);
        q1 = q1 + bass * 0.1;
        q2 = abs(sin(time))
    )";

    // Warp shader (HLSL-style)
    preset.warpShaderCode = R"(
        float2 offset = float2(0.01, 0.02) * saturate(bass);
        uv_warped += offset * frac(time)
    )";

    // Basic preset parameters
    preset.fDecay = 0.98f;
    preset.wave_r = 1.0f;
    preset.wave_g = 0.6f;
    preset.wave_b = 0.2f;

    std::cout << "Preset Name: " << preset.name << "\n";
    std::cout << "Author: " << preset.author << "\n";
    std::cout << "Per-Frame Code: " << preset.perFrameCode.length() << " bytes\n";
    std::cout << "Warp Shader: " << preset.warpShaderCode.length() << " bytes\n";

    // ========================================================================
    // Step 2: Initialize RenderState
    // ========================================================================
    printSeparator("Step 2: Load Preset into RenderState");

    RenderState renderState;

    if (!renderState.loadPreset(preset))
    {
        std::cout << "ERROR: Failed to load preset!\n";
        return 1;
    }

    std::cout << "✅ Preset loaded successfully\n";
    std::cout << "✅ Expression code compiled\n";
    std::cout << "✅ Shaders compiled\n";

    // ========================================================================
    // Step 3: Simulate audio input
    // ========================================================================
    printSeparator("Step 3: Simulate Audio Input");

    std::cout << "Simulating audio-reactive values over time...\n";
    std::cout << "Using synthetic bass/mid/treb patterns\n";

    // ========================================================================
    // Step 4: Execute rendering frames
    // ========================================================================
    printSeparator("Step 4: Execute 10 Rendering Frames");

    std::cout << "\nExecuting per-frame code with audio-reactive variables:\n\n";

    const int numFrames = 10;
    const float fps = 60.0f;
    const float deltaTime = 1.0f / fps;

    for (int frame = 0; frame < numFrames; ++frame)
    {
        // Simulate audio input (sine waves for testing)
        float t = frame * deltaTime;
        float bass = 0.5f + 0.5f * std::sin(t * 2.0f);
        float mid = 0.4f + 0.4f * std::sin(t * 3.0f);
        float treb = 0.3f + 0.3f * std::sin(t * 5.0f);

        float bassAtt = bass * 0.8f;
        float midAtt = mid * 0.8f;
        float trebAtt = treb * 0.8f;

        // Update audio data
        renderState.updateAudioData(bass, mid, treb, bassAtt, midAtt, trebAtt);

        // Execute per-frame code
        auto& ctx = renderState.executeFrame(deltaTime);

        // Print state
        printState(ctx, frame);
    }

    // ========================================================================
    // Step 5: Verify State Changes
    // ========================================================================
    printSeparator("Step 5: Verify State Changes");

    auto& finalCtx = renderState.getContext();

    std::cout << "\nFinal State After 10 Frames:\n";
    std::cout << "  time        = " << finalCtx.time << " seconds\n";
    std::cout << "  frame       = " << finalCtx.frame << "\n";
    std::cout << "  zoom        = " << finalCtx.zoom << "\n";
    std::cout << "  rot         = " << finalCtx.rot << " radians\n";
    std::cout << "  wave_r      = " << finalCtx.wave_r << "\n";
    std::cout << "  wave_g      = " << finalCtx.wave_g << "\n";
    std::cout << "  wave_b      = " << finalCtx.wave_b << "\n";
    std::cout << "  q1          = " << finalCtx.q[0] << "\n";
    std::cout << "  q2          = " << finalCtx.q[1] << "\n";

    // Verify changes occurred
    bool passed = true;

    if (finalCtx.time < 0.15f || finalCtx.time > 0.17f)
    {
        std::cout << "\n❌ FAIL: Time not advancing correctly\n";
        passed = false;
    }

    if (std::abs(finalCtx.rot) < 0.01f)
    {
        std::cout << "\n❌ FAIL: Rotation not changing\n";
        passed = false;
    }

    if (finalCtx.q[0] < 0.01f)
    {
        std::cout << "\n❌ FAIL: q1 not accumulating\n";
        passed = false;
    }

    // ========================================================================
    // Step 6: Test Shader Conversion
    // ========================================================================
    printSeparator("Step 6: Test Shader Conversion");

    ShaderCompiler compiler;
    std::string glsl = compiler.convertHLSLtoGLSL(preset.warpShaderCode, MilkDrop::ShaderType::Warp);

    std::cout << "HLSL Input:\n" << preset.warpShaderCode << "\n";
    std::cout << "\nGLSL Output:\n" << glsl << "\n";

    if (glsl.find("vec2") != std::string::npos &&
        glsl.find("clamp") != std::string::npos)
    {
        std::cout << "\n✅ HLSL→GLSL conversion working\n";
    }
    else
    {
        std::cout << "\n❌ FAIL: Shader conversion failed\n";
        passed = false;
    }

    // ========================================================================
    // Step 7: Test Complete Preset Loading
    // ========================================================================
    printSeparator("Step 7: Test Real Preset File");

    PresetLoader loader;
    auto loadedPreset = loader.loadPresetFromString(R"(
[preset00]
name=Test Audio Reactive Preset
author=flarkAUDIO
fDecay=0.950000
fRot=0.000000
zoom=1.000000
rot=0.000000

[per_frame_1]
zoom = zoom + 0.02*sin(time + bass);
rot = rot + 0.01*cos(time);
wave_r = 0.5 + 0.5*sin(time + bass_att*2);
wave_g = 0.5 + 0.5*cos(time + mid_att*2);
wave_b = 0.5 + 0.5*sin(time*1.5 + treb_att*2)
    )");

    if (loadedPreset)
    {
        std::cout << "✅ Preset loaded: " << loadedPreset->name << "\n";
        std::cout << "   Per-frame code: " << loadedPreset->perFrameCode.length() << " bytes\n";

        // Test loading it into RenderState
        RenderState testState;
        if (testState.loadPreset(*loadedPreset))
        {
            std::cout << "✅ Successfully integrated with RenderState\n";

            // Execute one frame
            testState.updateAudioData(0.8f, 0.5f, 0.3f, 0.6f, 0.4f, 0.2f);
            auto& testCtx = testState.executeFrame(1.0f / 60.0f);

            std::cout << "   Executed frame - zoom=" << testCtx.zoom
                      << ", wave_r=" << testCtx.wave_r << "\n";
        }
        else
        {
            std::cout << "❌ FAIL: Could not load into RenderState\n";
            passed = false;
        }
    }
    else
    {
        std::cout << "❌ FAIL: Could not parse preset\n";
        std::cout << "   Error: " << loader.getLastError() << "\n";
        passed = false;
    }

    // ========================================================================
    // Final Results
    // ========================================================================
    printSeparator("Integration Test Results");

    if (passed)
    {
        std::cout << "\n";
        std::cout << "   ✅ All tests PASSED!\n";
        std::cout << "\n";
        std::cout << "   FlarkViz integration is working:\n";
        std::cout << "   • Preset loading ✅\n";
        std::cout << "   • Expression evaluation ✅\n";
        std::cout << "   • Per-frame code execution ✅\n";
        std::cout << "   • Audio-reactive variables ✅\n";
        std::cout << "   • Shader compilation ✅\n";
        std::cout << "   • State management ✅\n";
        std::cout << "\n";
        std::cout << "   🎉 Ready for OpenGL rendering!\n";
        std::cout << "\n";
    }
    else
    {
        std::cout << "\n❌ Some tests FAILED\n";
        return 1;
    }

    printSeparator();

    return 0;
}
