#ifndef JucePlugin_Name
#define JucePlugin_Name "Plugin Name"
#endif

#include "PluginProcessor.h"
#include "generated/FaustEffect.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : parameters(*this, nullptr, juce::Identifier("Clipping"),
                 {std::make_unique<juce::AudioParameterFloat>("clipping",
                                                              "Saturation",
                                                              0.0f,
                                                              100.0f,
                                                              0.0f),
                  std::make_unique<juce::AudioParameterFloat>("drive",
                                                              "Drive",
                                                              0.0f,
                                                              20.0f,
                                                              0.0f),
                  std::make_unique<juce::AudioParameterFloat>("squeeze",
                                                              "Squeeze",
                                                              0.0f,
                                                              20.0f,
                                                              0.0f)}),
      AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true).withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      juce::AudioProcessorValueTreeState::Listener()
{
    parameters.addParameterListener("clipping", this);
    parameters.addParameterListener("drive", this);
    parameters.addParameterListener("squeeze", this);
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

void AudioPluginAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID.compare("clipping") == 0)
    {
        clipping_val = parameters.getParameterRange(parameterID).convertTo0to1(newValue);
        for (int i = 0; i < clipping_dirty.size(); i++)
        {
            clipping_dirty[i] = true;
        }
    }
    else
    {
        setParam(parameterID.toStdString(), parameters.getParameterRange(parameterID).convertTo0to1(newValue));
    }
}

void AudioPluginAudioProcessor::setParam(const std::string &name, float val)
{
    for (int i = 0; i < getTotalNumInputChannels(); i++)
    {
        fUI[i]->setParamValue(name, val);
    }
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    int num_channels = getTotalNumInputChannels();
    // inputs = new float *[num_channels];
    inputs = new float *[1];
    outputs = new float *[1];
    inputs[0] = new float[sampleRate * OVERSAMPLE_SCALE];
    outputs[0] = new float[sampleRate * OVERSAMPLE_SCALE];
    for (int channel = 0; channel < num_channels; channel++)
    {
        signs.push_back(0);
        prev_signs.push_back(0);
        clipping_dirty.push_back(false);
        fDSP.push_back(new mydsp());
        fDSP[channel]->init(sampleRate * OVERSAMPLE_SCALE);
        fUI.push_back(new MapUI());
        fDSP[channel]->buildUserInterface(fUI[channel]);
        // inputs[channel] = new float[1];
        // inputs[channel][0] = new float[samplesPerBlock * (2 ^ OVERSAMPLE_FACTOR)];
        // outputs[channel][0] = new float[samplesPerBlock * (2 ^ OVERSAMPLE_FACTOR)];
    }
    juce::ignoreUnused(sampleRate, samplesPerBlock);

    m_oversampling.setUsingIntegerLatency(true);
    m_oversampling.reset();

    m_oversampling.initProcessing(samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

int sign(float value)
{
    if (value < 0)
    {
        return -1;
    }
    else if (value > 0)
    {
        return 1;
    }
    return 0;
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                             juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // m_oversampling.reset();
    // m_oversampling.initProcessing(buffer.getNumSamples());
    // m_oversampling.addOversamplingStage(juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple, 0.1f, -18.0f, 0.1f, -96.0f);

    juce::dsp::AudioBlock<float> inputBlock(buffer);
    juce::dsp::AudioBlock<float> returnBlock = m_oversampling.processSamplesUp(inputBlock);
    // float **tempb = new float *[totalNumInputChannels];
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        int zerocrossing = 0;

        for (int i = 0; i < returnBlock.getNumSamples(); i++)
        {
            inputs[0][i] = returnBlock.getSample(channel, i);
            prev_signs[channel] = signs[channel];
            signs[channel] = sign(inputs[0][i]);
            if (clipping_dirty[channel])
            {
                // search for first zero-crossing
                if (signs[channel] == 0 || signs[channel] != prev_signs[channel])
                {
                    zerocrossing = i;
                    fDSP[channel]->compute(i, inputs, outputs);
                    for (int j = 0; j < i; j++)
                    {
                        returnBlock.setSample(channel, j, outputs[0][j]);
                    }
                    fUI[channel]->setParamValue("clipping", clipping_val);
                    clipping_dirty[channel] = false;
                }
            }
            // fDSP[channel]->compute(1, inputs, outputs);

            // returnBlock.setSample(channel, i, outputs[0][0]);
        }
        // zerocrossing = std::min(0, zerocrossing);
        fDSP[channel]->compute(returnBlock.getNumSamples(), inputs, outputs);
        for (int i = 0; i < (returnBlock.getNumSamples() - zerocrossing); i++)
        {
            returnBlock.setSample(channel, i + zerocrossing, outputs[0][i + zerocrossing]);
        }
        // fDSP[channel]->compute(returnBlock.getNumSamples(), inputs[channel], outputs);
    }

    // for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    // {
    //     for (int i = 0; i < returnBlock.getNumSamples(); i++)
    //     {
    //         returnBlock.setSample(channel, i, outputs[channel][0][i]);
    //     }
    // }

    m_oversampling.processSamplesDown(inputBlock);

    // for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    // {
    //     for (int i = 0; i < buffer.getNumSamples(); i++)
    //     {
    //         buffer.setSample(channel, i, tempb[channel][i]);
    //     }
    // }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return false;
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor()
{
    return nullptr;
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
