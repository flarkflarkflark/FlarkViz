#include "AudioAnalyzer.h"

AudioAnalyzer::AudioAnalyzer()
    : fftData(512, 0.0f)
    , waveformData(512, 0.0f)
    , fftInputBuffer(FFT_SIZE * 2, 0.0f)
    , fftOutputBuffer(FFT_SIZE * 2, 0.0f)
{
}

AudioAnalyzer::~AudioAnalyzer()
{
}

void AudioAnalyzer::processAudioBlock(const float** inputChannelData, 
                                     int numChannels, 
                                     int numSamples)
{
    const juce::ScopedLock sl(lock);
    
    if (numChannels == 0 || numSamples == 0)
        return;
    
    // Mix down to mono if multiple channels
    for (int i = 0; i < numSamples && i < FFT_SIZE; ++i)
    {
        float sum = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
        {
            sum += inputChannelData[ch][i];
        }
        fftInputBuffer[i] = sum / numChannels;
    }
    
    // Apply windowing function
    window.multiplyWithWindowingTable(fftInputBuffer.data(), FFT_SIZE);
    
    // Perform FFT
    fft.performFrequencyOnlyForwardTransform(fftInputBuffer.data());
    
    // Convert to usable frequency data (512 bins)
    for (int i = 0; i < 512; ++i)
    {
        // Get magnitude and smooth
        float magnitude = fftInputBuffer[i];
        fftData[i] = fftData[i] * SMOOTHING_FACTOR + magnitude * (1.0f - SMOOTHING_FACTOR);
    }
    
    // Store waveform data
    for (int i = 0; i < std::min(numSamples, 512); ++i)
    {
        float sum = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
        {
            sum += inputChannelData[ch][i];
        }
        waveformData[i] = sum / numChannels;
    }
    
    // Calculate frequency bands (MilkDrop-style)
    calculateFrequencyBands();
    
    // Update beat detection
    updateBeatDetection();
}

void AudioAnalyzer::calculateFrequencyBands()
{
    // Bass: 20-250 Hz (roughly bins 0-30 at 44.1kHz)
    float newBass = calculateBandAverage(0, 30);
    bass = bass * SMOOTHING_FACTOR + newBass * (1.0f - SMOOTHING_FACTOR);
    
    // Mid: 250-2000 Hz (bins 30-180)
    float newMid = calculateBandAverage(30, 180);
    mid = mid * SMOOTHING_FACTOR + newMid * (1.0f - SMOOTHING_FACTOR);
    
    // Treble: 2000-16000 Hz (bins 180-450)
    float newTreb = calculateBandAverage(180, 450);
    treb = treb * SMOOTHING_FACTOR + newTreb * (1.0f - SMOOTHING_FACTOR);
    
    // Attenuated versions (for visual damping)
    bassAtt = bassAtt * 0.95f + bass * 0.05f;
    midAtt = midAtt * 0.95f + mid * 0.05f;
    trebAtt = trebAtt * 0.95f + treb * 0.05f;
}

float AudioAnalyzer::calculateBandAverage(int startBin, int endBin)
{
    float sum = 0.0f;
    int count = 0;
    
    for (int i = startBin; i <= endBin && i < (int)fftData.size(); ++i)
    {
        sum += fftData[i];
        count++;
    }
    
    return (count > 0) ? sum / count : 0.0f;
}

void AudioAnalyzer::updateBeatDetection()
{
    // Update history
    bassHistory[historyIndex] = bass;
    trebHistory[historyIndex] = treb;
    historyIndex = (historyIndex + 1) % 8;
    
    // Calculate average from history
    float bassAvg = 0.0f;
    float trebAvg = 0.0f;
    for (int i = 0; i < 8; ++i)
    {
        bassAvg += bassHistory[i];
        trebAvg += trebHistory[i];
    }
    bassAvg /= 8.0f;
    trebAvg /= 8.0f;
}

AudioAnalyzer::Beat AudioAnalyzer::detectBeat() const
{
    Beat beat;
    
    // Calculate average bass from history
    float bassAvg = 0.0f;
    float trebAvg = 0.0f;
    for (int i = 0; i < 8; ++i)
    {
        bassAvg += bassHistory[i];
        trebAvg += trebHistory[i];
    }
    bassAvg /= 8.0f;
    trebAvg /= 8.0f;
    
    // MilkDrop3-style beat detection
    // hardcut1: bass > 1.5
    // hardcut2-5: treb > 2.9
    beat.isBassHit = (bass > bassAvg * beatThreshold) && (bass > 1.5f);
    beat.isTrebHit = (treb > trebAvg * beatThreshold) && (treb > 2.9f);
    beat.isBeat = beat.isBassHit || beat.isTrebHit;
    
    // Calculate intensity
    beat.intensity = std::max(bass, treb);
    
    return beat;
}
