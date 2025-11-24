#pragma once

#include <JuceHeader.h>
#include "../Audio/AudioAnalyzer.h"
#include "../Presets/Preset.h"
#include "RenderState.h"
#include "FramebufferManager.h"
#include "ShaderCompiler.h"

/**
 * @class PresetRenderer
 * @brief Complete MilkDrop preset renderer with full pipeline
 *
 * Implements the complete MilkDrop rendering pipeline:
 * 1. Execute per-frame expressions
 * 2. Render warp pass (texture feedback + warp shader)
 * 3. Render composite pass (final output)
 */
class PresetRenderer
{
public:
    PresetRenderer();
    ~PresetRenderer();

    //==========================================================================
    // OpenGL lifecycle
    void initializeGL();
    void cleanupGL();
    void setViewportSize (int width, int height);

    //==========================================================================
    // Rendering
    void beginFrame(float deltaTime);
    void renderPreset (float bass, float mid, float treb,
                      float bassAtt, float midAtt, float trebAtt);
    void endFrame();

    //==========================================================================
    // Preset management
    bool loadPreset (const MilkDropPreset& preset);
    void enableDoublePresetMode (bool enable);

private:
    //==========================================================================
    // OpenGL objects
    struct OpenGLObjects
    {
        GLuint fullscreenVAO = 0;
        GLuint fullscreenVBO = 0;
    } gl;

    // Viewport
    int viewportWidth = 1280;
    int viewportHeight = 720;

    // Rendering state
    std::unique_ptr<RenderState> renderState;
    std::unique_ptr<FramebufferManager> framebufferManager;

    // State
    bool doublePresetMode = false;
    bool presetLoaded = false;
    float deltaTime = 1.0f / 60.0f;

    //==========================================================================
    // Internal rendering
    void createFullscreenQuad();
    void renderWarpPass();
    void renderCompositePass();
    void bindShaderUniforms(const MilkDrop::CompiledShader& shader,
                           const MilkDrop::ExecutionContext& context);
    void drawFullscreenQuad();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetRenderer)
};
