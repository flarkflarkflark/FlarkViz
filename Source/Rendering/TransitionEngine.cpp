#include "TransitionEngine.h"
#include <cmath>

TransitionEngine::TransitionEngine()
    : active(false)
    , currentType(TransitionType::Crossfade)
    , progress(0.0f)
    , duration(2.0f)
    , elapsed(0.0f)
    , randomSeed(12345)
{
}

TransitionEngine::~TransitionEngine()
{
}

void TransitionEngine::startTransition(TransitionType type, float dur)
{
    currentType = type;
    duration = dur;
    progress = 0.0f;
    elapsed = 0.0f;
    active = true;
    randomSeed = static_cast<unsigned int>(juce::Time::currentTimeMillis());
}

void TransitionEngine::update(float deltaTime)
{
    if (!active)
        return;

    elapsed += deltaTime;
    progress = juce::jmin(1.0f, elapsed / duration);

    if (progress >= 1.0f)
        active = false;
}

void TransitionEngine::stop()
{
    active = false;
    progress = 1.0f;
}

float TransitionEngine::getBlendFactorAt(float x, float y) const
{
    switch (currentType)
    {
        case TransitionType::None:
            return progress >= 0.5f ? 1.0f : 0.0f;

        case TransitionType::Crossfade:
            return calculateCrossfade(x, y);

        case TransitionType::WipeLeft:
            return calculateWipeLeft(x, y);

        case TransitionType::WipeRight:
            return calculateWipeRight(x, y);

        case TransitionType::WipeUp:
            return calculateWipeUp(x, y);

        case TransitionType::WipeDown:
            return calculateWipeDown(x, y);

        case TransitionType::CircularExpand:
            return calculateCircularExpand(x, y);

        case TransitionType::CircularContract:
            return calculateCircularContract(x, y);

        case TransitionType::RadialWipe:
            return calculateRadialWipe(x, y);

        case TransitionType::SpiralOut:
        case TransitionType::SpiralIn:
            return calculateSpiral(x, y);

        case TransitionType::CheckerboardFade:
            return calculateCheckerboard(x, y);

        case TransitionType::PixelDissolve:
        case TransitionType::RandomBlocks:
            return calculatePixelDissolve(x, y);

        case TransitionType::IrisIn:
            return calculateIrisIn(x, y);

        case TransitionType::IrisOut:
            return calculateIrisOut(x, y);

        case TransitionType::DiamondWipe:
            return calculateDiamondWipe(x, y);

        case TransitionType::StarWipe:
            return calculateStarWipe(x, y);

        case TransitionType::ClockWipe:
            return calculateClockWipe(x, y);

        default:
            return easeInOut(progress);
    }
}

