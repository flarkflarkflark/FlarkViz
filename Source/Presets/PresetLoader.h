#pragma once

#include <JuceHeader.h>
#include "Preset.h"
#include <memory>

/**
 * @class PresetLoader
 * @brief Loads and parses MilkDrop .milk preset files
 *
 * Handles loading of MilkDrop presets from disk and parsing into
 * the MilkDropPreset structure.
 */
class PresetLoader
{
public:
    PresetLoader();
    ~PresetLoader();

    /**
     * @brief Load a preset from a .milk file
     * @param file The .milk file to load
     * @return Unique pointer to loaded preset, or nullptr on failure
     */
    std::unique_ptr<MilkDropPreset> loadPreset(const juce::File& file);

    /**
     * @brief Load a preset from a string
     * @param content The preset file content as a string
     * @return Unique pointer to loaded preset, or nullptr on failure
     */
    std::unique_ptr<MilkDropPreset> loadPresetFromString(const juce::String& content);

    /**
     * @brief Get the last error message
     * @return Error message from last failed load operation
     */
    juce::String getLastError() const { return lastError; }

private:
    juce::String lastError;
};
