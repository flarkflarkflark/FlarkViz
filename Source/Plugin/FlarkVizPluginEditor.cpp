#include "FlarkVizPluginEditor.h"

FlarkVizPluginEditor::FlarkVizPluginEditor(FlarkVizPlugin& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(800, 600);

    // Setup sliders
    brightnessSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    brightnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(brightnessSlider);

    contrastSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    contrastSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(contrastSlider);

    saturationSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    saturationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(saturationSlider);

    transitionTimeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    transitionTimeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    addAndMakeVisible(transitionTimeSlider);

    // Setup labels
    brightnessLabel.setText("Brightness", juce::dontSendNotification);
    brightnessLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(brightnessLabel);

    contrastLabel.setText("Contrast", juce::dontSendNotification);
    contrastLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(contrastLabel);

    saturationLabel.setText("Saturation", juce::dontSendNotification);
    saturationLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(saturationLabel);

    transitionTimeLabel.setText("Transition Time", juce::dontSendNotification);
    addAndMakeVisible(transitionTimeLabel);

    // Setup preset selector
    presetSelector.addItem("Random", 1);
    presetSelector.setSelectedId(1);
    addAndMakeVisible(presetSelector);

    // Setup randomize button
    randomizeButton.setButtonText("Randomize Preset");
    addAndMakeVisible(randomizeButton);

    // Attach parameters
    brightnessAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getParameters(), "brightness", brightnessSlider));

    contrastAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getParameters(), "contrast", contrastSlider));

    saturationAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getParameters(), "saturation", saturationSlider));

    transitionTimeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getParameters(), "transitionTime", transitionTimeSlider));

    // Start timer for updates
    startTimerHz(30); // 30 FPS
}

FlarkVizPluginEditor::~FlarkVizPluginEditor()
{
}

void FlarkVizPluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    // Draw visualization area
    juce::Rectangle<int> vizArea(10, 10, getWidth() - 20, getHeight() - 200);
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(vizArea);

    g.setColour(juce::Colours::white);
    g.drawRect(vizArea, 2);

    // Draw placeholder text
    g.setFont(24.0f);
    g.drawText("FlarkViz Visualization", vizArea, juce::Justification::centred);

    // Draw audio levels
    auto* analyzer = audioProcessor.getAudioAnalyzer();
    if (analyzer)
    {
        g.setColour(juce::Colours::cyan);
        g.setFont(14.0f);

        juce::String info;
        info << "Bass: " << juce::String(analyzer->getBass(), 2) << "  ";
        info << "Mid: " << juce::String(analyzer->getMid(), 2) << "  ";
        info << "Treble: " << juce::String(analyzer->getTreb(), 2);

        g.drawText(info, vizArea.removeFromBottom(30), juce::Justification::centred);
    }
}

void FlarkVizPluginEditor::resized()
{
    auto area = getLocalBounds();

    // Reserve top area for visualization
    auto vizArea = area.removeFromTop(getHeight() - 180);

    // Control area
    area.removeFromTop(10); // Padding

    // Rotary controls row
    auto knobArea = area.removeFromTop(100);
    int knobWidth = knobArea.getWidth() / 3;

    auto brightnessArea = knobArea.removeFromLeft(knobWidth);
    brightnessLabel.setBounds(brightnessArea.removeFromTop(20));
    brightnessSlider.setBounds(brightnessArea.reduced(10));

    auto contrastArea = knobArea.removeFromLeft(knobWidth);
    contrastLabel.setBounds(contrastArea.removeFromTop(20));
    contrastSlider.setBounds(contrastArea.reduced(10));

    auto saturationArea = knobArea.removeFromLeft(knobWidth);
    saturationLabel.setBounds(saturationArea.removeFromTop(20));
    saturationSlider.setBounds(saturationArea.reduced(10));

    area.removeFromTop(10); // Padding

    // Transition time slider
    auto transitionArea = area.removeFromTop(40);
    transitionTimeLabel.setBounds(transitionArea.removeFromLeft(120));
    transitionTimeSlider.setBounds(transitionArea.reduced(5));

    area.removeFromTop(10); // Padding

    // Preset controls
    auto presetArea = area.removeFromTop(30);
    presetSelector.setBounds(presetArea.removeFromLeft(200).reduced(5));
    randomizeButton.setBounds(presetArea.removeFromLeft(150).reduced(5));
}

void FlarkVizPluginEditor::timerCallback()
{
    // Trigger repaint to update visualization
    repaint();
}
