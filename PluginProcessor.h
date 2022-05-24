#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <vector>
#include <algorithm>

#define OVERSAMPLE_FACTOR 1
#define OVERSAMPLE_SCALE pow(2, OVERSAMPLE_FACTOR)
// #define NUM_CHANNELS 2

class faustdsp;
class MapUI;
//==============================================================================
class AudioPluginAudioProcessor : public juce::AudioProcessor, private juce::AudioProcessorParameter::Listener
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    void parameterValueChanged(int parameterIndex, float newValue);
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting);

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void setParam(const std::string &name, float val);
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

private:
    juce::AudioParameterFloat *clipping;
    float clipping_val = 0;
    std::vector<bool> clipping_dirty;
    std::vector<int> signs;
    std::vector<int> prev_signs;
    juce::AudioParameterFloat *drive;
    juce::AudioParameterFloat *squeeze;
    // juce::AudioParameterFloat *trim;
    juce::dsp::Oversampling<float> m_oversampling{2, OVERSAMPLE_FACTOR, juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple, true};
    // Faust stuff
    std::vector<MapUI *> fUI;
    std::vector<faustdsp *> fDSP;
    float **inputs;
    float **outputs;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
