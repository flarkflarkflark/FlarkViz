#pragma once

#include <JuceHeader.h>
#include <string>
#include <vector>
#include <map>

/**
 * @struct MilkDropPreset
 * @brief Data structure for MilkDrop .milk preset files
 *
 * Represents a complete MilkDrop preset with all parameters, equations, and shader code.
 */
struct MilkDropPreset
{
    // ========== Preset Metadata ==========
    std::string name;
    std::string author;

    // ========== Basic Parameters ==========
    float fRating = 3.0f;
    float fGammaAdj = 1.0f;
    float fDecay = 0.98f;
    float fVideoEchoZoom = 1.0f;
    float fVideoEchoAlpha = 0.5f;
    int nVideoEchoOrientation = 0;
    int nWaveMode = 0;
    bool bAdditiveWaves = false;
    bool bWaveDots = false;
    bool bWaveThick = false;
    bool bModWaveAlphaByVolume = false;
    bool bMaximizeWaveColor = false;
    bool bTexWrap = false;
    bool bDarkenCenter = false;
    bool bRedBlueStereo = false;
    bool bBrighten = false;
    bool bDarken = false;
    bool bSolarize = false;
    bool bInvert = false;

    float fWaveAlpha = 0.8f;
    float fWaveScale = 1.0f;
    float fWaveSmoothing = 0.75f;
    float fWaveParam = 0.0f;
    float fModWaveAlphaStart = 0.75f;
    float fModWaveAlphaEnd = 0.95f;
    float fWarpAnimSpeed = 1.0f;
    float fWarpScale = 1.0f;
    float fZoomExponent = 1.0f;
    float fShader = 0.0f;

    // ========== Motion Vectors ==========
    float fRotCX = 0.5f;
    float fRotCY = 0.5f;
    float fRot = 0.0f;
    float fXPush = 0.0f;
    float fYPush = 0.0f;
    float fWarpAmount = 1.0f;
    float fStretchX = 1.0f;
    float fStretchY = 1.0f;

    // ========== Colors ==========
    float fOuterBorderSize = 0.01f;
    float fOuterBorderR = 0.0f;
    float fOuterBorderG = 0.0f;
    float fOuterBorderB = 0.0f;
    float fOuterBorderA = 0.0f;

    float fInnerBorderSize = 0.01f;
    float fInnerBorderR = 0.25f;
    float fInnerBorderG = 0.25f;
    float fInnerBorderB = 0.25f;
    float fInnerBorderA = 0.0f;

    float fMvX = 12.0f;
    float fMvY = 9.0f;
    float fMvDX = 0.0f;
    float fMvDY = 0.0f;
    float fMvL = 1.0f;
    float fMvR = 1.0f;
    float fMvG = 1.0f;
    float fMvB = 1.0f;
    float fMvA = 1.0f;

    // ========== Wave Colors ==========
    float wave_r = 1.0f;
    float wave_g = 1.0f;
    float wave_b = 1.0f;
    float wave_x = 0.5f;
    float wave_y = 0.5f;
    float wave_mystery = 0.0f;

    // ========== Code Sections ==========
    std::string perFrameInitCode;
    std::string perFrameCode;
    std::string perPixelCode;

    // Warp shader (optional)
    std::string warpShaderCode;

    // Composite shader (optional)
    std::string compShaderCode;

    // ========== Wave/Shape Code ==========
    struct WaveOrShape
    {
        bool enabled = false;
        int sides = 4;
        bool thick = false;
        bool additive = false;
        bool dots = false;
        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        float a = 1.0f;
        float x = 0.5f;
        float y = 0.5f;
        float rad = 0.3f;
        float ang = 0.0f;
        float tex_ang = 0.0f;
        float tex_zoom = 1.0f;
        std::string initCode;
        std::string perFrameCode;
        std::string perPointCode;
    };

    std::vector<WaveOrShape> waves;  // Up to 4 waves
    std::vector<WaveOrShape> shapes; // Up to 4 shapes

    // ========== Custom Variables ==========
    std::map<std::string, float> variables;

    // ========== Helper Methods ==========
    bool loadFromFile(const juce::File& file);
    bool loadFromString(const juce::String& content);
    void reset();
};
