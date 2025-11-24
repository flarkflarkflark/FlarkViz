#pragma once

#include "../Expression/ExpressionTypes.h"
#include "../Expression/MilkdropEval.h"
#include "../Presets/Preset.h"
#include "ShaderCompiler.h"
#include "ShaderTypes.h"
#include <memory>

/**
 * @class RenderState
 * @brief Manages the complete rendering state for a MilkDrop preset
 *
 * Combines expression evaluation, shader compilation, and rendering state
 * into a cohesive system that executes per-frame code and manages shaders.
 */
class RenderState
{
public:
    RenderState();
    ~RenderState();

    /**
     * @brief Load a preset and compile its code
     * @param preset The MilkDrop preset to load
     * @return true if successful
     */
    bool loadPreset(const MilkDropPreset& preset);

    /**
     * @brief Execute per-frame code and update state
     * @param deltaTime Time since last frame (seconds)
     * @return Execution context with updated variables
     */
    MilkDrop::ExecutionContext& executeFrame(float deltaTime);

    /**
     * @brief Get the current execution context
     */
    MilkDrop::ExecutionContext& getContext() { return context; }
    const MilkDrop::ExecutionContext& getContext() const { return context; }

    /**
     * @brief Get compiled warp shader
     */
    MilkDrop::CompiledShader* getWarpShader() const { return warpShader.get(); }

    /**
     * @brief Get compiled composite shader
     */
    MilkDrop::CompiledShader* getCompositeShader() const { return compositeShader.get(); }

    /**
     * @brief Update audio variables from audio analyzer
     */
    void updateAudioData(float bass, float mid, float treb,
                        float bassAtt, float midAtt, float trebAtt);

    /**
     * @brief Reset to default state
     */
    void reset();

    /**
     * @brief Get the loaded preset
     */
    const MilkDropPreset* getPreset() const { return currentPreset; }

    /**
     * @brief Check if a preset is loaded
     */
    bool hasPreset() const { return currentPreset != nullptr; }

private:
    // Execution context with all variables
    MilkDrop::ExecutionContext context;

    // Expression evaluators
    std::unique_ptr<MilkdropEval> perFrameInitEval;
    std::unique_ptr<MilkdropEval> perFrameEval;
    std::unique_ptr<MilkdropEval> perPixelEval;

    // Compiled shaders
    std::unique_ptr<MilkDrop::CompiledShader> warpShader;
    std::unique_ptr<MilkDrop::CompiledShader> compositeShader;

    // Shader compiler
    ShaderCompiler shaderCompiler;

    // Current preset (not owned)
    const MilkDropPreset* currentPreset = nullptr;

    // Frame tracking
    int frameCount = 0;
    float totalTime = 0.0f;
    bool perFrameInitExecuted = false;
};
