#include "PresetLoader.h"

PresetLoader::PresetLoader()
{
}

PresetLoader::~PresetLoader()
{
}

std::unique_ptr<MilkDropPreset> PresetLoader::loadPreset(const juce::File& file)
{
    if (!file.existsAsFile())
    {
        lastError = "File does not exist: " + file.getFullPathName();
        return nullptr;
    }

    if (file.getFileExtension() != ".milk")
    {
        lastError = "Not a .milk file: " + file.getFileName();
        return nullptr;
    }

    auto preset = std::make_unique<MilkDropPreset>();

    if (!preset->loadFromFile(file))
    {
        lastError = "Failed to parse preset file: " + file.getFileName();
        return nullptr;
    }

    lastError = juce::String();
    return preset;
}

std::unique_ptr<MilkDropPreset> PresetLoader::loadPresetFromString(const juce::String& content)
{
    auto preset = std::make_unique<MilkDropPreset>();

    if (!preset->loadFromString(content))
    {
        lastError = "Failed to parse preset from string";
        return nullptr;
    }

    lastError = juce::String();
    return preset;
}
