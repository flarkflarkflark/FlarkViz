#pragma once

#include <JuceHeader.h>
#include "../Audio/AudioAnalyzer.h"

/**
 * @class PresetRenderer
 * @brief OpenGL-based preset renderer
 * 
 * Handles rendering of MilkDrop presets using OpenGL shaders
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
    void beginFrame();
    void renderPreset (const std::vector<float>& fftData,
                      const std::vector<float>& waveformData,
                      const AudioAnalyzer::Beat& beat);
    void endFrame();
    
    //==========================================================================
    // Preset management
    void loadPreset (void* preset);  // Will use proper Preset class later
    void enableDoublePresetMode (bool enable);
    void randomizeColors();
    
private:
    //==========================================================================
    // OpenGL objects
    struct OpenGLObjects
    {
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint shaderProgram = 0;
        GLuint waveformVBO = 0;
    } gl;
    
    // Viewport
    int viewportWidth = 1280;
    int viewportHeight = 720;
    
    // State
    bool doublePresetMode = false;
    float time = 0.0f;
    
    // flarkAUDIO colors
    const juce::Colour flarkOrange {0xFFFF6600};
    const juce::Colour flarkBlack {0xFF000000};
    
    //==========================================================================
    // Internal rendering
    void renderWaveform (const std::vector<float>& waveformData);
    void renderBackground();
    void createSimpleShader();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetRenderer)
};
