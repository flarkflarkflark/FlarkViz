#pragma once

#include <JuceHeader.h>

/**
 * @class AudioAnalyzer
 * @brief Performs FFT analysis and beat detection on audio input
 * 
 * Provides frequency spectrum data and beat detection for visualization.
 */
class AudioAnalyzer
{
public:
    AudioAnalyzer();
    ~AudioAnalyzer();
    
    //==========================================================================
    /**
     * @brief Process incoming audio block
     * @param inputChannelData Audio samples
     * @param numChannels Number of input channels
     * @param numSamples Number of samples in buffer
     */
    void processAudioBlock (const float** inputChannelData, 
                           int numChannels, 
                           int numSamples);
    
    //==========================================================================
    /**
     * @brief Get current FFT data (512 bins)
     * @return Array of frequency magnitudes (0.0 - 1.0)
     */
    const std::vector<float>& getFFTData() const { return fftData; }
    
    /**
     * @brief Get current waveform data
     * @return Array of time-domain samples
     */
    const std::vector<float>& getWaveformData() const { return waveformData; }
    
    /**
     * @brief Get MilkDrop-style audio variables
     */
    float getBass() const { return bass; }
    float getMid() const { return mid; }
    float getTreb() const { return treb; }
    float getBassAtt() const { return bassAtt; }
    float getMidAtt() const { return midAtt; }
    float getTrebAtt() const { return trebAtt; }
    
    /**
     * @brief Detect beat in current frame
     * @return Beat structure with bass/mid/treb beat flags
     */
    struct Beat {
        bool isBeat;
        bool isBassHit;
        bool isTrebHit;
        float intensity;
    };
    Beat detectBeat() const;

private:
    //==========================================================================
    static constexpr int FFT_ORDER = 10;  // 2^10 = 1024 samples
    static constexpr int FFT_SIZE = 1 << FFT_ORDER;
    
    juce::dsp::FFT fft {FFT_ORDER};
    juce::dsp::WindowingFunction<float> window {FFT_SIZE, 
                                                 juce::dsp::WindowingFunction<float>::hann};
    
    // Audio buffers
    std::vector<float> fftData;
    std::vector<float> waveformData;
    std::vector<float> fftInputBuffer;
    std::vector<float> fftOutputBuffer;
    
    // MilkDrop-style variables
    float bass = 0.0f;
    float mid = 0.0f;
    float treb = 0.0f;
    float bassAtt = 0.0f;
    float midAtt = 0.0f;
    float trebAtt = 0.0f;
    
    // Beat detection state
    float bassHistory[8] = {0};
    float trebHistory[8] = {0};
    int historyIndex = 0;
    float beatThreshold = 1.5f;
    
    // Smoothing
    static constexpr float SMOOTHING_FACTOR = 0.8f;
    
    //==========================================================================
    void calculateFrequencyBands();
    float calculateBandAverage (int startBin, int endBin);
    void updateBeatDetection();
    
    juce::CriticalSection lock;
};
