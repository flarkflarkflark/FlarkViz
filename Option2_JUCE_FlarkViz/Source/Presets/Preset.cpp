#include "Preset.h"

void MilkDropPreset::reset()
{
    name.clear();
    author.clear();
    variables.clear();
    waves.clear();
    shapes.clear();
    perFrameInitCode.clear();
    perFrameCode.clear();
    perPixelCode.clear();
    warpShaderCode.clear();
    compShaderCode.clear();

    // Reset to defaults
    fRating = 3.0f;
    fGammaAdj = 1.0f;
    fDecay = 0.98f;
    fVideoEchoZoom = 1.0f;
    fVideoEchoAlpha = 0.5f;
    nVideoEchoOrientation = 0;
    nWaveMode = 0;
    bAdditiveWaves = false;
    bWaveDots = false;
    bWaveThick = false;
    bModWaveAlphaByVolume = false;
    bMaximizeWaveColor = false;
    bTexWrap = false;
    bDarkenCenter = false;
    bRedBlueStereo = false;
    bBrighten = false;
    bDarken = false;
    bSolarize = false;
    bInvert = false;
    fWaveAlpha = 0.8f;
    fWaveScale = 1.0f;
    fWaveSmoothing = 0.75f;
}

bool MilkDropPreset::loadFromFile(const juce::File& file)
{
    if (!file.existsAsFile())
        return false;

    juce::String content = file.loadFileAsString();
    return loadFromString(content);
}

