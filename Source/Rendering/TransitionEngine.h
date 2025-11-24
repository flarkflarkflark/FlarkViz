#pragma once

#include <JuceHeader.h>
#include <functional>
#include <vector>

/**
 * @class TransitionEngine
 * @brief Manages visual transitions between presets with 30+ blend patterns
 *
 * Provides smooth blending and transition effects when switching between
 * MilkDrop presets, including various wipe, fade, and geometric patterns.
 */
class TransitionEngine
{
public:
    TransitionEngine();
    ~TransitionEngine();

    /**
     * @enum TransitionType
     * @brief Available transition patterns (30+ types)
     */
    enum class TransitionType
    {
        // Basic transitions (0-9)
        None = 0,
        Crossfade = 1,
        FadeToBlack = 2,
        FadeToWhite = 3,

        // Wipe transitions (10-19)
        WipeLeft = 10,
        WipeRight = 11,
        WipeUp = 12,
        WipeDown = 13,
        WipeDiagonalTL = 14,    // Top-left to bottom-right
        WipeDiagonalTR = 15,    // Top-right to bottom-left

        // Circular/Radial transitions (20-29)
        CircularExpand = 20,
        CircularContract = 21,
        RadialWipe = 22,
        SpiralOut = 23,
        SpiralIn = 24,

        // Grid transitions (30-39)
        CheckerboardFade = 30,
        GridSlide = 31,
        PixelDissolve = 32,
        BlockDissolve = 33,
        RandomBlocks = 34,

        // Wave transitions (40-49)
        WaveHorizontal = 40,
        WaveVertical = 41,
        WaveDiagonal = 42,
        Ripple = 43,

        // Geometric transitions (50-59)
        DiamondWipe = 50,
        HeartWipe = 51,
        StarWipe = 52,
        ClockWipe = 53,
        IrisIn = 54,
        IrisOut = 55,

        // Special effects (60-69)
        Glitch = 60,
        MotionBlur = 61,
        ZoomIn = 62,
        ZoomOut = 63,
        Rotate = 64,
        Pixelate = 65
    };

    /**
     * @brief Start a transition from one preset to another
     * @param type The transition pattern to use
     * @param duration Transition duration in seconds
     */
    void startTransition(TransitionType type, float duration);

    /**
     * @brief Update the transition state
     * @param deltaTime Time elapsed since last update (seconds)
     */
    void update(float deltaTime);

    /**
     * @brief Get current transition progress (0.0 to 1.0)
     */
    float getProgress() const { return progress; }

    /**
     * @brief Check if a transition is currently active
     */
    bool isActive() const { return active; }

    /**
     * @brief Get the current transition type
     */
    TransitionType getCurrentType() const { return currentType; }

    /**
     * @brief Compute blend factor at screen position (for spatial transitions)
     * @param x Normalized x coordinate (0.0 to 1.0)
     * @param y Normalized y coordinate (0.0 to 1.0)
     * @return Blend factor from 0.0 (preset A) to 1.0 (preset B)
     */
    float getBlendFactorAt(float x, float y) const;

    /**
     * @brief Stop current transition immediately
     */
    void stop();

    /**
     * @brief Get list of all available transition names
     */
    static std::vector<juce::String> getTransitionNames();

    /**
     * @brief Get transition type from name
     */
    static TransitionType getTransitionTypeFromName(const juce::String& name);

private:
    bool active;
    TransitionType currentType;
    float progress;          // 0.0 = start, 1.0 = end
    float duration;
    float elapsed;

    // Easing functions
    float easeInOut(float t) const;
    float easeIn(float t) const;
    float easeOut(float t) const;

    // Spatial blend calculators for each pattern
    float calculateCrossfade(float x, float y) const;
    float calculateWipeLeft(float x, float y) const;
    float calculateWipeRight(float x, float y) const;
    float calculateWipeUp(float x, float y) const;
    float calculateWipeDown(float x, float y) const;
    float calculateCircularExpand(float x, float y) const;
    float calculateCircularContract(float x, float y) const;
    float calculateRadialWipe(float x, float y) const;
    float calculateCheckerboard(float x, float y) const;
    float calculatePixelDissolve(float x, float y) const;
    float calculateIrisIn(float x, float y) const;
    float calculateIrisOut(float x, float y) const;
    float calculateSpiral(float x, float y) const;
    float calculateDiamondWipe(float x, float y) const;
    float calculateStarWipe(float x, float y) const;
    float calculateClockWipe(float x, float y) const;

    // Random seed for randomized patterns
    unsigned int randomSeed;
    float pseudoRandom(float x, float y) const;
};
