#pragma once

#include <JuceHeader.h>
#include "../Audio/AudioAnalyzer.h"
#include "../Presets/PresetManager.h"
#include "../Rendering/PresetRenderer.h"
#include "../Rendering/TransitionEngine.h"

/**
 * @class FlarkVizPlugin
 * @brief Audio plugin processor for FlarkViz (VST3/LV2/CLAP)
 *
 * Provides FlarkViz visualization as an audio plugin that can be used
 * in DAWs and other plugin hosts.
 */
class FlarkVizPlugin : public juce::AudioProcessor
{
public:
    FlarkVizPlugin();
    ~FlarkVizPlugin() override;

    //==============================================================================
    // AudioProcessor overrides
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // Plugin-specific API
    AudioAnalyzer* getAudioAnalyzer() { return &audioAnalyzer; }
    PresetManager* getPresetManager() { return &presetManager; }
    PresetRenderer* getRenderer() { return &renderer; }
    TransitionEngine* getTransitionEngine() { return &transitionEngine; }

    // Parameter management
    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }

private:
    //==============================================================================
    AudioAnalyzer audioAnalyzer;
    PresetManager presetManager;
    PresetRenderer renderer;
    TransitionEngine transitionEngine;

    // Parameters
    juce::AudioProcessorValueTreeState parameters;
    std::atomic<float>* brightnessParam = nullptr;
    std::atomic<float>* contrastParam = nullptr;
    std::atomic<float>* saturationParam = nullptr;
    std::atomic<float>* transitionTimeParam = nullptr;
    std::atomic<float>* autoChangeParam = nullptr;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlarkVizPlugin)
};
