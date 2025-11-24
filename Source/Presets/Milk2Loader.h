#pragma once

#include "Preset.h"
#include <JuceHeader.h>
#include <string>

/**
 * @class Milk2Loader
 * @brief Loader for .milk2 double preset format
 *
 * The .milk2 format contains two complete MilkDrop presets that blend together,
 * allowing for more complex visual transitions and layered effects.
 */
class Milk2Loader
{
public:
    Milk2Loader();
    ~Milk2Loader();

    /**
     * @struct DoublePreset
     * @brief Container for two presets with blend factor
     */
    struct DoublePreset
    {
        MilkDropPreset presetA;
        MilkDropPreset presetB;
        float blendFactor = 0.5f;  // 0.0 = all A, 1.0 = all B
        int transitionType = 0;     // Transition/blend pattern index
        float transitionDuration = 2.0f;  // Duration in seconds
    };

    /**
     * @brief Load a .milk2 file containing two presets
     * @param file Path to .milk2 file
     * @return DoublePreset structure with both presets loaded
     */
    static DoublePreset loadFromFile(const juce::File& file);

    /**
     * @brief Load from .milk2 string content
     * @param content The .milk2 file content
     * @return DoublePreset structure
     */
    static DoublePreset loadFromString(const juce::String& content);

    /**
     * @brief Save a double preset to .milk2 format
     * @param doublePreset The double preset to save
     * @param file Output file path
     * @return true if save succeeded
     */
    static bool saveToFile(const DoublePreset& doublePreset, const juce::File& file);

private:
    static bool parsePresetSection(const juce::StringArray& lines,
                                   int startIdx,
                                   int endIdx,
                                   MilkDropPreset& preset);
};
