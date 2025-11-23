#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (1280, 720);
    
    // Initialize OpenGL
    openGLContext.setRenderer (this);
    openGLContext.attachTo (*this);
    openGLContext.setContinuousRepainting (true);
    
    // Initialize audio analyzer
    audioAnalyzer = std::make_unique<AudioAnalyzer>();
    
    // Initialize preset system
    presetManager = std::make_unique<PresetManager>();
    renderer = std::make_unique<PresetRenderer>();
    
    // Setup audio input
    setupAudioInput();
    
    // Load default preset
    loadDefaultPreset();
    
    // Start timer for UI updates (30 FPS is enough for UI)
    startTimer (33);
    
    setWantsKeyboardFocus (true);
}

MainComponent::~MainComponent()
{
    stopTimer();
    openGLContext.detach();
    deviceManager.closeAudioDevice();
}

void MainComponent::paint (juce::Graphics& g)
{
    // OpenGL handles all rendering
}

void MainComponent::resized()
{
    // Update renderer viewport
    if (renderer != nullptr)
        renderer->setViewportSize (getWidth(), getHeight());
}

//==============================================================================
// OpenGL Callbacks

void MainComponent::newOpenGLContextCreated()
{
    // Initialize OpenGL resources
    if (renderer != nullptr)
        renderer->initializeGL();
}

void MainComponent::renderOpenGL()
{
    using namespace juce::gl;
    
    // Get audio data
    auto fftData = audioAnalyzer->getFFTData();
    auto waveformData = audioAnalyzer->getWaveformData();
    auto beat = audioAnalyzer->detectBeat();
    
    // Render current preset
    if (renderer != nullptr)
    {
        renderer->beginFrame();
        renderer->renderPreset (fftData, waveformData, beat);
        renderer->endFrame();
    }
}

void MainComponent::openGLContextClosing()
{
    // Cleanup OpenGL resources
    if (renderer != nullptr)
        renderer->cleanupGL();
}

//==============================================================================
// Key Handling (MilkDrop3-style shortcuts)

bool MainComponent::keyPressed (const juce::KeyPress& key)
{
    // Spacebar: Random transition
    if (key == juce::KeyPress::spaceKey)
    {
        presetManager->loadRandomPreset();
        return true;
    }
    
    // F2: Toggle FPS (60/90/120)
    if (key.getKeyCode() == juce::KeyPress::F2Key)
    {
        currentFPS = (currentFPS == 60) ? 90 : (currentFPS == 90) ? 120 : 60;
        // Update OpenGL swap interval
        return true;
    }
    
    // F7: Toggle fullscreen
    if (key.getKeyCode() == juce::KeyPress::F7Key)
    {
        isFullscreen = !isFullscreen;
        setFullScreen (isFullscreen);
        return true;
    }
    
    // F9: Enter double-preset mode
    if (key.getKeyCode() == juce::KeyPress::F9Key)
    {
        renderer->enableDoublePresetMode (true);
        return true;
    }
    
    // 'a': Random mash-up
    if (key.getTextCharacter() == 'a')
    {
        presetManager->mashupRandom();
        return true;
    }
    
    // 'A' (Shift+a): Previous random preset
    if (key.getTextCharacter() == 'A')
    {
        presetManager->loadPreviousPreset();
        return true;
    }
    
    // 'c': Randomize colors
    if (key.getTextCharacter() == 'c')
    {
        renderer->randomizeColors();
        return true;
    }
    
    return false;
}

//==============================================================================
// Private Methods

void MainComponent::timerCallback()
{
    // Update UI elements if needed
}

void MainComponent::setupAudioInput()
{
    // Initialize audio device manager
    deviceManager.initialiseWithDefaultDevices (2, 0);
    
    // Set up audio callback
    auto audioCallback = [this](const float** inputChannelData, int numInputChannels,
                                 int numSamples)
    {
        if (audioAnalyzer != nullptr)
            audioAnalyzer->processAudioBlock (inputChannelData, numInputChannels, numSamples);
    };
    
    // For PulseAudio/JACK capture on Linux, we need to set up system audio monitoring
    // This requires platform-specific code
    #if JUCE_LINUX
        // TODO: Implement PulseAudio loopback or JACK monitoring
        DBG ("Linux audio capture not yet implemented");
    #endif
}

void MainComponent::loadDefaultPreset()
{
    // Load a default preset
    auto defaultPreset = presetManager->getPreset (0);
    if (defaultPreset != nullptr && renderer != nullptr)
        renderer->loadPreset (defaultPreset);
}
