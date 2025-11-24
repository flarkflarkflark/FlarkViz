#include "FlarkVizPlugin.h"
#include "FlarkVizPluginEditor.h"

FlarkVizPlugin::FlarkVizPlugin()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , parameters(*this, nullptr, juce::Identifier("FlarkVizParameters"), createParameterLayout())
{
    // Get parameter pointers
    brightnessParam = parameters.getRawParameterValue("brightness");
    contrastParam = parameters.getRawParameterValue("contrast");
    saturationParam = parameters.getRawParameterValue("saturation");
    transitionTimeParam = parameters.getRawParameterValue("transitionTime");
    autoChangeParam = parameters.getRawParameterValue("autoChange");
}

FlarkVizPlugin::~FlarkVizPlugin()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout FlarkVizPlugin::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "brightness", "Brightness", 0.0f, 2.0f, 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "contrast", "Contrast", 0.0f, 2.0f, 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "saturation", "Saturation", 0.0f, 2.0f, 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "transitionTime", "Transition Time", 0.5f, 10.0f, 2.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "autoChange", "Auto Change Time", 0.0f, 60.0f, 15.0f));

    return {params.begin(), params.end()};
}

const juce::String FlarkVizPlugin::getName() const
{
    return JucePlugin_Name;
}

bool FlarkVizPlugin::acceptsMidi() const
{
    return false;
}

bool FlarkVizPlugin::producesMidi() const
{
    return false;
}

bool FlarkVizPlugin::isMidiEffect() const
{
    return false;
}

double FlarkVizPlugin::getTailLengthSeconds() const
{
    return 0.0;
}

int FlarkVizPlugin::getNumPrograms()
{
    return 1;
}

int FlarkVizPlugin::getCurrentProgram()
{
    return 0;
}

void FlarkVizPlugin::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String FlarkVizPlugin::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void FlarkVizPlugin::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void FlarkVizPlugin::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    audioAnalyzer.initialize(sampleRate, samplesPerBlock);
}

void FlarkVizPlugin::releaseResources()
{
}

bool FlarkVizPlugin::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void FlarkVizPlugin::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;

    // Analyze audio
    audioAnalyzer.processBlock(buffer);

    // Pass through audio unchanged
    // (This is a visualization plugin, not an audio effect)
}

bool FlarkVizPlugin::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* FlarkVizPlugin::createEditor()
{
    return new FlarkVizPluginEditor(*this);
}

void FlarkVizPlugin::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FlarkVizPlugin::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

// This creates the plugin instance
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlarkVizPlugin();
}
