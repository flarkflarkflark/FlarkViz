#pragma once

#include <JuceHeader.h>
#include "FlarkVizPlugin.h"

/**
 * @class FlarkVizPluginEditor
 * @brief GUI editor for FlarkViz plugin
 *
 * Provides a visual interface showing the visualization and controls
 * for brightness, contrast, saturation, and preset selection.
 */
class FlarkVizPluginEditor : public juce::AudioProcessorEditor,
                              private juce::Timer
{
public:
    FlarkVizPluginEditor(FlarkVizPlugin&);
    ~FlarkVizPluginEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    FlarkVizPlugin& audioProcessor;

    // UI Components
    juce::Slider brightnessSlider;
    juce::Slider contrastSlider;
    juce::Slider saturationSlider;
    juce::Slider transitionTimeSlider;

    juce::Label brightnessLabel;
    juce::Label contrastLabel;
    juce::Label saturationLabel;
    juce::Label transitionTimeLabel;

    juce::ComboBox presetSelector;
    juce::TextButton randomizeButton;

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> brightnessAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> contrastAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> saturationAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> transitionTimeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlarkVizPluginEditor)
};