bool MilkDropPreset::loadFromString(const juce::String& content)
{
    reset();

    juce::StringArray lines = juce::StringArray::fromLines(content);

    std::string currentSection;
    std::string multilineBuffer;
    bool inMultiline = false;
    int waveIndex = -1;
    int shapeIndex = -1;

    for (const auto& rawLine : lines)
    {
        juce::String line = rawLine.trim();

        // Skip empty lines and comments
        if (line.isEmpty() || line.startsWith("//"))
            continue;

        // Detect section headers [preset00], [wave_0], etc.
        if (line.startsWith("["))
        {
            if (inMultiline)
            {
                inMultiline = false;
                // Store accumulated multiline code
                if (currentSection == "per_frame_init_code")
                    perFrameInitCode = multilineBuffer;
                else if (currentSection == "per_frame_code")
                    perFrameCode = multilineBuffer;
                else if (currentSection == "per_pixel_code")
                    perPixelCode = multilineBuffer;
                else if (currentSection == "warp_shader")
                    warpShaderCode = multilineBuffer;
                else if (currentSection == "comp_shader")
                    compShaderCode = multilineBuffer;

                multilineBuffer.clear();
            }

            currentSection = line.fromFirstOccurrenceOf("[", false, false)
                                 .upToFirstOccurrenceOf("]", false, false)
                                 .toStdString();

            // Parse wave/shape indices
            if (currentSection.find("wave_") == 0)
            {
                waveIndex = currentSection[5] - '0';
                while (static_cast<int>(waves.size()) <= waveIndex)
                    waves.push_back(WaveOrShape());
                waves[waveIndex].enabled = true;
            }
            else if (currentSection.find("shape_") == 0)
            {
                shapeIndex = currentSection[6] - '0';
                while (static_cast<int>(shapes.size()) <= shapeIndex)
                    shapes.push_back(WaveOrShape());
                shapes[shapeIndex].enabled = true;
            }

            continue;
        }

        // Parse key=value pairs
        if (line.contains("="))
        {
            juce::String key = line.upToFirstOccurrenceOf("=", false, true).trim();
            juce::String value = line.fromFirstOccurrenceOf("=", false, true).trim();

            // Metadata
            if (key == "name" || key == "MILKDROP_PRESET_VERSION")
                name = value.toStdString();
            else if (key == "author")
                author = value.toStdString();

            // Basic parameters
            else if (key == "fRating") fRating = value.getFloatValue();
            else if (key == "fGammaAdj") fGammaAdj = value.getFloatValue();
            else if (key == "fDecay") fDecay = value.getFloatValue();
            else if (key == "fVideoEchoZoom") fVideoEchoZoom = value.getFloatValue();
            else if (key == "fVideoEchoAlpha") fVideoEchoAlpha = value.getFloatValue();
            else if (key == "nVideoEchoOrientation") nVideoEchoOrientation = value.getIntValue();
            else if (key == "nWaveMode") nWaveMode = value.getIntValue();
            else if (key == "bAdditiveWaves") bAdditiveWaves = (value.getIntValue() != 0);
            else if (key == "bWaveDots") bWaveDots = (value.getIntValue() != 0);
            else if (key == "bWaveThick") bWaveThick = (value.getIntValue() != 0);
            else if (key == "bModWaveAlphaByVolume") bModWaveAlphaByVolume = (value.getIntValue() != 0);
            else if (key == "bMaximizeWaveColor") bMaximizeWaveColor = (value.getIntValue() != 0);
            else if (key == "bTexWrap") bTexWrap = (value.getIntValue() != 0);
            else if (key == "bDarkenCenter") bDarkenCenter = (value.getIntValue() != 0);
            else if (key == "bRedBlueStereo") bRedBlueStereo = (value.getIntValue() != 0);
            else if (key == "bBrighten") bBrighten = (value.getIntValue() != 0);
            else if (key == "bDarken") bDarken = (value.getIntValue() != 0);
            else if (key == "bSolarize") bSolarize = (value.getIntValue() != 0);
            else if (key == "bInvert") bInvert = (value.getIntValue() != 0);

            // Wave parameters
            else if (key == "fWaveAlpha") fWaveAlpha = value.getFloatValue();
            else if (key == "fWaveScale") fWaveScale = value.getFloatValue();
            else if (key == "fWaveSmoothing") fWaveSmoothing = value.getFloatValue();
            else if (key == "fWaveParam") fWaveParam = value.getFloatValue();
            else if (key == "fModWaveAlphaStart") fModWaveAlphaStart = value.getFloatValue();
            else if (key == "fModWaveAlphaEnd") fModWaveAlphaEnd = value.getFloatValue();
            else if (key == "fWarpAnimSpeed") fWarpAnimSpeed = value.getFloatValue();
            else if (key == "fWarpScale") fWarpScale = value.getFloatValue();
            else if (key == "fZoomExponent") fZoomExponent = value.getFloatValue();
            else if (key == "fShader") fShader = value.getFloatValue();

            // Motion vectors
            else if (key == "fRotCX") fRotCX = value.getFloatValue();
            else if (key == "fRotCY") fRotCY = value.getFloatValue();
            else if (key == "fRot") fRot = value.getFloatValue();
            else if (key == "fXPush") fXPush = value.getFloatValue();
            else if (key == "fYPush") fYPush = value.getFloatValue();
            else if (key == "fWarpAmount") fWarpAmount = value.getFloatValue();
            else if (key == "fStretchX") fStretchX = value.getFloatValue();
            else if (key == "fStretchY") fStretchY = value.getFloatValue();

            // Wave colors
            else if (key == "wave_r") wave_r = value.getFloatValue();
            else if (key == "wave_g") wave_g = value.getFloatValue();
            else if (key == "wave_b") wave_b = value.getFloatValue();

            // Wave/Shape specific parameters
            else if (waveIndex >= 0 && waveIndex < static_cast<int>(waves.size()))
            {
                if (key == "enabled") waves[waveIndex].enabled = (value.getIntValue() != 0);
                else if (key == "r") waves[waveIndex].r = value.getFloatValue();
                else if (key == "g") waves[waveIndex].g = value.getFloatValue();
                else if (key == "b") waves[waveIndex].b = value.getFloatValue();
                else if (key == "a") waves[waveIndex].a = value.getFloatValue();
            }
            else if (shapeIndex >= 0 && shapeIndex < static_cast<int>(shapes.size()))
            {
                if (key == "enabled") shapes[shapeIndex].enabled = (value.getIntValue() != 0);
                else if (key == "sides") shapes[shapeIndex].sides = value.getIntValue();
                else if (key == "thick") shapes[shapeIndex].thick = (value.getIntValue() != 0);
                else if (key == "additive") shapes[shapeIndex].additive = (value.getIntValue() != 0);
                else if (key == "r") shapes[shapeIndex].r = value.getFloatValue();
                else if (key == "g") shapes[shapeIndex].g = value.getFloatValue();
                else if (key == "b") shapes[shapeIndex].b = value.getFloatValue();
                else if (key == "a") shapes[shapeIndex].a = value.getFloatValue();
                else if (key == "rad") shapes[shapeIndex].rad = value.getFloatValue();
            }

            continue;
        }

        // Multiline code sections
        if (currentSection == "per_frame_init_1" ||
            currentSection == "per_frame_1" ||
            currentSection == "per_pixel_1" ||
            currentSection == "warp_1" ||
            currentSection == "comp_1")
        {
            if (!multilineBuffer.empty())
                multilineBuffer += "\n";
            multilineBuffer += line.toStdString();
            inMultiline = true;
        }
    }

    // Store any remaining multiline code
    if (inMultiline)
    {
        if (currentSection == "per_frame_init_1")
            perFrameInitCode = multilineBuffer;
        else if (currentSection == "per_frame_1")
            perFrameCode = multilineBuffer;
        else if (currentSection == "per_pixel_1")
            perPixelCode = multilineBuffer;
        else if (currentSection == "warp_1")
            warpShaderCode = multilineBuffer;
        else if (currentSection == "comp_1")
            compShaderCode = multilineBuffer;
    }

    return true;
}
