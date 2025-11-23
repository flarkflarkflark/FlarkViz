#pragma once

#include <JuceHeader.h>
#include "Audio/AudioAnalyzer.h"
#include "Rendering/PresetRenderer.h"
#include "Presets/PresetManager.h"

/**
 * @class MainComponent
 * @brief Main application window for FlarkViz
 * 
 * Handles audio input, visualization rendering, and user interaction.
 * Designed with flarkAUDIO aesthetics: orange on black.
 */
class MainComponent : public juce::Component,
                      public juce::OpenGLRenderer,
                      private juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    //==========================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==========================================================================
    // OpenGLRenderer callbacks
    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing() override;

    //==========================================================================
    // Key handling for MilkDrop3-style shortcuts
    bool keyPressed (const juce::KeyPress& key) override;

private:
    //==========================================================================
    void timerCallback() override;
    void setupAudioInput();
    void loadDefaultPreset();

    //==========================================================================
    juce::OpenGLContext openGLContext;
    
    // Audio components
    std::unique_ptr<AudioAnalyzer> audioAnalyzer;
    juce::AudioDeviceManager deviceManager;
    
    // Rendering
    std::unique_ptr<PresetRenderer> renderer;
    std::unique_ptr<PresetManager> presetManager;
    
    // State
    bool isFullscreen = false;
    int currentFPS = 60;
    float transitionProgress = 0.0f;
    
    // flarkAUDIO branding colors
    const juce::Colour flarkOrange {0xFFFF6600};
    const juce::Colour flarkBlack {0xFF000000};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
