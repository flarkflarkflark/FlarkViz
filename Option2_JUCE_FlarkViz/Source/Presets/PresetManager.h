#pragma once

#include <JuceHeader.h>

/**
 * @class PresetManager
 * @brief Manages preset library and selection
 * 
 * Handles loading, caching, and organizing MilkDrop presets
 */
class PresetManager
{
public:
    PresetManager();
    ~PresetManager();
    
    //==========================================================================
    // Library management
    void scanPresetsFolder (const juce::File& folder);
    int getPresetCount() const { return presets.size(); }
    
    //==========================================================================
    // Preset selection
    void* getPreset (int index);
    void loadRandomPreset();
    void loadNextPreset();
    void loadPreviousPreset();
    
    //==========================================================================
    // Mash-up / mixing
    void mashupRandom();
    
private:
    std::vector<juce::File> presets;
    int currentPresetIndex = 0;
    std::vector<int> history;  // For undo functionality
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetManager)
};
