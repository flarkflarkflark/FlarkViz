#include "PresetManager.h"

PresetManager::PresetManager()
{
    // Scan for presets in default location
    auto presetsFolder = juce::File::getSpecialLocation (juce::File::userDocumentsDirectory)
                           .getChildFile ("FlarkViz/presets");
    
    if (!presetsFolder.exists())
    {
        presetsFolder.createDirectory();
        DBG ("FlarkViz: Created presets folder at " << presetsFolder.getFullPathName());
    }
    
    scanPresetsFolder (presetsFolder);
}

PresetManager::~PresetManager()
{
}

void PresetManager::scanPresetsFolder (const juce::File& folder)
{
    presets.clear();
    
    if (!folder.exists())
    {
        DBG ("FlarkViz: Presets folder does not exist: " << folder.getFullPathName());
        return;
    }
    
    // Find all .milk and .milk2 files
    auto milkFiles = folder.findChildFiles (juce::File::findFiles, 
                                           true, 
                                           "*.milk");
    
    auto milk2Files = folder.findChildFiles (juce::File::findFiles, 
                                            true, 
                                            "*.milk2");
    
    presets.insert (presets.end(), milkFiles.begin(), milkFiles.end());
    presets.insert (presets.end(), milk2Files.begin(), milk2Files.end());
    
    DBG ("FlarkViz: Found " << presets.size() << " presets");
}

void* PresetManager::getPreset (int index)
{
    if (index >= 0 && index < (int)presets.size())
    {
        currentPresetIndex = index;
        // TODO: Actually load and parse preset
        return nullptr;
    }
    return nullptr;
}

void PresetManager::loadRandomPreset()
{
    if (presets.empty())
    {
        DBG ("FlarkViz: No presets available");
        return;
    }
    
    // Save current index to history
    history.push_back (currentPresetIndex);
    if (history.size() > 50)
        history.erase (history.begin());
    
    // Select random preset
    currentPresetIndex = juce::Random::getSystemRandom().nextInt (presets.size());
    
    DBG ("FlarkViz: Loading random preset: " << presets[currentPresetIndex].getFileName());
}

void PresetManager::loadNextPreset()
{
    if (presets.empty())
        return;
    
    currentPresetIndex = (currentPresetIndex + 1) % presets.size();
    DBG ("FlarkViz: Loading next preset: " << presets[currentPresetIndex].getFileName());
}

void PresetManager::loadPreviousPreset()
{
    if (!history.empty())
    {
        currentPresetIndex = history.back();
        history.pop_back();
        DBG ("FlarkViz: Loading previous preset from history");
    }
}

void PresetManager::mashupRandom()
{
    // TODO: Implement mash-up functionality
    DBG ("FlarkViz: Mash-up activated");
}