// Easing functions
float TransitionEngine::easeInOut(float t) const
{
    return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

float TransitionEngine::easeIn(float t) const
{
    return t * t;
}

float TransitionEngine::easeOut(float t) const
{
    return 1.0f - (1.0f - t) * (1.0f - t);
}

// Blend calculators
float TransitionEngine::calculateCrossfade(float x, float y) const
{
    return easeInOut(progress);
}

float TransitionEngine::calculateWipeLeft(float x, float y) const
{
    float threshold = easeInOut(progress);
    return x < threshold ? 1.0f : 0.0f;
}

float TransitionEngine::calculateWipeRight(float x, float y) const
{
    float threshold = easeInOut(progress);
    return x > (1.0f - threshold) ? 1.0f : 0.0f;
}

float TransitionEngine::calculateWipeUp(float x, float y) const
{
    float threshold = easeInOut(progress);
    return y < threshold ? 1.0f : 0.0f;
}

float TransitionEngine::calculateWipeDown(float x, float y) const
{
    float threshold = easeInOut(progress);
    return y > (1.0f - threshold) ? 1.0f : 0.0f;
}

float TransitionEngine::calculateCircularExpand(float x, float y) const
{
    float dx = x - 0.5f;
    float dy = y - 0.5f;
    float dist = std::sqrt(dx * dx + dy * dy);
    float maxDist = std::sqrt(0.5f * 0.5f + 0.5f * 0.5f);
    float normalizedDist = dist / maxDist;

    float threshold = easeInOut(progress);
    return normalizedDist < threshold ? 1.0f : 0.0f;
}

float TransitionEngine::calculateCircularContract(float x, float y) const
{
    float dx = x - 0.5f;
    float dy = y - 0.5f;
    float dist = std::sqrt(dx * dx + dy * dy);
    float maxDist = std::sqrt(0.5f * 0.5f + 0.5f * 0.5f);
    float normalizedDist = dist / maxDist;

    float threshold = easeInOut(progress);
    return normalizedDist > (1.0f - threshold) ? 1.0f : 0.0f;
}

float TransitionEngine::calculateRadialWipe(float x, float y) const
{
    float dx = x - 0.5f;
    float dy = y - 0.5f;
    float angle = std::atan2(dy, dx);
    float normalizedAngle = (angle + 3.14159265f) / (2.0f * 3.14159265f);

    float threshold = easeInOut(progress);
    return normalizedAngle < threshold ? 1.0f : 0.0f;
}

float TransitionEngine::calculateCheckerboard(float x, float y) const
{
    const int gridSize = 16;
    int ix = static_cast<int>(x * gridSize);
    int iy = static_cast<int>(y * gridSize);
    bool isEven = ((ix + iy) % 2) == 0;

    float threshold = easeInOut(progress);
    float localProgress = isEven ? threshold : (threshold * 0.7f);

    return localProgress > 0.5f ? 1.0f : 0.0f;
}

float TransitionEngine::calculatePixelDissolve(float x, float y) const
{
    float random = pseudoRandom(x, y);
    float threshold = easeInOut(progress);
    return random < threshold ? 1.0f : 0.0f;
}

float TransitionEngine::calculateIrisIn(float x, float y) const
{
    return calculateCircularExpand(x, y);
}

float TransitionEngine::calculateIrisOut(float x, float y) const
{
    return calculateCircularContract(x, y);
}

float TransitionEngine::calculateSpiral(float x, float y) const
{
    float dx = x - 0.5f;
    float dy = y - 0.5f;
    float angle = std::atan2(dy, dx);
    float dist = std::sqrt(dx * dx + dy * dy);

    float spiralValue = (angle / (2.0f * 3.14159265f)) + dist * 2.0f;
    spiralValue = spiralValue - std::floor(spiralValue);

    float threshold = easeInOut(progress);
    return spiralValue < threshold ? 1.0f : 0.0f;
}

float TransitionEngine::calculateDiamondWipe(float x, float y) const
{
    float dx = std::abs(x - 0.5f);
    float dy = std::abs(y - 0.5f);
    float diamondDist = dx + dy;

    float threshold = easeInOut(progress);
    return diamondDist < threshold ? 1.0f : 0.0f;
}

float TransitionEngine::calculateStarWipe(float x, float y) const
{
    float dx = x - 0.5f;
    float dy = y - 0.5f;
    float angle = std::atan2(dy, dx);
    float dist = std::sqrt(dx * dx + dy * dy);

    const int points = 5;
    float starPattern = std::sin(angle * points) * 0.3f + 0.7f;
    float normalizedDist = dist / (starPattern * 0.7f);

    float threshold = easeInOut(progress);
    return normalizedDist < threshold ? 1.0f : 0.0f;
}

float TransitionEngine::calculateClockWipe(float x, float y) const
{
    float dx = x - 0.5f;
    float dy = y - 0.5f;
    float angle = std::atan2(dy, dx);

    // Rotate starting point to top (12 o'clock)
    angle += 3.14159265f / 2.0f;
    if (angle < 0.0f) angle += 2.0f * 3.14159265f;

    float normalizedAngle = angle / (2.0f * 3.14159265f);
    float threshold = easeInOut(progress);

    return normalizedAngle < threshold ? 1.0f : 0.0f;
}

float TransitionEngine::pseudoRandom(float x, float y) const
{
    unsigned int ix = static_cast<unsigned int>(x * 10000.0f);
    unsigned int iy = static_cast<unsigned int>(y * 10000.0f);
    unsigned int seed = (ix * 374761393u + iy * 668265263u + randomSeed);
    seed = (seed ^ (seed >> 13)) * 1274126177u;
    return static_cast<float>(seed & 0xFFFFFF) / 16777216.0f;
}

std::vector<juce::String> TransitionEngine::getTransitionNames()
{
    return {
        "None", "Crossfade", "Fade to Black", "Fade to White",
        "Wipe Left", "Wipe Right", "Wipe Up", "Wipe Down",
        "Wipe Diagonal TL", "Wipe Diagonal TR",
        "Circular Expand", "Circular Contract", "Radial Wipe",
        "Spiral Out", "Spiral In",
        "Checkerboard", "Grid Slide", "Pixel Dissolve",
        "Block Dissolve", "Random Blocks",
        "Wave Horizontal", "Wave Vertical", "Wave Diagonal", "Ripple",
        "Diamond Wipe", "Heart Wipe", "Star Wipe", "Clock Wipe",
        "Iris In", "Iris Out",
        "Glitch", "Motion Blur", "Zoom In", "Zoom Out", "Rotate", "Pixelate"
    };
}

TransitionEngine::TransitionType TransitionEngine::getTransitionTypeFromName(const juce::String& name)
{
    auto names = getTransitionNames();
    for (size_t i = 0; i < names.size(); ++i)
    {
        if (names[i].equalsIgnoreCase(name))
            return static_cast<TransitionType>(i);
    }
    return TransitionType::Crossfade;
}
