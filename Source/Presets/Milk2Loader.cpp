#include "Milk2Loader.h"
#include <sstream>

Milk2Loader::Milk2Loader() {}
Milk2Loader::~Milk2Loader() {}

Milk2Loader::DoublePreset Milk2Loader::loadFromFile(const juce::File& file)
{
    if (!file.existsAsFile())
        return DoublePreset();

    juce::String content = file.loadFileAsString();
    return loadFromString(content);
}

Milk2Loader::DoublePreset Milk2Loader::loadFromString(const juce::String& content)
{
    DoublePreset result;
    juce::StringArray lines = juce::StringArray::fromLines(content);

    // Find the separator between preset A and preset B
    // .milk2 files use [preset_a] and [preset_b] sections
    int presetAStart = -1;
    int presetAEnd = -1;
    int presetBStart = -1;
    int presetBEnd = -1;
    int metaStart = -1;

    for (int i = 0; i < lines.size(); ++i)
    {
        juce::String line = lines[i].trim();

        if (line.contains("[preset_a]") || line.contains("[preset00]"))
        {
            if (presetAStart == -1)
                presetAStart = i;
        }
        else if (line.contains("[preset_b]") || (line.contains("[preset00]") && presetAStart != -1))
        {
            presetAEnd = i;
            presetBStart = i;
        }
        else if (line.contains("[milk2_meta]"))
        {
            if (presetBStart != -1 && presetBEnd == -1)
                presetBEnd = i;
            metaStart = i;
        }
    }

    if (presetBEnd == -1)
        presetBEnd = lines.size();

    // Parse metadata section if exists
    if (metaStart != -1)
    {
        for (int i = metaStart + 1; i < lines.size(); ++i)
        {
            juce::String line = lines[i].trim();
            if (line.isEmpty() || line.startsWith("//"))
                continue;

            if (line.contains("="))
            {
                juce::String key = line.upToFirstOccurrenceOf("=", false, false).trim();
                juce::String value = line.fromFirstOccurrenceOf("=", false, false).trim();

                if (key == "blend_factor")
                    result.blendFactor = value.getFloatValue();
                else if (key == "transition_type")
                    result.transitionType = value.getIntValue();
                else if (key == "transition_duration")
                    result.transitionDuration = value.getFloatValue();
            }
        }
    }

    // Extract preset A content
    if (presetAStart != -1 && presetAEnd != -1)
    {
        juce::String presetAContent;
        for (int i = presetAStart; i < presetAEnd; ++i)
            presetAContent += lines[i] + "\n";
        result.presetA.loadFromString(presetAContent);
    }

    // Extract preset B content
    if (presetBStart != -1 && presetBEnd != -1)
    {
        juce::String presetBContent;
        for (int i = presetBStart; i < presetBEnd; ++i)
            presetBContent += lines[i] + "\n";
        result.presetB.loadFromString(presetBContent);
    }

    return result;
}

bool Milk2Loader::saveToFile(const DoublePreset& doublePreset, const juce::File& file)
{
    std::stringstream ss;

    // Write metadata
    ss << "[milk2_meta]\n";
    ss << "version=1.0\n";
    ss << "blend_factor=" << doublePreset.blendFactor << "\n";
    ss << "transition_type=" << doublePreset.transitionType << "\n";
    ss << "transition_duration=" << doublePreset.transitionDuration << "\n";
    ss << "\n";

    // Write preset A
    ss << "[preset_a]\n";
    ss << "[preset00]\n";
    ss << "fRating=" << doublePreset.presetA.fRating << "\n";
    ss << "fGammaAdj=" << doublePreset.presetA.fGammaAdj << "\n";
    ss << "fDecay=" << doublePreset.presetA.fDecay << "\n";
    // ... (add more preset A parameters)
    ss << "\n";

    // Write preset B
    ss << "[preset_b]\n";
    ss << "[preset00]\n";
    ss << "fRating=" << doublePreset.presetB.fRating << "\n";
    ss << "fGammaAdj=" << doublePreset.presetB.fGammaAdj << "\n";
    ss << "fDecay=" << doublePreset.presetB.fDecay << "\n";
    // ... (add more preset B parameters)
    ss << "\n";

    juce::String content(ss.str());
    return file.replaceWithText(content);
}

bool Milk2Loader::parsePresetSection(const juce::StringArray& lines,
                                     int startIdx,
                                     int endIdx,
                                     MilkDropPreset& preset)
{
    juce::String sectionContent;
    for (int i = startIdx; i < endIdx && i < lines.size(); ++i)
        sectionContent += lines[i] + "\n";

    return preset.loadFromString(sectionContent);
}
