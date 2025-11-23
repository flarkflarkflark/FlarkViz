#pragma once
#include <JuceHeader.h>

// Stub for system audio capture
class AudioCapture
{
public:
    AudioCapture() {}
    ~AudioCapture() {}
    void startCapture() { DBG("AudioCapture: Started"); }
    void stopCapture() { DBG("AudioCapture: Stopped"); }
};
